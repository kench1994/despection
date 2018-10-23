#include "rectificationlocalcache.h"
#include <QUuid>
#include <QFile>
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#define RECTIFICATION_DIRCTORY "/RECTIFICATION/"

RectificationLocalCache::RectificationLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_RECTIFICATION;
}
RectificationLocalCache::~RectificationLocalCache()
{
}
bool RectificationLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects << "  serialKey  " << dataPackage.binaries.count();
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    if(dataPackage.tables.length() > 0){
        QString strXmlName = writeXMLFile(dataPackage, dataPath + RECTIFICATION_DIRCTORY);
        qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;
        const DataRow &dataRow = dataPackage.tables.at(0).value.at(0);
        QString orderID = dataRow["OrderID"];
        QString taskStatus = dataRow["Proctype"];
        writeCacheTable(action, strXmlName, offlineModel, orderID, taskStatus);
        modifyTaskStatus(orderID, taskStatus);
    }

    if(dataPackage.binaries.length() > 0){
        for(int i = 0; i < dataPackage.binaries.length(); i++){
            QString binaryFilePath = saveBinaryLocalCache(dataPackage.binaries.at(i), dataPath + RECTIFICATION_DIRCTORY);
            if(binaryFilePath != ""){
                writeBinaryDBTable(action, dataPackage.binaries.at(i), RECTIFICATION_TABLE_ATTACHMENT_SUBMIT, binaryFilePath, offlineModel);
            } else {
                qDebug() << "saveBinaryLocalCache  return empty";
                continue;
            }
        }
    }
    return true;
}

void RectificationLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName;
    if(binaryType){
        tableName = RECTIFICATION_TABLE_ATTACHMENT_SUBMIT;
    } else {
        tableName = RECTIFICATION_TABLE_SUBMIT;
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

bool RectificationLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    if(binaryType)
        valueList = m_nDbHelper->select(RECTIFICATION_TABLE_ATTACHMENT_SUBMIT, column, "");
    else
        valueList = m_nDbHelper->select(RECTIFICATION_TABLE_SUBMIT, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

void RectificationLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    m_nFailedCount = 0;
    if(binaryType){
        submitBinary();
    } else {
        submitXml();
    }
}


void RectificationLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "RectificationLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
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
        QString orderID = m_nXmlValueList.at(0)["OrderID"];
        QString taskStatus = m_nXmlValueList.at(0)["RectifyStatus"];
        if(offline == "0"){
            qDebug() << "在线情况下发送信号出去给客户端   onResultOfResponse    " << objects << "  action  " << action << "  result " << result << "  actionStr " << actionStr;
            emit onResultOfResponse(objects, actionStr, result, error, map);
        }
        if(result == true){
            //从数据库中删除该条记录
            deleteLocalFile(ID, false);
            deleteTask(orderID, taskStatus);
        } else
            m_nFailedCount++;
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "resultProcess    commitData";
        resultProcess();
    }
}

void RectificationLocalCache::submitXml()
{
    QStringList column;
    m_nFailedCount = 0;
    m_nXmlValueList = m_nDbHelper->select(RECTIFICATION_TABLE_SUBMIT, column, " order by OfflineModel ASC");
    qDebug() << "RectificationLocalCache::submitXml   m_nXmlValueList  " << m_nXmlValueList.size();
    if(m_nXmlValueList.size() == 0){
        qDebug() << "submitXml onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, false);
    }else{
        commitData(false);
    }
}

void RectificationLocalCache::resultProcess() {
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

void RectificationLocalCache::submitBinary()
{
    QStringList column;
    m_nBinaryValueList = m_nDbHelper->select(RECTIFICATION_TABLE_ATTACHMENT_SUBMIT, column, " order by OfflineModel ASC");
    qDebug() << "RectificationLocalCache::submitBinary   m_nBinaryValueList  " << m_nBinaryValueList.size();
    if(m_nBinaryValueList.size() == 0){
        qDebug() << "submitBinary onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, true);
    }else{
        commitData(true);
    }
}

void RectificationLocalCache::commitData(bool binaryType)
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
        DataRow dataRow = m_nXmlValueList.at(0);
        DataPackage dataPackage;
        bool rv = readXmlFile(dataRow["FilePath"], dataPackage);
        if(!rv){
            qDebug() << "readXmlFile  return false";
            //从数据库中删除该条记录
            m_nXmlValueList.removeFirst();
            deleteLocalFile(dataRow["ID"], false);
            //继续提交下一条
            resultProcess();
        } else {
            sendData(dataPackage, dataRow["ID"]);
        }
    }
}

void RectificationLocalCache::writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel, const QString &orderID, const QString &taskStatus)
{
    qDebug() << Q_FUNC_INFO << " action :" << action << ", strXmlName :" << strXmlName << ", offlineModel:" << offlineModel << ", orderID : " << orderID << ", taskStatus : " << taskStatus;
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    fields.insert("OrderID", orderID);
    fields.insert("RectifyStatus", taskStatus);
    m_nDbHelper->insert(RECTIFICATION_TABLE_SUBMIT, fields);
}

void RectificationLocalCache::deleteLocalFile(const QString &ID, bool binaryType)
{
    qDebug() << Q_FUNC_INFO << "  ID  " << ID;
    QStringList column;
    QString tableName;
    QList<DataRow> dataRows;
    if(binaryType){
        tableName = RECTIFICATION_TABLE_ATTACHMENT_SUBMIT;
    } else {
        tableName = RECTIFICATION_TABLE_SUBMIT;
    }
    dataRows = m_nDbHelper->select(tableName, column, " where ID = '" + ID + "'");
    qDebug() << Q_FUNC_INFO << dataRows;
    for(int i = 0; i < dataRows.count(); i++){
        QString filePath = dataRows.at(i)["FilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    m_nDbHelper->deleteRow(tableName, " where ID = '" + ID + "'");
}

void RectificationLocalCache::modifyTaskStatus(const QString &orederID, const QString &taskStatus){
    DataRow updateRow;
    QString updateArg = " where " + QString(ORDER_ID) + "='" + orederID + "' and " +
                        QString(RECTIFY_STATUS) + "='" + taskStatus + "'";
    updateRow[LOCAL_STATUS] = TASK_STATUS_CACHE;
    m_nDbHelper->update(RECTIFICATION_TABLE_TASK, updateRow, updateArg);

}

void RectificationLocalCache::deleteTask(const QString &orederID, const QString &taskStatus){
    QString deleteArg = " where " + QString(ORDER_ID) + "='" + orederID + "' and " +
                        QString(LOCAL_STATUS) + "='" + TASK_STATUS_CACHE + "'";
    m_nDbHelper->deleteRow(RECTIFICATION_TABLE_TASK, deleteArg);

    deleteArg = " where " + QString(ORDER_ID) + "='" + orederID + "' and " +
                QString(RECTIFY_STATUS) +  "='" + taskStatus + "'";
    m_nDbHelper->deleteRow(RECTIFICATION_TABLE_SUBMIT, deleteArg);
    m_nDbHelper->deleteRow(RECTIFICATION_TABLE_USER_TASK_TIME, deleteArg);
}

