#include "inspectionsynchronizer.h"
#include "../networkcontrol.h"
#include "../common/dbhelper.h"
#include "../DBConfig/dbconfigmanager.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../common/bussinessmap.h"
#include "../common/dbdefine.h"
#include "../framework/common/convertdata.h"
#include <QDebug>
#define SYNC_PRETASK "SyncPreTask_"
InspectionSynchronizer::InspectionSynchronizer()
{
    m_nDataOperationType = BUSINESS_TYPE_INSPECTION;
}

InspectionSynchronizer::~InspectionSynchronizer()
{
}

void InspectionSynchronizer::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
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

void InspectionSynchronizer::syncPreTask(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  action " << action;
    m_nSyncPreTaskPlanIDs.clear();
    m_nSyncPreTaskAction = action;
    m_nSyncPreTaskFailCount = 0;
    sendData(dataPackage, action);
}

QString InspectionSynchronizer::getTaskLastSyncTime(const QString &userID, const QString &taskID){
    QString  syncDateTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QStringList columns;
    columns.append("LastTime");
    QString selectionArg = " where  TaskDataKey = '" + taskID + "' and UserID = '" + userID + "'";
    QList<QMap<QString, QString> > valueList;
    valueList = DBHelper::getInstance()->select("ISM_Inspection_Task_Time", columns, selectionArg);
    if(valueList.count() > 0){
        syncDateTime = valueList.at(0)["LastTime"];
    }
    return syncDateTime;
}

void InspectionSynchronizer::synTask(const QStringList &taskIdList, const QString &userId, InspectionSynchronizer::SyncTaskType syncType)
{
    qDebug() << Q_FUNC_INFO;
    for(int i = 0; i < taskIdList.count(); i++){
        QString taskId = taskIdList.at(i);
        DataPackage requestPackage;
        requestPackage.head.key = PACKAGE_HEAD_KEY;
        requestPackage.head.type = "1";
        requestPackage.head.objects = EISM_Off_GetSomeRunningTasksPacket;
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

void InspectionSynchronizer::taskDataHandle(const QString &action, const DataPackage &dataPackage)
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
         bool result = requestObjs(equipmentIds, taskId);
         if(!result){
             resultProcess(action, true);
         }
     } else {
         qDebug() << Q_FUNC_INFO <<" data error";
         resultProcess(action, false);
     }
}

void InspectionSynchronizer::updateTask(const QList<QMap<QString, QString> > &taskValueList, const QString &taskId)
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
        QList<QMap<QString, QString> > localValueList = DBHelper::getInstance()->select("ISM_InspectionTask", columns, selectArg);
        value[SYNC_STATUS] = TASK_STATUS_SYNC_ING;
        QString taskType = m_nSyncTaskInfoMap[taskId].toMap()["TaskType"].toString();
        if(taskType == QString::number((int)SYNC_TASK_ADD))
            value[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
        if(localValueList.count() == 0){
            DBHelper::getInstance()->insert("ISM_InspectionTask", value);
        } else {
            DBHelper::getInstance()->update("ISM_InspectionTask", value, selectArg);
        }
    }
}

