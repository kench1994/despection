#include "standardizationdatalocalcache.h"
#include <QUuid>
#include <QFile>
#include "../common/dbhelper.h"
#include "../common/bussinessmap.h"
#include "../../framework/common/functiontype.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../../framework/config/sdisruntimecontext.h"
#define STANDARDIZATION_DIRCTORY "/STANDARDIZATION/"

StandardizationDataLocalCache::StandardizationDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_STANDARDIZATION;
}

StandardizationDataLocalCache::~StandardizationDataLocalCache()
{

}


bool StandardizationDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects;
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    //离线模式
    if(dataPackage.tables.length() > 0){
        const DataRow &dataRow = dataPackage.tables.at(0).value.at(0);
        QString taskItemDataKey = dataRow["TaskItemDataKey"];
        QString taskDataKey = getTaskDataKey(taskItemDataKey);
        QString taskObjectDataKey = getTaskObjectDataKey(taskItemDataKey);
        /**
        * step 1 写缓存文件
        */
        QString strXmlName = writeXMLFile(dataPackage, dataPath + STANDARDIZATION_DIRCTORY);
        qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;
        /**
        * step 2 写缓存表
        */
        writeCacheTable(action, strXmlName, offlineModel, taskDataKey, taskObjectDataKey, taskItemDataKey);

        /**
        * step 3 修改任务状态，更新Item表
        */
        modifyTaskStatus(taskItemDataKey, taskObjectDataKey);
        updateTaskInfo(taskDataKey, taskObjectDataKey, taskItemDataKey, TASK_STATUS_CACHE);
    }

    /**
    * step 4 转存二进制文件 & 写入缓存表
    */
    if(dataPackage.binaries.length() > 0){
        for(int i = 0; i < dataPackage.binaries.length(); i++){
            QString binaryFilePath = saveBinaryLocalCache(dataPackage.binaries.at(i), dataPath + STANDARDIZATION_DIRCTORY);
            if(binaryFilePath != "")
                writeBinaryDBTable(action, dataPackage.binaries.at(i), EXAMINE_TABLE_BINARY_SUBMISSION, binaryFilePath, offlineModel);
            else {
                qDebug() << "saveBinaryLocalCache  return empty";
                continue;
            }
        }
    }
    return true;
}

void StandardizationDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    m_nFailedCount = 0;
    if(binaryType){
        submitBinary();
    } else {
        submitXml();
    }
}

void StandardizationDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    QStringList column;
    QList<DataRow> dataRows;
    QString tableName;
    if(binaryType){
        tableName = EXAMINE_TABLE_BINARY_SUBMISSION;
    } else {
        tableName = EXAMINE_TABLE_TEXT_SUBMISSION;
    }
    dataRows = m_nDbHelper->select(tableName, column, "");
    //删除缓存文件
    for(int i = 0; i < dataRows.size(); i++){
        QString filePath = dataRows.at(i)["FilePath"];
        QFile fl(filePath);
        if(fl.exists())
            fl.remove();
    }
    //清空数据库
    m_nDbHelper->deleteRow(tableName,  "");
}

bool StandardizationDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    QList<DataRow> dataRows;
    QStringList column;
    if(binaryType)
        dataRows = m_nDbHelper->select(EXAMINE_TABLE_BINARY_SUBMISSION, column, "");
    else
        dataRows = m_nDbHelper->select(EXAMINE_TABLE_TEXT_SUBMISSION, column, "");
    if(dataRows.size() > 0)
        return true;
    else
        return false;
}

void StandardizationDataLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "StandardizationDataLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
    if(objects == "binary" && (map.otherMap["binaryType"] == "binaryResponse" || map.otherMap["binaryType"] == "binaryError") ){
        m_nBinaryValueList.removeFirst();
        if(result){
            //从数据库中删除该条记录
            deleteLocalFile(action, true);
        }
        emit onSubmitFinished(m_nDataOperationType, result, true);
    } else if(objects == EXAM_ExamineTaskItem_Import){
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
            updateTaskInfo(dataRow["TaskDataKey"], dataRow["TaskObjectDataKey"], dataRow["TaskItemDataKey"], TASK_STATUS_FINISH);
        } else {
            m_nFailedCount++;
        }
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "networkResult    resultProcess";
        resultProcess();
    }
}

