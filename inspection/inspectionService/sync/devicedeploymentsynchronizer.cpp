#include "devicedeploymentsynchronizer.h"
#include "../../framework/common/systemconfig.h"
#include "../common/dbdefine.h"
#include "../common/bussinessmap.h"

DeviceDeploymentSynchronizer::DeviceDeploymentSynchronizer()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICE_DEPLOYMENT;
}

DeviceDeploymentSynchronizer::~DeviceDeploymentSynchronizer()
{

}

void DeviceDeploymentSynchronizer::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
{
    m_nRunnings = true;
    m_nSyncMap = map;
    m_nSyncUser = m_nSyncMap["UserID"].toString();
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
    synTaskByIds(addList, m_nSyncUser);
    m_nUserMaxTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    m_nSyncTaskTotalCount = m_nSyncTaskInfoMap.size();
}

void DeviceDeploymentSynchronizer::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << " reslut: " << result << ", error :" << error << ",  objects: " << objects << ", action :" << action;
    if(result == true && error == NETWORK_NO_ERROR){
        if(objects == EMA_Off_GetSomeRunningTasksPacket){
            /**
            * 调拨任务详情
            */

            m_nDbHelper->beginTransaction();
            /**
            * Step 1. 更新任务信息
            */
            const QList<DataRow> &dataRows0 = map.dataPackage.tables.at(0).value;
            updateTask(dataRows0);

            /**
            * Step 2. 更新任务项信息
            */
            const QList<DataRow> &dataRows1 = map.dataPackage.tables.at(1).value;
            QMap<QString, QString> deviceInfo;
            updateChild(dataRows1, deviceInfo);

            m_nDbHelper->endTransaction();

            /**
            * Step 3. 同步设备信息
            */
            qDebug() << "deviceInfo   is " << deviceInfo.size();
            bool result = requestObjs(deviceInfo, action);
            if(!result){
                resultProcess(action, true);
            }

        }else if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            /**
            * 设备信息
            */
            /**
            * Step 1. 结果处理，判断是否所有任务同步完成
            */
            resultProcess(action, true);
        }
    } else {
        resultProcess(action, false);
    }
}

void DeviceDeploymentSynchronizer::synTaskByIds(const QStringList &taskIDs, const QString &userID){
    foreach (QString taskID, taskIDs) {
        QString syncTaskTime = getTaskSyncTime(userID, taskID);
        QVariantMap valueMap;
        valueMap["TaskType"] = QString::number((int)SYNC_TASK_ADD);
        valueMap["TaskSyncTime"] = syncTaskTime;
        m_nSyncTaskInfoMap.insert(taskID, valueMap);
        syncTaskInfoById(taskID, syncTaskTime, taskID);
    }
}

QString DeviceDeploymentSynchronizer::getTaskSyncTime(const QString &userID, const QString &taskID){
    QString syncDateTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QStringList columns;
    QString selectionArg = " where " + QString(TASK_ID) + "='" + taskID + "' and " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(DEPLOYMENT_TABLE_USER_TASK_TIME, columns, selectionArg);
    if(!dataRows.isEmpty()){
        syncDateTime = dataRows.at(0).value(LAST_TIME);
    }
    return syncDateTime;
}

void DeviceDeploymentSynchronizer::syncTaskInfoById(const QString &taskID, const QString &syncDatetime, const QString &action){
    DataPackage dataPackage;
    dataPackage.head.key = PACKAGE_HEAD_KEY;
    dataPackage.head.type = TYPE_REQUEST;
    dataPackage.head.objects = EMA_Off_GetSomeRunningTasksPacket;
    dataPackage.para.insert("TaskID", taskID);
    dataPackage.para.insert("LastDEXTime", syncDatetime);
    sendData(dataPackage, action);
}

void DeviceDeploymentSynchronizer::updateTask(const QList<DataRow> &dataRows){
    qDebug() << Q_FUNC_INFO << " dataRows.size() :" << dataRows.size();
    if(!dataRows.isEmpty() && dataRows.size() == 1) {
        DataRow dataRow = dataRows[0];
        QString orderCode = dataRow.value(ORDER_CODE).toUpper();
        QString taskID = dataRow.value(DB_ID);
        QString taskTime = dataRow.value(SERVER_DATE);

        /**
        * 记录任务的时间
        */
        QVariantMap valueMap;
        valueMap["TaskType"] = QString::number((int)SYNC_TASK_ADD);
        valueMap["TaskSyncTime"] = taskTime;
        m_nSyncTaskInfoMap.insert(taskID, valueMap);

        /**
        * 此处不要更新LastTime时间字段,设备同步完成后再更新
        */
        dataRow.insert(ORDER_CODE, orderCode);
        dataRow.insert(SYNC_STATUS, TASK_STATUS_SYNC_ING);
        QStringList columns;
        columns.append(DB_ID);
        QString selection = " where " + QString(DB_ID) + "='" + taskID + "'";
        QList<DataRow> list = m_nDbHelper->select(DEPLOYMENT_TABLE_TASK, columns, selection);
        if(!list.isEmpty()){
            m_nDbHelper->update(DEPLOYMENT_TABLE_TASK, dataRow, selection);
        }else {
            dataRow.insert(LOCAL_STATUS, TASK_STATUS_NO_FINISH);
            m_nDbHelper->insert(DEPLOYMENT_TABLE_TASK, dataRow);
        }
    }
}

