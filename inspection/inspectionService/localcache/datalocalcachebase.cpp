#include "datalocalcachebase.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QTextStream>
#include <QUuid>
#include <errno.h>
#include "../../framework/dataManager/datadecoder.h"
#include "../../framework/dataManager/dataencoder.h"
#include "../../framework/common/functiontype.h"
#include "../../framework/network/networkmanager.h"
#include "../common/dbhelper.h"
#include "../networkcontrol.h"
static int fileNameRandSuffix  = 0;
DataLocalCacheBase::DataLocalCacheBase()
{
    m_nDbHelper = DBHelper::getInstance();
}

DataLocalCacheBase::~DataLocalCacheBase()
{
}

bool DataLocalCacheBase::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(dataPackage)
    Q_UNUSED(action)
    return true;
}

void DataLocalCacheBase::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(binaryType)
}

void DataLocalCacheBase::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(binaryType)
}

bool DataLocalCacheBase::hasLocalCache(bool binaryType)
{
    Q_UNUSED(binaryType)
    return false;
}
void DataLocalCacheBase::writeBinaryDBTable(const QString &action, const Attachment &attachment, const QString &tableName,
                                            const QString &binaryFilePath, const QString &offlineModel)
{
    int pos = binaryFilePath.lastIndexOf("/");
    QString fileName = binaryFilePath.right(binaryFilePath.length() - pos - 1);
    qDebug() << Q_FUNC_INFO << binaryFilePath << "   fileName is " << fileName << "  attachment.serialKey   " << attachment.serialKey;
    DBHelper *dbHelper = DBHelper::getInstance();
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("SerialKey", attachment.serialKey);
    fields.insert("Name", fileName);
    fields.insert("Title", attachment.title);
    fields.insert("Mime", attachment.mime);
    fields.insert("Description", attachment.description);
    fields.insert("FilePath", binaryFilePath);
    qDebug() << Q_FUNC_INFO << " title is " << attachment.title;
    dbHelper->insert(tableName, fields);
}

DataOperationType DataLocalCacheBase::getDataOperationType() const {
    return m_nDataOperationType;
}

QString DataLocalCacheBase::writeXMLFile(const DataPackage &dataPackage, const QString &savePath){
    if(fileNameRandSuffix == 10000)
        fileNameRandSuffix = 0;
    fileNameRandSuffix++;
    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmsszzz") + "_"  + QString::number(fileNameRandSuffix) + ".xml";
    QString filePath = savePath;
    qDebug() << " savePath is  " << savePath;
    QDir dir;
    if(!dir.exists(filePath)){
        dir.mkdir(filePath);
    }
    filePath = filePath + fileName;
    qDebug() << " filePath is  " << filePath;
    QFile fl(filePath);
    fl.open(QFile::WriteOnly | QFile::Truncate);
    DataEncoder dataEncoder;
    QString outputStr;
    if(dataEncoder.parsePackage2xmlStr(dataPackage, outputStr)){
        //qDebug() << " outputStr " << outputStr;
        qDebug() << "dataEncoder.parsePackage2xmlStr  ok";
        QTextStream out(&fl);
        out<< outputStr;
        fl.close();
        sync();
        return filePath;
    } else
        qDebug() << "dataEncoder.parsePackage2xmlStr  return false";
    return "";
}

bool DataLocalCacheBase::readXmlFile(const QString &filePath, DataPackage &dataPackage){
    QFile fl(filePath);
    if(fl.exists()){
        fl.open(QIODevice::ReadOnly);
        QString xmlStr = QString(fl.readAll());
        //qDebug() <<"filePath is " << filePath <<  "  readXmlFile:   " << xmlStr;
        DataDecoder dataDecoder;
        if(dataDecoder.parsePackage(xmlStr, dataPackage))
            return true;
        else
            return false;
    }
    return false;
}
QString DataLocalCacheBase::saveBinaryLocalCache(const Attachment &attachment, const QString &savePath)
{
    qDebug() << Q_FUNC_INFO << savePath;
    QDir dir;
    if(!dir.exists(savePath)){
       bool rv =  dir.mkdir(savePath);
       if(!rv)
           qDebug() << QString(strerror(errno));
    }
    if(fileNameRandSuffix == 10000)
        fileNameRandSuffix = 0;
    fileNameRandSuffix++;
    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmsszzz") + "_" + QString::number(fileNameRandSuffix);
    int pos = attachment.name.lastIndexOf(".");
    QString fileSuffix = attachment.name.right(attachment.name.length() - pos - 1);
    fileName = fileName + "." + fileSuffix;
    QString fileAboslutePath = savePath + fileName;
    QFile fl(attachment.path);
    bool rv = fl.copy(fileAboslutePath);
    qDebug() << "saveBinaryLocalCache  copy rv " << rv << " source file " << attachment.path << "  dest file is " << fileAboslutePath;
    sync();
    if(rv)
        return fileAboslutePath;
    else
        return "";
}

bool DataLocalCacheBase::isOnlieInterface(const QString &objects)
{
    Q_UNUSED(objects)
    return false;
}

void DataLocalCacheBase::networkResponse(QString objects, QString action, bool result, int error, DbusPackage map)
{
    qDebug() << Q_FUNC_INFO << " objects " << objects << "  action " << action << "  result " << result << "  error is " << error;
    if(objects == "binary" && map.otherMap["binaryType"] == "binaryProgress")
        return;
    QString requestAction = m_nActionSequences[action];
    if(requestAction == ""){
        qDebug() << "DataLocalCacheBase action is " << action << " not equal  "  << m_nActionSequences <<"   return";
        return;
    }
    m_nActionSequences.remove(action);
    networkResult(objects, requestAction, result, error, map);
}

void DataLocalCacheBase::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << Q_FUNC_INFO << " objects " << objects << "  action " << action << "  result " << result << "  error is " << error;
    Q_UNUSED(map)
}

QString DataLocalCacheBase::createUuid()
{
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    return uuid;
}

void DataLocalCacheBase::sendData(const DataPackage &dataPackage, const QString &action, const bool &binaryType)
{
    qDebug() << Q_FUNC_INFO << "sendData  xml file action is " << action;
    QString uuid = createUuid();
    m_nActionSequences.insert(uuid, action);
    NetworkControl::getInstance()->sendData(dataPackage, uuid, binaryType);
}
