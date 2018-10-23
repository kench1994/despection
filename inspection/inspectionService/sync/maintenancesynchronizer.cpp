#include "maintenancesynchronizer.h"
#include "../common/dbdefine.h"
#include "../common/bussinessmap.h"
#include "../framework/common/convertdata.h"
#include "../framework/common/systemconfig.h"
#define SYNC_PRETASK "SyncPreTask_"

MaintenanceSynchronizer::MaintenanceSynchronizer()
{
    m_nDataOperationType = BUSINESS_TYPE_MAINTENANCE;
}

MaintenanceSynchronizer::~MaintenanceSynchronizer()
{

}

void MaintenanceSynchronizer::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
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

void MaintenanceSynchronizer::syncPreTask(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  action " << action;
    m_nSyncPreTaskPlanIDs.clear();
    m_nSyncPreTaskAction = action;
    m_nSyncPreTaskFailCount = 0;
    sendData(dataPackage, action);
}

void MaintenanceSynchronizer::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << " objects " << objects << ", action " << action << " , result " << result << " , errror " << error;
    if(result == true && error == NETWORK_NO_ERROR){
        if(objects == EMM_Off_GetSomeRunningTasksPacket){
            /**
            * 保养任务详情
            */
            handleTasks(map.dataPackage, action);

        }else if(objects == EMM_Off_GetPrePlanIDsByUserID){
            /**
            * 保养预生成任务PlanIDs
            */
            handlePreTaskPlanIDs(map.dataPackage);
        }else if(objects == EMM_Off_GetPrePacketByPlanID){
            /**
            * 保养预生成任务详情
            */
            handlePreTasks(map.dataPackage, action);
        }else if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            /**
            * 设备信息
            */
            /**
            * Step 1. 结果处理，判断是否所有任务同步完成
            */
            int index = action.indexOf(SYNC_PRETASK);
            if(index != -1){
                QString planID = action.mid(index + strlen(SYNC_PRETASK));
                preTaskResultProcess(planID, true);
            }else{
                resultProcess(action, true);
            }
        }
    }else{
        /**
        * 结果处理，判断是否所有任务同步完成
        */
        if(objects == EMM_Off_GetSomeRunningTasksPacket){
            resultProcess(action, false);
        }else if(objects == EMM_Off_GetPrePlanIDsByUserID){
            DbusPackage map;
            map.dataPackage.head.objects = EMM_Off_GetPrePlanIDsByUserID;
            emit onResultOfResponse(map.dataPackage.head.objects, m_nSyncPreTaskAction, false, -1, map);
        }else if(objects == EMM_Off_GetPrePacketByPlanID){
            preTaskResultProcess(action, false);
        }else if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            int index = action.indexOf(SYNC_PRETASK);
            if(index != -1){
                QString planID = action.mid(index + strlen(SYNC_PRETASK));
                preTaskResultProcess(planID, false);
            }else{
                resultProcess(action, false);
            }
        }
    }
}

void MaintenanceSynchronizer::synTaskByIds(const QStringList &taskDataKeys, const QString &userID, const SyncTaskType &taskType){
    foreach (QString taskDataKey, taskDataKeys) {
        QString syncTaskTime = getTaskSyncTime(userID, taskDataKey);
        QVariantMap valueMap;
        valueMap["TaskType"] = taskType;
        valueMap["TaskSyncTime"] = syncTaskTime;
        m_nSyncTaskInfoMap.insert(taskDataKey, valueMap);
        syncTaskInfoById(taskDataKey, userID, syncTaskTime, QString::number(taskType));
    }
}

QString MaintenanceSynchronizer::getTaskSyncTime(const QString &userID, const QString &taskDataKey){
    QString syncDateTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QStringList columns;
    QString selectionArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "' and " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_USER_TASK_TIME, columns, selectionArg);
    if(!dataRows.isEmpty()){
        syncDateTime = dataRows.at(0).value(LAST_TIME);
    }
    return syncDateTime;
}

