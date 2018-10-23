#include "dailytaskhandler.h"
#include "../common/bussinessmap.h"
#include "../common/dbhelper.h"
#include "../common/comparetool.h"
#include "../common/dataoperationtype.h"
#include "../networkcontrol.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include <QDebug>
DailyTaskHandler::DailyTaskHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_DAILY;
}

DailyTaskHandler::~DailyTaskHandler()
{
}

void DailyTaskHandler ::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  dataPackage.para  " << dataPackage.para << " objects is " << dataPackage.head.objects;
    QString objects = dataPackage.head.objects;
    if(objects == EISM_GetDailyRunningTask ){
        QString userID = dataPackage.para["UserID"];
        QString lastDEXTime = queryLastTime(userID);
        DataPackage data;
        data.head = dataPackage.head;
        data.head.objects = EISM_Off_GetUserDailyTaskDataKeys;
        data.para.insert(USER_ID, userID);
        data.para.insert(LAST_DEX_TIME, lastDEXTime);
        sendData(data, action);
    } else {
        sendData(dataPackage, action);
    }
}

void DailyTaskHandler ::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << dataPackage.head.objects << "  action " << action;
    DbusPackage dbusPackage;
    if(dataPackage.head.objects == EISM_GetDailyRunningTask )
        getLocalTasks(dataPackage, dbusPackage.dataPackage);
    else if(dataPackage.head.objects == EISM_GetDailyTaskGroupList )
        getLocalTaskGroup(dataPackage, dbusPackage.dataPackage);
    else if(dataPackage.head.objects == EISM_GetDailyObjectListByTask  || dataPackage.head.objects == EISM_GetDailyObjectListByGroup )
        getLocalTaskObject(dataPackage, dbusPackage.dataPackage);
    else if(dataPackage.head.objects == EISM_GetDailyObjectDetail )
        getLocalTaskObjectDetail(dataPackage, dbusPackage.dataPackage);
    m_nRequestPkgMap.remove(action);
    qDebug() << Q_FUNC_INFO << " resultPackage is " << dbusPackage.dataPackage.tables.at(0).value;
    emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

int DailyTaskHandler ::getItemCount()
{
    return -1;
}

void DailyTaskHandler ::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    Q_UNUSED(objects)
    qDebug() <<Q_FUNC_INFO;
    DataPackage originalPkg = m_nRequestPkgMap[action];
    offLine(action, originalPkg);
}

void DailyTaskHandler ::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        if(objects == EISM_Off_GetUserDailyTaskDataKeys)
            handleTaskPatch(objects, action, map.dataPackage);
        else if(objects == EISM_GetDailyObjectDetail
                || objects == EISM_GetDailyTaskGroupList
                || (objects == EISM_GetDailyObjectListByTask || objects == EISM_GetDailyObjectListByGroup)){
            m_nRequestPkgMap.remove(action);
            emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, map);
        }
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}


