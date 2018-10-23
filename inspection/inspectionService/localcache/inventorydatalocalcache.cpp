#include "inventorydatalocalcache.h"
#include <QUuid>
#include <QFile>
#include "../DBConfig/dbconfiginventory.h"
#include "../networkcontrol.h"
#include "../common/dbhelper.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../../framework/config/sdisruntimecontext.h"
#define INVENTORY_DIRCTORY "/INVENTORY/"

InventoryDataLocalCache::InventoryDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_INVENTORY;
}
InventoryDataLocalCache::~InventoryDataLocalCache()
{
}
bool InventoryDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects << "  serialKey  " << dataPackage.binaries.count();
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    if(dataPackage.tables.length() > 0){
        QString strXmlName = writeXMLFile(dataPackage, dataPath + INVENTORY_DIRCTORY);
        qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;

        QString inventoryId = dataPackage.tables.at(0).value.at(0)["ID"];
        QString taskId = dataPackage.tables.at(0).value.at(0)["TaskID"];
        writeCacheTable(action, taskId, inventoryId, strXmlName, offlineModel);
    }

    if(dataPackage.binaries.length() > 0){
        for(int i = 0; i < dataPackage.binaries.length(); i++){
            QString binaryFilePath = saveBinaryLocalCache(dataPackage.binaries.at(i), dataPath + INVENTORY_DIRCTORY);
            if(binaryFilePath != ""){
                writeBinaryDBTable(action, dataPackage.binaries.at(i), DBConfigInventory::TABLE_INVENTORYSUBMIT_BINARY , binaryFilePath, offlineModel);
            } else {
                qDebug() << "saveBinaryLocalCache  return empty";
                continue;
            }
        }
    }
    return true;
}

void InventoryDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    m_nFailedCount = 0;
    if(binaryType){
        m_nBinaryValueList = dbHelper->select(DBConfigInventory::TABLE_INVENTORYSUBMIT_BINARY, column, " order by OfflineModel ASC");
    } else {
        m_nXmlValueList = dbHelper->select(DBConfigInventory::TABLE_INVENTORYSUBMIT, column, " order by OfflineModel ASC");
    }
    qDebug() << "InventoryDataLocalCache::submitLocalCache     ";
    commitData(binaryType);
}

void InventoryDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName;
    if(binaryType){
        tableName = DBConfigInventory::TABLE_INVENTORYSUBMIT_BINARY;
    } else {
        tableName = DBConfigInventory::TABLE_INVENTORYSUBMIT;
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

bool InventoryDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    if(binaryType)
        valueList = dbHelper->select(DBConfigInventory::TABLE_INVENTORYSUBMIT_BINARY, column, "");
    else
        valueList = dbHelper->select(DBConfigInventory::TABLE_INVENTORYSUBMIT, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

void InventoryDataLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "InventoryDataLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
    QString ID = action;
    //提交二进制文件返回结果
    if(objects == "binary" && (map.otherMap["binaryType"] == "binaryResponse" || map.otherMap["binaryType"] == "binaryError") ){
        m_nBinaryValueList.removeFirst();
        if(result == true){
            //从数据库中删除该条记录
            deleteLocalFile(ID, true);
        }
        emit onSubmitFinished(m_nDataOperationType, result, true);
    } else {
        //提交xml文件返回结果
        QString actionStr = m_nXmlValueList.at(0)["Action"];
        QString inventoryId = m_nXmlValueList.at(0)["InventoryID"];
        QString taskId = m_nXmlValueList.at(0)["TaskID"];
        QString offline = m_nXmlValueList.at(0)["OfflineModel"];
        if(offline == "0"){
            qDebug() << "在线发送信号出去给app   onResultOfResponse   " << objects << "  action  " << action << "  result " << result << "  actionStr " << actionStr;
            emit onResultOfResponse(objects, actionStr, result, error, map);
        }
        if(result == true){
            //从数据库中删除该条记录
            deleteLocalFile(ID, false);
            markLocalTaskObjectsFinished(taskId, inventoryId);
        } else
            m_nFailedCount++;
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "networkResult    commitData";
        commitData(false);
    }
}

void InventoryDataLocalCache::commitData(bool binaryType)
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
            qDebug() << "InventoryDataLocalCache m_nXmlValueList.count()== 0  emit onSubmitFinished ";
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

void InventoryDataLocalCache::writeCacheTable(const QString &action,  const QString &taskID,  const QString &inventoryId,
                                              const QString &strXmlName, const QString &offlineModel)
{
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("TaskID", taskID);
    fields.insert("InventoryID", inventoryId);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    DBHelper *dbHelper = DBHelper::getInstance();
     bool rv = dbHelper->insert(DBConfigInventory::TABLE_INVENTORYSUBMIT, fields);
     if(!rv)
         qDebug() << Q_FUNC_INFO <<" insert false";
     else
         markLocalTaskObjectsCached(taskID, inventoryId);
}

void InventoryDataLocalCache::deleteLocalFile(const QString &ID, bool binaryType)
{
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QString  tableName;
    QList<QMap<QString, QString> > valueList;
    if(binaryType){
        tableName = DBConfigInventory::TABLE_INVENTORYSUBMIT_BINARY;
    } else {
        tableName = DBConfigInventory::TABLE_INVENTORYSUBMIT;
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

void InventoryDataLocalCache::checkTaskFinished(const QString &taskId)
{
    QString strSql = "select ID from EMI_InventoryEquipment where TaskID = '" + taskId + "' and LocalStatus != '2' ";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    qDebug()<< Q_FUNC_INFO<< " taskId " << taskId <<  " valueList is " << valueList.size();
    if(valueList.size() == 0){
        DBHelper::getInstance()->deleteRow("EMI_InventoryEquipment", "where TaskID = '" + taskId + "'");
        DBHelper::getInstance()->deleteRow("EMI_InventoryTask", "where TaskID = '" + taskId + "'");
    }
}

void InventoryDataLocalCache::checkLocalTaskCached(const QString &taskId)
{
    QString strSql = "select ID from EMI_InventoryEquipment where TaskID = '" + taskId + "' and LocalStatus = '0' ";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    qDebug()<< Q_FUNC_INFO<< " taskId " << taskId <<  " valueList is " << valueList.size();
    if(valueList.size() == 0){
        QMap<QString, QString>  keyValue;
        keyValue[LOCAL_STATUS] = TASK_STATUS_CACHE;
        DBHelper::getInstance()->update("EMI_InventoryTask", keyValue, "where TaskID = '" + taskId + "'");
    }
}

void InventoryDataLocalCache::markLocalTaskObjectsFinished(const QString &taskId, const QString &inventoryId)
{
    qDebug() << Q_FUNC_INFO;
    QString updateArgs = " where ID = '" + inventoryId + "' and TaskID = '" + taskId + "'";
    QMap<QString, QString> keyValue;
    keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
    DBHelper::getInstance()->update("EMI_InventoryEquipment", keyValue, updateArgs);
    checkTaskFinished(taskId);
}

void InventoryDataLocalCache::markLocalTaskObjectsCached(const QString &taskId, const QString &inventoryId)
{
    qDebug() << Q_FUNC_INFO;
    QString updateArgs = " where ID = '" + inventoryId + "'";
    QList<QMap<QString, QString> > statusList;
    DBHelper::getInstance()->select("select LocalStatus from EMI_InventoryEquipment " + updateArgs,  statusList);
    if(statusList.size() > 0 && statusList.at(0)[LOCAL_STATUS] == TASK_STATUS_NO_FINISH){
        QMap<QString, QString> keyValue;
        keyValue[LOCAL_STATUS] = TASK_STATUS_CACHE;
        DBHelper::getInstance()->update("EMI_InventoryEquipment", keyValue, updateArgs);
        checkLocalTaskCached(taskId);
    }
}
