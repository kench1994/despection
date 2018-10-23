#include "standardizationhandler.h"
#include "../common/comparetool.h"
#include "../framework/common/convertdata.h"
#include "../common/runtimeparam.h"
#include "../framework/common/systemconfig.h"

StandardizationHandler::StandardizationHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_STANDARDIZATION;
}

StandardizationHandler::~StandardizationHandler()
{

}

void StandardizationHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  dataPackage.para  " << dataPackage.para;
    QString objects = dataPackage.head.objects;
    if(objects == EXAM_ExamineTaskList){
        requestTasksPatch(dataPackage.head, action);
    } else {
        sendData(dataPackage, action);
    }
}

void StandardizationHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO;
    QString objects = dataPackage.head.objects;
    if(objects == EXAM_Off_GetUserRunningTaskDataKeys ||
            objects == EXAM_ExamineTaskList){
        requestLocalTaskList(dataPackage, action);
    }else if(objects == EXAM_ExamineTaskObjectList){
        requestLocalObjectList(dataPackage, action);
    }else if(objects == EXAM_ExamineTaskItemList){
        requestLocalTaskItemList(dataPackage, action);
    }else if(objects == EXAM_ExamineTaskItemDetail){
        requestLocalTaskItemDetail(dataPackage, action);
    }
    m_nRequestPkgMap.remove(action);
}

int StandardizationHandler::getItemCount()
{
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    QStringList column;
    column.append(LAST_TIME);
    QString selectArg = " where " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> list = m_nDbHelper->select(EXAMINE_TABLE_SYNC_DATE, column, selectArg);
    if (list.isEmpty()) {
        return -1;
    }
    QString sql = "select distinct t.TaskDataKey"
            " from EXAM_ExamineTask t"
            " inner join EXAM_ExamineTaskObject tob on t.TaskDataKey=tob.TaskDataKey"
            " where t.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
            " and tob.TotalItemCount > tob.ExamItemCount" +
            " and datetime(TaskStartTime) < datetime('now','localtime') and  datetime(TaskEndTime) > datetime('now','localtime')"
            " and tob.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
            " group by t.TaskDataKey";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result && !dataRows.isEmpty()){
        return dataRows.size();
    }
    return 0;
}

void StandardizationHandler::cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID)
{
    /**
    *  Step 1. 删除任务-用户-时间表
    */
    QString selectArg = " where " + QString(USER_ID) + userID + "'";
    m_nDbHelper->deleteRow(EXAMINE_TABLE_TASK_SYNC_DATE, selectArg);

    /**
    * Step 2. 删除业务-用户-时间表
    */
    m_nDbHelper->deleteRow(EXAMINE_TABLE_SYNC_DATE, selectArg);

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
                m_nDbHelper->deleteRow(EXAMINE_TABLE_OBJECT, selectArg);
            }
        }
    }

    /**
    * Step 4. 删除任务项
    */
    selectArg = " where EXAM_ExamineTaskItem.TaskObjectDataKey not in (select TaskObjectDataKey from EXAM_ExamineTaskObject)";
    m_nDbHelper->deleteRow(EXAMINE_TABLE_ITEM, selectArg);

    /**
    * Step 5. 删除任务
    */
    QStringList column;
    QList<DataRow> list = m_nDbHelper->select(EXAMINE_TABLE_TASK, column, "");
    if(!list.isEmpty()){
        for(int i = 0 ; i < list.size(); i++){
            QString taskDataKey = list.at(i)[TASK_DATA_KEY];
            selectArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            QList<DataRow> list1 = m_nDbHelper->select(EXAMINE_TABLE_OBJECT, column, selectArg);
            if(list1.isEmpty()) {
                m_nDbHelper->deleteRow(EXAMINE_TABLE_TASK, selectArg);
            }
        }
    }
    Q_UNUSED(organiseUnitID)
    Q_UNUSED(departmentID)
}

void StandardizationHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO << " on ResultOfResponse";
    DbusPackage dbusPackage;
    getLocalTasks(m_nRequestPkgMap[action], dbusPackage.dataPackage);
    emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    m_nRequestPkgMap.remove(action);
    Q_UNUSED(map)
}

void StandardizationHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        if(objects == EXAM_Off_GetUserRunningTaskDataKeys) {
            /**
            *  handle tasks patch
            */
            handleTasksPatch(objects, action, map.dataPackage);
        }else if(objects == EXAM_ExamineTaskList) {
            /**
            *  handle tasks list response
            */
            handleTaskListResponse(action, map);
        }else if(objects == EXAM_ExamineTaskObjectList) {
            /**
            * warning:
            * In theory, this circumstance wouldn't happen
            * So ignore it.
            */
            handleTaskObjectResponse(action, map.dataPackage);
        }else if(objects == EXAM_ExamineTaskItemList) {
            /**
            * handle EquipmentList callback
            */
            handleTaskItemResponse(action, map.dataPackage);
        }else if(objects == EXAM_ExamineTaskItemDetail) {
            /**
            * handle Equipment Para
            */
            handleTaskItemDetailResponse(action, map);
        }
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}


void StandardizationHandler::requestTasksPatch(const DataHead &head, const QString &action) {
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    QString lastDEXTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QString selectionArg = " where "+ QString(USER_ID) + "='" + userID + "'";
    QStringList column;
    QList<DataRow > dataRows = m_nDbHelper->select(EXAMINE_TABLE_SYNC_DATE, column, selectionArg);
    if (dataRows.count() > 0) {
        lastDEXTime = dataRows.at(0).value(LAST_TIME);
    }
    DataPackage data;
    data.head = head;
    data.head.objects = EXAM_Off_GetUserRunningTaskDataKeys;
    data.para.insert(USER_ID, userID);
    data.para.insert(LAST_DEX_TIME, lastDEXTime);
    sendData(data, action);
}

void StandardizationHandler::handleTasksPatch(const QString &objects, const QString &action, DataPackage &dataPackage) {

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
            QString deleteArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            m_nDbHelper->beginTransaction();
            m_nDbHelper->deleteRow(EXAMINE_TABLE_TASK, deleteArg);
            m_nDbHelper->deleteRow(EXAMINE_TABLE_OBJECT, deleteArg);
            m_nDbHelper->deleteRow(EXAMINE_TABLE_ITEM, deleteArg);
            m_nDbHelper->endTransaction();
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
            m_nDbHelper->update(EXAMINE_TABLE_TASK, dataRow, updateArg);
        }
    }

    /**
    * step 4 :delete invalid pre task
    */
    deletePreTask(compareTool.getAddList());

    /**
    * step 5 : send response to application,  send added task to synchronizer
    */
    DataPackage requestPackage = m_nRequestPkgMap[action];
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
                    lvs.append(nv);
                    break;
                }
            }
        }
        emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, dbusPackage);
        QVariantMap syncMap;
        syncMap["UserID"] = requestPackage.para["UserID"];
        syncMap["addIDs"] = QVariant(compareTool.getAddList());
        syncMap["updateIDs"] = QVariant(compareTool.getUpdateList());
        emit onBeginSyncSignal(MESSAGE_SYNC_ADDTION, action, requestPackage.head.objects, syncMap);
    } else {
        QStringList taskDataKeys;
        deletePreTask(taskDataKeys);
        offLine(action, requestPackage);
    }
}

void StandardizationHandler::deletePreTask(const QStringList &taskDataKeys){
    QString sql = "select t.TaskDataKey from exam_examinetask  t"
                    " where datetime(TaskStartTime) < datetime('now','localtime')"
                    " and  datetime(TaskEndTime) > datetime ('now','localtime')"
                    " and isPreTask = '1'";
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
            QString taskDataKey = dataRow["TaskDataKey"];
            QString deleteArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            m_nDbHelper->beginTransaction();
            m_nDbHelper->deleteRow(EXAMINE_TABLE_TASK, deleteArg);
            m_nDbHelper->deleteRow(EXAMINE_TABLE_OBJECT, deleteArg);
            m_nDbHelper->deleteRow(EXAMINE_TABLE_ITEM, deleteArg);
            m_nDbHelper->endTransaction();
        }
    }
}