void MaintenanceSynchronizer::syncTaskInfoById(const QString &taskDataKey, const QString &userID, const QString &syncDatetime, const QString &taskType) {
    DataPackage dataPackage;
    dataPackage.head.key = PACKAGE_HEAD_KEY;
    dataPackage.head.type = TYPE_REQUEST;
    dataPackage.head.objects = EMM_Off_GetSomeRunningTasksPacket;
    dataPackage.para.insert("UserID", userID);
    dataPackage.para.insert("TaskDataKeys", taskDataKey);
    dataPackage.para.insert("LastDEXTime", syncDatetime);
    dataPackage.para.insert("TaskType", taskType);
    sendData(dataPackage, taskDataKey);
}

void MaintenanceSynchronizer::handleTasks(const DataPackage &dataPackage, const QString &taskDataKey)
{
    const QList<DataTable> &tables = dataPackage.tables;
    if(!tables.isEmpty()) {

         m_nDbHelper->beginTransaction();

         /**
          * Step 1. 更新任务信息
          */
         const DataTable &taskTable = tables.at(0);
         const QList<DataRow> &dataRows0 = taskTable.value;
         updateTask(dataRows0);

         /**
          * Step 2. 更新组信息
          */
         const DataTable &groupTable = tables.at(1);
         const QList<DataRow> &dataRows1 = groupTable.value;
         updateGroup(dataRows1);

         /**
          * Step 3. 更新对象信息
          */
         const DataTable &objectTable = tables.at(2);
         const QList<DataRow> &dataRows2 = objectTable.value;
         QMap<QString, QString> deviceInfo;
         updateItem(dataRows2, deviceInfo);

         m_nDbHelper->endTransaction();

         /**
          * Step 4. 同步设备信息
          */
         bool result = requestObjs(deviceInfo, taskDataKey);
         if(!result){
             resultProcess(taskDataKey, true);
         }
     }
}

void MaintenanceSynchronizer::updateTask(const QList<DataRow> &dataRows){
    if(!dataRows.isEmpty()) {
        for(int i = 0; i < dataRows.size(); i++) {
            DataRow dataRow = dataRows.at(i);
            const QString &taskDataKey = dataRow[TASK_DATA_KEY];
            /**
             * 记录任务的时间
             */
            const QString &taskTime = dataRow[SERVER_DATE];
            QVariantMap valueMap;
            valueMap["TaskType"] = m_nSyncTaskInfoMap[taskDataKey].toMap()["TaskType"];
            valueMap["TaskSyncTime"] = taskTime;
            m_nSyncTaskInfoMap.insert(taskDataKey, valueMap);
            dataRow[SERVER_DATE] = taskTime;

            /**
             * PlanID字段不为空时，标识是未来任务
             */
            dataRow[PLAN_ID] = "";
            dataRow[ISPRETASK] = "0";
            /**
             * 此处不要更新LastTime时间字段,设备同步完成后再更新
             */
            dataRow[SYNC_STATUS] = TASK_STATUS_SYNC_ING;
            QStringList columns;
            columns.append(TASK_DATA_KEY);
            QString selectArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            QList<DataRow> list = m_nDbHelper->select(MAINTENANCE_TABLE_TASK, columns, selectArg);
            if (!list.isEmpty()) {
                m_nDbHelper->update(MAINTENANCE_TABLE_TASK, dataRow, selectArg);
            } else {
                dataRow[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
                m_nDbHelper->insert(MAINTENANCE_TABLE_TASK, dataRow);
            }
        }
    }
}

void MaintenanceSynchronizer::updateGroup(const QList<DataRow> &dataRows){
    if(!dataRows.isEmpty()){
        for(int i = 0; i < dataRows.size(); i++) {
            DataRow dataRow = dataRows.at(i);
            const QString &taskDataKey = dataRow[TASK_DATA_KEY];
            const QString &taskGroupDataKey = dataRow[TASK_GROUP_DATA_KEY];
            QString selectArg = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDataKey + "' and "
                                + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            QStringList column;
            column.append(TASK_GROUP_DATA_KEY);
            const QString &taskType = m_nSyncTaskInfoMap[taskDataKey].toMap()["TaskType"].toString();
            if(taskType == QString::number(SYNC_TASK_ADD)) {
                QList<DataRow> list = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_GROUP, column, selectArg);
                if(!list.isEmpty()){
                    m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, dataRow, selectArg);
                }else{
                    dataRow[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
                    m_nDbHelper->insert(MAINTENANCE_TABLE_TASK_GROUP, dataRow);
                }
            }else if(taskType == QString::number(SYNC_TASK_UPDATE)){
                dataRow[LOCAL_STATUS] = TASK_STATUS_FINISH;
                m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, dataRow, selectArg);
                checkGroup(taskGroupDataKey);
                checkTask(taskDataKey, m_nSyncUser);
            }
        }
    }
}

