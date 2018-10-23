#include "inventorysynchronizer.h"
#include "../networkcontrol.h"
#include "../common/dbhelper.h"
#include "../DBConfig/dbconfigmanager.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../common/bussinessmap.h"
#include "../common/dbdefine.h"
#include <QDebug>
InventorySynchronizer::InventorySynchronizer()
{
    m_nDataOperationType = BUSINESS_TYPE_INVENTORY;
}

InventorySynchronizer::~InventorySynchronizer()
{
}

void InventorySynchronizer::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
{
    qDebug() <<Q_FUNC_INFO << "  action " << action << " objects " << objects << " map " << map << "  running is " << m_nRunnings;
    m_nRunnings = true;
    m_nSyncAction = action;
    m_nSyncObjects = objects;
    m_nSyncMap = map;
    m_nSyncTaskInfoMap.clear();
    m_nSyncTaskSuccessCount = 0;
    m_nSyncTaskFailCount = 0;
    m_nSyncTaskTotalCount = 0;
    QString syncUserID = m_nSyncMap["UserID"].toString();
    QStringList addList = m_nSyncMap["addIDs"].toStringList();
    QStringList updateList = m_nSyncMap["updateIDs"].toStringList();
    if(addList.count() == 0 && updateList.count() == 0){
        resultProcess(action, true);
        return;
    }

    // 同步任务
    synTask(addList, syncUserID, SYNC_TASK_ADD );
    synTask(updateList, syncUserID, SYNC_TASK_UPDATE);
    m_nSyncTaskTotalCount = m_nSyncTaskInfoMap.size();
}

QString InventorySynchronizer::getTaskLastSyncTime(const QString &userID, const QString &taskID){
    QString  syncDateTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QStringList columns;
    columns.append("LastTime");
    QString selectionArg = " where  TaskID = '" + taskID + "' and UserID = '" + userID + "'";
    QList<QMap<QString, QString> > valueList;
    valueList = DBHelper::getInstance()->select("EMI_Inventory_User_Task_Time", columns, selectionArg);
    if(valueList.count() > 0){
        syncDateTime = valueList.at(0)["LastTime"];
    }
    return syncDateTime;
}

void InventorySynchronizer::synTask(const QStringList &taskIdList, const QString &userId, InventorySynchronizer::SyncTaskType syncType)
{
    for(int i = 0; i < taskIdList.count(); i++){
        QString taskId = taskIdList.at(i);
        DataPackage requestPackage;
        requestPackage.head.key = PACKAGE_HEAD_KEY;
        requestPackage.head.type = "1";
        requestPackage.head.objects = EMI_Off_GetSomeRunningTasksPacket;
        requestPackage.para["UserID"] = userId;
        requestPackage.para["LastDEXTime"] = getTaskLastSyncTime(userId, taskIdList.at(i));
        requestPackage.para["TaskType"] = QString::number((int)syncType);
        requestPackage.para["TaskID"] = taskId;
        QVariantMap valueMap;
        valueMap["TaskType"] = QString::number((int)syncType);
        valueMap["TaskSyncTime"] = "";
        m_nSyncTaskInfoMap.insert(taskId, valueMap);
        sendData(requestPackage, taskId);
    }
}

void InventorySynchronizer::taskDataHandle(const QString &action, const DataPackage &dataPackage)
{
    if(dataPackage.tables.length() == 3 && dataPackage.tables.at(0).value.length() == 1){
        //更新任务信息
        DBHelper *dbHelper = DBHelper::getInstance();
        dbHelper->beginTransaction();
        QString taskId = dataPackage.tables.at(0).value.at(0)["ID"];
        // 将添加的任务和修改的任务写入数据库
        const QList<QMap<QString, QString> > &taskValueList = dataPackage.tables.at(0).value;
        qDebug() << "updateTask taskId   is " << taskId;
        updateTask(taskValueList, taskId);

        // 修改任务项表,并且返回需要同步的设备ID列表
        QMap<QString, QString> equipmentIds;
        const QList<QMap<QString, QString> > &objectValueList = dataPackage.tables.at(1).value;
        qDebug() << "updateObject taskId   is " << taskId;
        updateObject(objectValueList, equipmentIds, taskId);

        //修改盘点字典表
        const QList<QMap<QString, QString> > &dicValueList = dataPackage.tables.at(2).value;
        updateDic(dicValueList, taskId);
        dbHelper->endTransaction();

        //请求相关设备列表
        qDebug() << "equipmentIds   is " << equipmentIds.size();
        bool result = requestObjs(equipmentIds, taskId);
        if(!result){
            resultProcess(action, true);
        }
    } else
        resultProcess(action, false);
}

