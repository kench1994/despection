#include "rectificationsynchronizer.h"
#include "../../framework/common/systemconfig.h"
#include "../common/dbdefine.h"
#include "../common/bussinessmap.h"

RectificationSynchronizer::RectificationSynchronizer()
{
    m_nDataOperationType = BUSINESS_TYPE_RECTIFICATION;
}

RectificationSynchronizer::~RectificationSynchronizer()
{

}

void RectificationSynchronizer::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
{
    m_nRunnings = true;
    m_nSyncMap = map;
    m_nSyncUser = m_nSyncMap["UserID"].toString();
    m_nTaskStatus = m_nSyncMap["TaskStatus"].toString();
    QStringList addList = m_nSyncMap["addIDs"].toStringList();
    /**
     * Step 1. 同步变量初始化
     */
    m_nSyncAction = action;
    m_nSyncObjects = objects;
    m_nSyncTaskTotalCount = 0;
    m_nSyncTaskFailCount = 0;
    m_nSyncTaskSuccessCount = 0;
    m_nSyncTaskInfoMap.clear();
    if(addList.isEmpty()){
        m_nRunnings = false;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
        return;
    }
    /**
     * Step 2. 同步任务
     */
    synTaskByIds(addList, m_nSyncUser, m_nTaskStatus);
    m_nUserMaxTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    m_nSyncTaskTotalCount = m_nSyncTaskInfoMap.size();
}

void RectificationSynchronizer::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << " reslut: " << result << ", error :" << error << ",  objects: " << objects << ", action :" << action;
    if(result == true && error == NETWORK_NO_ERROR){
        if(objects == EXAMR_Off_GetSomeRunningTasksPacket){
            /**
            * 调拨任务详情
            */
            handleTasks(map.dataPackage, action, m_nTaskStatus);
        }
    } else {
        resultProcess(action, false, m_nTaskStatus);
    }
}

void RectificationSynchronizer::synTaskByIds(const QStringList &orderIDs, const QString &userID, const QString &taskStatus){
    foreach (QString orderID, orderIDs) {
        QString syncTaskTime = getTaskSyncTime(userID, orderID, taskStatus);
        QVariantMap valueMap;
        valueMap["TaskType"] = QString::number((int)SYNC_TASK_ADD);
        valueMap["TaskSyncTime"] = syncTaskTime;
        m_nSyncTaskInfoMap.insert(orderID, valueMap);
        syncTaskInfoById(orderID, syncTaskTime, orderID, taskStatus);
    }
}

QString RectificationSynchronizer::getTaskSyncTime(const QString &userID, const QString &orderID, const QString& taskStatus){
    QString syncDateTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QStringList columns;
    QString selectionArg = " where " + QString(ORDER_ID) + "='" + orderID + "' and " +
                           QString(USER_ID) + "='" + userID + "' and " +
                           QString(RECTIFY_STATUS) + "='" + taskStatus + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(RECTIFICATION_TABLE_USER_TASK_TIME, columns, selectionArg);
    if(!dataRows.isEmpty()){
        syncDateTime = dataRows.at(0).value(LAST_TIME);
    }
    return syncDateTime;
}

void RectificationSynchronizer::syncTaskInfoById(const QString &orderID, const QString &syncDatetime, const QString &action, const QString &taskStatus){
    DataPackage dataPackage;
    dataPackage.head.key = PACKAGE_HEAD_KEY;
    dataPackage.head.type = TYPE_REQUEST;
    dataPackage.head.objects = EXAMR_Off_GetSomeRunningTasksPacket;
    dataPackage.para.insert("OrderID", orderID);
    dataPackage.para.insert("LastDEXTime", syncDatetime);
    dataPackage.para.insert("TaskType", taskStatus);
    sendData(dataPackage, action);
}

void RectificationSynchronizer::handleTasks(const DataPackage &dataPackage, const QString &action, const QString &taskStatus){
    const QList<DataTable> &tables = dataPackage.tables;
    if(!tables.isEmpty()) {
        /**
        * 将添加的任务和修改的任务写入数据库（检查整改仅存在新增的任务,没有修改的任务）
        */
        const QList<DataRow> &dataRows = tables.at(0).value;
        /**
        * 情况：同步结果为部分成功，部分失败，再次同步时，已经同步成功的任务，不再进行同步
        */
        if(!dataRows.isEmpty()) {
            updateTask(dataRows);
            resultProcess(action, true, taskStatus);
        }
    }
}

