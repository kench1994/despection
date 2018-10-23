#include "inspectionhandler.h"
#include "../common/bussinessmap.h"
#include "../common/dbhelper.h"
#include "../common/comparetool.h"
#include "../common/dataoperationtype.h"
#include "../networkcontrol.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include <QDebug>
InspectionHandler::InspectionHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_INSPECTION;
}

InspectionHandler::~InspectionHandler()
{
}

void InspectionHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  dataPackage.para  " << dataPackage.para;
    QString objects = dataPackage.head.objects;
    if(objects == EISM_GetUserRunningTask){
        QString userID = dataPackage.para["UserID"];
        QString lastDEXTime = queryLastTime(userID);
        DataPackage data;
        data.head = dataPackage.head;
        data.head.objects = EISM_Off_GetUserRunningTaskDataKeys;
        data.para.insert(USER_ID, userID);
        data.para.insert(LAST_DEX_TIME, lastDEXTime);
        sendData(data, action);
    } else {
        sendData(dataPackage, action);
    }
}

void InspectionHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << dataPackage.head.objects << "  action " << action;
    DbusPackage dbusPackage;
    if(dataPackage.head.objects == EISM_GetUserRunningTask)
        getLocalTasks(dataPackage, dbusPackage.dataPackage);
    else if(dataPackage.head.objects == EISM_GetTaskGroupList)
        getLocalTaskGroup(dataPackage, dbusPackage.dataPackage);
    else if(dataPackage.head.objects == EISM_GetTaskObjectListByTask || dataPackage.head.objects == EISM_GetTaskObjectListByGroup)
        getLocalTaskObject(dataPackage, dbusPackage.dataPackage);
    else if(dataPackage.head.objects == EISM_GetTaskObjectDetail)
        getLocalTaskObjectDetail(dataPackage, dbusPackage.dataPackage);
    m_nRequestPkgMap.remove(action);
    qDebug() << Q_FUNC_INFO << " resultPackage is " << dbusPackage.dataPackage.tables.at(0).value;
    emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

int InspectionHandler::getItemCount()
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
            " from ISM_InspectionTask t"
            " inner join ISM_InspectionTaskGroup tg on tg.TaskDataKey=t.TaskDataKey"
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

void InspectionHandler::cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID)
{
    /**
    *  Step 1. 删除任务-用户-时间表
    */
    QString selectArg = " where " + QString(USER_ID) + userID + "'";
    m_nDbHelper->deleteRow(INSPECTION_TABLE_USER_TASK_TIME, selectArg);

    /**
    * Step 2. 删除业务-用户-时间表
    */
    m_nDbHelper->deleteRow(INSPECTION_TABLE_USER_TIME, selectArg);

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
                m_nDbHelper->deleteRow(INSPECTION_TABLE_TASK_GROUP, selectArg);
            }
        }
    }

    /**
    * Step 4. 删除任务项
    */
    selectArg = " where ISM_InspectionTaskObject.TaskGroupDataKey not in (select TaskGroupDataKey from ISM_InspectionTaskGroup)";
    m_nDbHelper->deleteRow(INSPECTION_TABLE_TASK_OBJECT, selectArg);

    /**
    * Step 5. 删除任务
    */
    QStringList column;
    QList<DataRow> list = m_nDbHelper->select(INSPECTION_TABLE_TASK, column, "");
    if(!list.isEmpty()){
        for(int i = 0 ; i < list.size(); i++){
            QString taskDataKey = list.at(i)[TASK_DATA_KEY];
            selectArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            QList<DataRow> list1 = m_nDbHelper->select(INSPECTION_TABLE_TASK_GROUP, column, selectArg);
            if(list1.isEmpty()) {
                m_nDbHelper->deleteRow(INSPECTION_TABLE_TASK, selectArg);
            }
        }
    }
    Q_UNUSED(organiseUnitID)
    Q_UNUSED(departmentID)
}

void InspectionHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    Q_UNUSED(objects)
    qDebug() <<Q_FUNC_INFO;
    DataPackage originalPkg = m_nRequestPkgMap[action];
    offLine(action, originalPkg);
}

void InspectionHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        if(objects == EISM_Off_GetUserRunningTaskDataKeys)
            handleTaskPatch(objects, action, map.dataPackage);
        else if(objects == EISM_GetTaskGroupList){
            handleTaskGroupPatch(objects, action, map.dataPackage);
        } else if(objects == EISM_GetTaskObjectListByTask || objects == EISM_GetTaskObjectListByGroup ){
            handleTaskObjectPatch(objects, action, map.dataPackage);
        } else if(objects == EISM_GetTaskObjectDetail){
            m_nRequestPkgMap.remove(action);
            emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, map);
        }
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}

bool InspectionHandler::getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    qDebug() << Q_FUNC_INFO << requestPackage.head.objects;
    QString userId = requestPackage.para["UserID"];
    QString strSql = "select distinct "
            "t.TaskDataKey,"
            "t.TaskCode,"
            "t.TaskName,"
            "t.LimitMode,"
            "t.EnableUploadAttach,"
            "t.IsPreTask,"
            "t.TaskStartTime,"
            "t.TaskEndTime,"
            "t.TaskStartTime|| '至' ||  t.TaskEndTime Circle,"
            "sum(tg.ExamObjectCount)|| '/' || sum(tg.TotalObjectCount) Rate,"
            "t.SyncStatus,"
            "t.InspectionMode "
            " from ISM_InspectionTask t"
            " inner join ISM_InspectionTaskGroup tg on tg.TaskDataKey=t.TaskDataKey"
            " where t.LocalStatus='0'"
            " and tg.TotalObjectCount > tg.ExamObjectCount"
            " and datetime(TaskStartTime) < datetime('now','localtime') and  datetime(TaskEndTime) > datetime('now','localtime')"
            " and tg.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userId + "') "
            " group by t.TaskDataKey"
            " order by t.TaskStartTime desc";
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionTask", "TaskDataKey", resultPackage);
    return true;
}
//<Field Value="TaskGroupDataKeyやTaskDataKeyやTaskGroupNameやTotalObjectCountやExamObjectCountやRFIDCodeやBarCodeやQRCodeやOrderIndex"/>
bool InspectionHandler::getLocalTaskGroup(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString userID = requestPackage.para["UserID"];
    QString taskDataKey = requestPackage.para["TaskDataKey"];
    QString strSql =  "select distinct "
            "tg.TaskGroupDataKey,"
            "tg.TaskDataKey,"
            "tg.TaskGroupName,"
            "tg.BarCode,"
            "tg.QRCode,"
            "tg.RFIDCode,"
            "tg.TotalObjectCount,"
            "tg.ExamObjectCount,"
            "tg.OrderIndex"
            " from ISM_InspectionTaskGroup tg"
            " where tg.TaskDataKey='" + taskDataKey + "'"
            " and tg.LocalStatus='0'"
            " and tg.TotalObjectCount > tg.ExamObjectCount"
            " and tg.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userID + "')"
            " and tg.TaskGroupDataKey in(select TaskGroupDataKey from ISM_InspectionTaskObject where TaskGroupDataKey=tg.TaskGroupDataKey)";

    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    qDebug() << Q_FUNC_INFO << valueList;
    if(!rv)
        return false;

    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionTaskGroup", "TaskGroupDataKey", resultPackage);
    return true;
}
//<Field Value="RealObjectIDやTaskObjectDataKeyやObjectNameやObjectFullNameやRealObjectTypeや
//RFIDCodeやBarCodeやQRCodeやOrderIndexやSerialKeyやIsDelegateやPositionsやModelNameやDeviceLastModifiedDate"/>
bool InspectionHandler::getLocalTaskObject(const DataPackage &requestPackage, DataPackage &resultPackage)
{

    QString strSql = "select distinct "
                        "taskobj.RealObjectID,"
                        "taskobj.TaskObjectDataKey,"
                        "taskobj.ObjectName,"
                        "taskobj.ObjectFullName,"
                        "taskobj.RealObjectType,"
                        "case when taskobj.RealObjectType='1' then equipment.RFIDCode else parts.RFIDCode end as RFIDCode,"
                        "case when taskobj.RealObjectType='1' then equipment.BarCode else parts.BarCode end as BarCode,"
                        "case when taskobj.RealObjectType='1' then equipment.QRCode  else parts.QRCode end as QRCode,"
                        "taskobj.OrderIndex,"
                        "taskobj.SerialKey,"
                        "taskobj.IsDelegate,"
                        "equipment.Positions,"
                        "equipment.ModelName,"
                        "taskobj.DeviceLastModifiedDate"
                        " from ISM_InspectionTaskObject taskobj ";
    if(requestPackage.head.objects == EISM_GetTaskObjectListByTask){
        QString taskDatakey = requestPackage.para[TASK_DATA_KEY];
        QString userId = requestPackage.para["UserID"];
        strSql +=  " inner join ISM_InspectionTaskGroup taskgroup on taskgroup.TaskGroupDataKey=taskobj.TaskGroupDataKey "
                   "left join EMB_Equipment equipment on taskobj.RealObjectID=equipment.ID and taskobj.RealObjectType='1' "
                   "left join EMB_Equipment_Part parts on taskobj.RealObjectID=parts.ID and taskobj.RealObjectType='2' "
                   "where taskgroup.TaskDataKey=  '" + taskDatakey + "' "
                   " and taskobj.LocalStatus='0'"
                   "and taskgroup.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userId +  "') "
                   "group by taskobj.TaskObjectDataKey";
    } else if(requestPackage.head.objects == EISM_GetTaskObjectListByGroup){
        QString taskGroupDataKey = requestPackage.para["TaskGroupDataKey"];
        strSql += " left join EMB_Equipment equipment on taskobj.RealObjectID=equipment.ID and taskobj.RealObjectType='1'"
                  " left join EMB_Equipment_Part parts on taskobj.RealObjectID=parts.ID and taskobj.RealObjectType='2'"
                  " where taskobj.TaskGroupDataKey='" + taskGroupDataKey + "'"
                  " and taskobj.LocalStatus='0'"
                  " order by taskobj.IsDelegate desc,taskobj.OrderIndex";
    } else
        return false;
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    qDebug() << Q_FUNC_INFO << valueList;
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionTaskGroup", "TaskGroupDataKey", resultPackage);
    return true;
}
//<Field Value="RealObjectIDやTaskObjectDataKeyやObjectNameやObjectFullNameやItemSchemaやRealObjectTypeやPositionsや
//CustodianやModelNameやDetaileやEorPCodeやRFIDCodeやBarCodeやQRCodeやOrderIndexやSerialKey"/>
bool InspectionHandler::getLocalTaskObjectDetail(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString taskObjectDataKey = requestPackage.para["TaskObjectDataKey"];
    QString strSql  = "select distinct "
                        "taskobj.RealObjectID,"
                        "taskobj.TaskObjectDataKey,"
                        "taskobj.ObjectName,"
                        "taskobj.ObjectFullName,"
                        "taskobj.ItemSchema,"
                        "taskobj.RealObjectType,"
                        "taskobj.SerialKey,"
                        "taskobj.OrderIndex,"
                        "equipment.Positions,"
                        "equipment.Custodian,"
                        "equipment.ModelName,"
                        "equipment.Detaile,"
                        "case when taskobj.RealObjectType='1' then equipment.EquipmentCode else parts.PartCode end as EorPCode,"
                        "case when taskobj.RealObjectType='1' then equipment.RFIDCode else parts.RFIDCode end as RFIDCode,"
                        "case when taskobj.RealObjectType='1' then equipment.BarCode else parts.BarCode end as BarCode,"
                        "case when taskobj.RealObjectType='1' then equipment.QRCode else parts.QRCode end as QRCode "
                        " from ISM_InspectionTaskObject taskobj"
                        " left join EMB_Equipment equipment on taskobj.RealObjectID=equipment.ID and taskobj.RealObjectType='1'"
                        "left join EMB_Equipment_Part parts on taskobj.RealObjectID=parts.ID and taskobj.RealObjectType='2'"
                        " where taskobj.TaskObjectDataKey='" + taskObjectDataKey + "'";
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    qDebug() << Q_FUNC_INFO << valueList;
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionTaskGroup", "TaskGroupDataKey", resultPackage);
    return true;
}

