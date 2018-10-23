#include "examinesynchronizer.h"
#include "../common/dbdefine.h"
#include "../common/bussinessmap.h"
#include "../framework/common/convertdata.h"
#include "../framework/common/systemconfig.h"

ExamineSynchronizer::ExamineSynchronizer()
{
    m_nDataOperationType = BUSINESS_TYPE_STANDARDIZATION;
}

ExamineSynchronizer::~ExamineSynchronizer()
{

}

void ExamineSynchronizer::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
{
    qDebug() <<Q_FUNC_INFO << "  action " << action << " objects " << objects << " map " << map << "  running is " << m_nRunnings;
    m_nRunnings = true;
    m_nSyncAction = action;
    m_nSyncMap = map;
    m_nSyncUser = m_nSyncMap["UserID"].toString();
    m_nSyncObjects = objects;
    m_nSyncTaskInfoMap.clear();
    m_nSyncTaskSuccessCount = 0;
    m_nSyncTaskFailCount = 0;
    m_nSyncTaskTotalCount = 0;
    QString syncUserID = m_nSyncMap["UserID"].toString();
    QStringList addList = m_nSyncMap["addIDs"].toStringList();
    QStringList updateList = m_nSyncMap["updateIDs"].toStringList();
    if(addList.count() == 0 && updateList.count() == 0){
        m_nRunnings = false;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
        return;
    }

    // 同步任务
    synTaskByIds(addList, syncUserID, SYNC_TASK_ADD);
    synTaskByIds(updateList, syncUserID, SYNC_TASK_UPDATE);
    m_nUserMaxTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    m_nSyncTaskTotalCount = m_nSyncTaskInfoMap.size();
}

void ExamineSynchronizer::syncPreTask(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  action " << action;
    m_nSyncPreTaskPlanIDs.clear();
    m_nSyncPreTaskAction = action;
    m_nSyncPreTaskFailCount = 0;
    sendData(dataPackage, action);
}

void ExamineSynchronizer::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << " objects " << objects << "  action " << action;
    if(result == true && error == NETWORK_NO_ERROR){
        if(objects == EXAM_Off_GetSomeRunningTasksPacket){
            /**
            * 规范检查任务详情
            */
            handleTasks(map.dataPackage, action);

            /**
            * 结果处理，判断是否所有任务同步完成
            */
            resultProcess(action, true);
        }else if(objects == EXAM_Off_GetPrePlanIDsByUserID){
            /**
            * 规范检查预生成任务PlanIDs
            */
            handlePreTaskPlanIDs(map.dataPackage);
        }else if(objects == EXAM_Off_GetPrePacketByPlanID){
            /**
            * 规范检查预生成任务详情
            */
            handlePreTasks(map.dataPackage, action);
        }
    }else{
        if(objects == EXAM_Off_GetSomeRunningTasksPacket){
            /**
            * 结果处理，判断是否所有任务同步完成
            */
            resultProcess(action, false);
        }else if(objects == EXAM_Off_GetPrePlanIDsByUserID){
            DbusPackage map;
            map.dataPackage.head.objects = objects;
            emit onResultOfResponse(map.dataPackage.head.objects, m_nSyncPreTaskAction, false, -1, map);
        }else if(objects == EXAM_Off_GetPrePacketByPlanID){
            preTaskResultProcess(action, false);
        }
    }
}

void ExamineSynchronizer::synTaskByIds(const QStringList &taskDataKeys, const QString &userID, const SyncTaskType &taskType){
    foreach (QString taskDataKey, taskDataKeys) {
        QString syncTaskTime = getTaskSyncTime(userID, taskDataKey);
        QVariantMap valueMap;
        valueMap["TaskType"] = taskType;
        valueMap["TaskSyncTime"] = syncTaskTime;
        m_nSyncTaskInfoMap.insert(taskDataKey, valueMap);
        syncTaskInfoById(taskDataKey, userID, syncTaskTime, QString::number(taskType));
    }
}

QString ExamineSynchronizer::getTaskSyncTime(const QString &userID, const QString &taskDataKey){
    QString syncDateTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QStringList columns;
    QString selectionArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "' and " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_TASK_SYNC_DATE, columns, selectionArg);
    if(!dataRows.isEmpty()){
        syncDateTime = dataRows.at(0).value(LAST_TIME);
    }
    return syncDateTime;
}