void InspectionSynchronizer::updateTaskGroup(const QList<QMap<QString, QString> > &taskGroupValueList, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    QString taskType = m_nSyncTaskInfoMap[taskId].toMap()["TaskType"].toString();
    if(taskType == QString::number((int)SYNC_TASK_UPDATE) && taskGroupValueList.size() > 0){
        //如果是更新的任务组列表，则说明该任务组已经被巡检，则在本地被标注为已完成，等待任务完成时再删除(原来是从本地数据库删除掉)
        QStringList taskGroupDataKeyList;
        for(int i = 0; i < taskGroupValueList.size(); i++){
            taskGroupDataKeyList.append(taskGroupValueList.at(i)["TaskGroupDataKey"]);
        }
        markLocalTaskGroupFinished(taskId, taskGroupDataKeyList);
    }else if(taskType == QString::number((int)SYNC_TASK_ADD) &&taskGroupValueList.size() > 0) {
        //如果是新增的任务组列表，添加到本地数据库
        for (int i = 0; i < taskGroupValueList.count(); i++) {
            QMap<QString, QString> value = taskGroupValueList.at(i);

            QStringList columns("TaskGroupDataKey");
            QString selectArg = " where TaskGroupDataKey = '" + value["TaskGroupDataKey"] + "'";
            QList<QMap<QString, QString> > localValueList = DBHelper::getInstance()->select("ISM_InspectionTaskGroup", columns, selectArg);
            if(localValueList.size() == 0){
                value[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
                DBHelper::getInstance()->insert("ISM_InspectionTaskGroup", value);
            }else
                DBHelper::getInstance()->update("ISM_InspectionTaskGroup", value, selectArg);
        }
    }
}

void InspectionSynchronizer::updateObject(const QList<QMap<QString, QString> > &objectValueList,
                                         QMap<QString, QString> &objectIDList, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    QString taskType = m_nSyncTaskInfoMap[taskId].toMap()["TaskType"].toString();
    if(taskType == QString::number((int)SYNC_TASK_UPDATE) && objectValueList.size() > 0){
        //如果是更新的任务项列表，则说明该任务项已经被巡检，则在本地被标注为已完成，等待任务完成时再删除(原来是从本地数据库删除掉)
        QStringList taskObjectDataKeyList;
        for(int i = 0; i < objectValueList.size(); i++){
            taskObjectDataKeyList.append(objectValueList.at(i)["TaskObjectDataKey"]);
        }
        markLocalTaskObjectsFinished(taskObjectDataKeyList);
    }else if(taskType == QString::number((int)SYNC_TASK_ADD) &&objectValueList.size() > 0) {
        //如果是新增的任务项列表，添加到本地数据库
        for (int i = 0; i < objectValueList.count(); i++) {
            QMap<QString, QString> value = objectValueList.at(i);

            QString equipmentId = value["RealObjectID"];
            objectIDList[equipmentId] = value["RealObjectModifiedDate"];
            QStringList columns("TaskObjectDataKey");
            QString selectArg = " where TaskObjectDataKey = '" + value["TaskObjectDataKey"] + "'";
            QList<QMap<QString, QString> > localValueList = DBHelper::getInstance()->select("ISM_InspectionTaskObject", columns, selectArg);
            if(localValueList.size() == 0){
                value[LOCAL_STATUS] = TASK_STATUS_NO_FINISH;
                DBHelper::getInstance()->insert("ISM_InspectionTaskObject", value);
            } else
                DBHelper::getInstance()->update("ISM_InspectionTaskObject", value, selectArg);
        }
    }
}

void InspectionSynchronizer::updateTaskSyncStatus(const QString &taskID, bool success){
    QMap<QString, QString> fieldValue;
    if(success)
        fieldValue["SyncStatus"] = TASK_STATUS_SYNC_FINISH;
    else
        fieldValue["SyncStatus"] = TASK_STATUS_SYNC_FAIL;
    QString updateArg = " where TaskDataKey = '" + taskID + "'";
    DBHelper::getInstance()->update("ISM_InspectionTask", fieldValue, updateArg);
}

void InspectionSynchronizer::resultProcess(const QString &action, bool result)
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

bool InspectionSynchronizer::markLocalTaskObjectsFinished(const QStringList &taskObjectList)
{
    qDebug() << Q_FUNC_INFO << "  taskObjectList  " << taskObjectList;
    //1.过滤出服务器已经完成，但是本地还没有提交或缓存的任务，用于更新本地任务进度数
    QString selectArgs = " where TaskObjectDataKey in ('" + taskObjectList.join("','") + "') and LocalStatus ='0' group by TaskGroupDataKey";
    QString selectSql = "select count(*) as noFinishedNum, TaskDataKey, TaskGroupDataKey from ISM_InspectionTaskObject " + selectArgs;
    QList<DataRow> tmpRow;
    DBHelper::getInstance()->select(selectSql, tmpRow);
    for(int i = 0 ; i < tmpRow.size(); i++){
        //更新组的已检查项
        updateLocalTaskGroupExamObjectCount(tmpRow.at(i)["TaskGroupDataKey"], tmpRow.at(i)["noFinishedNum"].toInt());
    }

    //2.更新本地任务项为已完成
    QString updateArgs = " where TaskObjectDataKey in ('" + taskObjectList.join("','") + "')";
    QMap<QString, QString> keyValue;
    keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
    DBHelper::getInstance()->update("ISM_InspectionTaskObject", keyValue, updateArgs);

    //3.查询出这些项的所属组,检查这些组是否已经完成
    QString strSql = "select distinct TaskGroupDataKey, TaskDataKey from ISM_InspectionTaskObject  " + updateArgs;
    QList<QMap<QString, QString> > taskGroupDataKeyMapList;
    DBHelper::getInstance()->select(strSql, taskGroupDataKeyMapList);
    if(taskGroupDataKeyMapList.size() > 0){
        QString taskDataKey = taskGroupDataKeyMapList.at(0)["TaskDataKey"];
        QStringList taskGroupDataKeyList;
        for(int i = 0; i < taskGroupDataKeyMapList.size(); i++)
            taskGroupDataKeyList.append(taskGroupDataKeyMapList.at(i)["TaskGroupDataKey"]);

        //检查组是否已经完成
        checkLocalTaskGroupFinished(taskDataKey, taskGroupDataKeyList);
    }
    return true;
}

bool InspectionSynchronizer::checkLocalTaskGroupFinished(const QString &taskDataKey, const QStringList &taskGroupDataKeyList)
{
    qDebug() << Q_FUNC_INFO << "  taskGroupDataKeyList " << taskGroupDataKeyList;
    for(int i = 0; i < taskGroupDataKeyList.size(); i++){
        QList<QMap<QString, QString> > valueList;
        QStringList column("TaskGroupDataKey");
        valueList = DBHelper::getInstance()->select("ISM_InspectionTaskObject", column, " where TaskGroupDataKey = '" + taskGroupDataKeyList.at(i) +  "' and LocalStatus != '2'");
        if(valueList.size() == 0){
            QString updateArgs = " where TaskGroupDataKey ='" + taskGroupDataKeyList.at(i) + "'";
            QMap<QString, QString> keyValue;
            keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
            DBHelper::getInstance()->update("ISM_InspectionTaskGroup", keyValue, updateArgs);
        }
    }
    checkLocalTaskFinished(taskDataKey);
    return true;
}

bool InspectionSynchronizer::markLocalTaskGroupFinished(const QString &taskDataKey, const QStringList &taskGroupDataKeyList)
{
    qDebug() << Q_FUNC_INFO << "  taskGroupDataKeyList " << taskGroupDataKeyList;
    QString updateArgs = " where TaskGroupDataKey in ('" + taskGroupDataKeyList.join("','") + "')";
    QMap<QString, QString> keyValue;
    keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
    DBHelper::getInstance()->update("ISM_InspectionTaskGroup", keyValue, updateArgs);
    DBHelper::getInstance()->update("ISM_InspectionTaskObject", keyValue, updateArgs);
    for(int i = 0; i < taskGroupDataKeyList.size(); i++){
        //更新组的已检查项
        updateLocalTaskGroupExamObjectCount(taskGroupDataKeyList.at(i), 0, true);
    }
    checkLocalTaskFinished(taskDataKey);
    return true;
}

void InspectionSynchronizer::checkLocalTaskFinished(const QString &taskDataKey)
{
    qDebug() << Q_FUNC_INFO << "  taskDataKey " << taskDataKey;
    QList<QMap<QString, QString> > valueList;
    QStringList column("TaskDataKey");
    valueList = DBHelper::getInstance()->select("ISM_InspectionTaskGroup", column, " where TaskDataKey = '" + taskDataKey +  "' and LocalStatus != '2'");
    if(valueList.size() == 0){
        QStringList taskDataKeyList(taskDataKey);
        markLocalTaskFinished(taskDataKeyList);
    }
}

void InspectionSynchronizer::markLocalTaskFinished(const QStringList &taskDataKeyList)
{
    QString taskDataKeys = taskDataKeyList.join("','");
    qDebug() << Q_FUNC_INFO << "  taskDataKey " << taskDataKeyList;
    DBHelper::getInstance()->deleteRow("ISM_InspectionTask", " where TaskDataKey in ('" + taskDataKeys + "')");
    DBHelper::getInstance()->deleteRow("ISM_InspectionTaskGroup", " where TaskDataKey in ('" + taskDataKeys + "')");
    DBHelper::getInstance()->deleteRow("ISM_InspectionTaskObject", " where TaskDataKey in ('" + taskDataKeys + "')");
    // 4. 删除任务最后获取时间
    QString timeDeleteArg = " where TaskDataKey in ('" + taskDataKeys + "') and UserID = '"
            + RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID") + "'";
    DBHelper::getInstance()->deleteRow(INSPECTION_TABLE_USER_TASK_TIME, timeDeleteArg);
}

void InspectionSynchronizer::updateLocalTaskGroupExamObjectCount(const QString &taskGroupDataKey, int count, bool all)
{
    qDebug() << Q_FUNC_INFO << "  taskGroupDataKey " << taskGroupDataKey << "  count is " << count << "  all " << all;
    QString updateArgs = " where TaskGroupDataKey ='" + taskGroupDataKey + "'";
    QList<QMap<QString, QString> > examCount;
    QString strSql = "select ExamObjectCount, TotalObjectCount  from ISM_InspectionTaskGroup " + updateArgs;
    DBHelper::getInstance()->select(strSql, examCount);
    if(examCount.size() > 0){
        QMap<QString, QString> examMap;
        if(all)
            examMap["ExamObjectCount"]  = examMap["TotalObjectCount"];
        else
            examMap["ExamObjectCount"] = QString::number(examCount.at(0)["ExamObjectCount"].toInt() + count);

        DBHelper::getInstance()->update("ISM_InspectionTaskGroup", examMap, updateArgs);
    }
}

void InspectionSynchronizer::updateUserTaskTime(const QString &time, const QString &taskId)
{
    qDebug() << Q_FUNC_INFO;
    QStringList columns;
    QString selectArg = " where UserID = '" + m_nSyncMap["UserID"].toString() + "' and TaskDataKey = '" + taskId + "'";
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select("ISM_Inspection_Task_Time", columns, selectArg);
    if(valueList.count() == 0){
        QString id = createUuid();
        QMap<QString, QString> value;
        value["ID"] = id;
        value["UserID"] = m_nSyncMap["UserID"].toString();
        value["TaskDataKey"] = taskId;
        value["LastTime"] = time;
        DBHelper::getInstance()->insert("ISM_Inspection_Task_Time", value);
    } else {
        QMap<QString, QString> value;
        value["LastTime"] = time;
        DBHelper::getInstance()->update("ISM_Inspection_Task_Time", value, selectArg);
    }
}

void InspectionSynchronizer::updateTaskTime(const QString &time){
    qDebug() << Q_FUNC_INFO;
    QStringList columns;
    QString selectArg = " where UserID = '" + m_nSyncMap["UserID"].toString() + "'";
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select("ISM_Inspection_Time", columns, selectArg);
    if(valueList.count() == 0){
        QMap<QString, QString> value;
        value["UserID"] = m_nSyncMap["UserID"].toString();
        value["LastTime"] = time;
        DBHelper::getInstance()->insert("ISM_Inspection_Time", value);
    } else {
        QMap<QString, QString> value;
        value["LastTime"] = time;
        DBHelper::getInstance()->update("ISM_Inspection_Time", value, selectArg);
    }
}

QString InspectionSynchronizer::getMaxTime()
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

void InspectionSynchronizer::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << " objects is " << objects << "  result is " << result;
    if(result == true && error == NETWORK_NO_ERROR){
        const DataPackage &dataPackage = map.dataPackage;
        //离线任务数据包
        if(objects == EISM_Off_GetSomeRunningTasksPacket) {
            taskDataHandle(action, dataPackage);
        } else if(objects == EISM_Off_GetPrePlanIDsByUserID){
            handlePreTaskPlanIDs(dataPackage);
        }else if(objects == EISM_Off_GetPrePacketByPlanID){
            handlePreTasks(dataPackage, action);
        }else if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            //更新任务同步状态为１（已完成）
            if(action.contains(SYNC_PRETASK))
                preTaskResultProcess(action, objects, true);
            else
                resultProcess(action, true);
        }
    } else {
        //巡检同步离线任务,　或者离线任务同步设备
        if(objects == EISM_Off_GetSomeRunningTasksPacket || (objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs && !action.contains(SYNC_PRETASK))){
            resultProcess(action, false);
        } else {
            //未来任务同步，或者未来任务同步设备
            preTaskResultProcess(action, objects, false);
        }
    }
}