void InspectionHandler::handleTaskPatch(const QString &objects, const QString &action, const DataPackage &data)
{
    DataPackage originalRequestPkg = m_nRequestPkgMap[action];
    CompareTool compareTool;
    compareTool.compare(data, "TaskDataKey");
    //step 1. 需要删除的任务 本地存在 服务器已经提交的任务
    QStringList deleteList = compareTool.getDeleteList();
    qDebug() << "dalete is " << deleteList;
    qDebug() << "addList is " << compareTool.getAddList();
    qDebug() << "updateList is " << compareTool.getUpdateList();

    if (deleteList.count() > 0) {
        DBHelper *dbHelper = DBHelper::getInstance();
        for(int i = 0; i < deleteList.count(); i++){
            dbHelper->beginTransaction();
            markLocalTaskFinished(deleteList.at(i));
            dbHelper->endTransaction();
        }
    }

    QStringList addList = compareTool.getAddList();
    QStringList updateList = compareTool.getUpdateList();
    bool needSync = addList.count() > 0 || updateList.count() > 0;
    if(needSync){
        DBHelper *dbHelper = DBHelper::getInstance();
        // step 2 : 根据从服务器获取的更新任务列表，将本地对应的任务的状态设置为正在更新
        for (int i = 0; i < updateList.count(); i++) {
            QMap<QString, QString> keyValue;
            keyValue["SyncStatus"] = "0";
            dbHelper->update(INSPECTION_TABLE_TASK, keyValue, " where TaskDataKey = '" + updateList.at(i) + "'");
        }

        //step 3 : 将服务器新增的任务列表和本地获取的任务列表合并，返回给app
        if(addList.count() > 0){
            DataPackage resultdata;
            getLocalTasks(originalRequestPkg, resultdata);
            qDebug() << "resultdata.tables count is " << resultdata.tables.count();
            QList<DataRow > &localValueList =  resultdata.tables[0].value; //本地数据
            const QList<DataRow > &netValueList = data.tables[0].value; //网络数据
            qDebug() << Q_FUNC_INFO << " getLocalTasks localValueList count is  " << localValueList.count() << "  netValueList is " << netValueList.count();

            for(int i = 0; i < addList.count(); i++){
                QString taskDataKey = addList.at(i);
                bool isInLocal = false;
                for(int k = 0; k < localValueList.count(); k++){
                    if(localValueList.at(k)["TaskDataKey"] == taskDataKey){
                        isInLocal = true;
                        break;
                    }
                }
                if(isInLocal){
                    continue;
                }
//"TaskDataKeyやTaskCodeやTaskNameやLimitModeやEnableUploadAttachやPlanIDやPlanStrategyやInspectionModeやTaskStartTimeやTaskEndTimeやRateやCircle"
                for(int j = 0; j < netValueList.count(); j++){
                    DataRow netValue = netValueList.at(j);
                    if( netValue["TaskDataKey"] == taskDataKey){
                        netValue["SyncStatus"] = TASK_STATUS_SYNC_ING;
                        netValue["TaskCode"] = "0";
                        netValue["LimitMode"] = "0";
                        netValue["EnableUploadAttach"] = "0";
                        netValue["PlanID"] = "0";
                        netValue["InspectionMode"] = "1";
                        netValue["Circle"] = "- 至 -";
                        netValue["Rate"] = "- / -";
                        localValueList.append(netValue);
                        break;
                    }
                }
                qDebug() << "localValueList  count is " << localValueList.count();
            }

            qDebug() << Q_FUNC_INFO << " after resultData localValueList count is  " << resultdata.tables[0].value.count();
            DbusPackage dbusPackage;
            dbusPackage.dataPackage = resultdata;
            emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, dbusPackage);
        }
        QVariantMap syncMap;
        syncMap["UserID"] = originalRequestPkg.para["UserID"];
        syncMap["addIDs"] = QVariant(addList);
        syncMap["updateIDs"] = QVariant(updateList);
        emit onBeginSyncSignal(MESSAGE_SYNC_ADDTION, action, originalRequestPkg.head.objects, syncMap);
    } else {
        offLine(action, originalRequestPkg);
    }
}