void MaintenanceSynchronizer::updateItem(const QList<DataRow> &dataRows, QMap<QString, QString> &deviceInfo){
    if(!dataRows.isEmpty()){
        for(int i = 0; i < dataRows.size(); i++) {
            DataRow dataRow = dataRows.at(i);
            const QString &taskDataKey = dataRow[TASK_DATA_KEY];
            const QString &taskGroupDataKey = dataRow[TASK_GROUP_DATA_KEY];
            const QString &taskObjectDataKey = dataRow[TASK_OBJECT_DATA_KEY];
            QString selectArg = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "' and "
                                + QString(TASK_GROUP_DATA_KEY) + "='" + taskDataKey + "' and "
                                + QString(TASK_DATA_KEY) + "='" + taskGroupDataKey + "'";
            QStringList column;
            column.append(TASK_OBJECT_DATA_KEY);
            const QString &taskType = m_nSyncTaskInfoMap[taskDataKey].toMap()["TaskType"].toString();
            if(taskType == QString::number(SYNC_TASK_ADD)){
                QList<DataRow> list = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_OBJECT, column, selectArg);
                if(!list.isEmpty()){
                    m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, dataRow, selectArg);
                }else{
                    dataRow[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
                    m_nDbHelper->insert(MAINTENANCE_TABLE_TASK_OBJECT, dataRow);
                }
                deviceInfo.insert(dataRow["RealObjectID"], dataRow["RealObjectModifiedDate"]);
            }else if(taskType == QString::number(SYNC_TASK_UPDATE)){
                dataRow[LOCAL_STATUS] = TASK_STATUS_FINISH;
                m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, dataRow, selectArg);
                checkGroup(taskGroupDataKey);
                checkTask(taskDataKey, m_nSyncUser);
            }
        }
    }
}

