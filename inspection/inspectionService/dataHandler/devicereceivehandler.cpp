#include "devicereceivehandler.h"
#include "../common/comparetool.h"
#include "../framework/common/convertdata.h"
#include "../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"

DeviceReceiveHandler::DeviceReceiveHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICE_RECEIVE;
}

DeviceReceiveHandler::~DeviceReceiveHandler()
{

}

void DeviceReceiveHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO << "  dataPackage.para  " << dataPackage.para;
    QString objects = dataPackage.head.objects;
    if(objects == EMD_Distribute_PlanList){
        QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
        QString lastDEXTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
        QString selectionArg = " where "+ QString(USER_ID) + "='" + userID + "'";
        QStringList column;
        QList<DataRow > dataRows = m_nDbHelper->select(DISTRIBUTION_TABLE_SYNC_DATE, column, selectionArg);
        if (dataRows.count() > 0) {
            lastDEXTime = dataRows.at(0).value(LAST_TIME);
        }
        DataPackage data;
        data.head = dataPackage.head;
        data.head.objects = EMD_Off_GetUserRunningTaskDataKeys;
        data.para.insert(USER_ID, userID);
        data.para.insert(LAST_DEX_TIME, lastDEXTime);
        sendData(data, action);
    } else {
        sendData(dataPackage, action);
    }
}

void DeviceReceiveHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO << dataPackage.para;
    QString objects = dataPackage.head.objects;
    if(objects == EMD_Off_GetUserRunningTaskDataKeys ||
            objects == EMD_Distribute_PlanList){
        requestLocalTaskList(dataPackage, action);
    }else if(objects == EMD_DistributeEquipmentList){
        requestLocalEquipmentList(dataPackage, action);
    }else if(objects == EMD_DistributeEquipment_Para){
        requestLocalEquipmentDetail(dataPackage, action);
    }
    m_nRequestPkgMap.remove(action);
}

int DeviceReceiveHandler::getItemCount()
{
    return -1;
}

void DeviceReceiveHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    qDebug() << Q_FUNC_INFO << " on ResultOfResponse";
    DbusPackage dbusPackage;
    const DataPackage &requestPackage = m_nRequestPkgMap[action];
    getLocalTasks(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    m_nRequestPkgMap.remove(action);
}

void DeviceReceiveHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        if (objects == EMD_Off_GetUserRunningTaskDataKeys) {
            /**
            *  handle tasks patch
            */
            handleTaskPatch(map.dataPackage, action);
        }else if(objects == EMD_Distribute_PlanList){
            /**
            * warning:
            * In theory, this circumstance wouldn't happen
            * So ignore it.
            */
            handleTaskListResponse(action, map);
        }else if(objects == EMD_DistributeEquipmentList){
            /**
            * handle EquipmentList callback
            */
            handleEquipmentListResponse(action, map.dataPackage);
        }else if(objects == EMD_DistributeEquipment_Para){
            /**
            * handle Equipment Para
            */
            handleEquipmentParaResponse(action, map);
        }
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}

void DeviceReceiveHandler::handleTaskPatch(const DataPackage &dataPackage, const QString &action){
    /**
    * step 1 : parse data
    */
    CompareTool compareTool;
    compareTool.compare(dataPackage, "ID");

    bool needSync = !compareTool.getAddList().isEmpty() || !compareTool.getUpdateList().isEmpty();

    /**
    * step 2 : delete removed task & task sync info
    */
    if (!compareTool.getDeleteList().isEmpty()) {
        for (int i = 0; i < compareTool.getDeleteList().size(); i++) {
            QString taskID = compareTool.getDeleteList().at(i);
            QString deleteArg = " where " + QString(DB_ID) + "='" + taskID + "'";
            m_nDbHelper->beginTransaction();
            m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_TASK, deleteArg);
            deleteArg = " where " + QString(DISTRIBUTE_ID) + "='" + taskID + "'";
            m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_EQUIPMENT, deleteArg);
            m_nDbHelper->endTransaction();
        }
    }

    /**
    * step 3 : set modified record 's SyncStatus to synchronizing
    */
    if (!compareTool.getUpdateList().isEmpty()) {
        for (int i = 0; i < compareTool.getUpdateList().size(); i++) {
            QString taskID = compareTool.getUpdateList().at(i);
            QString updateArg = " where " + QString(DB_ID) + "='" + taskID + "'";
            DataRow dataRow;
            dataRow.insert(SYNC_STATUS, TASK_STATUS_SYNC_ING);
            m_nDbHelper->update(DISTRIBUTION_TABLE_TASK, dataRow, updateArg);
        }
    }

    /**
    * step 4 : send response to application,  send added task to synchronizer
    */
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    if (needSync) {
        DbusPackage dbusPackage;
        getLocalTasks(requestPackage, dbusPackage.dataPackage);
        QList<DataRow> &lvs = dbusPackage.dataPackage.tables[0].value;
        const QList<DataRow> &nvs = dataPackage.tables[0].value;
        for (int i = 0; i < compareTool.getAddList().size(); i++) {
            QString id = compareTool.getAddList().at(i);
            bool isInLocal = false;
            for (int j = 0; j < lvs.size(); j++) {
                DataRow &lv = lvs[j];
                if (lv[DB_ID] == id) {
                    isInLocal = true;
                    break;
                }
            }
            if (isInLocal) {
                continue;
            }
            for (int j = 0; j < nvs.size(); j++) {
                DataRow nv = nvs[j];
                if (nv["ID"] == id) {
                    nv["SyncStatus"] = TASK_STATUS_SYNC_ING;
                    nv["OperatorDate"] = "";
                    nv["operatorrate"] = "";
                    lvs.append(nv);
                    break;
                }
            }
        }
        emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
        QVariantMap syncMap;
        syncMap["UserID"] = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
        syncMap["addIDs"] = QVariant(compareTool.getAddList());
        syncMap["updateIDs"] = QVariant(compareTool.getUpdateList());
        emit onBeginSyncSignal(MESSAGE_SYNC_ADDTION, action, requestPackage.head.objects, syncMap);
    } else {
        offLine(action, requestPackage);
    }
}

