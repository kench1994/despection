#include "dailysynchronizer.h"
#include "../networkcontrol.h"
#include "../common/dbhelper.h"
#include "../DBConfig/dbconfigmanager.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../common/bussinessmap.h"
#include "../common/dbdefine.h"
#include <QDebug>
#include <QDateTime>
DailySynchronizer::DailySynchronizer()
{
    m_nDataOperationType = BUSINESS_TYPE_DAILY;
}

DailySynchronizer::~DailySynchronizer()
{
}

void DailySynchronizer::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
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
    if(addList.count() == 0){
        resultProcess(action, true);
        return;
    }

    // 同步任务 日常任务没有更新，只有新增或者删除
    synTask(addList, syncUserID, SYNC_TASK_ADD );
    m_nSyncTaskTotalCount = m_nSyncTaskInfoMap.size();
}

QString DailySynchronizer::getTaskLastSyncTime(const QString &userID, const QString &taskID){
    QString  syncDateTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QStringList columns;
    columns.append("LastTime");
    QString selectionArg = " where  TaskDataKey = '" + taskID + "' and UserID = '" + userID + "'";
    QList<QMap<QString, QString> > valueList;
    valueList = DBHelper::getInstance()->select("ISM_Inspection_Plan_Task_Time", columns, selectionArg);
    if(valueList.count() > 0){
        syncDateTime = valueList.at(0)["LastTime"];
    }
    return syncDateTime;
}

void DailySynchronizer::synTask(const QStringList &taskIdList, const QString &userId, DailySynchronizer::SyncTaskType syncType)
{
    qDebug() << Q_FUNC_INFO;
    for(int i = 0; i < taskIdList.count(); i++){
        QString taskId = taskIdList.at(i);
        DataPackage requestPackage;
        requestPackage.head.key = PACKAGE_HEAD_KEY;
        requestPackage.head.type = "1";
        requestPackage.head.objects = EISM_Off_GetSomeDailyTasksPacket;
        requestPackage.para["UserID"] = userId;
        requestPackage.para["LastDEXTime"] = getTaskLastSyncTime(userId, taskId);
        requestPackage.para["TaskType"] = QString::number((int)syncType);
        requestPackage.para["TaskDataKeys"] = taskId;
        QVariantMap valueMap;
        valueMap["TaskType"] = QString::number((int)syncType);
        valueMap["TaskSyncTime"] = "";
        m_nSyncTaskInfoMap.insert(taskId, valueMap);
        sendData(requestPackage, taskId);
    }
}

void DailySynchronizer::taskDataHandle(const QString &action, const DataPackage &dataPackage)
{
     if(dataPackage.tables.length() == 3 && dataPackage.tables.at(0).value.length() == 1){
         //更新任务信息
         DBHelper *dbHelper = DBHelper::getInstance();
         dbHelper->beginTransaction();
         QString taskId = dataPackage.tables.at(0).value.at(0)["TaskDataKey"];
         // 将添加的任务和修改的任务写入数据库
         const QList<QMap<QString, QString> > &taskValueList = dataPackage.tables.at(0).value;
         qDebug() << "updateTask taskId   is " << taskId;
         updateTask(taskValueList, taskId);

         // 将添加的任务和修改的任务写入数据库
         const QList<QMap<QString, QString> > &groupValueList = dataPackage.tables.at(1).value;
         qDebug() << "updateTaskGroup taskId   is " << taskId;
         updateTaskGroup(groupValueList, taskId);

         // 修改任务项表,并且返回需要同步的设备ID列表
         QMap<QString, QString> equipmentIds;
         const QList<QMap<QString, QString> > &objectValueList = dataPackage.tables.at(2).value;
         qDebug() << "updateObject taskId   is " << taskId;
         updateObject(objectValueList, equipmentIds, taskId);
         dbHelper->endTransaction();

         //请求相关设备列表
         qDebug() << "equipmentIds   is " << equipmentIds.size();
         bool rv =  requestObjs(equipmentIds, taskId);
         if(!rv)
             resultProcess(action, true);
     } else{
         qDebug() << Q_FUNC_INFO <<" data error";
         resultProcess(action, false);
     }
}