void MaintenanceSynchronizer::resultProcess(const QString &taskDataKey, const bool &result){
    qDebug() <<Q_FUNC_INFO << "  taskDataKey " << taskDataKey << ", result "<< result;
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

void MaintenanceSynchronizer::updateUserTaskTime(const QString &userID, const QString &taskDataKey, const QString &time){
    qDebug() <<Q_FUNC_INFO << "  taskDataKey " << taskDataKey << ", time "<< time;
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(TASK_DATA_KEY, taskDataKey);
    dataRow.insert(LAST_TIME, time);
    QStringList columns;
    columns.append(USER_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "' and " +
                        QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
    QList<DataRow> list = m_nDbHelper->select(MAINTENANCE_TABLE_USER_TASK_TIME, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(MAINTENANCE_TABLE_USER_TASK_TIME, dataRow, selection);
    }else{
        m_nDbHelper->insert(MAINTENANCE_TABLE_USER_TASK_TIME, dataRow);
    }
}

void MaintenanceSynchronizer::updateUserTime(const QString &userID, const QString &time){
    qDebug() <<Q_FUNC_INFO << "  time " << time;
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(LAST_TIME, time);
    QStringList columns;
    columns.append(USER_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> list = m_nDbHelper->select(MAINTENANCE_TABLE_USER_TIME, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(MAINTENANCE_TABLE_USER_TIME, dataRow, selection);
    }else{
        m_nDbHelper->insert(MAINTENANCE_TABLE_USER_TIME, dataRow);
    }
}

void MaintenanceSynchronizer::updateTaskStatus(const QString &taskDataKey, const QString &syncStatus){
    qDebug() <<Q_FUNC_INFO << "  taskDataKey " << taskDataKey;
    DataRow dataRow;
    dataRow.insert(SYNC_STATUS, syncStatus);
    QString updateArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
    m_nDbHelper->update(MAINTENANCE_TABLE_TASK, dataRow, updateArg);
}

void MaintenanceSynchronizer::checkTask(const QString &taskDataKey, const QString &userID){
    qDebug() <<Q_FUNC_INFO << "  taskDataKey " << taskDataKey;
    QStringList columns;
    columns.append(TASK_DATA_KEY);
    QString whereClause = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "' and " +
                          QString(LOCAL_STATUS) + "!='" + TASK_STATUS_FINISH + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_GROUP, columns, whereClause);
    if(dataRows.isEmpty()){
        deleteTask(taskDataKey, userID);
    }
}

void MaintenanceSynchronizer::checkGroup(const QString &taskGroupDataKey){
    qDebug() <<Q_FUNC_INFO << "  taskGroupDataKey " << taskGroupDataKey;
    QStringList columns;
    columns.append(TASK_GROUP_DATA_KEY);
    QString whereClause = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDataKey + "' and " +
                          QString(LOCAL_STATUS) + "!='" + TASK_STATUS_FINISH + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_OBJECT, columns, whereClause);
    if(dataRows.isEmpty()){
        updateGroupToComplete(taskGroupDataKey);
    }
}

void MaintenanceSynchronizer::deleteTask(const QString &taskDataKey, const QString &userID){
    qDebug() <<Q_FUNC_INFO << "  taskDataKey " << taskDataKey;
    QString deleteArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK, deleteArg);
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK_GROUP, deleteArg);
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK_OBJECT, deleteArg);
    deleteArg += " and " + QString(USER_ID) + "='" + userID + "'";
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_USER_TASK_TIME, deleteArg);
}

void MaintenanceSynchronizer::updateGroupToComplete(const QString &taskGroupDataKey){
    qDebug() <<Q_FUNC_INFO << "  taskGroupDataKey " << taskGroupDataKey;
    DataRow dataRow;
    dataRow.insert(LOCAL_STATUS, TASK_STATUS_FINISH);
    QString updateArg = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDataKey + "'";
    m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, dataRow, updateArg);
    m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, dataRow, updateArg);
}

void MaintenanceSynchronizer::handlePreTaskPlanIDs(const DataPackage &dataPackage) {
    const QList<DataRow> &dataRows = dataPackage.tables[0].value;
    if(!dataRows.isEmpty()){
        for(int i = 0; i < dataRows.size(); i++){
            const DataRow &dataRow = dataRows.at(i);
            QString planID = dataRow["PlanID"];
            DataPackage dataPackage;
            dataPackage.head.type = TYPE_REQUEST;
            dataPackage.head.key = PACKAGE_HEAD_KEY;
            dataPackage.head.objects = EMM_Off_GetPrePacketByPlanID;
            dataPackage.para["PlanID"] = planID;
            dataPackage.para["PreBuildEndTimeStr"] = dataRow["PreBuildEndTimeStr"];
            m_nSyncPreTaskPlanIDs.append(planID);
            sendData(dataPackage, planID);
        }
    }else{
        DbusPackage map;
        map.dataPackage.head.objects = EMM_Off_GetPrePlanIDsByUserID;
        emit onResultOfResponse(map.dataPackage.head.objects, m_nSyncPreTaskAction, true, 0, map);
    }
}