void ExamineSynchronizer::syncTaskInfoById(const QString &taskDataKey, const QString &userID, const QString &syncDatetime, const QString &taskType) {
    DataPackage dataPackage;
    dataPackage.head.key = PACKAGE_HEAD_KEY;
    dataPackage.head.type = TYPE_REQUEST;
    dataPackage.head.objects = EXAM_Off_GetSomeRunningTasksPacket;
    dataPackage.para.insert("UserID", userID);
    dataPackage.para.insert("TaskDataKeys", taskDataKey);
    dataPackage.para.insert("LastDEXTime", syncDatetime);
    dataPackage.para.insert("TaskType", taskType);
    sendData(dataPackage, taskDataKey);
}

void ExamineSynchronizer::handleTasks(const DataPackage &dataPackage, const QString &taskDataKey){
    const QList<DataTable> &tables = dataPackage.tables;
    if(tables.size() > 0){
        /**
        * First step : fetch tables
        */
        const DataTable &taskTable = tables.at(0);
        const DataTable &objectTable = tables.at(1);
        const DataTable &itemTable = tables.at(2);
        const DataTable &organizationTable = tables.at(3);
        const DataTable &areaTable = tables.at(4);

        /**
        * step 1 : update task table
        */
        if (!taskTable.value.isEmpty()) {
            updateTaskTable(taskTable.value);
            /**
            * 更新任务同步时间
            */
            QVariantMap valueMap;
            valueMap["TaskType"] = m_nSyncTaskInfoMap[taskDataKey].toMap()["TaskType"];
            valueMap["TaskSyncTime"] = taskTable.value.at(0)["ServerDate"];
            m_nSyncTaskInfoMap.insert(taskDataKey, valueMap);
        }

        /**
        * step 2 : update object table
        */
        if (!objectTable.value.isEmpty()) {
            updateObjectTable(objectTable.value);
        }

        /**
        * step 3 : update item table
        */
        if (!itemTable.value.isEmpty()) {
            updateItemTable(itemTable.value);
        }

        /**
        * step 4 : update area table
        */
        if (!areaTable.value.isEmpty()) {
            updateAreaTable(areaTable.value);
        }

        /**
        * step 5 : update organization table
        */
        if (!organizationTable.value.isEmpty()) {
            updateOrganizationTable(organizationTable.value);
        }
    }
}

void ExamineSynchronizer::updateTaskTable(const QList<DataRow> &dataRows) {
    for(int i = 0; i < dataRows.size(); i++){
        /**
        * Fetch values and create content value object
        */
        const DataRow &dataRow = dataRows.at(i);
        DataRow updateRow;
        QString taskDataKey = dataRow["TaskDataKey"];
        updateRow["TaskDataKey"] = taskDataKey;
        updateRow["TaskCode"] = dataRow["TaskCode"];
        updateRow["TaskName"] = dataRow["TaskName"];
        updateRow["Descript"] = dataRow["Descript"];
        updateRow["EnableUploadAttach"] = dataRow["EnableUploadAttach"];
        updateRow["TaskType"] = dataRow["TaskType"];
        updateRow["TaskStartTime"] = dataRow["TaskStartTime"];
        updateRow["TaskEndTime"] = dataRow["TaskEndTime"];
        updateRow["TaskRemindTime"] = dataRow["TaskRemindTime"];
        updateRow["TaskStatus"] = dataRow["TaskStatus"];
        updateRow["CompanyID"] = dataRow["CompanyID"];
        updateRow["CompanyName"] = dataRow["CompanyName"];
        updateRow["ModifiedDate"] = dataRow["ModifiedDate"];
        updateRow["ExamObjectType"] = dataRow["ExamObjectType"];
        updateRow["LimitMode"] = dataRow["LimitMode"];
        updateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
        updateRow["SyncStatus"] = TASK_STATUS_SYNC_FINISH;
        updateRow["IsPreTask"] = "0";

        /**
        * PlanID字段不为空时，标识是未来任务
        */
        updateRow["PlanID"] =  "";

        if (isTaskExisted(taskDataKey)) {
            /**
            * if task is existed, update it
            */
            QString updateArgs = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            m_nDbHelper->update(EXAMINE_TABLE_TASK, updateRow, updateArgs);
        } else {
            /**
            * if task isn't existed, add it
            */
            m_nDbHelper->insert(EXAMINE_TABLE_TASK, updateRow);
        }
    }
}