void InventorySynchronizer::updateTask(const QList<QMap<QString, QString> > &taskValueList, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    for (int i = 0; i < taskValueList.count(); i++) {
        QMap<QString, QString> value = taskValueList.at(i);

        //记录任务的同步时间
        QString syncTaskTime = value["ServerDate"];
       QVariantMap valueMap = m_nSyncTaskInfoMap[taskId].toMap();
       valueMap["TaskSyncTime"] = syncTaskTime;
        m_nSyncTaskInfoMap[taskId] = valueMap;

        //更新盘点任务表
        QStringList columns;
        QString selectArg = " where ID = '" + value["ID"] + "'";
        QList<QMap<QString, QString> > localValueList = DBHelper::getInstance()->select("EMI_InventoryTask", columns, selectArg);
        value[SYNC_STATUS] = TASK_STATUS_SYNC_ING;
        QString taskType = m_nSyncTaskInfoMap[taskId].toMap()["TaskType"].toString();
        if(taskType == QString::number((int)SYNC_TASK_ADD))
            value[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
        if(localValueList.count() == 0){
            DBHelper::getInstance()->insert("EMI_InventoryTask", value);
        } else {
            DBHelper::getInstance()->update("EMI_InventoryTask", value, selectArg);
        }
    }
}

void InventorySynchronizer::updateObject(const QList<QMap<QString, QString> > &objectValueList,
                                         QMap<QString, QString> &objectIDList, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    QString taskType = m_nSyncTaskInfoMap[taskId].toMap()["TaskType"].toString();
    if(taskType == QString::number((int)SYNC_TASK_UPDATE) && objectValueList.size() > 0){
        //如果是更新的任务项列表，则说明该任务项已经被盘点，则在本地被标注为已完成，等待任务完成时再删除(原来是从本地数据库删除掉)
        QStringList objectIDList;
        for(int i = 0; i < objectValueList.size(); i++){
            objectIDList.append(objectValueList.at(i)["ID"]);
        }
        markLocalTaskObjectsFinished(taskId, objectIDList);
    } else if(taskType == QString::number((int)SYNC_TASK_ADD) &&objectValueList.size() > 0) {
        //如果是新增的任务项列表，添加到本地数据库
        for (int i = 0; i < objectValueList.count(); i++) {
            QMap<QString, QString> value = objectValueList.at(i);
            QString equipmentId = value["EquipmentID"];
            objectIDList[equipmentId] = value["EquipmentModifiedDate"];
            QStringList columns("ID");
            QString selectArg = " where ID = '" + value["ID"] + "'";
            QList<QMap<QString, QString> > localValueList = DBHelper::getInstance()->select("EMI_InventoryEquipment", columns, selectArg);
            if(localValueList.size() == 0){
                value[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
                DBHelper::getInstance()->insert("EMI_InventoryEquipment", value);
            } else
                DBHelper::getInstance()->update("EMI_InventoryEquipment", value, selectArg);
        }
    }
}

void InventorySynchronizer::updateDic(const QList<QMap<QString, QString> > &dicValueList, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(taskId)
    //如果本地数据库已经有数据，则直接返回
    QStringList columns;
    QList <QMap<QString, QString> > localDicList = DBHelper::getInstance()->select("EMI_InventoryDic", columns, "");
    if(localDicList.count() > 0)
        return;
    for (int i = 0; i < dicValueList.count(); i++) {
        DBHelper::getInstance()->insert("EMI_InventoryDic", dicValueList.at(i));
    }
}

void InventorySynchronizer::updateTaskSyncStatus(const QString &taskID, bool result){
    QMap<QString, QString> fieldValue;
    if(result)
        fieldValue["SyncStatus"] = TASK_STATUS_SYNC_FINISH;
    else
        fieldValue["SyncStatus"] = TASK_STATUS_SYNC_FAIL;
    QString updateArg = " where ID = '" + taskID + "'";
    DBHelper::getInstance()->update("EMI_InventoryTask", fieldValue, updateArg);
}

void InventorySynchronizer::resultProcess(const QString &action, bool result)
{
    if(m_nSyncTaskTotalCount == 0){
        m_nRunnings = false;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
        return;
    }
    result ? m_nSyncTaskSuccessCount++: m_nSyncTaskFailCount++;
    QString taskId = action;
    if(result){
        QString syncTime = m_nSyncTaskInfoMap[taskId].toMap()["TaskSyncTime"].toString();
        updateUserTaskTime(syncTime, taskId);
    }
    updateTaskSyncStatus(taskId, result);

    qDebug() << Q_FUNC_INFO << " result is " << result
             << " m_nSyncTaskSuccessCount " << m_nSyncTaskSuccessCount
             <<" m_nSyncTaskFailCount  " << m_nSyncTaskFailCount
            <<" m_nSyncTaskTotalCount " << m_nSyncTaskTotalCount;
    if (m_nSyncTaskSuccessCount + m_nSyncTaskFailCount == m_nSyncTaskTotalCount){
        if(m_nSyncTaskSuccessCount == m_nSyncTaskTotalCount){
            QString syncTime = getMaxTime();
            qDebug() << "updateTaskTime  syncTime is " << syncTime;
            updateTaskTime(syncTime);
        }
        m_nSyncTaskInfoMap.clear();
        m_nSyncTaskSuccessCount = 0;
        m_nSyncTaskFailCount = 0;
        m_nSyncTaskTotalCount = 0;
        m_nRunnings = false;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
    }
}

bool InventorySynchronizer::markLocalTaskObjectsFinished(const QString &taskID, const QStringList &taskObjectList)
{
    qDebug() << Q_FUNC_INFO << "  taskObjectList  " << taskObjectList;
    QString updateArgs = " where ID in ('" + taskObjectList.join("','") + "')";
    QMap<QString, QString> keyValue;
    keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
    DBHelper::getInstance()->update("EMI_InventoryEquipment", keyValue, updateArgs);
    checkLocalTaskFinished(taskID);
    return true;
}

void InventorySynchronizer::checkLocalTaskFinished(const QString &taskID)
{
    qDebug() << Q_FUNC_INFO << "  taskID " << taskID;
    QList<QMap<QString, QString> > valueList;
    QStringList column("ID");
    valueList = DBHelper::getInstance()->select("EMI_InventoryEquipment", column, " where TaskID = '" + taskID +  "' and LocalStatus != '2'");
    if(valueList.size() == 0){
        markLocalTaskFinished(taskID);
    }
}

void InventorySynchronizer::markLocalTaskFinished(const QString &taskID)
{
    qDebug() << Q_FUNC_INFO << " taskId " << taskID;
    DBHelper::getInstance()->deleteRow("EMI_InventoryTask", " where ID = '" + taskID + "'");
    DBHelper::getInstance()->deleteRow("EMI_InventoryEquipment", " where TaskID = '" + taskID + "'");
    //删除任务最后获取时间
    QString timeDeleteArg = " where TaskID = '" + taskID + "' and UserID = '"
            + RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID") + "'";
    DBHelper::getInstance()->deleteRow("EMI_Inventory_User_Task_Time", timeDeleteArg);
}

void InventorySynchronizer::updateUserTaskTime(const QString &time, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    QStringList columns;
    QString selectArg = " where UserID = '" + m_nSyncMap["UserID"].toString() + "' and TaskID = '" + taskId + "'";
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select("EMI_Inventory_User_Task_Time", columns, selectArg);
    if(valueList.count() == 0){
        QString id = createUuid();
        QMap<QString, QString> value;
        value["ID"] = id;
        value["UserID"] = m_nSyncMap["UserID"].toString();
        value["TaskID"] = taskId;
        value["LastTime"] = time;
        DBHelper::getInstance()->insert("EMI_Inventory_User_Task_Time", value);
    } else {
        QMap<QString, QString> value;
        value["LastTime"] = time;
        DBHelper::getInstance()->update("EMI_Inventory_User_Task_Time", value, selectArg);
    }
}

void InventorySynchronizer::updateTaskTime(const QString &time){
    qDebug() << Q_FUNC_INFO;
    QStringList columns;
    QString selectArg = " where UserID = '" + m_nSyncMap["UserID"].toString() + "'";
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select("EMI_Inventory_User_Time", columns, selectArg);
    if(valueList.count() == 0){
        QMap<QString, QString> value;
        value["UserID"] = m_nSyncMap["UserID"].toString();
        value["LastTime"] = time;
        DBHelper::getInstance()->insert("EMI_Inventory_User_Time", value);
    } else {
        QMap<QString, QString> value;
        value["LastTime"] = time;
        DBHelper::getInstance()->update("EMI_Inventory_User_Time", value, selectArg);
    }
}

QString InventorySynchronizer::getMaxTime()
{
    QString maxTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QVariantMap::iterator iter;
    for (iter = m_nSyncTaskInfoMap.begin(); iter != m_nSyncTaskInfoMap.end(); ++iter){
        QVariantMap tmpMap = iter.value().toMap();
        QString syncTime = tmpMap["TaskSyncTime"].toString();
        qDebug() << Q_FUNC_INFO << " syncTime " << syncTime;
        if(syncTime > maxTime)
            maxTime = syncTime;
    }
    qDebug() << "getmaxTime is " << maxTime;
    return maxTime;
}

void InventorySynchronizer::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    if(result == true && error == NETWORK_NO_ERROR){
        const DataPackage &dataPackage = map.dataPackage;
        //盘点离线任务数据包
        if(objects == EMI_Off_GetSomeRunningTasksPacket) {
            taskDataHandle(action, dataPackage);
        } else if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            resultProcess(action, true);
        }
    } else {
        resultProcess(action, false);
    }
}
