#include "homehandler.h"
#include "../common/runtimeparam.h"
#include "../DBConfig/dbconfiguserinfo.h"
#include "../DBConfig/dbconfighomeinfo.h"
#include "../framework/common/systemconfig.h"
#include "datahandlerfactory.h"

HomeHandler::HomeHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_HOME;
}

HomeHandler::~HomeHandler()
{
}

void HomeHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO;
    if(dataPackage.head.objects == Module_Query_ItemCounts_ForAndroid){
        DbusPackage dbusPackage;
        getLocalItemCounts(dataPackage, dbusPackage.dataPackage);
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
        m_nRequestPkgMap.remove(action);
    }
}

int HomeHandler::getItemCount()
{
    return 0;
}

void HomeHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    Q_UNUSED(action)
    Q_UNUSED(objects)
    qDebug() <<Q_FUNC_INFO;
}

void HomeHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        handleItemCounts(map.dataPackage);
        emit onResultOfResponse(objects, action, result, error, map);
        m_nRequestPkgMap.remove(action);
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}

void HomeHandler::handleItemCounts(const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO;
    QString uuid;
    QString userID = RuntimeParam::getInstance()->getLoginUserMsgByKey("UserID");
    m_nDbHelper->beginTransaction();
    m_nDbHelper->deleteRow(HOME_TABLE_MODULE_COUNT, " where " + QString(USER_ID) + "='" + userID + "'");
    QList<DataRow> dataRows0 = dataPackage.tables.at(0).value;
    foreach (DataRow dataRow, dataRows0) {
        uuid = SystemConfig::getInstance()->getUUID();
        dataRow.insert(DB_ID, uuid);
        dataRow.insert(USER_ID, userID);
        m_nDbHelper->insert(HOME_TABLE_MODULE_COUNT, dataRow);
    }
    m_nDbHelper->deleteRow(HOME_TABLE, " where " + QString(USER_ID) + "='" + userID + "'");
    QList<DataRow> dataRows1 = dataPackage.tables.at(1).value;
    foreach (DataRow dataRow, dataRows1) {
        dataRow.insert(USER_ID, userID);
        m_nDbHelper->insert(HOME_TABLE, dataRow);
    }
    m_nDbHelper->endTransaction();
}

void HomeHandler::getLocalItemCounts(const DataPackage &dataPackage, DataPackage &resultPackage){
    qDebug() << Q_FUNC_INFO;
    QMap<DataOperationType, QMap<QString,QString> > modules;
    initModules(modules);
    QString userID = dataPackage.para.value(USER_ID);
    resultPackage.head.objects = dataPackage.head.objects;
    resultPackage.head.type = dataPackage.head.type;
    resultPackage.head.key = dataPackage.head.key;
    resultPackage.head.date = dataPackage.head.date;
    DataTable table0;
    table0.name = HOME_TABLE_MODULE_COUNT;
    table0.key = MODULE_ID;
    QList<DataRow> dataRows0;
    int itemCount = 0, totalItemCount = 0;
    RuntimeParam * runtimeParam = RuntimeParam::getInstance();
    DataHandlerFactory *handlerFactory = new DataHandlerFactory();
    QMap<DataOperationType, QMap<QString,QString> >::const_iterator iterator = modules.constBegin();
    for(; iterator != modules.constEnd(); iterator++){
        DataOperationType operType = iterator.key();
        QString shortName = iterator.value().value(MODULE_NAME);
        QString moduleID = iterator.value().value(MODULE_ID);
        if(runtimeParam->checkUserPermission(shortName)){
            DataRow dataRow;
            itemCount = handlerFactory->getDataHandler(operType)->getItemCount();
            if(itemCount == -1){
                itemCount = getModuleItemCount(userID, moduleID);
            }
            dataRow.insert(MODULE_ID, moduleID);
            dataRow.insert("OperType", QString::number(operType));
            dataRow.insert(ITEM_COUNT, QString::number(itemCount));
            dataRows0.append(dataRow);
            totalItemCount += itemCount;
        }
    }
    table0.value.append(dataRows0);
    resultPackage.tables.append(table0);


    int totalCount = getTotalCount(userID);
    DataTable table1;
    table1.name = "CMDS_TotalTask";
    table1.key = "";
    QList<DataRow> dataRows1;
    DataRow dataRow;
    dataRow.insert("Total", QString::number(totalCount));
    dataRow.insert("finish", QString::number(totalCount-totalItemCount));
    dataRows1.append(dataRow);
    table1.value.append(dataRows1);
    resultPackage.tables.append(table1);
}

void HomeHandler::moduleInfo(const QString &module, QMap<QString, QString> &map){
    QStringList column;
    column.append(MODULE_ID);
    QList<DataRow > dataRows = m_nDbHelper->select(USER_TABLE_MODULE, column, " where " + QString(SHORT_NAME) + "='" + module +"'");
    map.insert(MODULE_NAME, module);
    map.insert(MODULE_ID, dataRows.at(0).value(MODULE_ID));
}

int HomeHandler::getTotalCount(const QString& userID){
    QStringList column;
    QList<DataRow > dataRows = m_nDbHelper->select(HOME_TABLE, column, " where " + QString(USER_ID) + "= '" + userID +"'");
    if(!dataRows.isEmpty()){
        return dataRows.at(0).value("Total").toInt();
    }else{
        return 0 ;
    }
}

int HomeHandler::getModuleItemCount(const QString &userID, const QString &moduleID)
{
    QStringList column;
    QList<DataRow > dataRows = m_nDbHelper->select(HOME_TABLE_MODULE_COUNT, column,
                                                " where " + QString(USER_ID) + "='" + userID + "' and " + QString(MODULE_ID) + "='"+ moduleID +"'");
    if(!dataRows.isEmpty()){
        return dataRows.at(0).value(ITEM_COUNT).toInt();
    }else{
        return 0 ;
    }
}

void HomeHandler::initModules(QMap<DataOperationType, QMap<QString,QString> > &modules){
    ///巡检
    QMap<QString, QString> map0;
    moduleInfo("Inspection", map0);
    modules.insert(BUSINESS_TYPE_INSPECTION, map0);

    ///盘点
    QMap<QString, QString> map1;
    moduleInfo("Inventory", map1);
    modules.insert(BUSINESS_TYPE_INVENTORY, map1);

    ///保养
    QMap<QString, QString> map2;
    moduleInfo("Maintenance", map2);
    modules.insert(BUSINESS_TYPE_MAINTENANCE, map2);

    ///标准化
    QMap<QString, QString> map3;
    moduleInfo("Standardization", map3);
    modules.insert(BUSINESS_TYPE_STANDARDIZATION, map3);

    ///运维
    QMap<QString, QString> map4;
    moduleInfo("Operation", map4);
    modules.insert(BUSINESS_TYPE_OPERATION, map4);

}
