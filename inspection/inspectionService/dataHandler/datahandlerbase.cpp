#include "datahandlerbase.h"
#include "../networkcontrol.h"
#include "../DBConfig/dbconfigmanager.h"
#include "../../framework/network/networkmanager.h"
#include "../../framework/config/sdisruntimecontext.h"
#include "../common/runtimeparam.h"
#include <QUuid>
#include <QDebug>
DataHandlerBase::DataHandlerBase()
{
    m_nDbHelper = DBHelper::getInstance();
}

DataHandlerBase::~DataHandlerBase()
{
    m_nActionList.clear();
    m_nPureOnLineActionList.clear();
}

void DataHandlerBase::request(const QString &action, const DataPackage &dataPackage)
{
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    //纯在线模式
    if(offlineModel == "0"){
        pureOnLine(action, dataPackage);
    } else {
        m_nRequestPkgMap[action] = dataPackage;
        if(offlineModel == "1"){
            //离线模式
            if (NetworkControl::getInstance()->getNetworkConnectStatus() != "") {
                //离线模式下有网络时，从服务器查询
                onLine(action, dataPackage);
            } else {
                //离线模式下没有网络时，直接执行离线查询
                offLine(action, dataPackage);
            }
        } else if(offlineModel == "2"){
            //纯离线模式下没有网络时，直接执行离线查询
            offLine(action, dataPackage);
        }
    }
}

void DataHandlerBase::requestTask(const QString &action, const DataPackage &dataPackage)
{
    if(NetworkControl::getInstance()->getNetworkConnectStatus() == ""){
        qDebug() <<Q_FUNC_INFO << " 纯在线模式下，并且没网，直接返回  action " <<action;
        sendNoNetworkResponse(dataPackage.head.objects, action);
    } else {
        m_nRequestPkgMap[action] = dataPackage;
        onLine(action, dataPackage);
    }

}

void DataHandlerBase::pureOnLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO << " action " << action;
    //纯在线模式下，并且没网，直接返回
    if(NetworkControl::getInstance()->getNetworkConnectStatus() == ""){
        qDebug() <<Q_FUNC_INFO << " 纯在线模式下，并且没网，直接返回  action " <<action;
        sendNoNetworkResponse(dataPackage.head.objects, action);
    } else {
        QString sendAction = sendData(dataPackage,  action);
        m_nPureOnLineActionList.append(sendAction);
    }
}

void DataHandlerBase::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO << "  action " << action << ", dataPackage.para  " << dataPackage.para;
    sendData(dataPackage, action);
}

void DataHandlerBase::offLine(const QString &action, const DataPackage &dataPackage)
{
    sendNoNetworkResponse(dataPackage.head.objects, action);
}

int DataHandlerBase::getItemCount()
{
    return -1;
}

void DataHandlerBase::cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID)
{
    Q_UNUSED(userID)
    Q_UNUSED(organiseUnitID)
    Q_UNUSED(departmentID)
}

void DataHandlerBase::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(action)
    Q_UNUSED(objects)
    Q_UNUSED(map)
    qDebug() <<Q_FUNC_INFO;
}

QString DataHandlerBase::sendData(const DataPackage &dataPackage, const QString &action, const bool &binaryType)
{
    QString uuid = createUuid();
    m_nActionList.insert(uuid, action);
    NetworkControl::getInstance()->sendData(dataPackage, uuid, binaryType);
    return uuid;
}

QStringList DataHandlerBase::getPostCodesByUser(const QString &userID)
{
    QStringList columns;
    columns.append(POST_CODE);
    QString selectArg = " where " + QString(USER_ID) + "='" + userID + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(USER_TABLE_POST, columns, selectArg);
    QStringList postCodes;
    for(int i = 0; i < dataRows.size(); i++){
        postCodes.append(dataRows.at(i)[POST_CODE]);
    }
    return postCodes;
}

