#include <QUuid>
#include <QFile>
#include "../DBConfig/dbconfigmessage.h"
#include "../networkcontrol.h"
#include "../common/dbhelper.h"
#include "messagelocalcache.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../../framework/config/sdisruntimecontext.h"
#define MESSAGE_DIRCTORY "/MESSAGE/"

MessageLocalCache::MessageLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_MESSAGE;
}

MessageLocalCache::~MessageLocalCache()
{

}

bool MessageLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects;
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    if(dataPackage.tables.length() > 0){
        QString strXmlName = writeXMLFile(dataPackage, dataPath + MESSAGE_DIRCTORY);
        qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;
        writeCacheTable(action, strXmlName, offlineModel);
    }
    return true;
}

void MessageLocalCache::submitLocalCache(bool binaryType)
{
    Q_UNUSED(binaryType);
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    m_nFailedCount = 0;
    m_nXmlValueList = dbHelper->select(DBConfigMessage::TABLE_MESSAGE_TOKEN, column, " order by ID ASC");
    commitData();
}

void MessageLocalCache::clearCache(bool binaryType)
{
    Q_UNUSED(binaryType);
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QString tableName = DBConfigMessage::TABLE_MESSAGE_TOKEN;
    QList<QMap<QString, QString> > valueList = dbHelper->select(tableName, column, "");
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

bool MessageLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO  << "  binaryType " << binaryType;
    if(binaryType)
        return false;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QString tableName = DBConfigMessage::TABLE_MESSAGE_TOKEN;
    QList<QMap<QString, QString> > valueList = dbHelper->select(tableName, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

void MessageLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "MessageLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
    QString ID = action;
    QString actionStr = m_nXmlValueList.at(0)["Action"];
    QString offline = m_nXmlValueList.at(0)["OfflineModel"];
    if(offline == "0"){
        qDebug() << "在线情况下发送信号出去给客户端   onResultOfResponse    " << objects << "  action  " << action << "  result " << result << "  actionStr " << actionStr;
        emit onResultOfResponse(objects, actionStr, result, error, map);
    }
    if(result){
        //从数据库中删除该条记录
        deleteLocalFile(ID);
    } else
        m_nFailedCount++;
    m_nXmlValueList.removeFirst();
    //继续提交下一条
    qDebug() << "networkResult    commitData";
    commitData();
}


void MessageLocalCache::writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel)
{    
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    DBHelper *dbHelper = DBHelper::getInstance();
    dbHelper->insert(DBConfigMessage::TABLE_MESSAGE_TOKEN , fields);
}

void MessageLocalCache::commitData()
{
    QString action;
    if(m_nXmlValueList.count() == 0){
        qDebug() << "MessageLocalCache m_nXmlValueList.count()== 0  emit onSubmitFinished ";
        if(m_nFailedCount > 0){
            emit onSubmitFinished(m_nDataOperationType, false, false);
        } else {
            emit onSubmitFinished(m_nDataOperationType, true, false);
        }
        return;
    }
    QMap<QString, QString> value = m_nXmlValueList.at(0);
    DataPackage dataPackage;
    bool rv = readXmlFile(value["FilePath"], dataPackage);
    if(!rv){
        qDebug() << "readXmlFile  return false";
        //从数据库中删除该条记录
        deleteLocalFile(value["ID"]);
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        commitData();
    } else {
        action = value["ID"];
        qDebug() << "sendData  xml file actionSequence is " << action;
        sendData(dataPackage, action, false);
    }
}

void MessageLocalCache::deleteLocalFile(const QString &ID)
{
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QString  tableName = DBConfigMessage::TABLE_MESSAGE_TOKEN;
    QList<QMap<QString, QString> > valueList = dbHelper->select(tableName, column, " where ID = '"+ ID + "'");
    qDebug() << Q_FUNC_INFO << valueList;
    for(int i = 0; i < valueList.count(); i++){
        QString filePath = valueList.at(i)["FilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    dbHelper->deleteRow(tableName, " where ID = '"+ ID + "'");
}

