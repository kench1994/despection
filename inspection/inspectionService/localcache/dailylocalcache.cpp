#include "dailylocalcache.h"
#include <QUuid>
#include <QFile>
#include "../DBConfig/dbconfigdaily.h"
#include "../networkcontrol.h"
#include "../common/dbhelper.h"
#include "../common/bussinessmap.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../framework/config/sdisruntimecontext.h"
#define DAILY_DIRCTORY "/DAILY/"
DailyLocalCache::DailyLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_DAILY;
}
DailyLocalCache::~DailyLocalCache()
{
}
bool DailyLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects << "  binaries  " << dataPackage.binaries.count();
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    if(dataPackage.tables.length() > 0){
        QString planObjectID = dataPackage.tables.at(0).value.at(0)["PlanObjectID"];
        QString strXmlName = writeXMLFile(dataPackage, dataPath + DAILY_DIRCTORY);
        qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;
        writeCacheTable(action, planObjectID, strXmlName, offlineModel);
    }

    if(dataPackage.binaries.length() > 0){
        for(int i = 0; i < dataPackage.binaries.length(); i++){
            QString binaryFilePath = saveBinaryLocalCache(dataPackage.binaries.at(i), dataPath + DAILY_DIRCTORY);
            if(binaryFilePath != ""){
                writeBinaryDBTable(action, dataPackage.binaries.at(i), DBConfigDaily::TABLE_DAILYSUBMIT_BINARY, binaryFilePath, offlineModel);
            } else {
                qDebug() << "saveBinaryLocalCache  return empty";
                continue;
            }
        }
    }
    return true;
}

void DailyLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    m_nFailedCount = 0;
    if(binaryType){
        m_nBinaryValueList = dbHelper->select(DBConfigDaily::TABLE_DAILYSUBMIT_BINARY, column, " order by OfflineModel ASC");
    } else {
        m_nXmlValueList = dbHelper->select(DBConfigDaily::TABLE_DAILYSUBMIT, column, " order by OfflineModel ASC");
    }
    qDebug() << "DailyLocalCache::submitLocalCache     ";
    commitData(binaryType);
}

void DailyLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName;
    if(binaryType){
        tableName = DBConfigDaily::TABLE_DAILYSUBMIT_BINARY;
    } else {
        tableName = DBConfigDaily::TABLE_DAILYSUBMIT;
    }

    valueList = dbHelper->select(tableName, column, "");
    //删除缓存文件
    for(int i = 0; i < valueList.size(); i++){
        QString filePath = valueList.at(i)["FilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    //清空数据库
    dbHelper->deleteRow(tableName,  "");
}

bool DailyLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    if(binaryType)
        valueList = dbHelper->select(DBConfigDaily::TABLE_DAILYSUBMIT_BINARY, column, "");
    else
        valueList = dbHelper->select(DBConfigDaily::TABLE_DAILYSUBMIT, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

void DailyLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "DailyLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
    QString ID = action;
    if(objects == "binary" && (map.otherMap["binaryType"] == "binaryResponse" || map.otherMap["binaryType"] == "binaryError") ){
        m_nBinaryValueList.removeFirst();
        if(result == true){
            //从数据库中删除该条记录
            deleteLocalFile(ID, true);
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
            deleteLocalFile(ID, false);
        } else
            m_nFailedCount++;
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "networkResult    commitData";
        commitData(false);
    }
}

void DailyLocalCache::commitData(bool binaryType)
{
    if(binaryType){
        if(m_nBinaryValueList.count() == 0)
            return;
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
        QString action =  value["ID"];
        sendData(dataPackage, action, true);
    } else {
        if(m_nXmlValueList.count() == 0){
            qDebug() << "DailyLocalCache m_nXmlValueList.count()== 0  emit onSubmitFinished ";
            if(m_nFailedCount > 0){
                qDebug() << "commitData    onSubmitFinished  begin false ";
                emit onSubmitFinished(m_nDataOperationType, false, binaryType);
            } else {
                qDebug() << "commitData    onSubmitFinished  begin  true";
                emit onSubmitFinished(m_nDataOperationType, true, binaryType);
            }
            return;
        }
        QMap<QString, QString> value = m_nXmlValueList.at(0);
        DataPackage dataPackage;
        bool rv = readXmlFile(value["FilePath"], dataPackage);
        if(!rv){
            qDebug() << "readXmlFile  return false";
            //从数据库中删除该条记录
            deleteLocalFile(value["ID"], false);
            m_nXmlValueList.removeFirst();
            //继续提交下一条
            commitData(false);
        } else {
            QString action =  value["ID"];
            qDebug() << "sendData  xml file action is " << action;
            sendData(dataPackage, action, false);
        }
    }
}

bool DailyLocalCache::writeCacheTable(const QString &action, const QString &planObjectID, const QString &strXmlName, const QString &offlineModel)
{
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("PlanObjectID", planObjectID);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    DBHelper *dbHelper = DBHelper::getInstance();
    bool rv = dbHelper->insert(DBConfigDaily::TABLE_DAILYSUBMIT, fields);
    return rv;
}

void DailyLocalCache::deleteLocalFile(const QString &ID, bool binaryType)
{
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QString  tableName;
    QList<QMap<QString, QString> > valueList;
    if(binaryType){
        tableName = DBConfigDaily::TABLE_DAILYSUBMIT_BINARY;
    } else {
        tableName = DBConfigDaily::TABLE_DAILYSUBMIT;
    }
    valueList = dbHelper->select(tableName, column, " where ID = '"+ ID + "'");
    qDebug() << Q_FUNC_INFO << valueList;
    for(int i = 0; i < valueList.count(); i++){
        QString filePath = valueList.at(i)["FilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    dbHelper->deleteRow(tableName, " where ID = '"+ ID + "'");
}