void DailySynchronizer::updateTask(const QList<QMap<QString, QString> > &taskValueList, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    for (int i = 0; i < taskValueList.count(); i++) {
        QMap<QString, QString> value = taskValueList.at(i);
        //记录任务的同步时间
        QString syncTaskTime = value["ServerDate"];
       QVariantMap valueMap = m_nSyncTaskInfoMap[taskId].toMap();
       valueMap["TaskSyncTime"] = syncTaskTime;
        m_nSyncTaskInfoMap[taskId] = valueMap;

        //更新巡检任务表
        QStringList columns;
        QString selectArg = " where TaskDataKey = '" + value["TaskDataKey"] + "'";
        QList<QMap<QString, QString> > localValueList = DBHelper::getInstance()->select("ISM_InspectionPlanTask", columns, selectArg);
        value[SYNC_STATUS] = TASK_STATUS_SYNC_ING;
        if(localValueList.count() == 0){
            DBHelper::getInstance()->insert("ISM_InspectionPlanTask", value);
        } else {
            DBHelper::getInstance()->update("ISM_InspectionPlanTask", value, selectArg);
        }
    }
}

void DailySynchronizer::updateTaskGroup(const QList<QMap<QString, QString> > &taskGroupValueList, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO <<" taskId " << taskId;
    //新增的任务组列表，添加到本地数据库
    for (int i = 0; i < taskGroupValueList.count(); i++) {
        QStringList columns("TaskGroupDataKey");
        QString selectArg = " where TaskGroupDataKey = '" + taskGroupValueList.at(i)["TaskGroupDataKey"] + "'";
        QList<QMap<QString, QString> > localValueList = DBHelper::getInstance()->select("ISM_InspectionPlanGroup", columns, selectArg);
        if(localValueList.size() == 0){
            DBHelper::getInstance()->insert("ISM_InspectionPlanGroup", taskGroupValueList.at(i));
        }else
            DBHelper::getInstance()->update("ISM_InspectionPlanGroup", taskGroupValueList.at(i), selectArg);
    }
}

void DailySynchronizer::updateObject(const QList<QMap<QString, QString> > &objectValueList,
                                         QMap<QString, QString> &objectIDList, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO << "  taskId " << taskId << "  size is " << objectValueList.size() << "  time is " << QDateTime::currentDateTime();
    //新增的任务项列表，添加到本地数据库
    for (int i = 0; i < objectValueList.count(); i++) {
        QString equipmentId = objectValueList.at(i)["RealObjectID"];
        objectIDList[equipmentId] = objectValueList.at(i)["RealObjectModifiedDate"];
        QStringList columns("PlanObjectID");
        QString selectArg = " where PlanObjectID = '" + objectValueList.at(i)["PlanObjectID"] + "'";
        QList<QMap<QString, QString> > localValueList = DBHelper::getInstance()->select("ISM_InspectionPlanObject", columns, selectArg);
        if(localValueList.size() == 0){
            DBHelper::getInstance()->insert("ISM_InspectionPlanObject", objectValueList.at(i));
        } else
            DBHelper::getInstance()->update("ISM_InspectionPlanObject", objectValueList.at(i), selectArg);
    }
    qDebug() << Q_FUNC_INFO << "   size is " << objectValueList.size() << "  time is " << QDateTime::currentDateTime();
}

void DailySynchronizer::updateTaskSyncStatus(const QString &taskID, bool success){
    QMap<QString, QString> fieldValue;
    if(success)
        fieldValue["SyncStatus"] = TASK_STATUS_SYNC_FINISH;
    else
        fieldValue["SyncStatus"] = TASK_STATUS_SYNC_FAIL;
    QString updateArg = " where TaskDataKey = '" + taskID + "'";
    DBHelper::getInstance()->update("ISM_InspectionPlanTask", fieldValue, updateArg);
}