//xunjian "TaskGroupDataKeyやTaskDataKeyやTaskGroupNameやPostIDやTotalObjectCountやExamObjectCountやRFIDCodeやBarCodeやQRCodeやOrderIndex"/>
void InspectionHandler::handleTaskGroupPatch(const QString &objects, const QString &action, const DataPackage &data)
{
    Q_UNUSED(objects)
    DataPackage localPackage;
    DataPackage originalPkg = m_nRequestPkgMap[action];
    getLocalTaskGroup(originalPkg, localPackage);
    CompareTool compareTool;
    QStringList netFinishedIDList = compareTool.compare(localPackage.tables.at(0).value, data.tables.at(0).value, TASK_GROUP_DATA_KEY);
    qDebug() << Q_FUNC_INFO<< "  netFinishedIDList is " << netFinishedIDList;
    if(netFinishedIDList.size() > 0){
        DBHelper::getInstance()->beginTransaction();
        QString taskDataKey = originalPkg.para[TASK_DATA_KEY];
        markLocalTaskGroupFinished(taskDataKey, netFinishedIDList);
        DBHelper::getInstance()->endTransaction();
    }
    offLine(action, originalPkg);
}
//"RealObjectIDやTaskObjectDataKeyやObjectNameやObjectFullNameやRealObjectTypeや
//RFIDCodeやBarCodeやQRCodeやOrderIndexやSerialKeyやIsDelegateやPositionsやModelNameやDeviceLastModifiedDate"
void InspectionHandler::handleTaskObjectPatch(const QString &objects, const QString &action, const DataPackage &data)
{
    Q_UNUSED(objects)
    DataPackage localPackage;
    DataPackage originalPkg = m_nRequestPkgMap[action];
    getLocalTaskObject(originalPkg, localPackage);
    const QList<DataRow> &netValus = data.tables.at(0).value;
    CompareTool compareTool;
    QStringList netFinishedIDList = compareTool.compare(localPackage.tables.at(0).value, data.tables.at(0).value, TASK_OBJECT_DATA_KEY);
    qDebug() << Q_FUNC_INFO<< "  netFinishedIDList is " << netFinishedIDList;
    if(netFinishedIDList.size() > 0){
        DBHelper::getInstance()->beginTransaction();
        markLocalTaskObjectsFinished(netFinishedIDList);
        DBHelper::getInstance()->endTransaction();
    }
    offLine(action, originalPkg);

    qDebug() << Q_FUNC_INFO << "netValus is " << netValus.size();
    QMap<QString, QString> deviceInfo;
    for(int i = 0; i < netValus.size(); i++){
        const DataRow& nv = netValus.at(i);
        deviceInfo.insert(nv["RealObjectID"], nv["DeviceLastModifiedDate"]);
    }
    qDebug() << Q_FUNC_INFO << "deviceInfo   is " << deviceInfo;
    requestObjs(deviceInfo, action);
}