void ExamineSynchronizer::updateObjectTable(const QList<DataRow> &dataRows) {
    for(int i = 0; i < dataRows.size(); i++){
        /**
        * Fetch values and create content value object
        */
        DataRow dataRow = dataRows.at(i);
        DataRow updateRow;
        QString taskObjectDataKey = dataRow["TaskObjectDataKey"];
        updateRow["TaskObjectDataKey"] = taskObjectDataKey;
        updateRow["TaskObjectName"] = dataRow["TaskObjectName"];
        updateRow["TaskDataKey"] = dataRow["TaskDataKey"];
        updateRow["PostCode"] = dataRow["PostCode"];
        updateRow["ExamObjectType"] = dataRow["ExamObjectType"];
        updateRow["RealObjectID"] = dataRow["RealObjectID"];
        updateRow["TotalItemCount"] = dataRow["TotalItemCount"];
        updateRow["ExamItemCount"] = dataRow["ExamItemCount"];
        updateRow["RFIDCode"] = dataRow["RFIDCode"];
        updateRow["BarCode"] = dataRow["BarCode"];
        updateRow["QRCode"] = dataRow["QRCode"];

        if (isObjectExisted(taskObjectDataKey)) {
            /**
            * if object is existed, update it
            */
            QString updateArgs = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
            m_nDbHelper->update(EXAMINE_TABLE_OBJECT, updateRow, updateArgs);
        } else {
            /**
            * if object isn't existed, add it
            */
            updateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
            m_nDbHelper->insert(EXAMINE_TABLE_OBJECT, updateRow);
        }
    }
}

void ExamineSynchronizer::updateItemTable(const QList<DataRow> &dataRows) {
    for(int i = 0; i < dataRows.size(); i++){
        /**
        * Fetch values and create content value object
        */
        DataRow dataRow = dataRows.at(i);
        DataRow updateRow;
        QString taskItemDataKey = dataRow["TaskItemDataKey"];
        updateRow["TaskItemDataKey"] = taskItemDataKey;
        updateRow["ItemName"] = dataRow["ItemName"];
        updateRow["ItemFullName"] = dataRow["ItemFullName"];
        updateRow["ModelItemID"] = dataRow["ModelItemID"];
        updateRow["TaskObjectDataKey"] = dataRow["TaskObjectDataKey"];
        updateRow["TaskDataKey"] = dataRow["TaskDataKey"];
        updateRow["ItemSchema"] = dataRow["ItemSchema"];
        updateRow["SerialKey"] = dataRow["SerialKey"];
        updateRow["LocalStatus"] = dataRow["ExamStatus"];
        updateRow["ModifiedDate"] = dataRow["ModifiedDate"];
        updateRow["RFIDCode"] = dataRow["RFIDCode"];
        updateRow["BarCode"] = dataRow["BarCode"];
        updateRow["QRCode"] = dataRow["QRCode"];

        if (isItemExisted(taskItemDataKey)) {
            /**
            * if object is existed, update it
            */
            QString updateArgs = " where " + QString(TASK_ITEM_DATA_KEY) + "='" + taskItemDataKey + "'";
            m_nDbHelper->update(EXAMINE_TABLE_ITEM, updateRow, updateArgs);
        } else {
            /**
            * if object isn't existed, add it
            */
            updateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
            m_nDbHelper->insert(EXAMINE_TABLE_ITEM, updateRow);
        }
    }
}

