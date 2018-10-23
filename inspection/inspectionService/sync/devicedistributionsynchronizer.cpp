#include "devicedistributionsynchronizer.h"
#include "../../framework/common/systemconfig.h"
#include "../common/dbdefine.h"
#include "../common/bussinessmap.h"

DeviceDistributionSynchronizer::DeviceDistributionSynchronizer()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICE_RECEIVE;
}

DeviceDistributionSynchronizer::~DeviceDistributionSynchronizer()
{

}

void DeviceDistributionSynchronizer::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
{
    m_nRunnings = true;
    m_nSyncMap = map;
    m_nSyncUser = m_nSyncMap["UserID"].toString();
    QStringList addList = m_nSyncMap["addIDs"].toStringList();
    QStringList updateList = m_nSyncMap["updateIDs"].toStringList();
    /**
     * Step 1. 同步变量初始化
     */
    m_nSyncAction = action;
    m_nSyncObjects = objects;
    m_nSyncTaskTotalCount = 0;
    m_nSyncTaskFailCount = 0;
    m_nSyncTaskSuccessCount = 0;
    m_nSyncTaskInfoMap.clear();
    if(addList.isEmpty() && updateList.isEmpty()){
        m_nRunnings = false;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
        return;
    }
    /**
     * Step 2. 同步任务
     */
    synTaskByIds(addList, m_nSyncUser, SYNC_TASK_ADD);
    synTaskByIds(updateList, m_nSyncUser, SYNC_TASK_UPDATE);
    m_nUserMaxTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    m_nSyncTaskTotalCount = m_nSyncTaskInfoMap.size();
}

void DeviceDistributionSynchronizer::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << " reslut: " << result << ", error :" << error << ",  objects: " << objects << ", action :" << action;
    if(result == true && error == NETWORK_NO_ERROR){
        if(objects == EMD_Off_GetSomeRunningTasksPacket){
            /**
            * 领用任务详情
            */
            handleTasks(map.dataPackage, action);

        }else if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            /**
            * 设备信息
            */
            resultProcess(action, true);
        }
    }else{
        /**
        * 结果处理，判断是否所有任务同步完成
        */
        if(objects == EMD_Off_GetSomeRunningTasksPacket){
            resultProcess(action, false);
        }else if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            resultProcess(action, false);
        }
    }
}

void DeviceDistributionSynchronizer::synTaskByIds(const QStringList &taskIDs, const QString &userID, const SyncTaskType &taskType){
    foreach (QString taskID, taskIDs) {
        QString syncTaskTime = getTaskSyncTime(userID, taskID);
        QVariantMap valueMap;
        valueMap["TaskType"] = taskType;
        valueMap["TaskSyncTime"] = syncTaskTime;
        m_nSyncTaskInfoMap.insert(taskID, valueMap);
        syncTaskInfoById(taskID, QString::number(taskType), syncTaskTime, taskID);
    }
}

QString DeviceDistributionSynchronizer::getTaskSyncTime(const QString &userID, const QString &taskID){
    QString syncDateTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QStringList columns;
    QString selectionArg = " where " + QString(TASK_ID) + "='" + taskID + "' and " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(DISTRIBUTION_TABLE_TASK_SYNC_DATE, columns, selectionArg);
    if(!dataRows.isEmpty()){
        syncDateTime = dataRows.at(0).value(LAST_TIME);
    }
    return syncDateTime;
}

void DeviceDistributionSynchronizer::syncTaskInfoById(const QString &taskID, const QString &taskType, const QString &syncDatetime, const QString &action){
    DataPackage dataPackage;
    dataPackage.head.key = PACKAGE_HEAD_KEY;
    dataPackage.head.type = TYPE_REQUEST;
    dataPackage.head.objects = EMD_Off_GetSomeRunningTasksPacket;
    dataPackage.para.insert("TaskID", taskID);
    dataPackage.para.insert("TaskType", taskType);
    dataPackage.para.insert("LastDEXTime", syncDatetime);
    sendData(dataPackage, action);
}

