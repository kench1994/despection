#include "devicereceivedatalocalcache.h"
#include <QUuid>
#include <QFile>
#include "../common/dbhelper.h"
#include "../common/bussinessmap.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#define DEVICE_RECEIVE_DIRCTORY "/DEVICE_RECEIVE/"

DeviceReceiveDataLocalCache::DeviceReceiveDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICE_RECEIVE;
}

DeviceReceiveDataLocalCache::~DeviceReceiveDataLocalCache()
{
}

bool DeviceReceiveDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects << "  binaries  " << dataPackage.binaries.count();
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    if(dataPackage.tables.length() > 0){

        const DataRow &dataRow = dataPackage.tables.at(0).value.at(0);
        QString orderCode = dataRow["OrderCode"].toUpper();
        QString ERBQCode = dataRow["ERBQCode"];
        QString codeType = dataRow["CodeType"];
        QString equipmentID =  getEquipmentID(codeType, ERBQCode);

        /**
        * step 1 写缓存文件
        */
        QString strXmlName = writeXMLFile(dataPackage, dataPath + DEVICE_RECEIVE_DIRCTORY);
        qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;

        /**
        * step 2 写缓存表
        */
        writeCacheTable(action, strXmlName, offlineModel, orderCode, equipmentID);

        /**
        * step 3 修改任务状态，更新equipment表
        */
        updateTaskInfo(orderCode, equipmentID, TASK_STATUS_CACHE);
    }
    return true;
}

void DeviceReceiveDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    if(binaryType){
        emit onSubmitFinished(m_nDataOperationType, true, true);
        return;
    }
    submitXml();
}

void DeviceReceiveDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    if(binaryType){
        return;
    }
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName = DISTRIBUTION_TABLE_SUBMISSION;
    valueList = m_nDbHelper->select(tableName, column, "");
    //删除缓存文件
    for(int i = 0; i < valueList.size(); i++){
        QString filePath = valueList.at(i)["FilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    //清空数据库
    m_nDbHelper->deleteRow(tableName,  "");
}

bool DeviceReceiveDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    if(binaryType){
        return false;
    }
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    valueList = m_nDbHelper->select(DISTRIBUTION_TABLE_SUBMISSION, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

void DeviceReceiveDataLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "DeviceReceiveDataLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
    if(objects == EMD_DistributeEquipment_Import){
        const DataRow &dataRow = m_nXmlValueList.at(0);
        QString actionStr = m_nXmlValueList.at(0)["Action"];
        QString offline = m_nXmlValueList.at(0)["OfflineModel"];
        if(offline == "0"){
            qDebug() << "在线情况下发送信号出去给客户端   onResultOfResponse    " << objects << "  action  " << action << "  result " << result << "  actionStr " << actionStr;
            emit onResultOfResponse(objects, actionStr, result, error, map);
        }
        if(result == true){
            //从数据库中删除该条记录
            deleteLocalFile(action);
            updateTaskInfo(dataRow["OrderCode"], dataRow["EquipmentID"], TASK_STATUS_FINISH);
        } else {
            m_nFailedCount++;
        }
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "networkResult    resultProcess";
        resultProcess();
    }
}

void DeviceReceiveDataLocalCache::writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel, const QString &orderCode, const QString &equipmentID)
{
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    fields.insert("OrderCode", orderCode);
    fields.insert("EquipmentID", equipmentID);
    m_nDbHelper->insert(DISTRIBUTION_TABLE_SUBMISSION, fields);
}