//<Field Value="TaskDataKeyやTaskCodeやTaskNameやLimitModeやEnableUploadAttachやPlanIDやPlanStrategyやInspectionModeやTaskStartTime"/>
bool DailyTaskHandler ::getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    qDebug() << Q_FUNC_INFO << requestPackage.head.objects;
    QString userId = requestPackage.para["UserID"];
    QString strSql = "select distinct "
            "planTask.TaskDataKey,"
            "planTask.TaskCode,"
            "planTask.TaskName,"
            "planTask.LimitMode,"
            "planTask.EnableUploadAttach,"
            "planTask.TaskStartTime,"
            "planTask.SyncStatus,"
            "planTask.InspectionMode "
            " from ISM_InspectionPlanTask planTask"
            " inner join ISM_InspectionPlanGroup planGroup on planGroup.TaskDataKey=planTask.TaskDataKey"
            " where datetime(TaskStartTime) < datetime('now','localtime') and  datetime(TaskEndTime) > datetime('now','localtime')"
            " and planGroup.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userId + "') "
            " group by planTask.TaskDataKey"
            " order by planTask.TaskStartTime desc";
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionTask", "TaskDataKey", resultPackage);
    return true;
}
//<Field Value="TaskGroupDataKeyやTaskDataKeyやTaskGroupNameやPostIDやTotalObjectCountやExamObjectCountやRFIDCodeやBarCodeやQRCodeやOrderIndex"/>
bool DailyTaskHandler ::getLocalTaskGroup(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString userId = requestPackage.para["UserID"];
    QString taskDataKey = requestPackage.para["TaskDataKey"];
    QString strSql = "select distinct "
            "taskGroup .TaskGroupDataKey,"
            "taskGroup .TaskDataKey,"
            "taskGroup .BarCode,"
            "taskGroup .QRCode,"
            "taskGroup .RFIDCode,"
            "taskGroup .TaskGroupName,"
            "taskGroup .OrderIndex"
            " from ISM_InspectionPlanGroup taskGroup "
            " where taskGroup .TaskDataKey='" + taskDataKey + "' "
            " and taskGroup .PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userId + "')"
            " and taskGroup .TaskGroupDataKey in(select TaskGroupDataKey from ISM_InspectionPlanObject where TaskGroupDataKey=taskGroup .TaskGroupDataKey)";
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    qDebug() << Q_FUNC_INFO << valueList;
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionTaskGroup", "TaskGroupDataKey", resultPackage);
    return true;
}
//<Field Value="PlanObjectIDやObjectNameやObjectFullNameやPlanGroupIDやTaskGroupDataKeyやRFIDCodeやBarCodeやQRCodeやPositionsやModelNameやRealObjectID"/>
bool DailyTaskHandler ::getLocalTaskObject(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString strSql = "select distinct "
                        "planObj.PlanObjectID,"
                        "planObj.ObjectName,"
                        "planObj.ObjectFullName,"
                        "planObj.PlanGroupID,"
                        "planObj.RealObjectID, "
                        "equipment.Positions, "
                        "equipment.ModelName, "
                        "case when planObj.RealObjectType='1' then equipment.RFIDCode else parts.RFIDCode end as RFIDCode,"
                        "case when planObj.RealObjectType='1' then equipment.BarCode else parts.BarCode end as BarCode,"
                        "case when planObj.RealObjectType='1' then equipment.QRCode else parts.QRCode end as QRCode "
                        " from ISM_InspectionPlanObject planObj ";
    if(requestPackage.head.objects == EISM_GetDailyObjectListByTask ){
        QString taskDatakey = requestPackage.para[TASK_DATA_KEY];
        QString userId = requestPackage.para["UserID"];
        strSql += "inner join ISM_InspectionPlanGroup planGroup on planGroup.TaskGroupDataKey=planObj.TaskGroupDataKey "
                "left join EMB_Equipment equipment on planObj.RealObjectID=equipment.ID and planObj.RealObjectType='1' "
                "left join EMB_Equipment_Part parts on planObj.RealObjectID=parts.ID and planObj.RealObjectType='2' "
                "where planGroup.TaskDataKey='" + taskDatakey + "' "
                "and planGroup.PostCode in(select SDMS_User_Post.PostCode from SDMS_User_Post where SDMS_User_Post.UserID='" + userId + "') "
                "group by planObj.PlanObjectID";
    } else if(requestPackage.head.objects == EISM_GetDailyObjectListByGroup ){
        QString taskGroupDataKey = requestPackage.para["TaskGroupDataKey"];
        strSql += " left join EMB_Equipment equipment  on planObj.RealObjectID=equipment.ID and planObj.RealObjectType='1' "
                " left join EMB_Equipment_Part parts on planObj.RealObjectID=parts.ID and planObj.RealObjectType='2' "
                " where planObj.TaskGroupDataKey='" + taskGroupDataKey + "' "
                " group by planObj.PlanObjectID";
    } else
        return false;
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    qDebug() << Q_FUNC_INFO << valueList;
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionPlanObject", "PlanObjectID", resultPackage);
    return true;
}

