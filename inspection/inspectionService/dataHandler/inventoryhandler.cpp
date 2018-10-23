#include "inventoryhandler.h"
#include "../common/bussinessmap.h"
#include "../common/dbhelper.h"
#include "../common/comparetool.h"
#include "../common/dataoperationtype.h"
#include "../networkcontrol.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include <QDebug>
InventoryHandler::InventoryHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_INVENTORY;
}

InventoryHandler::~InventoryHandler()
{
}

void InventoryHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  dataPackage.para  " << dataPackage.para;
    QString objects = dataPackage.head.objects;
    if(objects == EMI_InventoryTask_RuningList){
        DataPackage data;
        data.head = dataPackage.head;
        data.head.objects = EMI_Off_GetUserRunningTaskDataKeys;
        data.para.insert(USER_ID, dataPackage.para["UserID"]);
        data.para.insert(LAST_DEX_TIME, queryLastTime(dataPackage.para["UserID"]));
        sendData(data, action);
    } else
        sendData(dataPackage, action);
}

void InventoryHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << dataPackage.para;
    DbusPackage dbusPackage;
    QString objects = dataPackage.head.objects;
    if(objects == EMI_InventoryTask_RuningList){
        getLocalTasks(dataPackage, dbusPackage.dataPackage);
    } else if (objects == "EMI_InventoryEquipmentList"){
        getLocalTaskObjects(dataPackage, dbusPackage.dataPackage);
    } else if(objects == "EMI_InventoryEquipment_Para"){
        getLocalObjectPara(dataPackage, dbusPackage.dataPackage);
    }
    m_nRequestPkgMap.remove(action);
    emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

int InventoryHandler::getItemCount()
{
    QStringList column(LAST_TIME);
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey(USER_ID);
    QString selectArg = " where " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> list = m_nDbHelper->select(INVENTORY_TABLE_USER_TIME, column, selectArg);
    if (list.isEmpty()) {
        return -1;
    }

    QString strSql = "select   distinct EMI_InventoryTask.ID "
                    " from EMI_InventoryTask "
                    " left join EMI_InventoryEquipment  on  EMI_InventoryTask.ID = EMI_InventoryEquipment.TaskID "
                    " where  EMI_InventoryEquipment.postcode  in (select  postcode from SDMS_User_Post  u where  u.userID= '" + userID + "') "
                    " and EMI_InventoryTask.LocalStatus = '0' and datetime(StartDate) < datetime('now','localtime') and  datetime(EndDate) > datetime('now','localtime')";
    list.clear();
    m_nDbHelper->select(strSql, list);
    qDebug() << Q_FUNC_INFO << " strSql " << strSql;
    qDebug() << Q_FUNC_INFO << " list size is  " << list.size();
    return list.size();
}

void InventoryHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    Q_UNUSED(objects)
    qDebug() <<Q_FUNC_INFO << "  action is " << action;
    DataPackage originalPkg = m_nRequestPkgMap[action];
    offLine(action, originalPkg);
}

void InventoryHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    if(result  && error == NETWORK_NO_ERROR){
        if (objects == EMI_Off_GetUserRunningTaskDataKeys) {
            taskHandFunction(objects, action, map.dataPackage);
        } else if (objects == EMI_InventoryEquipmentList) {
            //盘点设备
            DataPackage dataPackage = map.dataPackage;
            objectsHandFunction(action, dataPackage);
        } else if(objects == EMI_InventoryEquipment_Para){
            m_nRequestPkgMap.remove(action);
            emit onResultOfResponse(objects, action, result, error, map);
        }
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}

bool InventoryHandler::getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString userId = requestPackage.para["UserID"];
    QString strSql = "select ID,TaskCode,TaskName,LimitMode,EnableImage,StartDate,EndDate,SyncStatus,"
                    " StartDate|| \"至\"||  EndDate Circle, "
                    " deviceCount||'/'|| totalCount Rate"
                    " from  (select  distinct EMI_InventoryTask.ID,EMI_InventoryTask.TaskCode, "
                    " EMI_InventoryTask.TaskName,EMI_InventoryTask.LimitMode,"
                    " EMI_InventoryTask.EnableImage,EMI_InventoryTask.StartDate,"
                    " EMI_InventoryTask.EndDate,EMI_InventoryTask.SyncStatus, "
                    " Count( case when EMI_InventoryEquipment.LocalStatus !='0' then 1 else null end) deviceCount,"
                    " Count(1) totalCount "
                    " from EMI_InventoryTask "
                    " inner join EMI_InventoryEquipment  on  EMI_InventoryTask.id = EMI_InventoryEquipment.taskid  "
                    " inner join SDMS_User_Post on SDMS_User_Post.PostCode = EMI_InventoryEquipment.postcode "
                    " where  EMI_InventoryTask.LocalStatus = '0' "
                    " and SDMS_User_Post.userID= '" + userId + "' "
                    " and datetime(StartDate) < datetime('now','localtime') and  datetime(EndDate) > datetime ('now','localtime') "
                    " Group by EMI_InventoryTask.ID) table1 "
                    " where deviceCount != totalCount "
                    " Order by StartDate desc";

    QList<QMap<QString, QString> > valueList;
    qDebug() << Q_FUNC_INFO << strSql;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "EMI_InventoryTask", "ID", resultPackage);
    return true;
}

