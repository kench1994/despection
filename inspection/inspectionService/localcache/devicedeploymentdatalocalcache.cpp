#include "devicedeploymentdatalocalcache.h"
#include <QUuid>
#include <QFile>
#include "../common/dbhelper.h"
#include "../common/bussinessmap.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../../framework/config/sdisruntimecontext.h"
#define DEVICE_DEPLOYMENT_DIRCTORY "/DEVICE_DEPLOYMENT/"

DeviceDeploymentDataLocalCache::DeviceDeploymentDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICE_DEPLOYMENT;
}
DeviceDeploymentDataLocalCache::~DeviceDeploymentDataLocalCache()
{
}
bool DeviceDeploymentDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects << "  binaries  " << dataPackage.binaries.count();
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    if(dataPackage.tables.length() > 0){
        /**
        * Step 1. 写 xml 文件
        */
        QString strXmlName = writeXMLFile(dataPackage, dataPath + DEVICE_DEPLOYMENT_DIRCTORY);
        QString orderCode = dataPackage.tables.at(0).value.at(0).value("OrderCode").toUpper();
        QString status = dataPackage.tables.at(0).value.at(0).value("Status");
        qDebug() <<"writeCacheTable  orderCode is " << orderCode;
        qDebug() <<"writeCacheTable    xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;
        /**
        * Step 2. 写缓存表
        */
        writeCacheTable(action, orderCode, strXmlName, offlineModel, status);

        /**
        * Step 3. 修改任务进度（状态）将该设备项删除
        */
        writeTaskTable(orderCode, status);
    }
    return true;
}

void DeviceDeploymentDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    if(binaryType){
        emit onSubmitFinished(m_nDataOperationType, true, true);
        return;
    }
    submitXml();
}

void DeviceDeploymentDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    if(binaryType){
        return;
    }
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName;
    tableName = DEPLOYMENT_TABLE_SUBMIT;

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

bool DeviceDeploymentDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    if(binaryType){
        return false;
    }
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    valueList = m_nDbHelper->select(DEPLOYMENT_TABLE_SUBMIT, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

void DeviceDeploymentDataLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "DeviceDeploymentDataLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
    if(objects == EMA_Allocation_Import){
        QString actionStr = m_nXmlValueList.at(0)["Action"];
        QString offline = m_nXmlValueList.at(0)["OfflineModel"];
        if(offline == "0"){
            qDebug() << "在线情况下发送信号出去给客户端   onResultOfResponse    " << objects << "  action  " << action << "  result " << result << "  actionStr " << actionStr;
            emit onResultOfResponse(objects, actionStr, result, error, map);
        }
        if(result == true){
            //从数据库中删除该条记录
            deleteLocalFile(action);
        } else {
            m_nFailedCount++;
        }
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "networkResult    resultProcess";
        resultProcess();
    }
}

void DeviceDeploymentDataLocalCache::commitData()
{
    QMap<QString, QString> value = m_nXmlValueList.at(0);
    DataPackage dataPackage;
    bool rv = readXmlFile(value["XmlFilePath"], dataPackage);
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

void DeviceDeploymentDataLocalCache::writeCacheTable(const QString &action,
                                                     const QString &orderCode,
                                                     const QString &strXmlName,
                                                     const QString &offlineModel,
                                                     const QString &status)
{
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("OrderCode", orderCode);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("XmlFilePath", strXmlName);
    fields.insert("Status", status);
    m_nDbHelper->insert(DEPLOYMENT_TABLE_SUBMIT, fields);
}

void DeviceDeploymentDataLocalCache::writeTaskTable(const QString &orderCode, const QString &status) {
    DataRow dataRow;
    QString whereArgs = " where " + QString(ORDER_CODE) + "='" + orderCode + "'";
    dataRow.insert(LOCAL_STATUS, TASK_STATUS_CACHE);
    dataRow.insert("Status", QString::number(status.toInt() + 1));
    m_nDbHelper->update(DEPLOYMENT_TABLE_TASK, dataRow, whereArgs);
}

void DeviceDeploymentDataLocalCache::deleteLocalFile(const QString &ID)
{
    QStringList column;
    QString  tableName = DEPLOYMENT_TABLE_SUBMIT;
    QList<QMap<QString, QString> > valueList;
    valueList = m_nDbHelper->select(tableName, column, " where ID = '"+ ID + "'");
    qDebug() << Q_FUNC_INFO << valueList;
    for(int i = 0; i < valueList.count(); i++){
        QString filePath = valueList.at(i)["XmlFilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    m_nDbHelper->deleteRow(tableName, " where ID = '"+ ID + "'");
}

void DeviceDeploymentDataLocalCache::submitXml()
{
    QStringList column;
    m_nFailedCount = 0;
    m_nXmlValueList = m_nDbHelper->select(DEPLOYMENT_TABLE_SUBMIT, column, " order by Status ASC, OfflineModel ASC");
    qDebug() << "DeviceDeploymentDataLocalCache::submitXml   m_nXmlValueList  " << m_nXmlValueList.size();
    if(m_nXmlValueList.size() == 0){
        qDebug() << "submitXml onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, false);
    }else{
        commitData();
    }
}

void DeviceDeploymentDataLocalCache::resultProcess() {
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