void DeviceDistributionSynchronizer::handleTasks(const DataPackage &dataPackage, const QString &taskID)
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
        * Step 2. 更新设备信息
        */
        const DataTable &equipmentTable = tables.at(1);
        const QList<DataRow> &dataRows1 = equipmentTable.value;
        QMap<QString, QString> deviceInfo;
        updateEquipment(dataRows1, deviceInfo);

        m_nDbHelper->endTransaction();

        /**
        * Step 4. 同步设备信息
        */
        bool result = requestObjs(deviceInfo, taskID);
        if(!result){
            resultProcess(taskID, true);
        }
    }
}

void DeviceDistributionSynchronizer::updateTask(const QList<DataRow> &dataRows){
    if(!dataRows.isEmpty()) {
        for(int i = 0; i < dataRows.size(); i++) {
            DataRow dataRow = dataRows[i];
            const QString &taskID = dataRow[DB_ID];
            /**
            * 记录任务的时间
            */
            QVariantMap valueMap;
            valueMap["TaskType"] = m_nSyncTaskInfoMap[taskID].toMap()["TaskType"];
            valueMap["TaskSyncTime"] = dataRow[SERVER_DATE];
            m_nSyncTaskInfoMap.insert(taskID, valueMap);
            /**
            * 此处不要更新LastTime时间字段,设备同步完成后再更新
            */
            dataRow[SYNC_STATUS] = TASK_STATUS_SYNC_ING;
            dataRow[ORDER_CODE] = dataRow[ORDER_CODE].toUpper();
            QStringList columns;
            columns.append(DB_ID);
            QString selectArg = " where " + QString(DB_ID) + "='" + taskID + "'";
            QList<DataRow> list = m_nDbHelper->select(DISTRIBUTION_TABLE_TASK, columns, selectArg);
            if (!list.isEmpty()) {
                m_nDbHelper->update(DISTRIBUTION_TABLE_TASK, dataRow, selectArg);
            } else {
                dataRow[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
                m_nDbHelper->insert(DISTRIBUTION_TABLE_TASK, dataRow);
            }
        }
    }
}

void DeviceDistributionSynchronizer::updateEquipment(const QList<DataRow> &dataRows, QMap<QString, QString> &deviceInfo){
    if(!dataRows.isEmpty()){
        for(int i = 0; i < dataRows.size(); i++) {
            DataRow dataRow = dataRows.at(i);
            dataRow.remove("ClassName");
            dataRow.remove("EnterpriseID");
            dataRow.remove("EquipmentCode");
            dataRow.remove("EquipmentModifiedDate");
            dataRow.remove("EquipmentName");
            dataRow.remove("ManufacturerName");
            dataRow.remove("ModelName");
            dataRow.remove("Status");
            dataRow[ORDER_CODE] = dataRow[ORDER_CODE].toUpper();
            const QString &taskID = dataRow[DISTRIBUTE_ID];
            const QString &objectID = dataRow[DB_ID];
            QString selectArg = " where " + QString(DB_ID) + "='" + objectID + "' and "
                                + QString(DISTRIBUTE_ID) + "='" + taskID + "'";
            QStringList column;
            column.append(DB_ID);
            const QString &taskType = m_nSyncTaskInfoMap[taskID].toMap()["TaskType"].toString();
            if(taskType == QString::number(SYNC_TASK_ADD)){
                QList<DataRow> list = m_nDbHelper->select(DISTRIBUTION_TABLE_EQUIPMENT, column, selectArg);
                if(!list.isEmpty()){
                    m_nDbHelper->update(DISTRIBUTION_TABLE_EQUIPMENT, dataRow, selectArg);
                }else{
                    dataRow[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
                    m_nDbHelper->insert(DISTRIBUTION_TABLE_EQUIPMENT, dataRow);
                }
                deviceInfo.insert(dataRow["EquipmentID"], dataRow["EquipmentModifiedDate"]);
            }else if(taskType == QString::number(SYNC_TASK_UPDATE)){
                dataRow[LOCAL_STATUS] = TASK_STATUS_FINISH;
                m_nDbHelper->update(DISTRIBUTION_TABLE_EQUIPMENT, dataRow, selectArg);
                checkTask(taskID, m_nSyncUser);
            }
        }
    }
}

void DeviceDistributionSynchronizer::checkTask(const QString &taskID, const QString &userID){
    qDebug() << Q_FUNC_INFO << "  taskID " << taskID;
    QStringList columns;
    columns.append(DISTRIBUTE_ID);
    QString whereClause = " where " + QString(DISTRIBUTE_ID) + "='" + taskID + "' and " +
                          QString(LOCAL_STATUS) + "!='" + TASK_STATUS_FINISH + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(DISTRIBUTION_TABLE_EQUIPMENT, columns, whereClause);
    if(dataRows.isEmpty()){
        deleteTask(taskID, userID);
    }
}

void DeviceDistributionSynchronizer::deleteTask(const QString &taskID, const QString &userID){
    qDebug() << Q_FUNC_INFO << "  taskID " << taskID;
    QString deleteArg = " where " + QString(DB_ID) + "='" + taskID + "'";
    m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_TASK, deleteArg);
    deleteArg = " where " + QString(DISTRIBUTE_ID) + "='" + taskID + "'";
    m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_EQUIPMENT, deleteArg);
    deleteArg = " where " + QString(TASK_ID) + "='" + taskID + "' and " + QString(USER_ID) + "='" + userID + "'";
    m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_TASK_SYNC_DATE, deleteArg);
}

