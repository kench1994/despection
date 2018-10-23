#include "devicedeploymenthandler.h"
#include "../common/bussinessmap.h"
#include "../common/comparetool.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include <QDebug>

/**
 * 组装 DataPackage 中的表名
 */
#define TABLE_NAME_OF_DATA_PACKAGE "EMA_Allocation"

DeviceDeploymentHandler::DeviceDeploymentHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICE_DEPLOYMENT;
}

DeviceDeploymentHandler::~DeviceDeploymentHandler()
{
}

void DeviceDeploymentHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << "  dataPackage.para  " << dataPackage.para;
    QString objects = dataPackage.head.objects;
    if(objects == EMA_Allocation_PlanList){
        QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
        QString lastDEXTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
        QString selectionArg = " where "+ QString(USER_ID) + "='" + userID + "'";
        QStringList column;
        QList<DataRow > dataRows = m_nDbHelper->select(DEPLOYMENT_TABLE_USER_TIME, column, selectionArg);
        if (dataRows.count() > 0) {
            lastDEXTime = dataRows.at(0).value(LAST_TIME);
        }
        DataPackage data;
        data.head = dataPackage.head;
        data.head.objects = EMA_Off_GetUserRunningTaskDataKeys;
        data.para.insert(USER_ID, userID);
        data.para.insert(LAST_DEX_TIME, lastDEXTime);
        sendData(data, action);
    } else {
        sendData(dataPackage, action);
    }
}

void DeviceDeploymentHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << dataPackage.para;
    DbusPackage dbusPackage;
    QString objects = dataPackage.head.objects;
    if(objects == EMA_Allocation_PlanList ||
            objects == EMA_Off_GetUserRunningTaskDataKeys){
        getLocalTasks(dataPackage, dbusPackage.dataPackage);
    }else if(objects == EMA_AllocationEquipmentList){
        getLocalEquipments(dataPackage, dbusPackage.dataPackage);
    }
    emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    m_nRequestPkgMap.remove(action);
}

int DeviceDeploymentHandler::getItemCount()
{
    QString organiseUnitID = RuntimeParam::getInstance()->getLoginUserMsgByKey("OrganiseUnitIDs");
    QString departmentID = RuntimeParam::getInstance()->getLoginUserMsgByKey("DepartmentIDs");
    QString sql = "select "
            "t.ID"
            " from EMA_Allocation_Task t"
            " where t.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'" +
            " and t.SyncStatus='" + QString(TASK_STATUS_SYNC_FINISH) + "'" +
            " and ((t.status=1 and ((t.OutOrganiseUnit='" + organiseUnitID + "'" +
            " and (t.OutDepartMent is null or t.OutDepartMent='')) or t.OutDepartMent='" + departmentID + "'))" +
            " or (t.status=2 and ((t.InOrganiseUnit='" + organiseUnitID + "'" +
            " and (t.InDepartMent is null or t.InDepartMent='')) or t.InDepartMent='" + departmentID + "'')))";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        return dataRows.size();
    }
    return -1;
}

void DeviceDeploymentHandler::cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID)
{

    /**
    *  Step 1. 删除任务-用户-时间表
    */
    QString selectArg = " where " + QString(USER_ID) + "='" + userID + "'";
    m_nDbHelper->deleteRow(DEPLOYMENT_TABLE_USER_TASK_TIME, selectArg);

    /**
    * Step 2. 删除业务-用户-时间表
    */
    m_nDbHelper->deleteRow(DEPLOYMENT_TABLE_USER_TIME, selectArg);

    /**
    * Step 3. 删除任务
    */
    QStringList postCodes = getPostCodesByUser(userID);
    if(!postCodes.isEmpty()){
        for (int i = 0; i < postCodes.size(); i++) {
            QString postCode = postCodes.at(i);
            QStringList userIDs = getUsersByPostCode(postCode);
            if(userIDs.size() == 1){
                selectArg = " where ((Status=1 and ((OutOrganiseUnit='" + organiseUnitID + "' and (OutDepartMent is null or OutDepartMent='')) or OutDepartMent='" + departmentID +"'))"
                                " or (Status=2 and ((InOrganiseUnit='" + organiseUnitID + "' and (InDepartMent is null or InDepartMent='')) or InDepartMent='" + departmentID +"')))";
                m_nDbHelper->deleteRow(DEPLOYMENT_TABLE_TASK, selectArg);
            }
        }
    }

    /**
    * Step 4. 删除任务项
    */
    selectArg = " where EMA_Allocation_Equipment.AllocationID not in (select ID from EMA_Allocation_Task)";
    m_nDbHelper->deleteRow(DEPLOYMENT_TABLE_EQUIPMENT, selectArg);
}

void DeviceDeploymentHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    qDebug() <<Q_FUNC_INFO << " on ResultOfResponse";
    DbusPackage dbusPackage;
    const DataPackage &requestPackage = m_nRequestPkgMap[action];
    getLocalTasks(requestPackage, dbusPackage.dataPackage);
    emit onResultOfResponse(objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    m_nRequestPkgMap.remove(action);
}

void DeviceDeploymentHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        if (objects == EMA_Off_GetUserRunningTaskDataKeys) {
            /**
             * 调拨任务同步处理
             */
            handleTaskPatch(map.dataPackage, action);
        } else if (objects == EMA_AllocationEquipmentList) {
            /**
             * 在线请求任务项的接口
             */
            /**
            * Step 1. 检查提交表中是否有此记录，有则移除
            */
            handleEquipmentResponse(map.dataPackage);
            /**
            * Step 2. 返回给app结果
            */
            emit onResultOfResponse(objects, action, result, error, map);
            m_nRequestPkgMap.remove(action);
        }
    } else {
        DataPackage &requestPackage = m_nRequestPkgMap[action];
        offLine(action, requestPackage);
    }
}

void DeviceDeploymentHandler::handleTaskPatch(const DataPackage &data, const QString &action){
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    QString maxTime = SystemConfig::getInstance()->getOfflineTaskInitTime();
    bool updateTime = false;
    QStringList addList;
    m_nDbHelper->beginTransaction();
    if(!data.tables.isEmpty()){
        QList<DataRow> dataRows = data.tables.at(0).value;
        for (int i = 0; i < dataRows.size(); i++) {
            DataRow dataRow = dataRows.at(i);
            QString status = dataRow.value("Status");
            QString taskId = dataRow.value("ID");
            if(status == "3"){
                /**
                 *  删除已经完成的任务
                 */
                deleteTask(dataRow);
                QString time = dataRow.value("ServerDate");
                if(time > maxTime){
                    maxTime = time;
                }
                updateTime = true;
            }else{
                /**
                 *  更新未完成的任务
                 */
                QString selectionArg = " where "+ QString(DB_ID) + "='" + taskId + "'";
                QStringList column;
                column.append("Status");
                QList<DataRow> list = m_nDbHelper->select(DEPLOYMENT_TABLE_TASK, column, selectionArg);
                if(!list.isEmpty()){
                    QString localStatus = list.at(0).value("Status");
                    if(localStatus != status){
                        continue;
                    }
                }
                addList.append(taskId);
                updateTask(dataRow);
            }
        }
    }
    if(updateTime && addList.isEmpty()) {
        /**
         * 当没有新增或修改的任务时，更新业务时间
         */

        QString sql = "update " + QString(DEPLOYMENT_TABLE_USER_TIME) +
                " set " + QString(LAST_TIME) + "='" + maxTime + "'" +
                " where " + QString(USER_ID) + "='" + userID + "'";
        m_nDbHelper->execSQL(sql);

    }
    m_nDbHelper->endTransaction();
    DataPackage &requestPackage = m_nRequestPkgMap[action];
    if(!addList.isEmpty()) {
        DbusPackage dbusPackage;
        getLocalTasks(requestPackage, dbusPackage.dataPackage);
        emit onResultOfResponse(requestPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
        /**
         * 发送增量同步请求
         */
        QVariantMap syncMap;
        syncMap["UserID"] = userID;
        syncMap["addIDs"] = QVariant(addList);
        qDebug() << Q_FUNC_INFO << "------ addList: " << addList;
        emit onBeginSyncSignal(MESSAGE_SYNC_ADDTION, action, requestPackage.head.objects, syncMap);
    } else {
        offLine(action, requestPackage);
    }
}


void DeviceDeploymentHandler::deleteTask(DataRow &dataRow){

  QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");

  QString taskID = dataRow.value("ID");

  /**
   * Step 1. 删除任务
   */
  QString sql0 = "delete from " + QString(DEPLOYMENT_TABLE_TASK) +
          " where " + QString(DB_ID) + " in ('" + taskID + "')";
  m_nDbHelper->execSQL(sql0);

  /**
   * Step 2. 删除任务项
   */
  QString sql1 = "delete from " + QString(DEPLOYMENT_TABLE_EQUIPMENT) + " where "
          + QString(ALLOCATION_ID) + " in ('" + taskID + "')";
  m_nDbHelper->execSQL(sql1);

  /**
   * Step 3. 删除任务最后获取时间
   */
  QString sql2 = "delete from " + QString(DEPLOYMENT_TABLE_USER_TASK_TIME) + " where "
          + QString(DB_ID) + " in ('" + taskID + "') and " + QString(USER_ID) +"='" + userID + "'";
  m_nDbHelper->execSQL(sql2);
}


void DeviceDeploymentHandler::updateTask(DataRow &dataRow){
  QString taskID = dataRow.value("ID");
  QStringList column;
  dataRow.insert(SYNC_STATUS, TASK_STATUS_SYNC_ING);
  dataRow.insert(LOCAL_STATUS, TASK_STATUS_NO_FINISH);
  QList<DataRow> dataRows = m_nDbHelper->select(DEPLOYMENT_TABLE_TASK, column, " where " + QString(DB_ID) + "='" + taskID +"'");
  if(!dataRows.isEmpty()){
      m_nDbHelper->update(DEPLOYMENT_TABLE_TASK, dataRow, " where " + QString(DB_ID) + "='" + taskID + "'");
  }else{
      m_nDbHelper->insert(DEPLOYMENT_TABLE_TASK, dataRow);
  }
}

void DeviceDeploymentHandler::handleEquipmentResponse(DataPackage &data) {
    QStringList column;
    column.append(ORDER_CODE);
    QList<DataRow> dataRows = m_nDbHelper->select(DEPLOYMENT_TABLE_SUBMIT, column, "");
    DataTable &table = data.tables[0];
    for (int i = 0; i < table.value.size(); i++) {
        DataRow dataRow = table.value.at(i);
        for (int i = 0; i < dataRows.size(); i++) {
            if(dataRows.at(i).value(ORDER_CODE) == dataRow.value(ORDER_CODE)){
                table.value.removeAt(i);
            }
        }
    }
}


bool DeviceDeploymentHandler::getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage){
    QString organiseUnitID = requestPackage.para.value("OrganiseUnitID");
    QString departmentID = requestPackage.para.value("DepartmentID");
    if(organiseUnitID == "" || departmentID == ""){
        organiseUnitID = RuntimeParam::getInstance()->getLoginUserMsgByKey("OrganiseUnitIDs");
        departmentID = RuntimeParam::getInstance()->getLoginUserMsgByKey("DepartmentIDs");
    }
    QString sql = "select "
                    "t.ID,"
                    "t.OrderCode,"
                    "t.OrderName,"
                    "t.OutOrganiseUnit,"
                    "t.OutDepartMent,"
                    "t.InOrganiseUnit,"
                    "t.InDepartMent,"
                    "t.OperatorDate,"
                    "t.SyncStatus,"
                    "t.Status,"
                    "t.OutOrganiseUnitName,"
                    "t.InOrganiseUnitName "
                    "from EMA_Allocation_Task t"
                    " where t.LocalStatus='" + QString(TASK_STATUS_NO_FINISH) + "'" +
                    " and ((t.status=1 and ((t.OutOrganiseUnit='" + organiseUnitID + "'" +
                    " and (t.OutDepartMent is null or t.OutDepartMent='')) or t.OutDepartMent='" + departmentID + "'))" +
                    " or (t.status=2 and ((t.InOrganiseUnit='" + organiseUnitID + "'" +
                    " and (t.InDepartMent is null or t.InDepartMent='')) or t.InDepartMent='" + departmentID + "')))" +
                    " order by t.OperatorDate desc";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        convertToDataPackage(dataRows, requestPackage.head, TABLE_NAME_OF_DATA_PACKAGE, DB_ID, resultPackage);
    }
    return result;
}

bool DeviceDeploymentHandler::getLocalEquipments(const DataPackage &requestPackage, DataPackage &resultPackage){
    QString orderCode = requestPackage.para.value("OrderCode");
    QString sql = "select ae.ID,ae.AllocationID,ae.EquipmentID,"
                    "eq.EquipmentName,eq.EquipmentCode,eq.ManufacturerName,eq.ModelName "
                    "from EMA_Allocation_Equipment ae "
                    "left join EMB_Equipment eq on ae.EquipmentID=eq.ID "
                    "where ae.OrderCode='" + orderCode + "'";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result){
        convertToDataPackage(dataRows, requestPackage.head, TABLE_NAME_OF_DATA_PACKAGE, DB_ID, resultPackage);
    }
    return result;
}
