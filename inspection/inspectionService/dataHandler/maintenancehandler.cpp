#include "maintenancehandler.h"
#include "../common/comparetool.h"
#include "../framework/common/convertdata.h"
#include "../common/runtimeparam.h"
#include "../framework/common/systemconfig.h"
MaintenanceHandler::MaintenanceHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_MAINTENANCE;
}

MaintenanceHandler::~MaintenanceHandler()
{
}

void MaintenanceHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    QString objects = dataPackage.head.objects;
    if(objects == EMM_GetUserRunningTask){
        requestTasksPatch(dataPackage.head, action);
    } else {
        sendData(dataPackage, action);
    }
}

void MaintenanceHandler::offLine(const QString &action, const DataPackage &dataPackage){
    QString objects = dataPackage.head.objects;
    if(objects == EMM_Off_GetUserRunningTaskDataKeys ||
            objects == EMM_GetUserRunningTask){
        requestLocalTaskList(dataPackage, action);
    }else if(objects == EMM_GetTaskGroupList){
        requestLocalGroupList(dataPackage, action);
    }else if(objects == EMM_GetTaskObjectListByGroup){
        requestLocalGroupObjectList(dataPackage, action);
    }else if(objects == EMM_GetTaskObjectListByTask){
        requestLocalTaskObjectList(dataPackage, action);
    }else if(objects == EMM_GetTaskObjectDetail){
        requestLocalTaskObjectDetail(dataPackage, action);
    }
    m_nRequestPkgMap.remove(action);
}

int MaintenanceHandler::getItemCount()
{
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    QStringList column;
    column.append(LAST_TIME);
    QString selectArg = " where " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> list = m_nDbHelper->select(MAINTENANCE_TABLE_USER_TIME, column, selectArg);
    if (list.isEmpty()) {
        return -1;
    }
    QString sql = "select distinct t.TaskDataKey"
            " from EMM_Maintenance_Task t"
            " inner join EMM_Maintenance_Task_Group tg on tg.TaskDataKey=t.TaskDataKey"
            " where t.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
            " and tg.TotalObjectCount > tg.ExamObjectCount"
            " and datetime(TaskStartTime) < datetime('now','localtime') and  datetime(TaskEndTime) > datetime('now','localtime')"
            " and tg.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
            " group by t.TaskDataKey";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result && !dataRows.isEmpty()){
        return dataRows.size();
    }
    return 0;
}

void MaintenanceHandler::cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID)
{
    /**
    *  Step 1. 删除任务-用户-时间表
    */
    QString selectArg = " where " + QString(USER_ID) + userID + "'";
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_USER_TASK_TIME, selectArg);

    /**
    * Step 2. 删除业务-用户-时间表
    */
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_USER_TIME, selectArg);

    /**
    * Step 3. 删除对象
    */
    QStringList postCodes = getPostCodesByUser(userID);
    if(!postCodes.isEmpty()){
        for (int i = 0; i < postCodes.size(); i++) {
            QString postCode = postCodes.at(i);
            QStringList userIDs = getUsersByPostCode(postCode);
            if(userIDs.size() == 1){
                selectArg = "where PostCode='" + postCode + "'";
                m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK_GROUP, selectArg);
            }
        }
    }

    /**
    * Step 4. 删除任务项
    */
    selectArg = " where EMM_Maintenance_Task_Object.TaskGroupDataKey not in (select TaskGroupDataKey from EMM_Maintenance_Task_Group)";
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK_OBJECT, selectArg);

    /**
    * Step 5. 删除任务
    */
    QStringList column;
    QList<DataRow> list = m_nDbHelper->select(MAINTENANCE_TABLE_TASK, column, "");
    if(!list.isEmpty()){
        for(int i = 0 ; i < list.size(); i++){
            QString taskDataKey = list.at(i)[TASK_DATA_KEY];
            selectArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            QList<DataRow> list1 = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_GROUP, column, selectArg);
            if(list1.isEmpty()) {
                m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK, selectArg);
            }
        }
    }
    Q_UNUSED(organiseUnitID)
    Q_UNUSED(departmentID)
}

void MaintenanceHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    DbusPackage dbusPackage;
    const DataPackage &requestPackage = m_nRequestPkgMap[action];
    getLocalTasks(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    m_nRequestPkgMap.remove(action);
}

void MaintenanceHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        if(objects == EMM_Off_GetUserRunningTaskDataKeys) {
            /**
            *  handle tasks patch
            */
            handleTasksPatch(objects, action, map.dataPackage);
        }else if(objects == EMM_GetUserRunningTask) {
            /**
            *  handle tasks list response
            */
            handleTaskListResponse(action, map);
        }else if(objects == EMM_GetTaskGroupList) {
            /**
            * warning:
            * In theory, this circumstance wouldn't happen
            * So ignore it.
            */
            handleTaskGroupResponse(action, map.dataPackage);
        }else if(objects == EMM_GetTaskObjectListByGroup) {
            /**
            * handle objectList callback
            */
            handleGroupObjectResponse(action, map.dataPackage);
        }else if(objects == EMM_GetTaskObjectListByTask) {
            /**
            * handle objectList callback
            */
            handleTaskObjectResponse(action, map.dataPackage);
        }else if(objects == EMM_GetTaskObjectDetail) {
            /**
            * handle object para
            */
            handleTaskObjectDetailResponse(action, map);
        }
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}

void MaintenanceHandler::requestTasksPatch(const DataHead &head, const QString &action) {
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    QString lastDEXTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QString selectionArg = " where "+ QString(USER_ID) + "='" + userID + "'";
    QStringList column;
    QList<DataRow > dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_USER_TIME, column, selectionArg);
    if (dataRows.count() > 0) {
        lastDEXTime = dataRows.at(0).value(LAST_TIME);
    }
    DataPackage data;
    data.head = head;
    data.head.objects = EMM_Off_GetUserRunningTaskDataKeys;
    data.para.insert(USER_ID, userID);
    data.para.insert(LAST_DEX_TIME, lastDEXTime);
    sendData(data, action);
}

void MaintenanceHandler::handleTasksPatch(const QString &objects, const QString &action, DataPackage &dataPackage) {

    DataPackage requestPackage = m_nRequestPkgMap[action];
    QString userID = requestPackage.para["UserID"];
    /**
    * step 1 : parse data
    */
    CompareTool compareTool;
    compareTool.compare(dataPackage, "TaskDataKey");

    bool needSync = !compareTool.getAddList().isEmpty() || !compareTool.getUpdateList().isEmpty();

    /**
    * step 2 : delete removed task & task sync info
    */
    if (!compareTool.getDeleteList().isEmpty()) {
        for (int i = 0; i < compareTool.getDeleteList().size(); i++) {
            QString taskDataKey = compareTool.getDeleteList().at(i);
            deleteTask(taskDataKey, userID);
        }
    }

    /**
    * step 3 : set modified record 's SyncStatus to synchronizing
    */
    if (!compareTool.getUpdateList().isEmpty()) {
        for (int i = 0; i < compareTool.getUpdateList().size(); i++) {
            QString taskDataKey = compareTool.getUpdateList().at(i);
            QString updateArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            DataRow dataRow;
            dataRow.insert(SYNC_STATUS, TASK_STATUS_SYNC_ING);
            m_nDbHelper->update(MAINTENANCE_TABLE_TASK, dataRow, updateArg);
        }
    }

    /**
    * step 4 :delete invalid pre task
    */
    deletePreTask(compareTool.getAddList(), userID);

    /**
    * step 5 : send response to application,  send added task to synchronizer
    */
    if (needSync) {
        DbusPackage dbusPackage;
        getLocalTasks(requestPackage, dbusPackage.dataPackage);
        QList<DataRow> &lvs = dbusPackage.dataPackage.tables[0].value;
        QList<DataRow> &nvs = dataPackage.tables[0].value;
        for (int i = 0; i < compareTool.getAddList().size(); i++) {
            QString taskDataKey = compareTool.getAddList().at(i);
            bool isInLocal = false;
            for (int j = 0; j < lvs.size(); j++) {
                DataRow &lv = lvs[j];
                if (lv["TaskDataKey"] == taskDataKey) {
                    isInLocal = true;
                    break;
                }
            }
            if (isInLocal) {
                continue;
            }
            for (int j = 0; j < nvs.size(); j++) {
                DataRow &nv = nvs[j];
                if (nv["TaskDataKey"] == taskDataKey) {
                    nv["SyncStatus"] = TASK_STATUS_SYNC_ING;
                    nv["Rate"] = "";
                    nv["Circle"] = "";
                    nv["ModelType"] = "0";
                    lvs.append(nv);
                    break;
                }
            }
        }
        emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, dbusPackage);
        QVariantMap syncMap;
        syncMap["UserID"] = userID;
        syncMap["addIDs"] = QVariant(compareTool.getAddList());
        syncMap["updateIDs"] = QVariant(compareTool.getUpdateList());
        emit onBeginSyncSignal(MESSAGE_SYNC_ADDTION, action, requestPackage.head.objects, syncMap);
    } else {
        QStringList taskDataKeys;
        deletePreTask(taskDataKeys, userID);
        offLine(action, requestPackage);
    }
}