void DeviceDeploymentSynchronizer::updateChild(const QList<DataRow> &dataRows, QMap<QString, QString> &deviceInfo){
    qDebug() << Q_FUNC_INFO << " dataRows.size() :" << dataRows.size();
    if(!dataRows.isEmpty()){
        QStringList columns;
        columns.append(DB_ID);
        QList<DataRow> list;
        for(int i = 0; i < dataRows.size(); i++){
            DataRow dataRow = dataRows[i];
            QString allocationID = dataRow.value(ALLOCATION_ID);
            QString equipmentID = dataRow.value(EQUIPMENT_ID);
            QString orderCode = dataRow.value(ORDER_CODE).toUpper();
            QString equipmentModifiedDate = dataRow.value(EQUIPMENT_MODIFIED_DATE);
            QString selection = " where " + QString(DB_ID) + "='" + equipmentID + "' and " +
            QString(ALLOCATION_ID) + "='" + allocationID + "'";
            list = m_nDbHelper->select(DEPLOYMENT_TABLE_EQUIPMENT, columns, selection);
            if(!list.isEmpty()){
                m_nDbHelper->deleteRow(DEPLOYMENT_TABLE_EQUIPMENT, selection);
            }else{
                deviceInfo.insert(equipmentID, equipmentModifiedDate);
                dataRow.insert(ORDER_CODE, orderCode);
                m_nDbHelper->insert(DEPLOYMENT_TABLE_EQUIPMENT, dataRow);
            }
        }
    }
}

void DeviceDeploymentSynchronizer::updateTaskStatus(const QString &taskID, const QString &syncStatus){
    qDebug() << Q_FUNC_INFO << " taskID :" << taskID << ",  syncStatus: " << syncStatus;
    DataRow dataRow;
    dataRow.insert(SYNC_STATUS, syncStatus);
    QString updateArg = " where " + QString(DB_ID) + "='" + taskID + "'";
    m_nDbHelper->update(DEPLOYMENT_TABLE_TASK, dataRow, updateArg);
}

void DeviceDeploymentSynchronizer::resultProcess(const QString &taskID, const bool &result){
    qDebug() << Q_FUNC_INFO << " taskID :" << taskID << ",  result: " << result;
    qDebug() << Q_FUNC_INFO << " 00000 m_nSyncTaskInfoMap.size(): " << m_nSyncTaskInfoMap.size();
    QVariant varMap = m_nSyncTaskInfoMap.value(taskID);
    QString time = varMap.toMap().value("TaskSyncTime").toString();
    if(time > m_nUserMaxTime){
        m_nUserMaxTime = time;
    }
    m_nSyncTaskInfoMap.remove(taskID);
    qDebug() << Q_FUNC_INFO << " 11111 m_nSyncTaskInfoMap.size(): " << m_nSyncTaskInfoMap.size();
    /**
    * Step 1. 成功计数,更新任务表状态
    */
    if(result){
        m_nSyncTaskSuccessCount++;
        updateTaskStatus(taskID, TASK_STATUS_SYNC_FINISH);
        updateUserTaskTime(m_nSyncUser, taskID, time);
    }else{
        m_nSyncTaskFailCount++;
        updateTaskStatus(taskID, TASK_STATUS_SYNC_FAIL);
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

void DeviceDeploymentSynchronizer::updateUserTaskTime(const QString &userID, const QString &taskID, const QString &time){
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(TASK_ID, taskID);
    dataRow.insert(LAST_TIME, time);
    QStringList columns;
    columns.append(DB_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "' and " +
                        QString(TASK_ID) + "='" + taskID + "'";
    QList<DataRow> list = m_nDbHelper->select(DEPLOYMENT_TABLE_USER_TASK_TIME, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(DEPLOYMENT_TABLE_USER_TASK_TIME, dataRow, selection);
    }else{
        QString uuid = createUuid();
        dataRow.insert(DB_ID, uuid);
        m_nDbHelper->insert(DEPLOYMENT_TABLE_USER_TASK_TIME, dataRow);
    }
}

void DeviceDeploymentSynchronizer::updateUserTime(const QString &userID, const QString &time){
    qDebug() << Q_FUNC_INFO << " userID: " << userID << "   time: " << time;
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(LAST_TIME, time);
    QStringList columns;
    columns.append(USER_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> list = m_nDbHelper->select(DEPLOYMENT_TABLE_USER_TIME, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(DEPLOYMENT_TABLE_USER_TIME, dataRow, selection);
    }else{
        m_nDbHelper->insert(DEPLOYMENT_TABLE_USER_TIME, dataRow);
    }
}


