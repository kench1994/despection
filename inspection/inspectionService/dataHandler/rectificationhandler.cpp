#include "rectificationhandler.h"
#include "../common/comparetool.h"
#include "../../framework/common/convertdata.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"

#define TABLE_NAME_OF_DATA_PACKAGE  "EXAM_ExamineRectify"

RectificationHandler::RectificationHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_RECTIFICATION;
}

RectificationHandler::~RectificationHandler()
{
}

void RectificationHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    qDebug() <<Q_FUNC_INFO << " on ResultOfResponse";
    DbusPackage dbusPackage;
    const DataPackage &requestPackage = m_nRequestPkgMap[action];
    getLocalTasks(requestPackage, dbusPackage.dataPackage, m_nTaskStatus);
    emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    m_nRequestPkgMap.remove(action);
}
void RectificationHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  dataPackage.para  " << dataPackage.para;
    QString objects = dataPackage.head.objects;
    if(objects == EXAM_ExamineRectifyProcList){
        m_nTaskStatus = dataPackage.para["ProcType"];
        QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
        QString lastDEXTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
        QString selectionArg = " where "+ QString(USER_ID) + "='" + userID + "' and " +
                               QString(RECTIFY_STATUS) + "='" + m_nTaskStatus + "'";
        QStringList column;
        QList<DataRow > dataRows = m_nDbHelper->select(RECTIFICATION_TABLE_USER_TIME, column, selectionArg);
        if (dataRows.count() > 0) {
            lastDEXTime = dataRows.at(0).value(LAST_TIME);
        }
        DataPackage data;
        data.head = dataPackage.head;
        data.head.objects = EXAMR_Off_GetUserRunningTaskDataKeys;
        data.para.insert(USER_ID, userID);
        data.para.insert(LAST_DEX_TIME, lastDEXTime);
        data.para.insert("Status", m_nTaskStatus);
        sendData(data, action);
    } else {
        sendData(dataPackage, action);
    }
}

void RectificationHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << dataPackage.para;
    DbusPackage dbusPackage;
    QString objects = dataPackage.head.objects;
    if(objects == EXAM_ExamineRectifyProcList){
        QString taskStatus = dataPackage.para["ProcType"];
        getLocalTasks(dataPackage, dbusPackage.dataPackage, taskStatus);
    }if(objects == EXAMR_Off_GetUserRunningTaskDataKeys){
        QString taskStatus = dataPackage.para["Status"];
        getLocalTasks(dataPackage, dbusPackage.dataPackage, taskStatus);
    }else if(objects == EXAM_ExamineRectifyProcDetail){
        getLocalProcDetail(dataPackage, dbusPackage.dataPackage);
    }
    emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    m_nRequestPkgMap.remove(action);
}

void RectificationHandler::cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID)
{
    /**
    *  Step 1. 删除任务-用户-时间表
    */
    QString selectArg = " where " + QString(USER_ID) + "='" + userID + "'";
    m_nDbHelper->deleteRow(RECTIFICATION_TABLE_USER_TASK_TIME, selectArg);

    /**
    * Step 2. 删除业务-用户-时间表
    */
    m_nDbHelper->deleteRow(RECTIFICATION_TABLE_USER_TIME, selectArg);

    /**
    * Step 3. 删除任务
    */
    QStringList postCodes = getPostCodesByUser(userID);
    if(!postCodes.isEmpty()){
        for (int i = 0; i < postCodes.size(); i++) {
            QString postCode = postCodes.at(i);
            QStringList userIDs = getUsersByPostCode(postCode);
            if(userIDs.size() == 1){
                selectArg = " where RectifyOrg='" + organiseUnitID + "' or RectifyOrg ='" + departmentID + "'";
                m_nDbHelper->deleteRow(RECTIFICATION_TABLE_TASK, selectArg);
            }
        }
    }
}

void RectificationHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        if (objects == EXAMR_Off_GetUserRunningTaskDataKeys) {
            /**
             * 任务同步处理
             */
            handleTaskPatch(map.dataPackage, action, m_nTaskStatus);
        } else if (objects == EXAM_ExamineRectifyProcList) {
            /**
             * 在线请求任务的接口
             */
            emit onResultOfResponse(objects, action, result, error, map);
        }else if (objects == EXAM_ExamineRectifyProcDetail) {
           /**
            * 在线请求任务项的接口
            */
           emit onResultOfResponse(objects, action, result, error, map);
           m_nRequestPkgMap.remove(action);
       }
    } else {
        DataPackage &requestPackage = m_nRequestPkgMap[action];
        offLine(action, requestPackage);
    }
}