void StandardizationDataLocalCache::commitData(bool binaryType)
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

void StandardizationDataLocalCache::writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel, const QString &taskDataKey, const QString &taskObjectDataKey, const QString &taskItemDataKey)
{
    qDebug() << Q_FUNC_INFO << " taskDataKey " << taskDataKey << ", taskObjectDataKey "<< taskObjectDataKey << ", taskItemDataKey " << taskItemDataKey;
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("FilePath", strXmlName);
    fields.insert("TaskDataKey", taskDataKey);
    fields.insert("TaskObjectDataKey", taskObjectDataKey);
    fields.insert("TaskItemDataKey", taskItemDataKey);
    m_nDbHelper->insert(EXAMINE_TABLE_TEXT_SUBMISSION, fields);
}

void StandardizationDataLocalCache::deleteLocalFile(const QString &ID, const bool &binaryType)
{
    qDebug() << Q_FUNC_INFO << " ID " << ID;
    QStringList column;
    QString tableName;
    QList<DataRow> dataRows;
    if(binaryType){
        tableName = EXAMINE_TABLE_BINARY_SUBMISSION;
    } else {
        tableName = EXAMINE_TABLE_TEXT_SUBMISSION;
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

void StandardizationDataLocalCache::updateTaskInfo(const QString &taskDataKey, const QString &taskObjectDataKey, const QString &taskItemDataKey, const QString &localStatus) {
    qDebug() << Q_FUNC_INFO << " taskDataKey " << taskDataKey << ", taskObjectDataKey "<< taskObjectDataKey << ", taskItemDataKey " << taskItemDataKey << ", localStatus "<< localStatus;
    /**
    * step 1 : update item
    */
    DataRow updateRow;
    QString updateArg = " where " + QString(TASK_ITEM_DATA_KEY) + "='" + taskItemDataKey + "'";
    updateRow[LOCAL_STATUS] = localStatus;
    m_nDbHelper->update(EXAMINE_TABLE_ITEM, updateRow, updateArg);

    /**
    * step 2 : if object doesn't have any items then update object
    */
    if(!hasItem(taskObjectDataKey, localStatus)) {
        updateArg = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "' and "
                    + QString(LOCAL_STATUS) + "<'" + localStatus + "'";
        m_nDbHelper->update(EXAMINE_TABLE_OBJECT, updateRow, updateArg);
    }

    /**
    * step 3 : if task doesn't have any objects then delete task
    */
    if(!hasObject(taskDataKey, localStatus)) {
        if(localStatus == TASK_STATUS_FINISH){
            updateArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "'";
            m_nDbHelper->deleteRow(EXAMINE_TABLE_TASK,   updateArg);
            m_nDbHelper->deleteRow(EXAMINE_TABLE_OBJECT, updateArg);
            m_nDbHelper->deleteRow(EXAMINE_TABLE_ITEM,   updateArg);
        }else{
            updateArg = " where " + QString(TASK_DATA_KEY) + "='" + taskDataKey + "' and "
                        + QString(LOCAL_STATUS) + "<'" + localStatus + "'";
            m_nDbHelper->update(EXAMINE_TABLE_TASK, updateRow, updateArg);
            m_nDbHelper->update(EXAMINE_TABLE_OBJECT, updateRow, updateArg);
            m_nDbHelper->update(EXAMINE_TABLE_ITEM, updateRow, updateArg);
        }
    }
}

bool StandardizationDataLocalCache::hasItem(const QString &objectDataKey, const QString &localStatus) {
    qDebug() << Q_FUNC_INFO << ", objectDataKey "<< objectDataKey << ", localStatus "<< localStatus;
    QStringList column;
    QString selectArg = " where " + QString(TASK_OBJECT_DATA_KEY) +
                        "='" + objectDataKey + "' and " + QString(LOCAL_STATUS) +
                        "<'" + localStatus + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_ITEM, column, selectArg);
    if (!dataRows.isEmpty())
        return true;
    return false;
}

bool StandardizationDataLocalCache::hasObject(const QString &taskDataKey, const QString &localStatus) {
    qDebug() << Q_FUNC_INFO << ", taskDataKey "<< taskDataKey << ", localStatus "<< localStatus;
    QStringList column;
    QString selectArg = " where " + QString(TASK_DATA_KEY) +
                        "='" + taskDataKey + "' and " + QString(LOCAL_STATUS) +
                        "<'" + localStatus + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_OBJECT, column, selectArg);
    if (!dataRows.isEmpty())
        return true;
    return false;
}