void MaintenanceHandler::deletePreTask(const QStringList &taskDataKeys, const QString &userID){
    QString sql = "select t.TaskDataKey from emm_maintenance_task  t"
                    " where datetime(TaskStartTime) < datetime('now','localtime')"
                    " and datetime(TaskEndTime) > datetime ('now','localtime')"
                    " and IsPreTask = '1'";
    QList<DataRow> dataRows;
    m_nDbHelper->select(sql, dataRows);
    if(!taskDataKeys.isEmpty() && !dataRows.isEmpty()){
        for (int i = 0; i < taskDataKeys.size(); i++) {
            QString taskDataKey = taskDataKeys.at(i);
            for(int i = 0; i < dataRows.size(); i++) {
                const DataRow dataRow = dataRows.at(i);
                if(taskDataKey == dataRow.value(TASK_DATA_KEY)){
                    dataRows.removeOne(dataRow);
                    break;
                }
            }
        }
    }
    if(!dataRows.isEmpty()) {
        for (int i = 0; i < dataRows.size(); i++) {
            DataRow &dataRow = dataRows[i];
            QString taskDataKey = dataRow[TASK_DATA_KEY];
            deleteTask(taskDataKey, userID);
        }
    }
}

void MaintenanceHandler::getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage) {
    /**
    * 查询task层的sql语句
    */
    QString userID = requestPackage.para.value("UserID");
    QString sql = "select distinct "
            "t.TaskDataKey,"
            "t.TaskCode,"
            "t.TaskName,"
            "t.LimitMode,"
            "t.EnableUploadAttach,"
            "t.ModelType,"
            "t.TaskStartTime,"
            "t.TaskEndTime,"
            "t.TaskStartTime|| \"至\"||  t.TaskEndTime Circle,"
            "sum(tg.ExamObjectCount)|| \"/\" || sum(tg.TotalObjectCount) Rate,"
            "t.SyncStatus "
            " from EMM_Maintenance_Task t"
            " inner join EMM_Maintenance_Task_Group tg on tg.TaskDataKey=t.TaskDataKey"
            " where t.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
            " and tg.TotalObjectCount > tg.ExamObjectCount"
            " and datetime(TaskStartTime) < datetime('now','localtime') and  datetime(TaskEndTime) > datetime('now','localtime')"
            " and tg.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
            " group by t.TaskDataKey order by t.TaskStartTime desc";
    /**
    * 查询结果
    */
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EMM_GetUserRunningTask;
        DataTable table;
        table.name = MAINTENANCE_TABLE_TASK;
        table.key = TASK_DATA_KEY;
        for (int i = 0; i < dataRows.size(); i++) {
            DataRow &dataRow = dataRows[i];
            table.value.append(dataRow);
        }
        resultPackage.tables.append(table);
        resultPackage.summary.code = 100;
        resultPackage.summary.result = 1;
        resultPackage.summary.description = "请求数据成功完成";
    }
}

