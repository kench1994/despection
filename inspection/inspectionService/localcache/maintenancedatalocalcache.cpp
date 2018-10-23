#include "maintenancedatalocalcache.h"
#include <QUuid>
#include <QFile>
#include "../common/bussinessmap.h"
#include "../../framework/common/functiontype.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../../framework/config/sdisruntimecontext.h"
#define MAINTENANCE_DIRCTORY "/MAINTENANCE/"

MaintenanceDataLocalCache::MaintenanceDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_MAINTENANCE;
}
MaintenanceDataLocalCache::~MaintenanceDataLocalCache()
{
}
bool MaintenanceDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects << "   binarise count  " << dataPackage.binaries.count();
    /**
        * 更新提交表
        */
        QString dataPath = SystemConfig::getInstance()->getAppDataPath();
        QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
        if(dataPackage.tables.length() > 0){
            const DataRow &dataRow = dataPackage.tables.at(0).value.at(0);
            QString taskObjectDataKey = dataRow["TaskObjectDataKey"];
            QString taskDataKey = getTaskDataKey(taskObjectDataKey);
            QString taskGroupDataKey = getTaskGroupDataKey(taskObjectDataKey);

            /**
            * step 1 写缓存文件
            */
            QString strXmlName = writeXMLFile(dataPackage, dataPath + MAINTENANCE_DIRCTORY);
            qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
            if(strXmlName.isEmpty())
                return false;

            /**
            * step 2 写缓存表
            */
            writeCacheTable(action, strXmlName, offlineModel, taskDataKey, taskGroupDataKey, taskObjectDataKey);

            /**
            * step 3 修改任务状态，更新Item表
            */
            modifyTaskStatus(taskObjectDataKey, taskGroupDataKey);
            updateTaskInfo(taskDataKey, taskGroupDataKey, taskObjectDataKey, TASK_STATUS_CACHE);
        }

        if(dataPackage.binaries.length() > 0){
            for(int i = 0; i < dataPackage.binaries.length(); i++){
                QString binaryFilePath = saveBinaryLocalCache(dataPackage.binaries.at(i), dataPath + MAINTENANCE_DIRCTORY);
                if(binaryFilePath != ""){
                    writeBinaryDBTable(action, dataPackage.binaries.at(i), MAINTENANCE_TABLE_ATTACHMENT_SUBMIT, binaryFilePath, offlineModel);
                } else {
                    qDebug() << "saveBinaryLocalCache  return empty";
                    continue;
                }
            }
        }
    return true;
}

void MaintenanceDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    m_nFailedCount = 0;
    if(binaryType){
        submitBinary();
    } else {
        submitXml();
    }
}

void MaintenanceDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName;
    if(binaryType){
        tableName = MAINTENANCE_TABLE_ATTACHMENT_SUBMIT;
    } else {
        tableName = MAINTENANCE_TABLE_SUBMIT;
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

bool MaintenanceDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    if(binaryType)
        valueList = m_nDbHelper->select(MAINTENANCE_TABLE_ATTACHMENT_SUBMIT, column, "");
    else
        valueList = m_nDbHelper->select(MAINTENANCE_TABLE_SUBMIT, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

void MaintenanceDataLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "MaintenanceDataLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
    if(objects == "binary" && (map.otherMap["binaryType"] == "binaryResponse" || map.otherMap["binaryType"] == "binaryError") ){
        m_nBinaryValueList.removeFirst();
        if(result){
            //从数据库中删除该条记录
            deleteLocalFile(action, true);
        }
        emit onSubmitFinished(m_nDataOperationType, result, true);
    } else {
        DataRow dataRow = m_nXmlValueList.at(0);
        QString actionStr = dataRow["Action"];
        QString offline = dataRow["OfflineModel"];
        if(offline == "0"){
            qDebug() << "在线情况下发送信号出去给客户端   onResultOfResponse    " << objects << "  action  " << action << "  result " << result << "  actionStr " << actionStr;
            emit onResultOfResponse(objects, actionStr, result, error, map);
        }
        if(result){
            //从数据库中删除该条记录
            deleteLocalFile(action, false);
            updateTaskInfo(dataRow["TaskDataKey"], dataRow["TaskGroupDataKey"], dataRow["TaskObjectDataKey"], TASK_STATUS_FINISH);
        } else {
            m_nFailedCount++;
        }
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "networkResult    resultProcess";
        resultProcess();
    }
}

void MaintenanceDataLocalCache::commitData(bool binaryType)
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

void MaintenanceDataLocalCache::writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel, const QString &taskDataKey, const QString &taskGroupDataKey, const QString &taskObjectDataKey)
{
    qDebug() << Q_FUNC_INFO << "  taskDataKey  " << taskDataKey << ",  taskGroupDataKey  " << taskGroupDataKey << ",  taskObjectDataKey  "<< taskObjectDataKey;
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    fields.insert("TaskDataKey", taskDataKey);
    fields.insert("TaskGroupDataKey", taskGroupDataKey);
    fields.insert("TaskObjectDataKey", taskObjectDataKey);
    m_nDbHelper->insert(MAINTENANCE_TABLE_SUBMIT, fields);
}

void MaintenanceDataLocalCache::deleteLocalFile(const QString &ID, bool binaryType)
{
    qDebug() << Q_FUNC_INFO << "  ID  " << ID;
    QStringList column;
    QString tableName;
    QList<DataRow> dataRows;
    if(binaryType){
        tableName = MAINTENANCE_TABLE_ATTACHMENT_SUBMIT;
    } else {
        tableName = MAINTENANCE_TABLE_SUBMIT;
    }
    dataRows = m_nDbHelper->select(tableName, column, " where ID = '"+ ID + "'");
    qDebug() << Q_FUNC_INFO << dataRows;
    for(int i = 0; i < dataRows.count(); i++){
        QString filePath = dataRows.at(i)["FilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    m_nDbHelper->deleteRow(tableName, " where ID = '"+ ID + "'");
}


void MaintenanceDataLocalCache::updateTaskInfo(const QString &taskDataKey,
                                               const QString &taskGroupDataKey,
                                               const QString &taskObjectDataKey,
                                               const QString &localStatus) {
    qDebug() << Q_FUNC_INFO << " taskDataKey " << taskDataKey << ", taskGroupDataKey "<< taskGroupDataKey << ", taskObjectDataKey " << taskObjectDataKey << ", localStatus "<< localStatus;
    /**
    * step 1 : update object
    */
    DataRow updateRow;
    QString updateArg = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
    updateRow[LOCAL_STATUS] = localStatus;
    m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, updateRow, updateArg);

    /**
    * step 2 : if group doesn't have any objects then update group
    */
    if(!hasObject(taskGroupDataKey, localStatus)) {
        updateArg = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDataKey + "' and "
                    + QString(LOCAL_STATUS) + "<'" + localStatus + "'";
        m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, updateRow, updateArg);
    }

    /**
    * step 3 : if task doesn't have any group then delete task
    */
    if(!hasGroup(taskDataKey, localStatus)) {
        if(localStatus == TASK_STATUS_FINISH){
            updateArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK,   updateArg);
            m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK_GROUP, updateArg);
            m_nDbHelper->deleteRow(MAINTENANCE_TABLE_TASK_OBJECT,   updateArg);
        }else{
            updateArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "' and "
                        + QString(LOCAL_STATUS) + "<'" + localStatus + "'";
            m_nDbHelper->update(MAINTENANCE_TABLE_TASK, updateRow, updateArg);
            m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, updateRow, updateArg);
            m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, updateRow, updateArg);
        }
    }
}