void MaintenanceSynchronizer::handlePreTasks(const DataPackage &dataPackage, const QString &planID){
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
        list =  m_nDbHelper->select(MAINTENANCE_TABLE_TASK, columns, selectArg);
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
            QMap<QString, QString> deviceInfo;
            const DataRow &taskRow = dataRows0[0];
            const QList<DataRow> &groupRows = tables[1].value;
            const QList<DataRow> &objectRows = tables[2].value;
            const QList<DataRow> &mappingRows = tables[3].value;
            QStringList taskDataKes;
            QStringList groupDataKeys;
            m_nDbHelper->beginTransaction();
            for (int i = 0; i < mappingRows.size(); i++) {
                const DataRow &mappingRow = mappingRows.at(i);
                QString taskDataKey = mappingRow["TaskDataKey"];
                QString taskGroupDataKey = mappingRow["TaskGroupDataKey"];
                QString taskObjectDataKey = mappingRow["TaskObjectDataKey"];
                QString taskGroupDataKeyFlag = mappingRow["TaskGroupDataKeyFlag"];
                QString taskObjectDataKeyFlag = mappingRow["TaskObjectDataKeyFlag"];

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
                    taskUpdateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
                    taskUpdateRow["SyncStatus"] = TASK_STATUS_SYNC_ING;
                    taskUpdateRow["IsPreTask"] = "1";
                    taskDataKes.append(taskDataKey);
                    columns.clear();
                    selectArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
                    list = m_nDbHelper->select(MAINTENANCE_TABLE_TASK, columns, selectArg);
                    if(!list.isEmpty()){
                        m_nDbHelper->update(MAINTENANCE_TABLE_TASK, taskUpdateRow, selectArg);
                    }else{
                        m_nDbHelper->insert(MAINTENANCE_TABLE_TASK, taskUpdateRow);
                    }
                }

                /**
                * Step 2. 更新组信息
                */
                if (!groupDataKeys.contains(taskGroupDataKey)) {
                    for (int j = 0; j < groupRows.size(); j++) {
                        const DataRow &groupRow = groupRows[j];
                        QString taskGroupDataKeyFlag0 = groupRow["TaskGroupDataKeyFlag"];
                        if (taskGroupDataKeyFlag == taskGroupDataKeyFlag0) {
                            DataRow groupUpdateRow = groupRow;
                            groupUpdateRow["TaskGroupDataKey"] = taskGroupDataKey;
                            groupUpdateRow["TaskDataKey"] = taskDataKey;
                            groupUpdateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
                            groupUpdateRow.remove("TaskGroupDataKeyFlag");
                            groupDataKeys.append(taskGroupDataKey);
                            selectArg = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDataKey + "'";
                            list = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_GROUP, columns, selectArg);
                            if(!list.isEmpty()){
                                m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, groupUpdateRow, selectArg);
                            }else{
                                m_nDbHelper->insert(MAINTENANCE_TABLE_TASK_GROUP, groupUpdateRow);
                            }
                            break;
                        }
                    }
                }

                /**
                * Step 3. 更新对象信息
                */
                for (int k = 0; k < objectRows.size(); k++) {
                    const DataRow &objectRow = objectRows[k];
                    QString taskObjectDataKeyFlag0 = objectRow["TaskObjectDataKeyFlag"];
                    if (taskObjectDataKeyFlag == taskObjectDataKeyFlag0) {
                        DataRow objectUpdateRow = objectRow;
                        objectUpdateRow.remove("TaskObjectDataKeyFlag");
                        objectUpdateRow.remove("TaskGroupDataKeyFlag");
                        objectUpdateRow.remove("InspectStatus");
                        objectUpdateRow["TaskDataKey"] = taskDataKey;
                        objectUpdateRow["TaskGroupDataKey"] = taskGroupDataKey;
                        objectUpdateRow["TaskObjectDataKey"] = taskObjectDataKey;
                        objectUpdateRow["LocalStatus"] = TASK_STATUS_NO_FINISH;
                        columns.clear();
                        selectArg = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
                        list = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_OBJECT, columns, selectArg);
                        if(!list.isEmpty()){
                            m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, objectUpdateRow, selectArg);
                        }else{
                            m_nDbHelper->insert(MAINTENANCE_TABLE_TASK_OBJECT, objectUpdateRow);
                        }
                        deviceInfo.insert(objectRow["RealObjectID"], objectRow["RealObjectModifiedDate"]);
                        break;
                    }
                }
            }
            m_nDbHelper->endTransaction();
            /**
            * Step 4. 同步设备信息
            */
            QString action = SYNC_PRETASK + planID;
            bool result = requestObjs(deviceInfo, action);
            if(!result){
                preTaskResultProcess(planID, true);
            }
        }else{
            preTaskResultProcess(planID, true);
        }
    }else{
        preTaskResultProcess(planID, true);
    }
    qint64 time2 = QDateTime::currentMSecsSinceEpoch();
    qDebug() << Q_FUNC_INFO << " time2-time1 : " << time2 - time1;
}