QStringList DataHandlerBase::getUsersByPostCode(const QString &postCode)
{
    QStringList columns;
    columns.append(USER_ID);
    QString selectArg = " where " + QString(POST_CODE) + "='" + postCode + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(USER_TABLE_POST, columns, selectArg);
    QStringList userIDs;
    for(int i = 0; i < dataRows.size(); i++){
        userIDs.append(dataRows.at(i)[USER_ID]);
    }
    return userIDs;
}

void DataHandlerBase::convertToDataPackage(const QList<QMap<QString, QString> > &keyValue,
                                           const DataHead &dataHead, const QString &tableName,
                                           const QString &tableKeyID, DataPackage &dataPackage)
{
    // Head
    dataPackage.head = dataHead;
    // Summary
    dataPackage.summary.code = 100;
    dataPackage.summary.result = 1;
    dataPackage.summary.description = "请求数据成功完成";
    // Table
    DataTable dataTable;
    dataTable.key = tableKeyID;
    dataTable.name = tableName;
    dataTable.value.append(keyValue);
    dataPackage.tables.append(dataTable);
}

void DataHandlerBase::convertToDataPackage(const QList<QMap<QString, QString> > &keyValue, const DataHead &dataHead,
                                           const QMap<QString, QString> &para, const QString &tableName, const QString &tableKeyID, DataPackage &dataPackage)
{
    // Head
    dataPackage.head = dataHead;
    // Summary
    dataPackage.summary.code = 100;
    dataPackage.summary.result = 1;
    dataPackage.summary.description = "请求数据成功完成";
    dataPackage.para = para;
    dataPackage.para["OfflineModel"] = "1";
    // Table
    DataTable dataTable;
    dataTable.key = tableKeyID;
    dataTable.name = tableName;
    dataTable.value.append(keyValue);
    dataPackage.tables.append(dataTable);
}

DataOperationType DataHandlerBase::getDataOperationType()
{
    return m_nDataOperationType;
}

void DataHandlerBase::networkResponse(QString objects, QString action, bool result, int error, DbusPackage map)
{
    qDebug() << Q_FUNC_INFO <<" action " << action << " objects " << objects;
    QString requestAction = m_nActionList[action];
    if(requestAction == ""){
        qDebug() << "!m_nActionList.contains(action)  return";
        return;
    }
    if(objects == "binary" && map.otherMap["binaryType"] == "binaryProgress"){
        emit onResultOfResponse(objects, requestAction, result, error, map);
    } else{
        m_nActionList.remove(action);
        //纯在线请求,直接返回信号
        if(m_nPureOnLineActionList.contains(action)){
            m_nPureOnLineActionList.removeOne(action);
            emit onResultOfResponse(objects, requestAction, result, error, map);
        }else{
            if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs &&
                    result == true &&
                    error == NETWORK_NO_ERROR){
                //写设备表
                writeDeviceToDB(map.dataPackage);
            }
            networkResult(objects, requestAction, result, error, map);
        }
    }
}

void DataHandlerBase::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    emit onResultOfResponse(objects, action, result, error, map);
    m_nRequestPkgMap.remove(action);
}

QString DataHandlerBase::createUuid()
{
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    return uuid;
}

void DataHandlerBase::sendNoNetworkResponse(const QString &objects, const QString &action)
{
    qDebug() << Q_FUNC_INFO << " 没网，直接返回  action " << action;
    DbusPackage map;
    map.otherMap.insert("objects", objects);
    map.otherMap.insert("serverCode", "0");
    map.otherMap.insert("description", "no network");
    emit onResultOfResponse(objects, action, false, NETWORK_ERROR_NONETWORK, map);
}

bool DataHandlerBase::requestObjs(const QMap<QString, QString> &objIDModifyTimeMap, const QString &taskId)
{
    QStringList objIDs;
    QMap<QString, QString>::const_iterator iter = objIDModifyTimeMap.begin();
    while (iter != objIDModifyTimeMap.end()) {
        QString objID = iter.key();
        if(!objIDs.contains(objID) && isNeedSync(objID, iter.value()) ){
            objIDs.append(objID);
        }
        ++iter;
    }
    qDebug() << Q_FUNC_INFO << " objIDs.count() :" << objIDs.count();
    if(objIDs.count() > 0){
        DataPackage dataPackage;
        dataPackage.head.key = PACKAGE_HEAD_KEY;
        dataPackage.head.objects = EMB_Off_GetEquipmentAndPartsListByJoinIDs;
        dataPackage.head.type = "1";
        dataPackage.para["joinids"] = objIDs.join(",");
        sendData(dataPackage, taskId);
        return true;
    }
    return false;
}