void StandardizationHandler::getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage) {
    /**
    * 查询task层的sql语句
    */
    QString userID = requestPackage.para.value("UserID");
    QString sql = "select distinct "
                    "t.TaskDataKey,"
                    "t.TaskCode,"
                    "t.TaskType,"
                    "t.TaskName,"
                    "t.LimitMode,"
                    "t.ExamObjectType,"
                    "t.EnableUploadAttach,"
                    "t.TaskStartTime|| \"至\"||  t.TaskEndTime as Circle,"
                    "sum(tob.ExamItemCount)|| \"/\" || sum(tob.TotalItemCount) Rate,"
                    "t.SyncStatus"
                    " from EXAM_ExamineTask t"
                    " inner join EXAM_ExamineTaskObject tob on t.TaskDataKey=tob.TaskDataKey"
                    " where t.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
                    " and tob.TotalItemCount > tob.ExamItemCount"
                    " and datetime(TaskStartTime) < datetime('now','localtime') and  datetime(TaskEndTime) > datetime('now','localtime')"
                    " and tob.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
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
        resultPackage.head.objects = EXAM_ExamineTaskList;
        DataTable table;
        table.name = EXAMINE_TABLE_TASK;
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

void StandardizationHandler::requestLocalTaskList(const DataPackage &requestPackage, const QString &action) {
    DbusPackage dbusPackage;
    getLocalTasks(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void StandardizationHandler::handleTaskListResponse(const QString &action, const DbusPackage &map) {
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, map);
    m_nRequestPkgMap.remove(action);
}

void StandardizationHandler::handleTaskObjectResponse(const QString &action, const DataPackage &dataPackage) {

    /**
    * Step 1 : get local task objects
    */
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    QString taskDataKey = requestPackage.para["TaskDataKey"];
    QString userID = requestPackage.para["UserID"];
    /**
    * 查询object层的sql语句
    */
    QString sql = "select distinct "
            "tob.TaskDataKey,"
            "tob.TaskObjectDataKey,"
            "tob.TaskObjectName,"
            "tob.ExamObjectType,"
            "tob.RealObjectID,"
            "tob.RFIDCode,"
            "tob.BarCode,"
            "tob.QRCode,"
            "tob.TotalItemCount,"
            "tob.ExamItemCount "
            " from EXAM_ExamineTaskObject tob ,EXAM_ExamineTask task"
            " where tob.TaskDataKey='" + taskDataKey + "'"
            " and tob.TotalItemCount > tob.ExamItemCount"
            " and tob.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
            " and tob.TaskObjectDataKey in(select TaskObjectDataKey from EXAM_ExamineTaskObject where TaskObjectDataKey=tob.TaskObjectDataKey)"
            " group by tob.TaskObjectDataKey";
    QList<DataRow> lvs;
    m_nDbHelper->select(sql, lvs);
    const QList<DataRow> &nvs = dataPackage.tables[0].value;

    /**
    * Step 2 : compare objects
    */
    CompareTool compareTool;
    QStringList deleteList = compareTool.compare(lvs, nvs, "TaskObjectDataKey");

    /**
    * Step 3 : update all finished objects and their items
    */
    updateObjectToComplete(deleteList);

    /**
    * Step 4 : update exam item count(item has been finished)
    */
    updateExamItemCount(dataPackage, deleteList);

    /**
    * Step 5 : check task
    */
    checkTask(taskDataKey, userID);

    /**
    * Final step : send data to upper application
    */
    offLine(action, requestPackage);
}

void StandardizationHandler::updateObjectToComplete(const QStringList &objectDataKeyList) {
    QString objectDataKeys = ConvertData::getInstance()->convertForSql(objectDataKeyList);
    if (!objectDataKeys.isEmpty()) {
        DataRow dataRow;
        dataRow[LOCAL_STATUS] = TASK_STATUS_FINISH;
        QString updateArg = " where " + QString(TASK_OBJECT_DATA_KEY) + " in (" + objectDataKeys + ")";
        m_nDbHelper->update(EXAMINE_TABLE_OBJECT, dataRow, updateArg);
        m_nDbHelper->update(EXAMINE_TABLE_ITEM, dataRow, updateArg);
    }
}

void StandardizationHandler::updateExamItemCount(const DataPackage &dataPackage, const QStringList &deleteList) {
    if (dataPackage.tables.isEmpty())
        return;
    const QList<DataRow> &dataRows = dataPackage.tables[0].value;
    for(int i = 0; i < dataRows.size(); i++){
        const DataRow &dataRow = dataRows.at(i);
        QString taskObjectDateKey = dataRow["TaskObjectDataKey"];
        if (deleteList.contains(taskObjectDateKey))
            continue;
        QString itemCount = dataRow[EXAM_ITEM_COUNT];
        QString updateArg = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDateKey + "'";
        DataRow updateRow;
        updateRow[EXAM_ITEM_COUNT] =  itemCount;
        m_nDbHelper->update(EXAMINE_TABLE_OBJECT, updateRow, updateArg);
    }
}

void StandardizationHandler::requestLocalObjectList(const DataPackage &requestPackage, const QString &action) {
    DbusPackage dbusPackage;
    getLocalTaskObject(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void StandardizationHandler::getLocalTaskObject(DataPackage requestPackage, DataPackage &resultPackage) {
    /**
    * 查询结果
    */
    QString taskDataKey = requestPackage.para["TaskDataKey"];
    QString userID = requestPackage.para["UserID"];
    /**
    * 查询object层的sql语句
    */
    QString sql = "select distinct "
                    "tob.TaskDataKey,"
                    "tob.TaskObjectDataKey,"
                    "tob.TaskObjectName,"
                    "tob.ExamObjectType,"
                    "tob.RealObjectID,"
                    "tob.RFIDCode,"
                    "tob.BarCode,"
                    "tob.QRCode,"
                    "tob.TotalItemCount,"
                    "tob.ExamItemCount "
                    " from EXAM_ExamineTaskObject tob ,EXAM_ExamineTask task"
                    " where tob.TaskDataKey='" + taskDataKey + "'"
                    " and tob.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
                    " and tob.TotalItemCount > tob.ExamItemCount"
                    " and tob.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
                    " and tob.TaskObjectDataKey in(select TaskObjectDataKey from EXAM_ExamineTaskObject where TaskObjectDataKey=tob.TaskObjectDataKey)"
                    " group by tob.TaskObjectDataKey";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EXAM_ExamineTaskObjectList;
        DataTable table;
        table.name = EXAMINE_TABLE_OBJECT;
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

void StandardizationHandler::handleTaskItemResponse(const QString &action, const DataPackage &dataPackage) {

    /**
     * Step 1 : get local task items
     */
    DataPackage requestPackage = m_nRequestPkgMap[action];
    QString taskObjectDataKey = requestPackage.para["TaskObjectDataKey"];
    /**
     * 查询task子项的sql语句
     */
    QString sql = "select distinct "
                    "ti.TaskItemDataKey,"
                    "ti.ItemName,"
                    "ti.ItemFullName,"
                    "ti.TaskObjectDataKey,"
                    "ti.RFIDCode,"
                    "ti.BarCode,"
                    "ti.QRCode,"
                    "ti.TaskDataKey,"
                    "t.EnableUploadAttach,"
                    "ti.SerialKey ,"
                    "t.TaskStartTime, "
                    "t.IsPreTask "
                    " from EXAM_ExamineTaskItem ti"
                    " left join EXAM_ExamineTask t on ti.TaskDataKey=t.TaskDataKey"
                    " where TaskObjectDataKey='" + taskObjectDataKey + "'"
                    " and ti.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'"
                    " group by ti.TaskItemDataKey";
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
    QStringList deleteList = ompareTool.compare(lvs, nvs, "TaskItemDataKey");

    /**
    * Step 3 : update all finished items
    */
    updateItemToComplete(deleteList);

    /**
    * Step 4 : update exam item count (item has been finished)
    */
    updateExamItemCount(deleteList.size(), taskObjectDataKey);

    /**
    * Step 5 : check object
    */
    checkObject(taskObjectDataKey);

    /**
    * Step 6 : check task
    */
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    checkTask(taskDataKey, userID);

    /**
    * Final step : send data to upper application
    */
    offLine(action, requestPackage);
}

void StandardizationHandler::updateItemToComplete(QStringList itemDataKeyList) {
    QString itemDataKeys = ConvertData::getInstance()->convertForSql(itemDataKeyList);
    if (!itemDataKeys.isEmpty()) {
        DataRow dataRow;
        dataRow[LOCAL_STATUS] = TASK_STATUS_FINISH;
        QString updateArg = " where " + QString(TASK_ITEM_DATA_KEY) + " in (" + itemDataKeys + ")";
        m_nDbHelper->update(EXAMINE_TABLE_ITEM, dataRow, updateArg);
    }
}

void StandardizationHandler::updateExamItemCount(const int &count, const QString &taskObjectDataKey) {
    if(count > 0 ){
        QStringList columns;
        columns.append(EXAM_ITEM_COUNT);
        QString whereArgs = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
        QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_OBJECT, columns, whereArgs);
        if (!dataRows.isEmpty()) {
            DataRow updateRow;
            int oldExamCount = dataRows.at(0)[EXAM_ITEM_COUNT].toInt();
            int newExamCount = oldExamCount + count;
            updateRow[EXAM_ITEM_COUNT] = QString::number(newExamCount);
            m_nDbHelper->update(EXAMINE_TABLE_OBJECT, updateRow, whereArgs);
        }
    }
}

void StandardizationHandler::requestLocalTaskItemList(const DataPackage &requestPackage, const QString &action) {
    DbusPackage dbusPackage;
    getLocalTaskItemList(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void StandardizationHandler::getLocalTaskItemList(const DataPackage &requestPackage, DataPackage &resultPackage) {
    QString taskObjectDataKey = requestPackage.para["TaskObjectDataKey"];
    /**
    * 查询task子项的sql语句
    */
    QString sql = "select distinct "
                    "ti.TaskItemDataKey,"
                    "ti.ItemName,"
                    "ti.ItemFullName,"
                    "ti.TaskObjectDataKey,"
                    "ti.RFIDCode,"
                    "ti.BarCode,"
                    "ti.QRCode,"
                    "ti.TaskDataKey,"
                    "t.EnableUploadAttach,"
                    "ti.SerialKey ,"
                    "t.TaskStartTime, "
                    "t.IsPreTask "
                    " from EXAM_ExamineTaskItem ti"
                    " left join EXAM_ExamineTask t on ti.TaskDataKey=t.TaskDataKey"
                    " where TaskObjectDataKey='" + taskObjectDataKey + "' and ti.LocalStatus='0'"
                    " group by ti.TaskItemDataKey";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EXAM_ExamineTaskItemList;
        DataTable table;
        table.name = EXAMINE_TABLE_ITEM;
        table.key = "TaskItemDataKey";
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

void StandardizationHandler::requestLocalTaskItemDetail(const DataPackage &requestPackage, const QString &action) {
    DbusPackage dbusPackage;
    getTaskItemDetail(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void StandardizationHandler::handleTaskItemDetailResponse(const QString &action, const DbusPackage &map) {
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, map);
    m_nRequestPkgMap.remove(action);
}

void StandardizationHandler::getTaskItemDetail(const DataPackage &requestPackage, DataPackage &resultPackage) {
    QString taskItemDataKey = requestPackage.para["TaskItemDataKey"];
    /**
    * 查询task子项详情的的sql语句
    */
    QString sql = "select distinct "
                    "ti.TaskItemDataKey,"
                    "ti.SerialKey,"
                    "ti.ItemName,"
                    "ti.ItemFullName,"
                    "ti.ItemSchema"
                    " from EXAM_ExamineTaskItem ti"
                    " where ti.TaskItemDataKey='" + taskItemDataKey + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EXAM_ExamineTaskItemDetail;
        DataTable table;
        table.name = EXAMINE_TABLE_ITEM;
        table.key = "TaskItemDataKey";
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

void StandardizationHandler::checkTask(const QString &taskDataKey, const QString &userID){
    qDebug() <<Q_FUNC_INFO << "  taskDataKey " << taskDataKey;
    QStringList columns;
    columns.append(TASK_DATA_KEY);
    QString whereClause = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "' and " +
                          QString(LOCAL_STATUS) + "!='" + TASK_STATUS_FINISH + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_OBJECT, columns, whereClause);
    if(dataRows.isEmpty()){
        deleteTask(taskDataKey, userID);
    }
}

void StandardizationHandler::checkObject(const QString &taskObjectDataKey){
    qDebug() <<Q_FUNC_INFO << "  taskObjectDataKey " << taskObjectDataKey;
    QStringList columns;
    columns.append(TASK_OBJECT_DATA_KEY);
    QString whereClause = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "' and " +
                          QString(LOCAL_STATUS) + "!='" + TASK_STATUS_FINISH + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_ITEM, columns, whereClause);
    if(dataRows.isEmpty()){
        QStringList taskObjectDataKeys;
        taskObjectDataKeys.append(taskObjectDataKey);
        updateObjectToComplete(taskObjectDataKeys);
    }
}

void StandardizationHandler::deleteTask(const QString &taskDataKey, const QString &userID){
    qDebug() <<Q_FUNC_INFO << "  taskDataKey " << taskDataKey;
    QString deleteArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
    m_nDbHelper->beginTransaction();
    m_nDbHelper->deleteRow(EXAMINE_TABLE_TASK, deleteArg);
    m_nDbHelper->deleteRow(EXAMINE_TABLE_OBJECT, deleteArg);
    m_nDbHelper->deleteRow(EXAMINE_TABLE_ITEM, deleteArg);
    deleteArg += " and " + QString(USER_ID) + "='" + userID + "'";
    m_nDbHelper->deleteRow(EXAMINE_TABLE_TASK_SYNC_DATE, deleteArg);
    m_nDbHelper->endTransaction();
}