QString StandardizationDataLocalCache::getTaskDataKey(const QString &taskItemDataKey){
    QString taskDataKey;
    QStringList columns;
    columns.append(TASK_DATA_KEY);
    QString selection = " where " + QString(TASK_ITEM_DATA_KEY) + "='" + taskItemDataKey + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_ITEM, columns, selection);
    if(!dataRows.isEmpty()){
        taskDataKey = dataRows.at(0).value(TASK_DATA_KEY);
    }
    return taskDataKey;
}

QString StandardizationDataLocalCache::getTaskObjectDataKey(const QString &taskItemDataKey){
    QString taskGroupDataKey;
    QStringList columns;
    columns.append(TASK_OBJECT_DATA_KEY);
    QString selection = " where " + QString(TASK_ITEM_DATA_KEY) + "='" + taskItemDataKey + "'";
    QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_ITEM, columns, selection);
    if(!dataRows.isEmpty()){
        taskGroupDataKey = dataRows.at(0).value(TASK_OBJECT_DATA_KEY);
    }
    return taskGroupDataKey;
}

void StandardizationDataLocalCache::modifyTaskStatus(const QString &taskItemDataKey, const QString &taskObjectDataKey){
    QString updateArg = " where " + QString(TASK_ITEM_DATA_KEY) + "='" + taskItemDataKey + "'";
    DataRow updateRow;
    updateRow[LOCAL_STATUS] = TASK_STATUS_CACHE;
    m_nDbHelper->update(EXAMINE_TABLE_ITEM, updateRow, updateArg);
    QString whereArgs = " where " + QString(TASK_OBJECT_DATA_KEY) + "='" + taskObjectDataKey + "'";
    QStringList columns;
    columns.append(EXAM_ITEM_COUNT);
    QList<DataRow> dataRows = m_nDbHelper->select(EXAMINE_TABLE_OBJECT, columns, whereArgs);
    if(!dataRows.isEmpty()){
        DataRow updateRow0;
        int examCount = dataRows.at(0).value(EXAM_ITEM_COUNT).toInt() + 1;
        updateRow0[EXAM_ITEM_COUNT] = QString::number(examCount);
        m_nDbHelper->update(EXAMINE_TABLE_OBJECT, updateRow0, whereArgs);
    }
}

void StandardizationDataLocalCache::submitXml()
{
    QStringList column;
    m_nFailedCount = 0;
    m_nXmlValueList = m_nDbHelper->select(EXAMINE_TABLE_TEXT_SUBMISSION, column, " order by OfflineModel ASC");
    qDebug() << "StandardizationDataLocalCache::submitXml   m_nXmlValueList  " << m_nXmlValueList.size();
    if(m_nXmlValueList.size() == 0){
        qDebug() << "submitXml onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, false);
    }else{
        commitData(false);
    }
}

void StandardizationDataLocalCache::resultProcess() {
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

void StandardizationDataLocalCache::submitBinary()
{
    QStringList column;
    m_nBinaryValueList = m_nDbHelper->select(EXAMINE_TABLE_BINARY_SUBMISSION, column, " order by OfflineModel ASC");
    qDebug() << "StandardizationDataLocalCache::submitBinary   m_nBinaryValueList  " << m_nBinaryValueList.size();
    if(m_nBinaryValueList.size() == 0){
        qDebug() << "submitBinary onSubmitFinished  begin  true";
        emit onSubmitFinished(m_nDataOperationType, true, true);
    }else{
        commitData(true);
    }
}