void RectificationHandler::handleTaskPatch(const DataPackage &data, const QString &action, const QString &taskStatus){
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    CompareTool compareTool;
    compareTool.compare(data, ORDER_ID);

    /**
    *    需要删除的任务 本地任务存在 服务器已经提交的任务
    */
    if (!compareTool.getDeleteList().isEmpty()) {
        QString orderCodes = ConvertData::getInstance()->convertForSql(compareTool.getDeleteList());
        QString deleteArg0 = " where (" + QString(RECTIFY_STATUS) + "='" + taskStatus + "' and "
                + QString(ORDER_ID) + " in(" + orderCodes + ")) "
                " or date(EndTime) < date('now','localtime')";
        m_nDbHelper->deleteRow(RECTIFICATION_TABLE_TASK, deleteArg0);
        QString deleteArg1 = " where " + QString(RECTIFY_STATUS) + "='" + taskStatus + "' and "
                + QString(ORDER_ID) + " in(" + orderCodes + ") and "
                + QString(USER_ID) + "='" + userID + "'";
        m_nDbHelper->deleteRow(RECTIFICATION_TABLE_USER_TASK_TIME, deleteArg1);
    }

    /**
    *    仅存在需要删除的任务，需要修改业务的同步时间
    */
    if (compareTool.getAddList().isEmpty() && compareTool.getUpdateList().isEmpty() && !compareTool.getDeleteList().isEmpty()) {
        QString syncTime = getMaxTime(compareTool.getTimeList());
        updateSyncTime(syncTime, taskStatus, userID);
    }
    /**
    *    通知app更新界面
    */
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    if(!compareTool.getAddList().isEmpty()) {
        DbusPackage dbusPackage;
        getLocalTasks(requestPackage, dbusPackage.dataPackage, taskStatus);
        emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
        /**
         * 发送增量同步请求
         */
        QVariantMap syncMap;
        syncMap["UserID"] = userID;
        syncMap["TaskStatus"] = taskStatus;
        syncMap["addIDs"] = QVariant(compareTool.getAddList());
        syncMap["updateIDs"] = QVariant(compareTool.getUpdateList());
        emit onBeginSyncSignal(MESSAGE_SYNC_ADDTION, action, requestPackage.head.objects, syncMap);
    } else {
        offLine(action, requestPackage);
    }
}

bool RectificationHandler::getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage, const QString &taskStatus){
    QString organiseUnitID = requestPackage.para.value("UserCompany");
    QString departmentID = requestPackage.para.value("UserDepartment");
    if(organiseUnitID == "" || departmentID == ""){
        organiseUnitID = RuntimeParam::getInstance()->getLoginUserMsgByKey("OrganiseUnitIDs");
        departmentID = RuntimeParam::getInstance()->getLoginUserMsgByKey("DepartmentIDs");
    }
    QString sql = "select OrderID, OrderName,DataSrcType,"
            "StartTime|| \"至\"|| EndTime Circle "
            "from Exam_Examine_Rectify where (RectifyOrg ='" + organiseUnitID + "' or  RectifyOrg ='" + departmentID + "') "
            "and RectifyStatus ='" + taskStatus + "' "
            "and date(StartTime) <= date('now','localtime') and  date(EndTime) >= date('now','localtime') "
            "and LocalStatus ='" + QString(TASK_STATUS_NO_FINISH) + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        convertToDataPackage(dataRows, requestPackage.head, TABLE_NAME_OF_DATA_PACKAGE, ORDER_ID, resultPackage);
    }
    return result;
}

QString RectificationHandler::getMaxTime(const QStringList &syncTimeList){
    QString maxTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    for(int i = 0; i < syncTimeList.size(); i++){
        const QString &syncTime = syncTimeList.at(i);
        if(maxTime < syncTime){
            maxTime = syncTime;
        }
    }
    return maxTime;
}

void RectificationHandler::updateSyncTime(const QString &syncTime, const QString &taskStatus, const QString & userID){
    QStringList column;
    column.append(RECTIFY_STATUS);
    DataRow updateRow;
    QString selectArg = " where " + QString(USER_ID) + "='" + userID + "' and " + QString(RECTIFY_STATUS) + "='" + taskStatus + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(RECTIFICATION_TABLE_USER_TIME, column, selectArg);
    if (!dataRows.isEmpty()) {
        updateRow[LAST_TIME] = syncTime;
        m_nDbHelper->update(RECTIFICATION_TABLE_USER_TIME, updateRow, selectArg);
    }else {
        updateRow[USER_ID] = userID;
        updateRow[RECTIFY_STATUS] = taskStatus;
        updateRow[LAST_TIME] = syncTime;
        m_nDbHelper->insert(RECTIFICATION_TABLE_USER_TIME, updateRow);
    }
}

bool RectificationHandler::getLocalProcDetail(const DataPackage &requestPackage, DataPackage &resultPackage){
    QString orderID = requestPackage.para["OrderId"];
    QString sql = "select OrderID, OrderCode,OrderName,DataSrcType, RectifyOrg as RectifyOrgName,"
            "RectifyContent,StartTime,EndTime,SerialKey,OrderUserName,"
            "OrderDate,OrderAuditDate,ProcDescript,ProcRecordUser as ProcRecordUserName,"
            "ProcRecordDate ,RectifyStatus,OrderCompanyName,"
            "OrderDepartmentName "
            "from Exam_Examine_Rectify where " + QString(ORDER_ID) + "='"+ orderID + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        convertToDataPackage(dataRows, requestPackage.head, TABLE_NAME_OF_DATA_PACKAGE, ORDER_ID, resultPackage);
    }
    return result;
}