//<Field Value="PlanObjectIDやObjectNameやObjectFullNameやPlanGroupIDやItemSchemaやRealObjectIDやPositionsやCustodianやModelName"/>
bool DailyTaskHandler ::getLocalTaskObjectDetail(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString planObjectID = requestPackage.para["PlanObjectID"];
    QString strSql  = "select distinct "
                        "planObj.PlanObjectID,"
                        "planObj.ObjectName,"
                        "planObj.ObjectFullName,"
                        "planObj.PlanGroupID,"
                        "planObj.ItemSchema,"
                        "planObj.RealObjectID,"
                        "equipment.Positions, "
                        "equipment.Custodian, "
                        "equipment.ModelName "
                        " from ISM_InspectionPlanObject planObj "
                        " left join EMB_Equipment equipment on planObj.RealObjectID=equipment.ID and planObj.RealObjectType='1' "
                        " where planObj.PlanObjectID='" + planObjectID + "'";
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    qDebug() << Q_FUNC_INFO << valueList;
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionTaskGroup", "TaskGroupDataKey", resultPackage);
    return true;
}

void DailyTaskHandler ::handleTaskPatch(const QString &objects, const QString &action, const DataPackage &data)
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
        dbHelper->beginTransaction();
        markLocalTaskFinished(deleteList);
        dbHelper->endTransaction();
    }

    QStringList addList = compareTool.getAddList();
    QStringList updateList = compareTool.getUpdateList();
    bool needSync = addList.count() > 0 || updateList.count() > 0;
    if(needSync){
        DBHelper *dbHelper = DBHelper::getInstance();
        // step 2 : 根据从服务器获取的更新任务列表，将本地对应的任务的状态设置为正在更新
        if(updateList.count() > 0){
            QMap<QString, QString> keyValue;
            keyValue["SyncStatus"] = TASK_STATUS_SYNC_ING;
            dbHelper->update("ISM_InspectionPlanTask", keyValue, " where TaskDataKey = in('" + updateList.join("','") + "')");
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

                for(int j = 0; j < netValueList.count(); j++){
                    DataRow netValue = netValueList.at(j);
                    if( netValue["TaskDataKey"] == taskDataKey){
                        netValue["SyncStatus"] = TASK_STATUS_SYNC_ING;
                        netValue["TaskCode"] = "0";
                        netValue["LimitMode"] = "0";
                        netValue["EnableUploadAttach"] = "0";
                        netValue["PlanID"] = "0";
                        netValue["InspectionMode"] = "1";
                        netValue["TaskStartTime"] = "0";
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

void DailyTaskHandler ::markLocalTaskFinished(const QStringList &taskDataKeyList)
{
    qDebug() << Q_FUNC_INFO << " taskDataKeyList is " << taskDataKeyList;
    QString deleteArg = " where TaskDataKey  in ('" + taskDataKeyList.join("','") + "')";
    DBHelper::getInstance()->deleteRow("ISM_InspectionPlanTask", deleteArg);
    DBHelper::getInstance()->deleteRow("ISM_InspectionPlanGroup", deleteArg);
    DBHelper::getInstance()->deleteRow("ISM_InspectionPlanObject", deleteArg);
    // 4. 删除任务最后获取时间
    QString timeDeleteArg = " where UserID = '" + RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID") + "' and TaskDataKey  in ('" + taskDataKeyList.join("','") + "')";
    DBHelper::getInstance()->deleteRow("ISM_Inspection_Plan_Task_Time", timeDeleteArg);
}

QString DailyTaskHandler ::queryLastTime(const QString &userID)
{
    QString strTaskTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    QString selectionArg = " where UserID = '" + userID + "'";
    QStringList column;
    column.append("LastTime");
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select("ISM_Inspection_Plan_Time", column, selectionArg);
    if (valueList.count() > 0)
        strTaskTime = valueList.at(0)["LastTime"];
    return strTaskTime;
}