void ExamineSynchronizer::updateAreaTable(const QList<DataRow> &dataRows) {
    for(int i = 0; i < dataRows.size(); i++){
        /**
        * Fetch values and create content value object
        */
        DataRow dataRow = dataRows.at(i);
        DataRow updateRow;
        QString areaID = dataRow["AreaID"];
        updateRow["AreaID"] = areaID;
        updateRow["AreaCode"] = dataRow["AreaCode"];
        updateRow["AreaName"] = dataRow["AreaName"];
        updateRow["Descript"] = dataRow["Descript"];
        updateRow["RFIDCode"] = dataRow["RFIDCode"];
        updateRow["BarCode"] = dataRow["BarCode"];
        updateRow["QRCode"] = dataRow["QRCode"];
        updateRow["CompanyID"] = dataRow["CompanyID"];
        updateRow["CompanyName"] = dataRow["CompanyName"];
        updateRow["DepartmentID"] = dataRow["DepartmentID"];
        updateRow["DepartmentName"] = dataRow["DepartmentName"];
        updateRow["ModifiedDate"] = dataRow["ModifiedDate"];

        if (isAreaExisted(areaID)) {
            /**
            * if object is existed, update it
            */
            QString updateArgs = " where AreaID ='" + areaID + "'";
            m_nDbHelper->update(EXAMINE_TABLE_AREA, updateRow, updateArgs);

        } else {
            /**
            * if object isn't existed, add it
            */
            m_nDbHelper->insert(EXAMINE_TABLE_AREA, updateRow);
        }
    }
}

void ExamineSynchronizer::updateOrganizationTable(const QList<DataRow> &dataRows) {
    for(int i = 0; i < dataRows.size(); i++){
        /**
        * Fetch values and create content value object
        */
        DataRow dataRow = dataRows.at(i);
        DataRow updateRow;
        QString organiseUnitID = dataRow["OrganiseUnitID"];
        updateRow["OrganiseUnitID"] = organiseUnitID;
        updateRow["OrganiseUnitCode"] = dataRow["OrganiseUnitCode"];
        updateRow["StandardCode"] = dataRow["StandardCode"];
        updateRow["OrganiseUnitName"] = dataRow["OrganiseUnitName"];
        updateRow["ShortName"] = dataRow["ShortName"];
        updateRow["FullName"] = dataRow["FullName"];
        updateRow["ABName"] = dataRow["ABName"];
        updateRow["RFIDCode"] = dataRow["RFIDCode"];
        updateRow["BarCode"] = dataRow["BarCode"];
        updateRow["QRCode"] = dataRow["QRCode"];
        updateRow["Remark"] = dataRow["Remark"];
        updateRow["ModifiedDate"] = dataRow["ModifiedDate"];

        if (isOrganizationExisted(organiseUnitID)) {
            /**
            * if object is existed, update it
            */
            QString updateArgs = " where OrganiseUnitID ='" + organiseUnitID + "'";
            m_nDbHelper->update(EXAMINE_TABLE_ORGANIZATION, updateRow, updateArgs);

        } else {
            /**
            * if object isn't existed, add it
            */
            m_nDbHelper->insert(EXAMINE_TABLE_ORGANIZATION, updateRow);
        }
    }
}

bool ExamineSynchronizer::isTaskExisted(const QString &taskDataKey) {
    QString sql = "select TaskDataKey from EXAM_ExamineTask where TaskDataKey = '" + taskDataKey + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if (result && !dataRows.isEmpty()){
        return true;
    }
    return false;
}


bool ExamineSynchronizer::isObjectExisted(const QString &objectDataKey) {
    QString sql = "select TaskObjectDataKey from EXAM_ExamineTaskObject where TaskObjectDataKey = '" + objectDataKey + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if (result && !dataRows.isEmpty()){
        return true;
    }
    return false;
}

bool ExamineSynchronizer::isItemExisted(const QString &itemDataKey) {
    QString sql = "select TaskItemDataKey from EXAM_ExamineTaskItem where TaskItemDataKey = '" + itemDataKey + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if (result && !dataRows.isEmpty()){
        return true;
    }
    return false;
}


bool ExamineSynchronizer::isAreaExisted(const QString &areaID) {
    QString sql = "select AreaID from EXAM_ExamineTaskArea where AreaID = '" + areaID + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if (result && !dataRows.isEmpty()){
        return true;
    }
    return false;
}