bool InventoryHandler::getLocalTaskObjects(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString taskId = requestPackage.para["TaskID"];
    QString userId = requestPackage.para["UserID"];
    qDebug() << Q_FUNC_INFO << "  object " << requestPackage.head.objects << "  TaskID " << taskId << " UserID " << userId;
    QString strSql = "select inventoryObj.ID,inventoryObj.EquipmentID,EquipmentName,EquipmentCode,"
             "ModelName,Custodian,RFIDCode,BarCode,QRCode "
             "from EMI_InventoryEquipment inventoryObj "
             "left join emb_equipment on inventoryObj.EquipmentID = emb_equipment.ID "
            " inner join  sdms_user_post  on sdms_user_post.PostCode = inventoryObj.PostCode "
             " where inventoryObj.LocalStatus = '0'  and inventoryObj.TaskID = '" + taskId + "' and sdms_user_post.UserID = '" + userId + "'";
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "EMI_InventoryEquipment", "ID", resultPackage);
    return true;
}

bool InventoryHandler::getLocalObjectPara(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    qDebug() << Q_FUNC_INFO;
    QString taskId = requestPackage.para["TaskID"];
    QString barCode = requestPackage.para["BarCode"];
    QString rfidCode = requestPackage.para["RFIDCode"];
    QString id = requestPackage.para["ID"];
    QString selectionArg;
    QString strSql = "select EMI_InventoryEquipment.ID,TaskID,EquipmentID, EquipmentName, ModelName,ClassName,"
            "EquipmentCode,Custodian,OrganiseUnitName, ManufacturerName as EnterpriseName from EMI_InventoryEquipment "
            "left join emb_equipment on EMI_InventoryEquipment.EquipmentID = emb_equipment.ID ";
    if(!barCode.isEmpty()){
        selectionArg = "where EMI_InventoryEquipment.TaskID = '" + taskId +  "' and emb_equipment.BarCode = '" + barCode + "'";
    } else if(!rfidCode.isEmpty()){
        selectionArg = "where EMI_InventoryEquipment.TaskID = '" + taskId +  "' and emb_equipment.RFIDCode = '" + rfidCode + "'";
    } else {
        selectionArg = "where EMI_InventoryEquipment.TaskID = '" + taskId +  "' and EMI_InventoryEquipment.ID = '" + id + "'";
    }
    strSql += selectionArg;
    QList<QMap<QString, QString> > valueList;
    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    if(rv){
        convertToDataPackage(valueList, requestPackage.head, "EMI_InventoryEquipment", "ID", resultPackage);
        strSql = "select * from EMI_InventoryDic";
        QList<QMap<QString, QString> > dicValueList;
        DBHelper::getInstance()->select(strSql, dicValueList);
        DataTable dicTable;
        dicTable.key = "DicItemCode";
        dicTable.name = "InventoryResult";
        dicTable.value.append(dicValueList);
        resultPackage.tables.append(dicTable);
    } else
        return false;
    return true;
}