void MaintenanceHandler::requestLocalTaskList(const DataPackage &requestPackage, const QString &action) {
    DbusPackage dbusPackage;
    getLocalTasks(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void MaintenanceHandler::handleTaskListResponse(const QString &action, const DbusPackage &map) {
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, map);
    m_nRequestPkgMap.remove(action);
}

void MaintenanceHandler::handleTaskGroupResponse(const QString &action, const DataPackage &dataPackage) {

    /**
    * Step 1 : get local task groups
    */
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    QString taskDataKey = requestPackage.para["TaskDataKey"];
    QString userID = requestPackage.para["UserID"];
    /**
    * 查询group层的sql语句
    */
    QString sql = "select distinct "
            "tg.TaskGroupDataKey"
            " from EMM_Maintenance_Task_Group tg"
            " where tg.TaskDataKey='" + taskDataKey + "'"
            " and tg.TotalObjectCount > tg.ExamObjectCount"
            " and tg.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
            " and tg.TaskGroupDataKey in(select TaskGroupDataKey from EMM_Maintenance_Task_Object where TaskGroupDataKey=tg.TaskGroupDataKey)"
            " group by tg.TaskGroupDataKey";

    QList<DataRow> lvs;
    m_nDbHelper->select(sql, lvs);
    const QList<DataRow> &nvs = dataPackage.tables[0].value;

    /**
    * Step 2 : compare groups
    */
    CompareTool compareTool;
    QStringList deleteList = compareTool.compare(lvs, nvs, "TaskGroupDataKey");

    /**
    * Step 3 : update all finished groups and their objects
    */
    updateGroupToComplete(deleteList);

    /**
    * Step 4 : update exam item count(object has been finished)
    */
    updateExamObjectCount(dataPackage, deleteList);

    /**
    * Step 5 : check task
    */
    checkTask(taskDataKey, userID);

    /**
    * Final step : send data to upper application
    */
    offLine(action, requestPackage);
}

void MaintenanceHandler::updateGroupToComplete(const QStringList &groupDataKeyList) {
    qDebug() <<Q_FUNC_INFO << "  groupDataKeyList " << groupDataKeyList;
    QString groupDataKeys = ConvertData::getInstance()->convertForSql(groupDataKeyList);
    if (!groupDataKeys.isEmpty()) {
        DataRow dataRow;
        dataRow.insert(LOCAL_STATUS, TASK_STATUS_FINISH);
        QString updateArg = " where " + QString(TASK_GROUP_DATA_KEY) + " in (" + groupDataKeys + ")";
        m_nDbHelper->beginTransaction();
        m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, dataRow, updateArg);
        m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, dataRow, updateArg);
        m_nDbHelper->endTransaction();
    }
}

void MaintenanceHandler::updateExamObjectCount(const DataPackage &dataPackage, const QStringList &deleteList) {
    qDebug() <<Q_FUNC_INFO << "  deleteList " << deleteList;
    if (dataPackage.tables.isEmpty())
        return;
    const QList<DataRow> &dataRows = dataPackage.tables[0].value;
    for(int i = 0; i < dataRows.size(); i++){
        const DataRow &dataRow = dataRows.at(i);
        QString taskGroupDateKey = dataRow[TASK_GROUP_DATA_KEY];
        if (deleteList.contains(taskGroupDateKey))
            continue;
        QString itemCount = dataRow[EXAM_OBJECT_COUNT];
        QString updateArg = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDateKey + "'";
        DataRow updateRow;
        updateRow[EXAM_OBJECT_COUNT] =  itemCount;
        m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, updateRow, updateArg);
    }
}