void RectificationSynchronizer::updateTask(const QList<DataRow> &dataRows){
    qDebug() << Q_FUNC_INFO << " dataRows.size() :" << dataRows.size();
    if(!dataRows.isEmpty() && dataRows.size() == 1) {
        DataRow dataRow = dataRows[0];
        QString orderID = dataRow.value(ORDER_ID);
        QString taskTime = dataRow.value(SERVER_DATE);

        /**
        * 记录任务的时间
        */
        QVariantMap valueMap;
        valueMap["TaskType"] = QString::number((int)SYNC_TASK_ADD);
        valueMap["TaskSyncTime"] = taskTime;
        m_nSyncTaskInfoMap.insert(orderID, valueMap);

        /**
        * 此处不要更新LastTime时间字段,设备同步完成后再更新
        */
        dataRow.remove(SERVER_DATE);
        dataRow.insert(SYNC_STATUS, TASK_STATUS_SYNC_ING);
        QStringList columns;
        columns.append(ORDER_ID);
        QString selection = " where " + QString(ORDER_ID) + "='" + orderID + "'";
        QList<DataRow> list = m_nDbHelper->select(RECTIFICATION_TABLE_TASK, columns, selection);
        if(!list.isEmpty()){
            m_nDbHelper->update(RECTIFICATION_TABLE_TASK, dataRow, selection);
        }else {
            dataRow.insert(LOCAL_STATUS, TASK_STATUS_NO_FINISH);
            m_nDbHelper->insert(RECTIFICATION_TABLE_TASK, dataRow);
        }
    }
}

void RectificationSynchronizer::resultProcess(const QString &orderID, const bool &result, const QString &taskStatus){
    qDebug() << Q_FUNC_INFO << " orderID :" << orderID << ",  result: " << result;
    qDebug() << Q_FUNC_INFO << " 00000 m_nSyncTaskInfoMap.size(): " << m_nSyncTaskInfoMap.size();
    QVariant varMap = m_nSyncTaskInfoMap.value(orderID);
    QString syncTime = varMap.toMap().value("TaskSyncTime").toString();
    if(syncTime > m_nUserMaxTime){
        m_nUserMaxTime = syncTime;
    }
    m_nSyncTaskInfoMap.remove(orderID);
    qDebug() << Q_FUNC_INFO << " 11111 m_nSyncTaskInfoMap.size(): " << m_nSyncTaskInfoMap.size();
    /**
    * Step 1. 成功计数,更新任务表状态
    */
    if(result){
        m_nSyncTaskSuccessCount++;
        updateTaskStatus(orderID, TASK_STATUS_SYNC_FINISH);
        updateUserTaskTime(m_nSyncUser, orderID, syncTime, taskStatus);
    }else{
        m_nSyncTaskFailCount++;
        updateTaskStatus(orderID, TASK_STATUS_SYNC_FAIL);
    }

    /**
    * Step 2. 更新用户业务表的获取的时间戳
    */
    if(m_nSyncTaskInfoMap.isEmpty()) {
        if(m_nSyncTaskFailCount == 0){
            updateUserTime(m_nSyncUser, m_nUserMaxTime, taskStatus);
        }
        m_nRunnings = false;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
    }
}

void RectificationSynchronizer::updateTaskStatus(const QString &orderID, const QString &syncStatus){
    qDebug() << Q_FUNC_INFO << " orderID :" << orderID << ",  syncStatus: " << syncStatus;
    DataRow dataRow;
    dataRow.insert(SYNC_STATUS, syncStatus);
    QString updateArg = " where " + QString(ORDER_ID) + "='" + orderID + "'";
    m_nDbHelper->update(RECTIFICATION_TABLE_TASK, dataRow, updateArg);
}

void RectificationSynchronizer::updateUserTaskTime(const QString &userID, const QString &orderID, const QString &syncTime, const QString &taskStatus){
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(ORDER_ID, orderID);
    dataRow.insert(LAST_TIME, syncTime);
    dataRow.insert(RECTIFY_STATUS, taskStatus);
    QStringList columns;
    columns.append(DB_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "' and " +
                        QString(ORDER_ID) + "='" + orderID + "' and " +
                        QString(RECTIFY_STATUS) + "='" + taskStatus + "'";
    QList<DataRow> list = m_nDbHelper->select(RECTIFICATION_TABLE_USER_TASK_TIME, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(RECTIFICATION_TABLE_USER_TASK_TIME, dataRow, selection);
    }else{
        QString uuid = createUuid();
        dataRow.insert(DB_ID, uuid);
        m_nDbHelper->insert(RECTIFICATION_TABLE_USER_TASK_TIME, dataRow);
    }
}

void RectificationSynchronizer::updateUserTime(const QString &userID, const QString &syncTime, const QString& taskStatus){
    qDebug() << Q_FUNC_INFO << " userID: " << userID << "   syncTime: " << syncTime;
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(LAST_TIME, syncTime);
    dataRow.insert(RECTIFY_STATUS, taskStatus);
    QStringList columns;
    columns.append(USER_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "' and " +
                        QString(RECTIFY_STATUS) + "='" + taskStatus + "'";
    QList<DataRow> list = m_nDbHelper->select(RECTIFICATION_TABLE_USER_TIME, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(RECTIFICATION_TABLE_USER_TIME, dataRow, selection);
    }else{
        m_nDbHelper->insert(RECTIFICATION_TABLE_USER_TIME, dataRow);
    }
}