void DailySynchronizer::resultProcess(const QString &action, bool result)
{
    if(m_nSyncTaskTotalCount == 0){
        m_nRunnings = false;
        emit onSyncFinishedSignal(BUSINESS_TYPE_DAILY, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
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
        if(m_nSyncTaskSuccessCount == m_nSyncTaskTotalCount && m_nSyncTaskTotalCount != 0){
            QString syncTime = getMaxTime();
            qDebug() << "update TaskTime  syncTime is " << syncTime;
            updateTaskTime(syncTime);
        }
        m_nSyncTaskInfoMap.clear();
        m_nSyncTaskSuccessCount = 0;
        m_nSyncTaskFailCount = 0;
        m_nSyncTaskTotalCount = 0;
        m_nRunnings = false;
        qDebug() << " on SyncFinishedSignal m_nSyncAction is " << m_nSyncAction<< "  m_nSyncObjects is  " << m_nSyncObjects << " m_nSyncMap " << m_nSyncMap;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
    }
}

void DailySynchronizer::updateUserTaskTime(const QString &time, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    QStringList columns;
    QString selectArg = " where UserID = '" + m_nSyncMap["UserID"].toString() + "' and TaskDataKey = '" + taskId + "'";
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select("ISM_Inspection_Plan_Task_Time", columns, selectArg);
    if(valueList.count() == 0){
        QString id = createUuid();
        QMap<QString, QString> value;
        value["ID"] = id;
        value["UserID"] = m_nSyncMap["UserID"].toString();
        value["TaskDataKey"] = taskId;
        value["LastTime"] = time;
        DBHelper::getInstance()->insert("ISM_Inspection_Plan_Task_Time", value);
    } else {
        QMap<QString, QString> value;
        value["LastTime"] = time;
        DBHelper::getInstance()->update("ISM_Inspection_Plan_Task_Time", value, selectArg);
    }
}

void DailySynchronizer::updateTaskTime(const QString &time){
    qDebug() << Q_FUNC_INFO;
    QStringList columns;
    QString selectArg = " where UserID = '" + m_nSyncMap["UserID"].toString() + "'";
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select("ISM_Inspection_Plan_Time", columns, selectArg);
    if(valueList.count() == 0){
        QMap<QString, QString> value;
        value["UserID"] = m_nSyncMap["UserID"].toString();
        value["LastTime"] = time;
        DBHelper::getInstance()->insert("ISM_Inspection_Plan_Time", value);
    } else {
        QMap<QString, QString> value;
        value["LastTime"] = time;
        DBHelper::getInstance()->update("ISM_Inspection_Plan_Time", value, selectArg);
    }
}

QString DailySynchronizer::getMaxTime()
{
    QString maxTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QVariantMap::iterator iter;
    for (iter = m_nSyncTaskInfoMap.begin(); iter != m_nSyncTaskInfoMap.end(); ++iter){
        QVariantMap tmpMap = iter.value().toMap();
        QString syncTime = tmpMap["TaskSyncTime"].toString();
        if(syncTime > maxTime)
            maxTime = syncTime;
    }
    qDebug() << Q_FUNC_INFO << " maxTime " << maxTime;
    return maxTime;
}

void DailySynchronizer::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    if(result == true && error == NETWORK_NO_ERROR){
        const DataPackage &dataPackage = map.dataPackage;
        //日常离线任务数据包
        if(objects == EISM_Off_GetSomeDailyTasksPacket) {
            taskDataHandle(action, dataPackage);
        } else if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            //更新任务同步状态为１（已完成）
            resultProcess(action, true);
        }
    } else {
        resultProcess(action, false);
    }
}