void MaintenanceHandler::requestLocalGroupList(const DataPackage &requestPackage, const QString &action) {
    DbusPackage dbusPackage;
    getLocalTaskGroup(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void MaintenanceHandler::getLocalTaskGroup(const DataPackage &requestPackage, DataPackage &resultPackage) {
    /**
    * 查询结果
    */
    QString taskDataKey = requestPackage.para["TaskDataKey"];
    QString userID = requestPackage.para["UserID"];
    /**
    * 查询group层的sql语句
    */
    QString sql = "select distinct "
            "tg.TaskGroupDataKey,"
            "tg.TaskDataKey,"
            "tg.TaskGroupName,"
            "tg.TotalObjectCount,"
            "tg.ExamObjectCount,"
            "tg.OrderIndex"
            " from EMM_Maintenance_Task_Group tg"
            " where tg.TaskDataKey='" + taskDataKey + "'"
            " and tg.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
            " and tg.TotalObjectCount > tg.ExamObjectCount"
            " and tg.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
            " and tg.TaskGroupDataKey in(select TaskGroupDataKey from EMM_Maintenance_Task_Object where TaskGroupDataKey=tg.TaskGroupDataKey)";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EMM_GetTaskGroupList;
        DataTable table;
        table.name = MAINTENANCE_TABLE_TASK_GROUP;
        table.key = TASK_GROUP_DATA_KEY;
        for (int i = 0; i < dataRows.size(); i++) {
            DataRow &dataRow = dataRows[i];
            table.value.append(dataRow);
        }
        resultPackage.tables.append(table);
        resultPackage.summary.code = 100;
        resultPackage.summary.result = 1;
        resultPackage.summary.description = "请求数据成功完成";
    }
}

void MaintenanceHandler::handleGroupObjectResponse(const QString &action, const DataPackage &dataPackage) {

    /**
     * Step 1 : get local group objects
     */
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    QString taskGroupDataKey = requestPackage.para["TaskGroupDataKey"];
    /**
    * 查询group子项的sql语句
    */
    QString sql = "select distinct "
            "tob.TaskObjectDataKey,"
            "tob.TaskDataKey"
            " from EMM_Maintenance_Task_Object tob"
            " where tob.TaskGroupDataKey='" + taskGroupDataKey + "'"
            " and tob.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'";
    QList<DataRow> lvs;
    m_nDbHelper->select(sql, lvs);
    QString taskDataKey = "";
    if(!lvs.isEmpty()){
        taskDataKey = lvs.at(0)["TaskDataKey"];
    }
    const QList<DataRow> &nvs = dataPackage.tables[0].value;

    /**
    * Step 2 : compare items
    */
    CompareTool ompareTool;
    QStringList deleteList = ompareTool.compare(lvs, nvs, "TaskObjectDataKey");

    /**
    * Step 3 : update all deleted objects
    */
    updateObjectToComplete(deleteList);

    /**
    * Step 4 : update item count (item has been finished)
    */
    updateExamObjectCount(deleteList.size(), taskGroupDataKey);

    /**
    * Step 5 : check group
    */
    checkGroup(taskGroupDataKey);

    /**
    * Step 6 : check task
    */
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    checkTask(taskDataKey, userID);

    /**
    * Step 7 : send data to upper application
    */
    offLine(action, requestPackage);

    /**
    * Step 8. sync devices
    */
    QMap<QString, QString> deviceInfo;
    for(int i = 0; i < nvs.size(); i++){
        const DataRow& nv = nvs.at(i);
        deviceInfo.insert(nv["RealObjectID"], nv["RealObjectModifiedDate"]);
    }
    qDebug() << Q_FUNC_INFO << "deviceInfo   is " << deviceInfo.size();
    requestObjs(deviceInfo, action);
}

void MaintenanceHandler::updateObjectToComplete(const QStringList &objectDataKeyList) {
    qDebug() << Q_FUNC_INFO << "  objectDataKeyList " << objectDataKeyList;
    QString objectDataKeys = ConvertData::getInstance()->convertForSql(objectDataKeyList);
    if (!objectDataKeys.isEmpty()) {
        DataRow dataRow;
        dataRow.insert(LOCAL_STATUS, TASK_STATUS_FINISH);
        QString updateArg = " where " + QString(TASK_OBJECT_DATA_KEY) + " in (" + objectDataKeys + ")";
        m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, dataRow, updateArg);
    }
}

void MaintenanceHandler::updateExamObjectCount(const int &count, const QString &taskGroupDataKey) {
    qDebug() << Q_FUNC_INFO << "  taskGroupDataKey " << taskGroupDataKey << ", count "<< count;
    if(count > 0 ){
        QStringList columns;
        columns.append(EXAM_OBJECT_COUNT);
        QString whereArgs = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDataKey + "'";
        QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_GROUP, columns, whereArgs);
        if (!dataRows.isEmpty()) {
            DataRow updateRow;
            int oldExamCount = dataRows.at(0)[EXAM_OBJECT_COUNT].toInt();
            int newExamCount = oldExamCount + count;
            updateRow[EXAM_OBJECT_COUNT] = QString::number(newExamCount);
            m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, updateRow, whereArgs);
        }
    }
}