bool ExamineSynchronizer::isOrganizationExisted(const QString &organiseUnitID) {
    QString sql = "select OrganiseUnitID from EXAM_ExamineOrganiseUnit where OrganiseUnitID = '" + organiseUnitID + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if (result && !dataRows.isEmpty()){
        return true;
    }
    return false;
}

void ExamineSynchronizer::resultProcess(const QString &taskDataKey, const bool &result){
    QVariant varMap = m_nSyncTaskInfoMap.value(taskDataKey);
    QString time = varMap.toMap().value("TaskSyncTime").toString();
    if(time > m_nUserMaxTime){
        m_nUserMaxTime = time;
    }
    m_nSyncTaskInfoMap.remove(taskDataKey);

    /**
    * Step 1. 成功计数
    */
    if(result){
        m_nSyncTaskSuccessCount++;
        updateTaskStatus(taskDataKey, TASK_STATUS_SYNC_FINISH);
        updateUserTaskTime(m_nSyncUser, taskDataKey, time);
    }else{
        m_nSyncTaskFailCount++;
        updateTaskStatus(taskDataKey, TASK_STATUS_SYNC_FAIL);
    }

    /**
    * Step 2. 更新用户业务表的获取的时间戳
    */
    if(m_nSyncTaskInfoMap.isEmpty()) {
        if(m_nSyncTaskFailCount == 0){
            updateUserTime(m_nSyncUser, m_nUserMaxTime);
        }
        m_nRunnings = false;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
    }
}