void InspectionSynchronizer::handlePreTaskPlanIDs(const DataPackage &dataPackage) {
    DbusPackage map;
    map.dataPackage.head.objects = EISM_Off_GetPrePlanIDsByUserID;
    const QList<DataRow> &dataRows = dataPackage.tables[0].value;
    if(!dataRows.isEmpty()){
        for(int i = 0; i < dataRows.size(); i++){
            const DataRow &dataRow = dataRows.at(i);
            QString planID = dataRow["PlanID"];
            DataPackage dataPackage;
            dataPackage.head.type = TYPE_REQUEST;
            dataPackage.head.key = PACKAGE_HEAD_KEY;
            dataPackage.head.objects = EISM_Off_GetPrePacketByPlanID;
            dataPackage.para["PlanID"] = planID;
            dataPackage.para["PreBuildEndTimeStr"] = dataRow["PreBuildEndTimeStr"];
            m_nSyncPreTaskPlanIDs.append(planID);
            sendData(dataPackage, planID);
        }
    } else {
        preTaskResultProcess("", EISM_Off_GetPrePlanIDsByUserID, true);
    }
}

void InspectionSynchronizer::handlePreTasks(const DataPackage &dataPackage, const QString &planID){
    const QList<DataTable> &tables = dataPackage.tables;
    QStringList columns;
    QString selectArg;
    QList<DataRow> list;

    qint64 time1 = QDateTime::currentMSecsSinceEpoch();
    if(!tables.isEmpty() && !tables[0].value.isEmpty()) {
        const QList<DataRow> &dataRows0 = tables[0].value;
        columns.append(PLAN_ID_CREATE_TIME);
        columns.append(TASK_DATA_KEY);
        selectArg = " where " + QString(PLAN_ID) + "='" + planID + "'";
        list =  m_nDbHelper->select(MAINTENANCE_TABLE_TASK, columns, selectArg);
        if(!list.isEmpty() && dataRows0[0][PLAN_ID_CREATE_TIME] !=  list[0][PLAN_ID_CREATE_TIME]){
                QStringList taskDataKeys;
                for(int m = 0; m < list.size(); m++){
                    taskDataKeys.append(list[m][TASK_DATA_KEY]);
                }
                markLocalTaskFinished(taskDataKeys);
        }

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
                //<Field Value="TaskCodeやTaskNameやLimitModeやEnableUploadAttachやPlanIDやInspectionModeやCreatedTime"/>
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
                    m_nDbHelper->select("select TaskDataKey from " + QString(INSPECTION_TABLE_TASK) + selectArg, list);
                    list = m_nDbHelper->select(INSPECTION_TABLE_TASK, columns, selectArg);
                    if(!list.isEmpty()){
                        m_nDbHelper->update(INSPECTION_TABLE_TASK, taskUpdateRow, selectArg);
                    }else{
                        m_nDbHelper->insert(INSPECTION_TABLE_TASK, taskUpdateRow);
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
                            list = m_nDbHelper->select(INSPECTION_TABLE_TASK_GROUP, columns, selectArg);
                            if(!list.isEmpty()){
                                m_nDbHelper->update(INSPECTION_TABLE_TASK_GROUP, groupUpdateRow, selectArg);
                            }else{
                                m_nDbHelper->insert(INSPECTION_TABLE_TASK_GROUP, groupUpdateRow);
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
                        list = m_nDbHelper->select(INSPECTION_TABLE_TASK_OBJECT, columns, selectArg);
                        if(!list.isEmpty()){
                            m_nDbHelper->update(INSPECTION_TABLE_TASK_OBJECT, objectUpdateRow, selectArg);
                        }else{
                            m_nDbHelper->insert(INSPECTION_TABLE_TASK_OBJECT, objectUpdateRow);
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
                preTaskResultProcess(planID, dataPackage.head.objects, true);
            }
    }else{
        preTaskResultProcess(planID, dataPackage.head.objects, true);
    }
    qint64 time2 = QDateTime::currentMSecsSinceEpoch();
    qDebug() << Q_FUNC_INFO << " time2-time1 : " << time2 - time1;
}

void InspectionSynchronizer::preTaskResultProcess(const QString &action, const QString &object, const bool &result)
{
    qDebug() << Q_FUNC_INFO << "  action " << action << "    object  "<< object <<   "  result " << result;
    if(object == EISM_Off_GetPrePlanIDsByUserID){
        DbusPackage map;
        map.dataPackage.head.objects = EISM_Off_GetPrePlanIDsByUserID;
        emit onResultOfResponse(EISM_Off_GetPrePlanIDsByUserID, m_nSyncPreTaskAction, result, result? 0 : -1, map);
    } else {
        QString planID = action;
        if(object == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            int index = planID.indexOf(SYNC_PRETASK);
            if(index != -1){
                planID = planID.mid(index + strlen(SYNC_PRETASK));
            }
        }
        ///Step 1. 清除数据
        m_nSyncPreTaskPlanIDs.removeOne(planID);
        ///Step 2. 失败计数
        if(!result){
            m_nSyncPreTaskFailCount++;
        }
        ///更新任务状态
        updateTaskStatusByPlanID(planID, result);

        if(m_nSyncPreTaskPlanIDs.isEmpty()) {
            DbusPackage map;
            map.dataPackage.head.objects = EISM_Off_GetPrePlanIDsByUserID;
            if(m_nSyncPreTaskFailCount == 0){
                emit onResultOfResponse(EISM_Off_GetPrePlanIDsByUserID, m_nSyncPreTaskAction, true, 0, map);
            }else{
                emit onResultOfResponse(EISM_Off_GetPrePlanIDsByUserID, m_nSyncPreTaskAction, false, -1, map);
            }
        }
    }
}

void InspectionSynchronizer::updateTaskStatusByPlanID(const QString &planID, bool success){
    qDebug() <<Q_FUNC_INFO << "  planID " << planID;
    QString syncStatus = success ? TASK_STATUS_SYNC_FINISH: TASK_STATUS_SYNC_FAIL;
    DataRow dataRow;
    dataRow.insert(SYNC_STATUS, syncStatus);
    QString updateArg = " where " + QString(PLAN_ID) + "='" + planID + "'";
    m_nDbHelper->update(INSPECTION_TABLE_TASK, dataRow, updateArg);
}