void MaintenanceHandler::requestLocalGroupObjectList(const DataPackage &requestPackage, const QString &action)
{
    DbusPackage dbusPackage;
    getLocalGroupObject(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void MaintenanceHandler::getLocalGroupObject(DataPackage requestPackage, DataPackage &resultPackage)
{
    QString taskGroupDataKey = requestPackage.para["TaskGroupDataKey"];
    /**
    * 查询group子项的sql语句
    */
    QString sql = "select distinct "
            "tob.TaskObjectDataKey,"
            "tob.ObjectName,"
            "tob.ObjectFullName,"
            "tob.RealObjectType,"
            "tob.RealObjectID,"
            "tob.SerialKey,"
            "tob.TaskDataKey,"
            "task.TaskStartTime, "
            "task.IsPreTask, "
            "case when tob.RealObjectType=1 then eq.Positions else null end as Positions,"
            "case when tob.RealObjectType=1 then eq.ModelName else null end as ModelName,"
            "case when tob.RealObjectType=1 then eq.RFIDCode else eqp.RFIDCode end as RFIDCode,"
            "case when tob.RealObjectType=1 then eq.BarCode else eqp.BarCode end as BarCode,"
            "case when tob.RealObjectType=1 then eq.QRCode else eqp.QRCode end as QRCode"
            " from EMM_Maintenance_Task_Object tob , EMM_Maintenance_Task task"
            " left join EMB_Equipment eq on tob.RealObjectID=eq.ID and tob.RealObjectType=1"
            " left join EMB_Equipment_Part eqp on tob.RealObjectID=eqp.ID and tob.RealObjectType=2 "
            " where tob.TaskGroupDataKey='" + taskGroupDataKey + "' and tob.TaskDataKey = task.TaskDataKey"
            " and tob.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
            " and ((eq.ID is not null and tob.RealObjectType=1) or (eqp.ID is not null and tob.RealObjectType=2))"
            " group by tob.TaskObjectDataKey";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EMM_GetTaskObjectListByGroup;
        DataTable table;
        table.name = MAINTENANCE_TABLE_TASK_OBJECT;
        table.key = TASK_OBJECT_DATA_KEY;
        for (int i = 0; i < dataRows.size(); i++) {
        DataRow &dataRow = dataRows[i];
            table.value.append(dataRow);
        }
        resultPackage.tables.append(table);
        resultPackage.summary.code = 100;
        resultPackage.summary.result = 1;
        resultPackage.summary.description = "请求数据成功完成";
    }
}

void MaintenanceHandler::handleTaskObjectResponse(const QString &action, const DataPackage &dataPackage)
{
    /**
     * Step 1 : get local task objects
     */
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    QString taskDataKey = requestPackage.para["TaskDataKey"];
    QString userID = requestPackage.para["UserID"];

    /**
    * 查询task子项的sql语句
    */
    QString sql = "select distinct "
            "tob.TaskObjectDataKey,"
            "tob.ObjectName,"
            "tob.ObjectFullName,"
            "tob.SerialKey,"
            "tob.TaskDataKey,"
            "tob.TaskGroupDataKey,"
            "case when tob.RealObjectType=1 then eq.Positions else null end as Positions,"
            "case when tob.RealObjectType=1 then eq.ModelName else null end as ModelName,"
            "case when tob.RealObjectType=1 then eq.RFIDCode else eqp.RFIDCode end as RFIDCode,"
            "case when tob.RealObjectType=1 then eq.BarCode else eqp.BarCode end as BarCode,"
            "case when tob.RealObjectType=1 then eq.QRCode else eqp.QRCode end as QRCode"
            " from EMM_Maintenance_Task_Object tob"
            " inner join EMM_Maintenance_Task_Group tg on tg.TaskGroupDataKey=tob.TaskGroupDataKey"
            " left join EMB_Equipment eq on tob.RealObjectID=eq.ID and tob.RealObjectType=1"
            " left join EMB_Equipment_Part eqp on tob.RealObjectID=eqp.ID and tob.RealObjectType=2 "
            " where tg.TaskDataKey='" + taskDataKey + "'"
            " and tob.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
            " and tg.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
            " group by tob.TaskObjectDataKey";

    QList<DataRow> lvs;
    m_nDbHelper->select(sql, lvs);
    QString taskGroupDataKey = "";
    if(!lvs.isEmpty()){
        taskGroupDataKey = lvs.at(0)["TaskGroupDataKey"];
    }
    const QList<DataRow> &nvs = dataPackage.tables[0].value;
    /**
    * Step 2 : compare items
    */
    CompareTool ompareTool;
    QStringList deleteList = ompareTool.compare(lvs, nvs, "TaskObjectDataKey");

    /**
    * Step 3 : update all deleted objects
    */
    updateObjectToComplete(deleteList);

    /**
    * Step 4 : update item count (item has been finished)
    */
    updateExamObjectCount(deleteList.size(), taskGroupDataKey);

    /**
    * Step 5 : check group
    */
    checkGroup(taskGroupDataKey);

    /**
    * Step 6 : check task
    */
    checkTask(taskDataKey, userID);

    /**
    * Step 7 : send data to upper application
    */
    offLine(action, requestPackage);

    /**
    * Step 8. sync devices
    */
    QMap<QString, QString> deviceInfo;
    for(int i = 0; i < nvs.size(); i++){
        const DataRow& nv = nvs.at(i);
        deviceInfo.insert(nv["RealObjectID"], nv["RealObjectModifiedDate"]);
    }
    qDebug() << Q_FUNC_INFO << "deviceInfo   is " << deviceInfo.size();
    requestObjs(deviceInfo, action);
}

void MaintenanceHandler::requestLocalTaskObjectList(const DataPackage &requestPackage, const QString &action)
{
    DbusPackage dbusPackage;
    getLocalTaskObject(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void MaintenanceHandler::getLocalTaskObject(DataPackage requestPackage, DataPackage &resultPackage)
{
    QString userID = requestPackage.para["UserID"];
    QString taskDataKey = requestPackage.para["TaskDataKey"];
    /**
    * 查询task子项的sql语句
    */
    QString sql = "select distinct "
            "tob.TaskObjectDataKey,"
            "tob.ObjectName,"
            "tob.ObjectFullName,"
            "tob.RealObjectType,"
            "tob.RealObjectID,"
            "tob.SerialKey,"
            "tob.TaskDataKey,"
            "task.TaskStartTime, "
            "task.IsPreTask, "
            "case when tob.RealObjectType=1 then eq.Positions else null end as Positions,"
            "case when tob.RealObjectType=1 then eq.ModelName else null end as ModelName,"
            "case when tob.RealObjectType=1 then eq.RFIDCode else eqp.RFIDCode end as RFIDCode,"
            "case when tob.RealObjectType=1 then eq.BarCode else eqp.BarCode end as BarCode,"
            "case when tob.RealObjectType=1 then eq.QRCode else eqp.QRCode end as QRCode"
            " from EMM_Maintenance_Task_Object tob , EMM_Maintenance_Task task "
            "inner join EMM_Maintenance_Task_Group tg on tg.TaskGroupDataKey=tob.TaskGroupDataKey "
            "left join EMB_Equipment eq on tob.RealObjectID=eq.ID and tob.RealObjectType=1 "
            "left join EMB_Equipment_Part eqp on tob.RealObjectID=eqp.ID and tob.RealObjectType=2 "
            "where tg.TaskDataKey='" + taskDataKey + "' and task.TaskDataKey='" + taskDataKey + "' "
            "and tob.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "' "
            "and ((eq.ID is not null and tob.RealObjectType=1) or (eqp.ID is not null and tob.RealObjectType=2)) "
            "and tg.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "') "
            "group by tob.TaskObjectDataKey";

    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EMM_GetTaskObjectListByTask;
        DataTable table;
        table.name = MAINTENANCE_TABLE_TASK_OBJECT;
        table.key = TASK_OBJECT_DATA_KEY;
        for (int i = 0; i < dataRows.size(); i++) {
        DataRow &dataRow = dataRows[i];
            table.value.append(dataRow);
        }
        resultPackage.tables.append(table);
        resultPackage.summary.code = 100;
        resultPackage.summary.result = 1;
        resultPackage.summary.description = "请求数据成功完成";
    }
}

void MaintenanceHandler::handleTaskObjectDetailResponse(const QString &action, const DbusPackage &map)
{
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, map);
    m_nRequestPkgMap.remove(action);
}

void MaintenanceHandler::requestLocalTaskObjectDetail(const DataPackage &requestPackage, const QString &action)
{
    DbusPackage dbusPackage;
    getTaskObjectDetail(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void MaintenanceHandler::getTaskObjectDetail(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString taskObjectDataKey = requestPackage.para["TaskObjectDataKey"];
    /**
    * 查询task子项详情的的sql语句
    */
    QString sql = "select distinct "
            "tob.TaskObjectDataKey,"
            "tob.ObjectName,"
            "tob.ObjectFullName,"
            "tob.ItemSchema,"
            "tob.RealObjectType,"
            "tob.RealObjectID,"
            "tob.SerialKey, "
            "tob.TaskDataKey, "
            "tob.TaskGroupDataKey, "
            "case when tob.RealObjectType=1 then eq.Custodian else null end as Custodian,"
            "case when tob.RealObjectType=1 then eq.ModelName else null end as ModelName,"
            "case when tob.RealObjectType=1 then eq.RFIDCode else eqp.RFIDCode end as RFIDCode,"
            "case when tob.RealObjectType=1 then eq.BarCode else eqp.BarCode end as BarCode"
            " from EMM_Maintenance_Task_Object tob , EMM_Maintenance_Task task "
            " left join EMB_Equipment eq on tob.RealObjectID=eq.ID and tob.RealObjectType=1"
            " left join EMB_Equipment_Part eqp on tob.RealObjectID=eqp.ID and tob.RealObjectType=2"
            " where tob.TaskObjectDataKey='" + taskObjectDataKey + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EMM_GetTaskObjectDetail;
        DataTable table;
        table.name = MAINTENANCE_TABLE_TASK_OBJECT;
        table.key = TASK_OBJECT_DATA_KEY;
        for (int i = 0; i < dataRows.size(); i++) {
        DataRow &dataRow = dataRows[i];
            table.value.append(dataRow);
        }
        resultPackage.tables.append(table);
        resultPackage.summary.code = 100;
        resultPackage.summary.result = 1;
        resultPackage.summary.description = "请求数据成功完成";
    }
}

void MaintenanceHandler::checkTask(const QString &taskDataKey, const QString &userID){
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

void MaintenanceHandler::checkGroup(const QString &taskGroupDataKey){
    qDebug() <<Q_FUNC_INFO << "  taskGroupDataKey " << taskGroupDataKey;
    QStringList columns;
    columns.append(TASK_GROUP_DATA_KEY);
    QString whereClause = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDataKey + "' and " +
                          QString(LOCAL_STATUS) + "!='" + TASK_STATUS_FINISH + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_OBJECT, columns, whereClause);
    if(dataRows.isEmpty()){
        QStringList taskGroupDataKeys;
        taskGroupDataKeys.append(taskGroupDataKey);
        updateGroupToComplete(taskGroupDataKeys);
    }
}

void MaintenanceHandler::deleteTask(const QString &taskDataKey, const QString &userID){
    qDebug() <<Q_FUNC_INFO << "  taskDataKey " << taskDataKey;
    QString deleteArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
    m_nDbHelper->beginTransaction();
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK, deleteArg);
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK_GROUP, deleteArg);
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK_OBJECT, deleteArg);
    deleteArg += " and " + QString(USER_ID) + "='" + userID + "'";
    m_nDbHelper->deleteRow(MAINTENANCE_TABLE_USER_TASK_TIME, deleteArg);
    m_nDbHelper->endTransaction();
}