bool InspectionHandler::markLocalTaskObjectsFinished(const QStringList &taskObjectList)
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

bool InspectionHandler::checkLocalTaskGroupFinished(const QString &taskDataKey, const QStringList &taskGroupDataKeyList)
{
    qDebug() << Q_FUNC_INFO << "  taskGroupDataKeyList  " << taskGroupDataKeyList;
    bool updated = false;
    for(int i = 0; i < taskGroupDataKeyList.size(); i++){
        QString strSql = "select TaskGroupDataKey from ISM_InspectionTaskObject where TaskGroupDataKey = '" + taskGroupDataKeyList.at(i) +  "' and LocalStatus != '2'";
        QList<QMap<QString, QString> > valueList;
        DBHelper::getInstance()->select(strSql, valueList);
        if(valueList.size() == 0){
            QString updateArgs = " where TaskGroupDataKey ='" + taskGroupDataKeyList.at(i) + "'";
            QMap<QString, QString> keyValue;
            keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
            DBHelper::getInstance()->update("ISM_InspectionTaskGroup", keyValue, updateArgs);
            updated = true;
        }
    }

    if(updated)
        checkLocalTaskFinished(taskDataKey);
    return true;
}

bool InspectionHandler::markLocalTaskGroupFinished(const QString &taskDataKey, const QStringList &taskGroupDataKeyList)
{
    qDebug() << Q_FUNC_INFO << "  taskGroupDataKeyList  " << taskGroupDataKeyList;
    QString updateArgs = " where TaskGroupDataKey in ('" + taskGroupDataKeyList.join("','") + "')";
    QMap<QString, QString> keyValue;
    keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
    DBHelper::getInstance()->update("ISM_InspectionTaskGroup", keyValue, updateArgs);
    DBHelper::getInstance()->update("ISM_InspectionTaskObject", keyValue, updateArgs);
    //更新组的已检查项
    for(int i = 0; i < taskGroupDataKeyList.size(); i++)
        updateLocalTaskGroupExamObjectCount(taskGroupDataKeyList.at(i), 0, true);

    checkLocalTaskFinished(taskDataKey);
    return true;
}