void InventoryHandler::taskHandFunction(const QString &objects, const QString &action, const DataPackage &data)
{
    DataPackage originalRequestPkg = m_nRequestPkgMap[action];
    CompareTool compareTool;
    compareTool.compare(data, "ID");
    //step 1. 需要删除的任务 本地存在 服务器已经提交的任务
    QStringList deleteList = compareTool.getDeleteList();
    if (deleteList.count() > 0) {
        DBHelper *dbHelper = DBHelper::getInstance();
        dbHelper->beginTransaction();
        for(int i = 0; i < deleteList.count(); i++){
            markLocalTaskFinished(deleteList.at(i));
        }
        dbHelper->endTransaction();
    }

    QStringList addList = compareTool.getAddList();
    QStringList updateList = compareTool.getUpdateList();
    qDebug() << "addList is " << addList;
    qDebug() << "updateList is " << updateList;
    bool needSync = addList.count() > 0 || updateList.count() > 0;
    if(needSync){
        DBHelper *dbHelper = DBHelper::getInstance();
        // step 2 : 根据从服务器获取的更新任务列表，将本地对应的任务的状态设置为正在更新
        for (int i = 0; i < updateList.count(); i++) {
            QMap<QString, QString> keyValue;
            keyValue["SyncStatus"] = "0";
            dbHelper->update("EMI_InventoryTask", keyValue, " where ID = '" + updateList.at(i) + "'");
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
                QString ID = addList.at(i);
                bool isInLocal = false;
                for(int k = 0; k < localValueList.count(); k++){
                    if(localValueList.at(k)["ID"] == ID){
                        isInLocal = true;
                        break;
                    }
                }
                if(isInLocal){
                    continue;
                }

                for(int j = 0; j < netValueList.count(); j++){
                    DataRow netValue = netValueList.at(j);
                    if( netValue["ID"] == ID){
                        netValue["SyncStatus"] = TASK_STATUS_SYNC_ING;
                        netValue["Status"] = TASK_STATUS_NO_FINISH;
                        netValue["LimitMode"] = "0";
                        netValue["EnableImage"] = "0";
                        netValue["TaskCode"] = "0";
                        netValue["Circle"] = "- 至 -";
                        netValue["Rate"] = "- / -";
                        localValueList.append(netValue);
                        qDebug() << "localValueList  count is " << localValueList.count();
                        break;
                    }
                }
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

void  InventoryHandler::objectsHandFunction(const QString &action, const DataPackage &data){
    //获取本地任务项列表
    DataPackage localPackage;
    DataPackage originalPkg = m_nRequestPkgMap[action];
    getLocalTaskObjects(originalPkg, localPackage);
    CompareTool compareTool;
    QStringList netFinishedIDList = compareTool.compare(localPackage.tables.at(0).value, data.tables.at(0).value, "ID");
    qDebug() << Q_FUNC_INFO<< "  netFinishedIDList is " << netFinishedIDList;
    if(netFinishedIDList.size() > 0){
        DBHelper::getInstance()->beginTransaction();
        markLocalTaskObjectsFinished(originalPkg.para["TaskID"], netFinishedIDList);
        DBHelper::getInstance()->endTransaction();
    }
    offLine(action, originalPkg);
}

bool InventoryHandler::markLocalTaskObjectsFinished(const QString &taskID, const QStringList &IDList)
{
    QString updateArgs = " where ID in ('" + IDList.join("','") + "')";
    QMap<QString, QString> keyValue;
    keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
    DBHelper::getInstance()->update("EMI_InventoryEquipment", keyValue, updateArgs);
    checkLocalTaskFinished(taskID);
    return true;
}

void InventoryHandler::checkLocalTaskFinished(const QString &taskID){
    QList<QMap<QString, QString> > valueList;
    QStringList column("TaskID");
    valueList = DBHelper::getInstance()->select("EMI_InventoryEquipment", column, " where LocalStatus != '2' and TaskID = '" + taskID + "'");
    if(valueList.size() == 0){
        markLocalTaskFinished(taskID);
    }
}

void InventoryHandler::markLocalTaskFinished(const QString &taskID)
{
    DBHelper::getInstance()->deleteRow("EMI_InventoryTask", " where ID = '" + taskID + "'");
    DBHelper::getInstance()->deleteRow("EMI_InventoryEquipment", " where TaskID = '" + taskID + "'");
    //删除任务最后获取时间
    QString timeDeleteArg = " where TaskID = '" + taskID + "' and UserID = '"
            + RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID") + "'";
    DBHelper::getInstance()->deleteRow("EMI_Inventory_User_Task_Time", timeDeleteArg);
}

QString InventoryHandler::queryLastTime(const QString &userID)
{
    QString strTaskTime ;
    QString selectionArg = " where UserID = '" + userID + "'";
    QStringList column;
    column.append("LastTime");
    QList<QMap<QString, QString> > valueList = DBHelper::getInstance()->select("EMI_Inventory_User_Time", column, selectionArg);
    if (valueList.count() > 0) {
        strTaskTime = valueList.at(0)["LastTime"];
    } else
        strTaskTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    return strTaskTime;
}

