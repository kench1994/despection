#include "operationdatalocalcache.h"
#include <QUuid>
#include <QFile>
#include "../DBConfig/dbconfigoperation.h"
#include "../networkcontrol.h"
#include "../common/dbhelper.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../../framework/config/sdisruntimecontext.h"
#include "../common/bussinessmap.h"
#define OPERATION_DIRCTORY "/OPERATION/"

OperationDataLocalCache::OperationDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_OPERATION;
}
OperationDataLocalCache::~OperationDataLocalCache()
{
}
bool OperationDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects << "  serialKey  " << dataPackage.binaries.count();
        QString dataPath = SystemConfig::getInstance()->getAppDataPath();
        QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
        //如果是纯在线接口,强制将提交数据改成在线模式
        if(isOnlieInterface(dataPackage.head.objects))
            offlineModel = "0";

        if(dataPackage.tables.length() > 0){
            QString strXmlName = writeXMLFile(dataPackage, dataPath + OPERATION_DIRCTORY);
            qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
            if(strXmlName.isEmpty())
                return false;
            QString taskID = dataPackage.tables.at(0).value.at(0).value("ID");
            bool rv = writeCacheTable(action, strXmlName, offlineModel, taskID);
            if(!rv){
                qDebug() << "writeCacheTable   false return";
                return false;
            }
        }

        if(dataPackage.binaries.length() > 0){
            for(int i = 0; i < dataPackage.binaries.length(); i++){
                QString binaryFilePath = saveBinaryLocalCache(dataPackage.binaries.at(i), dataPath + OPERATION_DIRCTORY);
                if(binaryFilePath != ""){
                    writeBinaryDBTable(action, dataPackage.binaries.at(i), DBConfigOperation::TABLE_FAULTREPORTSUBMIT_BINARY, binaryFilePath, offlineModel);
                } else {
                    qDebug() << "saveBinaryLocalCache  return empty";
                    continue;
                }
            }
        }
    return true;
}

void OperationDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    m_nFailedCount = 0;
    if(binaryType){
        m_nBinaryValueList = dbHelper->select(DBConfigOperation::TABLE_FAULTREPORTSUBMIT_BINARY, column, " order by OfflineModel ASC");
    } else {
        m_nXmlValueList = dbHelper->select(DBConfigOperation::TABLE_FAULTREPORTSUBMIT, column, " order by OfflineModel ASC");
    }
    qDebug() << "OperationDataLocalCache::submitLocalCache     ";
    commitData(binaryType);
}

void OperationDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName;
    if(binaryType){
        tableName = DBConfigOperation::TABLE_FAULTREPORTSUBMIT_BINARY;
    } else {
        tableName = DBConfigOperation::TABLE_FAULTREPORTSUBMIT;
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

bool OperationDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    if(binaryType)
        valueList = dbHelper->select(DBConfigOperation::TABLE_FAULTREPORTSUBMIT_BINARY, column, "");
    else
        valueList = dbHelper->select(DBConfigOperation::TABLE_FAULTREPORTSUBMIT, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

bool OperationDataLocalCache::isOnlieInterface(const QString &objects)
{
    if(objects == EMF_FaultRepair_Import_Sync)
        return false;
    return true;
}

void OperationDataLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "OperationDataLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
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

void OperationDataLocalCache::commitData(bool binaryType)
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
            qDebug() << "OperationDataLocalCache m_nXmlValueList.count()== 0  emit onSubmitFinished ";
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

bool OperationDataLocalCache::writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel, const QString &taskID)
{
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    fields.insert("TaskID", taskID);
    DBHelper *dbHelper = DBHelper::getInstance();
    bool rv = dbHelper->insert(DBConfigOperation::TABLE_FAULTREPORTSUBMIT, fields);
    return rv;
}

void OperationDataLocalCache::deleteLocalFile(const QString &ID, bool binaryType)
{
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QString  tableName;
    QList<QMap<QString, QString> > valueList;
    if(binaryType){
        tableName = DBConfigOperation::TABLE_FAULTREPORTSUBMIT_BINARY;
    } else {
        tableName = DBConfigOperation::TABLE_FAULTREPORTSUBMIT;
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