void DeviceReceiveHandler::handleTaskListResponse(const QString &action, const DbusPackage &map) {
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, map);
    m_nRequestPkgMap.remove(action);
}

void DeviceReceiveHandler::handleEquipmentListResponse(const QString &action, const DataPackage &dataPackage) {

    /**
     * Step 1 : get local equipments
     */
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    QString orderCode = requestPackage.para["OrderCode"];
    /**
     * 查询task子项的sql语句
     */
    QString sql = "select ID, DistributeID"
            " from EMD_DistributeEquipment"
            " where OrderCode='" + orderCode + "' and LocalStatus='0'";
    QList<DataRow> lvs;
    m_nDbHelper->select(sql, lvs);
    QString taskID = "";
    if(!lvs.isEmpty()){
        taskID = lvs.at(0)["DistributeID"];
    }
    const QList<DataRow> &nvs = dataPackage.tables[0].value;

    /**
    * Step 2 : compare items
    */
    CompareTool ompareTool;
    QStringList deleteList = ompareTool.compare(lvs, nvs, "ID");

    /**
    * Step 3 : update all finished equipments
    */
    updateEquipmentToComplete(deleteList);

    /**
    * Step 4 : check task
    */
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    checkTask(taskID, userID);

    /**
    * Final step : send data to upper application
    */
    offLine(action, requestPackage);
}

void DeviceReceiveHandler::updateEquipmentToComplete(QStringList idList) {
    qDebug() << Q_FUNC_INFO << "  idList " << idList;
    QString ids = ConvertData::getInstance()->convertForSql(idList);
    if (!ids.isEmpty()) {
        DataRow dataRow;
        dataRow[LOCAL_STATUS] = TASK_STATUS_FINISH;
        QString updateArg = " where " + QString(DB_ID) + " in (" + ids + ")";
        m_nDbHelper->update(DISTRIBUTION_TABLE_EQUIPMENT, dataRow, updateArg);
    }
}

void DeviceReceiveHandler::checkTask(const QString &taskID, const QString &userID)
{
    qDebug() << Q_FUNC_INFO << "  taskID " << taskID;
    QStringList columns;
    columns.append(DB_ID);
    QString whereClause = " where " + QString(DISTRIBUTE_ID) + "='" + taskID + "' and " +
                          QString(LOCAL_STATUS) + "!='" + TASK_STATUS_FINISH + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(DISTRIBUTION_TABLE_EQUIPMENT, columns, whereClause);
    if(dataRows.isEmpty()){
        deleteTask(taskID, userID);
    }
}

void DeviceReceiveHandler::deleteTask(const QString &taskID, const QString &userID)
{
    qDebug() << Q_FUNC_INFO << "  taskID " << taskID;
    m_nDbHelper->beginTransaction();
    QString deleteArg = " where " + QString(DB_ID) + "='" + taskID + "'";
    m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_TASK, deleteArg);
    deleteArg = " where " + QString(DISTRIBUTE_ID) + "='" + taskID + "'";
    m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_EQUIPMENT, deleteArg);
    deleteArg = " where " + QString(TASK_ID) + "='" + taskID + "' and " + QString(USER_ID) + "='" + userID + "'";
    m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_TASK_SYNC_DATE, deleteArg);
    m_nDbHelper->endTransaction();
}

void DeviceReceiveHandler::handleEquipmentParaResponse(const QString &action, const DbusPackage &map) {
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, map);
    m_nRequestPkgMap.remove(action);
}