void DeviceReceiveDataLocalCache::updateTaskInfo(const QString &orderCode,
                                                 const QString &equipmentID,
                                                 const QString &localStatus) {
    qDebug() << Q_FUNC_INFO << " orderCode " << orderCode << ", equipmentID "<< equipmentID << ", localStatus "<< localStatus;
    /**
    * step 1 : update object
    */
    DataRow updateRow;
    QString updateArg = " where " + QString(ORDER_CODE) + "='" + orderCode + "' and "+ QString(EQUIPMENT_ID) + "='" + equipmentID + "'";
    updateRow[LOCAL_STATUS] = localStatus;
    m_nDbHelper->update(DISTRIBUTION_TABLE_EQUIPMENT, updateRow, updateArg);

    /**
    * step 2 : if task doesn't have any equipment then delete task
    */
    if(!hasEquipment(orderCode, localStatus)) {
        if(localStatus == TASK_STATUS_FINISH){
            updateArg = " where " + QString(ORDER_CODE) + "='" + orderCode + "'";
            m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_TASK,   updateArg);
            m_nDbHelper->deleteRow(DISTRIBUTION_TABLE_EQUIPMENT,   updateArg);
        }else{
            updateArg = " where " + QString(ORDER_CODE) + "='" + orderCode + "' and "
                        + QString(LOCAL_STATUS) + "<'" + localStatus + "'";
            m_nDbHelper->update(DISTRIBUTION_TABLE_TASK, updateRow, updateArg);
            m_nDbHelper->update(DISTRIBUTION_TABLE_EQUIPMENT, updateRow, updateArg);
        }
    }
}

QString DeviceReceiveDataLocalCache::getEquipmentID(const QString &type, const QString &code) {
    QString equipmentID;
    QString sql;
    if(type ==  "1"){
        sql = "select ID from EMB_Equipment where EquipmentCode='" + code + "'";
    }else if(type == "2"){
        sql = "select ID from EMB_Equipment where LOWER(RFIDCode)='" + code.toLower() + "'";
    }else if(type == "3"){
        sql = "select ID from EMB_Equipment where LOWER(BarCode)='" + code.toLower() + "'";
    }else if(type == "4"){
        sql = "select ID from EMB_Equipment where LOWER(QRCode)='" + code.toLower() + "'";
    }
    sql += " order by ModifiedDate desc";
    QList<DataRow> dataRows;
    bool result = m_nDbHelper->select(sql, dataRows);
    if(result && dataRows.size() != 0){
        equipmentID = dataRows.at(0).value("ID");
    }
    return equipmentID;
}

bool DeviceReceiveDataLocalCache::hasEquipment(const QString &orderCode, const QString &localStatus) {
    QStringList column;
    QString selectArg = " where " + QString(ORDER_CODE) +
                        "='" + orderCode + "' and " + QString(LOCAL_STATUS) +
                        "<'" + localStatus + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(DISTRIBUTION_TABLE_EQUIPMENT, column, selectArg);
    return !dataRows.isEmpty();
}

void DeviceReceiveDataLocalCache::submitXml()
{
    QStringList column;
    m_nFailedCount = 0;
    m_nXmlValueList = m_nDbHelper->select(DISTRIBUTION_TABLE_SUBMISSION, column, " order by OfflineModel ASC");
    qDebug() << "DeviceReceiveDataLocalCache::submitXml   m_nXmlValueList  " << m_nXmlValueList.size();
    if(m_nXmlValueList.size() == 0){
        qDebug() << "submitXml onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, false);
    }else{
        commitData();
    }
}

void DeviceReceiveDataLocalCache::commitData()
{
    QMap<QString, QString> value = m_nXmlValueList.at(0);
    DataPackage dataPackage;
    bool rv = readXmlFile(value["FilePath"], dataPackage);
    if(!rv){
        qDebug() << "readXmlFile  return false";
        //从数据库中删除该条记录
        m_nXmlValueList.removeFirst();
        deleteLocalFile(value["ID"]);
        //继续提交下一条
        resultProcess();
    } else {
        sendData(dataPackage, value["ID"]);
    }
}

void DeviceReceiveDataLocalCache::deleteLocalFile(const QString &ID)
{
    QStringList column;
    QString  tableName = DISTRIBUTION_TABLE_SUBMISSION;
    QList<QMap<QString, QString> > valueList;
    valueList = m_nDbHelper->select(tableName, column, " where ID = '"+ ID + "'");
    qDebug() << Q_FUNC_INFO << valueList;
    for(int i = 0; i < valueList.count(); i++){
        QString filePath = valueList.at(i)["FilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    m_nDbHelper->deleteRow(tableName, " where ID = '"+ ID + "'");
}

void DeviceReceiveDataLocalCache::resultProcess() {
    if (m_nXmlValueList.size() == 0) {
        if (m_nFailedCount == 0) {
            emit onSubmitFinished(m_nDataOperationType, true, false);
        } else {
            emit onSubmitFinished(m_nDataOperationType, false, false);
        }
    }else{
        commitData();
    }
}