bool DataHandlerBase::isNeedSync(const QString &objID, const QString &modifiedDate){
    QString selectionArg = " where ID = '" + objID + "' and ModifiedDate >= '" + modifiedDate + "'";
    QStringList columns;
    QList<QMap<QString, QString> > valueList;
    valueList = m_nDbHelper->select("EMB_Equipment", columns, selectionArg);
    return (valueList.count() == 0);
}

void DataHandlerBase::writeDeviceToDB(const DataPackage &dataPackage)
{
    if(dataPackage.tables.count() > 0){
        m_nDbHelper->beginTransaction();

        //获取本地数据库设备表中的字段
        DBConfigManager dbConfigMng;
        QStringList deviceFieldNameList, partFieldNameList;
        dbConfigMng.getTableFieldNameList(BUSINESS_TYPE_DEVICES, "EMB_Equipment", deviceFieldNameList);
        dbConfigMng.getTableFieldNameList(BUSINESS_TYPE_DEVICES, "EMB_Equipment_Part", partFieldNameList);

        //设备表
        QStringList column;
        QString selection;
        QString deviceID;
        QList<DataRow> dataRows;
        column.append("ID");
        const QList<QMap<QString, QString> > &deviceValueList = dataPackage.tables.at(0).value;
        for(int i = 0; i < deviceValueList.count(); i++){
            QMap<QString, QString> value;
            //字段过滤，删除本地数据库设备表没有的字段
            for(int j = 0; j < deviceFieldNameList.count(); j++){
                QString fieldName = deviceFieldNameList.at(j);
                if(deviceValueList.at(i).contains(fieldName))
                    value[fieldName] = deviceValueList.at(i)[fieldName];
            }
            QString RFIDCode = value["RFIDCode"];
            if(RFIDCode != ""){
                DataRow updateRow;
                updateRow["RFIDCode"] = "";
                selection = " where RFIDCode = '" + RFIDCode + "'";
                m_nDbHelper->update("EMB_Equipment", updateRow, selection);
            }
            QString BarCode = value["BarCode"];
            if(BarCode != ""){
                selection = " where BarCode = '" + BarCode + "'";
                DataRow updateRow;
                updateRow["BarCode"] = "";
                m_nDbHelper->update("EMB_Equipment", value, selection);
            }
            value["SyncStatus"] = "1";
            deviceID = value[DB_ID];
            selection = " where " + QString(DB_ID) + "='" + deviceID + "'";
            dataRows = m_nDbHelper->select("EMB_Equipment", column, selection);
            if(dataRows.isEmpty()){
                m_nDbHelper->insert("EMB_Equipment", value);
            }else{
                m_nDbHelper->update("EMB_Equipment", value, selection);
            }
        }

        //部件表
        const QList<QMap<QString, QString> > &partValueList = dataPackage.tables.at(1).value;
        for(int i = 0; i < partValueList.count(); i++){
            QMap<QString, QString> value;
            //字段过滤，删除本地数据库部件表没有的字段
            for(int j = 0; j < partFieldNameList.count(); j++){
                QString fieldName = partFieldNameList.at(j);
                if(partValueList.at(i).contains(fieldName))
                    value[fieldName] = partValueList.at(i)[fieldName];
            }
            deviceID = value[DB_ID];
            selection = " where " + QString(DB_ID) + "='" + deviceID + "'";
            dataRows = m_nDbHelper->select("EMB_Equipment_Part", column, selection);
            if(dataRows.isEmpty()){
                m_nDbHelper->insert("EMB_Equipment_Part", value);
            }else{
                m_nDbHelper->update("EMB_Equipment_Part", value, selection);
            }
        }
        m_nDbHelper->endTransaction();
    }
}