void DeviceDistributionSynchronizer::resultProcess(const QString &taskID, const bool &result){
    qDebug() << Q_FUNC_INFO << "  taskID " << taskID << ", result "<< result;
    QVariant varMap = m_nSyncTaskInfoMap.value(taskID);
    QString time = varMap.toMap().value("TaskSyncTime").toString();
    if(time > m_nUserMaxTime){
        m_nUserMaxTime = time;
    }
    m_nSyncTaskInfoMap.remove(taskID);

    /**
    * Step 1. 成功计数
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

void DeviceDistributionSynchronizer::updateUserTaskTime(const QString &userID, const QString &taskID, const QString &time){
    qDebug() << Q_FUNC_INFO << "  taskID " << taskID << ", time "<< time;
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(TASK_ID, taskID);
    dataRow.insert(LAST_TIME, time);
    QStringList columns;
    columns.append(USER_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "' and " +
                        QString(TASK_ID) + "='" + taskID + "'";
    QList<DataRow> list = m_nDbHelper->select(DISTRIBUTION_TABLE_TASK_SYNC_DATE, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(DISTRIBUTION_TABLE_TASK_SYNC_DATE, dataRow, selection);
    }else{
        m_nDbHelper->insert(DISTRIBUTION_TABLE_TASK_SYNC_DATE, dataRow);
    }
}

void DeviceDistributionSynchronizer::updateUserTime(const QString &userID, const QString &time){
    qDebug() << Q_FUNC_INFO << "  time " << time;
    DataRow dataRow;
    dataRow.insert(USER_ID, userID);
    dataRow.insert(LAST_TIME, time);
    QStringList columns;
    columns.append(USER_ID);
    QString selection = " where " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> list = m_nDbHelper->select(DISTRIBUTION_TABLE_SYNC_DATE, columns, selection);
    if(!list.isEmpty()){
        m_nDbHelper->update(DISTRIBUTION_TABLE_SYNC_DATE, dataRow, selection);
    }else{
        m_nDbHelper->insert(DISTRIBUTION_TABLE_SYNC_DATE, dataRow);
    }
}

void DeviceDistributionSynchronizer::updateTaskStatus(const QString &taskID, const QString &syncStatus){
    qDebug() << Q_FUNC_INFO << "  taskID " << taskID << "  syncStatus " << syncStatus;
    DataRow dataRow;
    dataRow.insert(SYNC_STATUS, syncStatus);
    QString updateArg = " where " + QString(DB_ID) + "='" + taskID + "'";
    m_nDbHelper->update(DISTRIBUTION_TABLE_TASK, dataRow, updateArg);
}