void InspectionHandler::checkLocalTaskFinished(const QString &taskDataKey)
{
    qDebug() << Q_FUNC_INFO << "  taskDataKey is " << taskDataKey;
    QList<QMap<QString, QString> > valueList;
    QStringList column("TaskDataKey");
    valueList = DBHelper::getInstance()->select("ISM_InspectionTaskGroup", column, " where TaskDataKey = '" + taskDataKey +  "' and LocalStatus != '2'");
    if(valueList.size() == 0){
        markLocalTaskFinished(taskDataKey);
    }
}

void InspectionHandler::markLocalTaskFinished(const QString &taskDataKey)
{
    qDebug() << Q_FUNC_INFO << " taskDataKey is " << taskDataKey;
    DBHelper::getInstance()->deleteRow("ISM_InspectionTask", " where TaskDataKey = '" + taskDataKey + "'");
    DBHelper::getInstance()->deleteRow("ISM_InspectionTaskGroup", " where TaskDataKey = '" + taskDataKey + "'");
    DBHelper::getInstance()->deleteRow("ISM_InspectionTaskObject", " where TaskDataKey = '" + taskDataKey + "'");
    // 4. 删除任务最后获取时间
    QString timeDeleteArg = " where TaskDataKey = '" + taskDataKey + "' and UserID = '"
            + RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID") + "'";
    DBHelper::getInstance()->deleteRow(INSPECTION_TABLE_USER_TASK_TIME, timeDeleteArg);
}

void InspectionHandler::updateLocalTaskGroupExamObjectCount(const QString &taskGroupDataKey, int count, bool all)
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

//    for(int i = 0; i < taskGroupDataKeyList.size(); i++){
//        QList<QMap<QString, QString> > examCount;
//        QString strSql = "select count(*) as examNum from ISM_InspectionTaskObject where TaskGroupDataKey = '" + taskGroupDataKeyList.at(i) + "' and LocalStatus != '0'";
//        DBHelper::getInstance()->select(strSql, examCount);
//        QString updateArgs = " where TaskGroupDataKey ='" + taskGroupDataKeyList.at(i) + "'";
//        QMap<QString, QString> examMap;
//        examMap["ExamObjectCount"] = examCount.at(0)["examNum"];
//        DBHelper::getInstance()->update("ISM_InspectionTaskGroup", examMap, updateArgs);
//    }
}

QString InspectionHandler::queryLastTime(const QString &userID)
{
    QString strTaskTime ;
    QString selectionArg = " where UserID = '" + userID + "'";
    QStringList column;
    column.append("LastTime");
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select(INSPECTION_TABLE_USER_TIME, column, selectionArg);
    if (valueList.count() > 0) {
        strTaskTime = valueList.at(0)["LastTime"];
    } else
        strTaskTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    return strTaskTime;
}