void DeviceReceiveHandler::requestLocalTaskList(const DataPackage &requestPackage, const QString &action) {
    DbusPackage dbusPackage;
    getLocalTasks(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void DeviceReceiveHandler::getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    /**
    * 查询task层的sql语句
    */
    QString organiseUnitIDs  = requestPackage.para["OrganiseUnitID"];
    QString departmentIDs  = requestPackage.para["DepartmentID"];
    QString sql = "select ID, OrderCode, OrderName, SyncStatus, OperatorUserName, OperatorDate,"
                    "(select TotalEquipments-COUNT(id) from EMD_DistributeEquipment where DistributeID=EMD_Distribute.ID and LocalStatus=='0') || \"/\"|| TotalEquipments as operatorrate, "
                    "AuditDate,AuditUserName,OrganiseUnitID,DepartMentID"
                    " from EMD_Distribute where LocalStatus='0'"
                    " and ((OrganiseUnitID='" + organiseUnitIDs + "' and  DepartMentID='') or DepartMentID='" + departmentIDs + "')"
                    " order by OperatorDate desc";

    /**
    * 查询结果
    */
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EMD_Distribute_PlanList;
        DataTable table;
        table.name = DISTRIBUTION_TABLE_TASK;
        table.key = DB_ID;
        for (int i = 0; i < dataRows.size(); i++) {
            DataRow &dataRow = dataRows[i];
            table.value.append(dataRow);
        }
        resultPackage.tables.append(table);
        resultPackage.summary.code = 100;
        resultPackage.summary.result = 1;
        resultPackage.summary.description = "请求数据成功完成";
    }
}

void DeviceReceiveHandler::requestLocalEquipmentList(const DataPackage &requestPackage, const QString &action)
{
    DbusPackage dbusPackage;
    getLocalEquipmentList(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void DeviceReceiveHandler::getLocalEquipmentList(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString orderCode = requestPackage.para["OrderCode"];
    /**
    * 查询task子项的sql语句
    */
    QString sql = "select de.ID,de.DistributeID,de.EquipmentID,"
            "de.ReceiveUserID,de.ReceiveDate,de.IsRFIDScan,"
            "de.PDAID,de.ReceiveUserName,"
            "eq.EquipmentName,eq.EquipmentCode,eq.ManufacturerName,"
            "eq.ManufacturerID as EnterpriseID,eq.ModelName,eq.ClassName"
            " from EMD_DistributeEquipment de"
            " left join EMB_Equipment eq on de.EquipmentID=eq.ID"
            " where de.OrderCode='" + orderCode + "' and de.LocalStatus='0'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EMD_DistributeEquipmentList;
        DataTable table;
        table.name = DEPLOYMENT_TABLE_EQUIPMENT;
        table.key = "EquipmentCode";
        for (int i = 0; i < dataRows.size(); i++) {
        DataRow &dataRow = dataRows[i];
            table.value.append(dataRow);
        }
        resultPackage.tables.append(table);
        resultPackage.summary.code = 100;
        resultPackage.summary.result = 1;
        resultPackage.summary.description = "请求数据成功完成";
    }
}

void DeviceReceiveHandler::requestLocalEquipmentDetail(const DataPackage &requestPackage, const QString &action)
{
    DbusPackage dbusPackage;
    getEquipmentDetail(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
}

void DeviceReceiveHandler::getEquipmentDetail(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString equipmentID = requestPackage.para["EquipmentID"];
    QString code =  requestPackage.para["RFIDCode"];
    if(code.isEmpty())
        code = requestPackage.para["BarCode"];
    QString distributeID = requestPackage.para["DistributeID"];
    /**
    * 查询task子项详情的的sql语句
    */
    QString sql = "select de.ID,de.OrderCode,"
            "de.DistributeID,de.EquipmentID,"
            "de.ReceiveUserID,de.ReceiveDate,de.IsRFIDScan,"
            "eq.EquipmentName,eq.EquipmentCode,eq.RFIDCode,eq.ManufacturerName as EnterpriseName,"
            "eq.Longitude,eq.Latitude,eq.ModelName,eq.ManufacturerName,"
            "eq.Positions,eq.Custodian,eq.Recipient from EMD_DistributeEquipment de"
            " left join EMB_Equipment eq on de.EquipmentID=eq.ID";
    QString whereArg;
    if(equipmentID != ""){
        whereArg = " where de.EquipmentID='" + equipmentID + "'";
    }else if(code != ""){
        whereArg = " where '" + code.toLower() + "' in (LOWER(eq.RFIDCode), LOWER(eq.BarCode), LOWER(eq.QRCode)) ";
    }
    whereArg += " and de.LocalStatus = '0' and de.DistributeID='" + distributeID + "'";
    sql += whereArg;
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        /**
        * 填充数据
        */
        resultPackage.head.key = requestPackage.head.key;
        resultPackage.head.type = TYPE_REQUEST;
        resultPackage.head.objects = EMD_DistributeEquipment_Para;
        DataTable table;
        table.name = DISTRIBUTION_TABLE_EQUIPMENT;
        table.key = DB_ID;
        for (int i = 0; i < dataRows.size(); i++) {
        DataRow &dataRow = dataRows[i];
            table.value.append(dataRow);
        }
        resultPackage.tables.append(table);
        resultPackage.summary.code = 100;
        resultPackage.summary.result = 1;
        resultPackage.summary.description = "请求数据成功完成";
    }
}