void ExamineSynchronizer::updateUserTaskTime(const QString &userID, const QString &taskDataKey, const QString &time){
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(TASK_DATA_KEY, taskDataKey);
    dataRow.insert(LAST_TIME, time);
    QStringList columns;
    columns.append(USER_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "' and " +
                        QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
    QList<DataRow> list = m_nDbHelper->select(EXAMINE_TABLE_TASK_SYNC_DATE, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(EXAMINE_TABLE_TASK_SYNC_DATE, dataRow, selection);
    }else{
        m_nDbHelper->insert(EXAMINE_TABLE_TASK_SYNC_DATE, dataRow);
    }
}

void ExamineSynchronizer::updateUserTime(const QString &userID, const QString &time){
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(LAST_TIME, time);
    QStringList columns;
    columns.append(USER_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> list = m_nDbHelper->select(EXAMINE_TABLE_SYNC_DATE, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(EXAMINE_TABLE_SYNC_DATE, dataRow, selection);
    }else{
        m_nDbHelper->insert(EXAMINE_TABLE_SYNC_DATE, dataRow);
    }
}

void ExamineSynchronizer::updateTaskStatus(const QString &taskDataKey, const QString &syncStatus){
    DataRow dataRow;
    dataRow.insert(SYNC_STATUS, syncStatus);
    QString updateArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
    m_nDbHelper->update(EXAMINE_TABLE_TASK, dataRow, updateArg);
}

void ExamineSynchronizer::handlePreTaskPlanIDs(const DataPackage &dataPackage) {
    const QList<DataRow> &dataRows = dataPackage.tables[0].value;
    if(!dataRows.isEmpty()){
        for(int i = 0; i < dataRows.size(); i++){
            const DataRow &dataRow = dataRows.at(i);
            QString planID = dataRow["PlanID"];
            DataPackage dataPackage;
            dataPackage.head.type = TYPE_REQUEST;
            dataPackage.head.key = PACKAGE_HEAD_KEY;
            dataPackage.head.objects = EXAM_Off_GetPrePacketByPlanID;
            dataPackage.para["PlanID"] = planID;
            dataPackage.para["PreBuildEndTimeStr"] = dataRow["PreBuildEndTimeStr"];
            m_nSyncPreTaskPlanIDs.append(planID);
            sendData(dataPackage, planID);
        }
    }else{
        DbusPackage map;
        map.dataPackage.head.objects = EXAM_Off_GetPrePlanIDsByUserID;
        emit onResultOfResponse(map.dataPackage.head.objects, m_nSyncPreTaskAction, true, 0, map);
    }
}

void ExamineSynchronizer::handlePreTasks(const DataPackage &dataPackage, const QString &planID){
    const QList<DataTable> &tables = dataPackage.tables;
    QStringList columns;
    QString selectArg;
    QList<DataRow> list;

    qint64 time1 = QDateTime::currentMSecsSinceEpoch();
    if(!tables.isEmpty()) {
        const QList<DataRow> &dataRows0 = tables[0].value;
        columns.append(PLAN_ID_CREATE_TIME);
        columns.append(TASK_DATA_KEY);
        selectArg = " where " + QString(PLAN_ID) + "='" + planID + "'";
        list =  m_nDbHelper->select(EXAMINE_TABLE_TASK, columns, selectArg);
        if(!list.isEmpty() && !dataRows0.isEmpty()){
            if(dataRows0[0].value(PLAN_ID_CREATE_TIME) != list[0].value(PLAN_ID_CREATE_TIME)){
                QStringList taskDataKeys;
                for(int m = 0; m < list.size(); m++){
                    taskDataKeys.append(list[m][TASK_DATA_KEY]);
                }
                deleteTask(taskDataKeys);
            }
        }
        if(!dataRows0.isEmpty()) {
            const DataRow &taskRow = dataRows0[0];
            const QList<DataRow> &objectRows = tables[1].value;
            const QList<DataRow> &itemRows = tables[2].value;
            const QList<DataRow> &mappingRows = tables[3].value;
            QStringList taskDataKes;
            QStringList objectDataKeys;
            m_nDbHelper->beginTransaction();
            for (int i = 0; i < mappingRows.size(); i++) {
                const DataRow &mappingRow = mappingRows.at(i);
                QString taskDataKey = mappingRow["TaskDataKey"];
                QString taskObjectDataKey = mappingRow["TaskObjectDataKey"];
                QString taskItemDataKey = mappingRow["TaskItemDataKey"];
                QString taskObjectDataKeyFlag = mappingRow["TaskObjectDataKeyFlag"];
                QString taskItemDataKeyFlag = mappingRow["TaskItemDataKeyFlag"];

                /**
                * Step 1. 更新任务信息
                */
                if (!taskDataKes.contains(taskDataKey)) {
                    DataRow taskUpdateRow = taskRow;
                    QString taskStartTime = mappingRow["TaskStartTime"];
                    QString taskEndTime = mappingRow["TaskEndTime"];
                    taskUpdateRow["TaskDataKey"] = taskDataKey;
                    taskUpdateRow["TaskStartTime"] = taskStartTime;
                    taskUpdateRow["TaskEndTime"] = taskEndTime;
                    taskUpdateRow["PlanID"] = planID;
                    taskUpdateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
                    taskUpdateRow["SyncStatus"] = TASK_STATUS_SYNC_FINISH;
                    taskUpdateRow["IsPreTask"] = "1";
                    taskDataKes.append(taskDataKey);
                    columns.clear();
                    selectArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
                    list = m_nDbHelper->select(EXAMINE_TABLE_TASK, columns, selectArg);
                    if(!list.isEmpty()){
                        m_nDbHelper->update(EXAMINE_TABLE_TASK, taskUpdateRow, selectArg);
                    }else{
                        m_nDbHelper->insert(EXAMINE_TABLE_TASK, taskUpdateRow);
                    }
                }

                /**
                * Step 2. 更新组信息
                */
                if (!objectDataKeys.contains(taskObjectDataKey)) {
                    for (int j = 0; j < objectRows.size(); j++) {
                        const DataRow &objectRow = objectRows[j];
                        QString taskObjectDataKeyFlag0 = objectRow["TaskObjectDataKeyFlag"];
                        if (taskObjectDataKeyFlag == taskObjectDataKeyFlag0) {
                            DataRow objectUpdateRow = objectRow;
                            objectUpdateRow["TaskObjectDataKey"] = taskObjectDataKey;
                            objectUpdateRow["TaskDataKey"] = taskDataKey;
                            objectUpdateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
                            objectUpdateRow.remove("TaskObjectDataKeyFlag");
                            objectDataKeys.append(taskObjectDataKey);
                            selectArg = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
                            list = m_nDbHelper->select(EXAMINE_TABLE_OBJECT, columns, selectArg);
                            if(!list.isEmpty()){
                                m_nDbHelper->update(EXAMINE_TABLE_OBJECT, objectUpdateRow, selectArg);
                            }else{
                                m_nDbHelper->insert(EXAMINE_TABLE_OBJECT, objectUpdateRow);
                            }
                            break;
                        }
                    }
                }

                /**
                * Step 3. 更新对象信息
                */
                for (int k = 0; k < itemRows.size(); k++) {
                    const DataRow &itemRow = itemRows[k];
                    QString taskItemDataKeyFlag0 = itemRow["TaskItemDataKeyFlag"];
                    if (taskItemDataKeyFlag == taskItemDataKeyFlag0) {
                        DataRow itemUpdateRow = itemRow;
                        itemUpdateRow.remove("TaskObjectDataKeyFlag");
                        itemUpdateRow.remove("TaskItemDataKeyFlag");
                        itemUpdateRow["TaskDataKey"] = taskDataKey;
                        itemUpdateRow["TaskObjectDataKey"] = taskObjectDataKey;
                        itemUpdateRow["TaskItemDataKey"] = taskItemDataKey;
                        itemUpdateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
                        columns.clear();
                        selectArg = " where " + QString(TASK_ITEM_DATA_KEY) + "='" + taskItemDataKey + "'";
                        list = m_nDbHelper->select(EXAMINE_TABLE_ITEM, columns, selectArg);
                        if(!list.isEmpty()){
                            m_nDbHelper->update(EXAMINE_TABLE_ITEM, itemUpdateRow, selectArg);
                        }else{
                            m_nDbHelper->insert(EXAMINE_TABLE_ITEM, itemUpdateRow);
                        }
                        break;
                    }
                }
            }
            m_nDbHelper->endTransaction();
            preTaskResultProcess(planID, true);

        }else{
            preTaskResultProcess(planID, true);
        }
    }else{
        preTaskResultProcess(planID, true);
    }
    qint64 time2 = QDateTime::currentMSecsSinceEpoch();
    qDebug() << Q_FUNC_INFO << " time2-time1 : " << time2 - time1;
}

void ExamineSynchronizer::preTaskResultProcess(const QString &planID, const bool &result){
    qDebug() << Q_FUNC_INFO << "  planID " << planID << "  result " << result;
    /**
    * Step 1. 清除数据
    */
    m_nSyncPreTaskPlanIDs.removeOne(planID);

    /**
    * Step 2. 失败计数
    */
    if(!result){
        m_nSyncPreTaskFailCount++;
    }
    /**
    * Step 3. 发送结果
    */
    if(m_nSyncPreTaskPlanIDs.isEmpty()) {
        DbusPackage map;
        map.dataPackage.head.objects = EXAM_Off_GetPrePlanIDsByUserID;
        if(m_nSyncPreTaskFailCount == 0){
            emit onResultOfResponse(map.dataPackage.head.objects, m_nSyncPreTaskAction, true, 0, map);
        }else{
            emit onResultOfResponse(map.dataPackage.head.objects, m_nSyncPreTaskAction, false, -1, map);
        }
    }
}

void ExamineSynchronizer::deleteTask(const QStringList &taskDataKeyList){
    QString taskDataKeys = ConvertData::getInstance()->convertForSql(taskDataKeyList);
    if(taskDataKeys != "") {
        /**
        * Step 1. 删除任务信息
        */
        m_nDbHelper->beginTransaction();
        QString sql0 = "delete from " + QString(EXAMINE_TABLE_TASK) + " where " +
                       QString(TASK_DATA_KEY) + " in (" + taskDataKeys + ")";
        m_nDbHelper->execSQL(sql0);

        /**
        * Step 2. 删除组信息
        */
        QString sql1 = "delete from " + QString(EXAMINE_TABLE_OBJECT) + " where " +
                       QString(TASK_DATA_KEY) + " in (" + taskDataKeys + ")";
        m_nDbHelper->execSQL(sql1);

        /**
        * Step 3. 删除对象信息
        */
        QString sql2 = "delete from " + QString(EXAMINE_TABLE_ITEM) + " where " +
                       QString(TASK_DATA_KEY) + " in (" + taskDataKeys + ")";
        m_nDbHelper->execSQL(sql2);

        m_nDbHelper->endTransaction();
    }
}