void MaintenanceSynchronizer::preTaskResultProcess(const QString &planID, const bool &result){
    qDebug() << Q_FUNC_INFO << "  planID " << planID << "  result " << result;
    /**
    * Step 1. 清除数据
    */
    m_nSyncPreTaskPlanIDs.removeOne(planID);

    /**
    * Step 2. 成功更新任务状态,失败计数
    */
    if(result){
        updateTaskStatusByPlanID(planID, TASK_STATUS_SYNC_FINISH);
    }else{
        m_nSyncPreTaskFailCount++;
        updateTaskStatusByPlanID(planID, TASK_STATUS_SYNC_FAIL);
    }
    /**
    * Step 3. 更新用户业务表的获取的时间戳
    */
    if(m_nSyncPreTaskPlanIDs.isEmpty()) {
        DbusPackage map;
        map.dataPackage.head.objects = EMM_Off_GetPrePlanIDsByUserID;
        if(m_nSyncPreTaskFailCount == 0){
            emit onResultOfResponse(map.dataPackage.head.objects, m_nSyncPreTaskAction, true, 0, map);
        }else{
            emit onResultOfResponse(map.dataPackage.head.objects, m_nSyncPreTaskAction, false, -1, map);
        }
    }
}

void MaintenanceSynchronizer::deleteTask(const QStringList &taskDataKeyList){
    QString taskDataKeys = ConvertData::getInstance()->convertForSql(taskDataKeyList);
    if(taskDataKeys != "") {
        /**
        * Step 1. 删除任务信息
        */
        m_nDbHelper->beginTransaction();
        QString sql0 = "delete from " + QString(MAINTENANCE_TABLE_TASK) + " where " +
                       QString(TASK_DATA_KEY) + " in (" + taskDataKeys + ")";
        m_nDbHelper->execSQL(sql0);

        /**
        * Step 2. 删除组信息
        */
        QString sql1 = "delete from " + QString(MAINTENANCE_TABLE_TASK_GROUP) + " where " +
                       QString(TASK_DATA_KEY) + " in (" + taskDataKeys + ")";
        m_nDbHelper->execSQL(sql1);

        /**
        * Step 3. 删除对象信息
        */
        QString sql2 = "delete from " + QString(MAINTENANCE_TABLE_TASK_OBJECT) + " where " +
                       QString(TASK_DATA_KEY) + " in (" + taskDataKeys + ")";
        m_nDbHelper->execSQL(sql2);

        m_nDbHelper->endTransaction();
    }
}

void MaintenanceSynchronizer::updateTaskStatusByPlanID(const QString &planID, const QString &syncStatus){
    qDebug() <<Q_FUNC_INFO << "  planID " << planID;
    DataRow dataRow;
    dataRow.insert(SYNC_STATUS, syncStatus);
    QString updateArg = " where " + QString(PLAN_ID) + "='" + planID + "'";
    m_nDbHelper->update(MAINTENANCE_TABLE_TASK, dataRow, updateArg);
}


