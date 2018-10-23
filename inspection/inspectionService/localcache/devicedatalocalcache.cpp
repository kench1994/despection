#include "devicedatalocalcache.h"
#include "../common/bussinessmap.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include <QDebug>
#define DEVIES_DIRCTORY "/DEVICES/"
DeviceDataLocalCache::DeviceDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICES;
}

DeviceDataLocalCache::~DeviceDataLocalCache()
{

}

bool DeviceDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO;
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    //如果是纯在线接口,强制将提交数据改成在线模式
    if(isOnlieInterface(dataPackage.head.objects))
        offlineModel = "0";

    if(dataPackage.tables.length() > 0){
        /**
        * Step 1. 写 xml 文件
        */
        QString strXmlName = writeXMLFile(dataPackage, dataPath + DEVIES_DIRCTORY);
        qDebug() <<"writeCacheTable    xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;
        /**
        * Step 2. 写缓存表
        */
        writeCacheTable(action, strXmlName, offlineModel);
    }
    if(dataPackage.binaries.length() > 0){
        for(int i = 0; i < dataPackage.binaries.length(); i++){
            QString binaryFilePath = saveBinaryLocalCache(dataPackage.binaries.at(i), dataPath + DEVIES_DIRCTORY);
            if(binaryFilePath != ""){
                writeBinaryDBTable(action, dataPackage.binaries.at(i), DEVICE_SUBMIT_ATTACH_TABLE, binaryFilePath, offlineModel);
            } else {
                qDebug() << "saveBinaryLocalCache  return empty";
                continue;
            }
        }
    }
    return true;
}

void DeviceDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    m_nFailedCount = 0;
    if(binaryType){
        submitBinary();
    } else {
        submitXml();
    }
}

void DeviceDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName;
    if(binaryType){
        tableName = DEVICE_SUBMIT_ATTACH_TABLE;
    } else {
        tableName = DEVICE_SUBMIT_TABLE;
    }
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

bool DeviceDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    QString tableName;
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    if(binaryType){
        tableName = DEVICE_SUBMIT_ATTACH_TABLE;
    } else {
        tableName = DEVICE_SUBMIT_TABLE;
    }
    valueList = m_nDbHelper->select(tableName, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

bool DeviceDataLocalCache::isOnlieInterface(const QString &objects)
{
    if(objects == EMB_Equipment_QRBCode_Import ||
            objects == EMB_Equipment_UpdateSerialKey){
        return true;
    }
    return false;
}

void DeviceDataLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "DeviceDataLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
    if(objects == "binary" && (map.otherMap["binaryType"] == "binaryResponse" || map.otherMap["binaryType"] == "binaryError") ){
        m_nBinaryValueList.removeFirst();
        if(result){
            //从数据库中删除该条记录
            deleteLocalFile(action, true);
        }
        emit onSubmitFinished(m_nDataOperationType, result, true);
    } else {
        QString actionStr = m_nXmlValueList.at(0)["Action"];
        QString offline = m_nXmlValueList.at(0)["OfflineModel"];
        if(offline == "0"){
            qDebug() << "在线情况下发送信号出去给客户端   onResultOfResponse    " << objects << "  action  " << action << "  result " << result << "  actionStr " << actionStr;
            emit onResultOfResponse(objects, actionStr, result, error, map);
        }
        if(result == true){
            //从数据库中删除该条记录
            deleteLocalFile(action, false);
        } else {
            m_nFailedCount++;
        }
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "networkResult    resultProcess";
        resultProcess();
    }
}

void DeviceDataLocalCache::writeCacheTable(const QString &action, const QString &strXmlName, const QString &offlineModel)
{
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    m_nDbHelper->insert(DEVICE_SUBMIT_TABLE, fields);
}

void DeviceDataLocalCache::submitXml()
{
    QStringList column;
    m_nFailedCount = 0;
    m_nXmlValueList = m_nDbHelper->select(DEVICE_SUBMIT_TABLE, column, " order by OfflineModel ASC");
    qDebug() << "DeviceDataLocalCache::submitXml   m_nXmlValueList  " << m_nXmlValueList.size();
    if(m_nXmlValueList.size() == 0){
        qDebug() << "submitXml onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, false);
    }else{
        commitData(false);
    }
}

void DeviceDataLocalCache::submitBinary()
{
    QStringList column;
    m_nBinaryValueList = m_nDbHelper->select(DEVICE_SUBMIT_ATTACH_TABLE, column, " order by OfflineModel ASC");
    qDebug() << "DeviceDataLocalCache::submitBinary   m_nBinaryValueList  " << m_nBinaryValueList.size();
    if(m_nBinaryValueList.size() == 0){
        qDebug() << "submitBinary onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, true);
    }else{
        commitData(true);
    }
}

void DeviceDataLocalCache::commitData(const bool &binaryType)
{
    if(binaryType){
        QMap<QString, QString> value = m_nBinaryValueList.at(0);
        DataPackage dataPackage;
        Attachment attch;
        attch.description = value["Description"];
        attch.title = value["Title"];
        attch.mime = value["Mime"];
        attch.serialKey = value["SerialKey"];
        attch.path = value["FilePath"];
        attch.name = value["Name"];
        dataPackage.attachments.append(attch);
        sendData(dataPackage, value["ID"], true);
    } else {
        QMap<QString, QString> value = m_nXmlValueList.at(0);
        DataPackage dataPackage;
        bool rv = readXmlFile(value["FilePath"], dataPackage);
        if(!rv){
            qDebug() << "readXmlFile  return false";
            //从数据库中删除该条记录
            m_nXmlValueList.removeFirst();
            deleteLocalFile(value["ID"], false);
            //继续提交下一条
            resultProcess();
        } else {
            sendData(dataPackage, value["ID"]);
        }
    }
}

void DeviceDataLocalCache::deleteLocalFile(const QString &ID, const bool &binaryType)
{
    QStringList column;
    QString  tableName;
    if(binaryType){
        tableName = DEVICE_SUBMIT_ATTACH_TABLE;
    } else {
        tableName = DEVICE_SUBMIT_TABLE;
    }
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

void DeviceDataLocalCache::resultProcess() {
    if (m_nXmlValueList.size() == 0) {
        if (m_nFailedCount == 0) {
            emit onSubmitFinished(m_nDataOperationType, true, false);
        } else {
            emit onSubmitFinished(m_nDataOperationType, false, false);
        }
    }else{
        commitData(false);
    }
}