bool MaintenanceDataLocalCache::hasObject(const QString &groupDataKey, const QString &localStatus) {
    QStringList column;
    QString selectArg = " where " + QString(TASK_GROUP_DATA_KEY) +
                        "='" + groupDataKey + "' and " + QString(LOCAL_STATUS) +
                        "<'" + localStatus + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_OBJECT, column, selectArg);
    if (!dataRows.isEmpty())
        return true;
    return false;
}

bool MaintenanceDataLocalCache::hasGroup(const QString &taskDataKey, const QString &localStatus) {
    QStringList column;
    QString selectArg = " where " + QString(TASK_DATA_KEY) +
                        "='" + taskDataKey + "' and " + QString(LOCAL_STATUS) +
                        "<'" + localStatus + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_GROUP, column, selectArg);
    if (!dataRows.isEmpty())
        return true;
    return false;
}

QString MaintenanceDataLocalCache::getTaskDataKey(const QString &taskObjectDataKey){
    QString taskDataKey;
    QStringList columns;
    columns.append(TASK_DATA_KEY);
    QString selection = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_OBJECT, columns, selection);
    if(!dataRows.isEmpty()){
        taskDataKey = dataRows.at(0).value(TASK_DATA_KEY);
    }
    return taskDataKey;
}

QString MaintenanceDataLocalCache::getTaskGroupDataKey(const QString &taskObjectDataKey){
    QString taskGroupDataKey;
    QStringList columns;
    columns.append(TASK_GROUP_DATA_KEY);
    QString selection = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_OBJECT, columns, selection);
    if(!dataRows.isEmpty()){
        taskGroupDataKey = dataRows.at(0).value(TASK_GROUP_DATA_KEY);
    }
    return taskGroupDataKey;
}

void MaintenanceDataLocalCache::modifyTaskStatus(const QString &taskObjectDataKey, const QString &taskGroupDataKey)
{
    QString updateArg = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
    DataRow updateRow;
    updateRow[LOCAL_STATUS] = TASK_STATUS_CACHE;
    m_nDbHelper->update(MAINTENANCE_TABLE_TASK_OBJECT, updateRow, updateArg);
    QString whereArgs = " where " + QString(TASK_GROUP_DATA_KEY) + "='" + taskGroupDataKey + "'";
    QStringList columns;
    columns.append(EXAM_OBJECT_COUNT);
    QList<DataRow> dataRows = m_nDbHelper->select(MAINTENANCE_TABLE_TASK_GROUP, columns, whereArgs);
    if(!dataRows.isEmpty()){
        DataRow updateRow0;
        int examCount = dataRows.at(0).value(EXAM_OBJECT_COUNT).toInt() + 1;
        updateRow0[EXAM_OBJECT_COUNT] = QString::number(examCount);
        m_nDbHelper->update(MAINTENANCE_TABLE_TASK_GROUP, updateRow0, whereArgs);
    }
}

void MaintenanceDataLocalCache::submitXml()
{
    QStringList column;
    m_nFailedCount = 0;
    m_nXmlValueList = m_nDbHelper->select(MAINTENANCE_TABLE_SUBMIT, column, " order by OfflineModel ASC");
    qDebug() << "MaintenanceDataLocalCache::submitXml   m_nXmlValueList  " << m_nXmlValueList.size();
    if(m_nXmlValueList.size() == 0){
        qDebug() << "submitXml onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, false);
    }else{
        commitData(false);
    }
}

void MaintenanceDataLocalCache::resultProcess() {
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

void MaintenanceDataLocalCache::submitBinary()
{
    QStringList column;
    m_nBinaryValueList = m_nDbHelper->select(MAINTENANCE_TABLE_ATTACHMENT_SUBMIT, column, " order by OfflineModel ASC");
    qDebug() << "MaintenanceDataLocalCache::submitBinary   m_nBinaryValueList  " << m_nBinaryValueList.size();
    if(m_nBinaryValueList.size() == 0){
        qDebug() << "submitBinary onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, true);
    }else{
        commitData(true);
    }
}
