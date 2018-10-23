#include "inspectiondatalocalcache.h"
#include <QUuid>
#include <QFile>
#include "../DBConfig/dbconfiginspection.h"
#include "../networkcontrol.h"
#include "../common/dbhelper.h"
#include "../common/bussinessmap.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../framework/config/sdisruntimecontext.h"
#define INSPECTION_DIRCTORY "/INSPECTION/"
#define INSPECTION_TASK_TYPE_NORMAL "0"
#define INSPECTION_TASK_TYPE_GROUP "2"
#define INSPECTION_TASK_TYPE_PRE_NORMAL "3"
#define INSPECTION_TASK_TYPE_PRE_GROUP "4"
InspectionDataLocalCache::InspectionDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_INSPECTION;
}
InspectionDataLocalCache::~InspectionDataLocalCache()
{
}
bool InspectionDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  objects  " << dataPackage.head.objects << "  binaries  " << dataPackage.binaries.count();
    QString dataPath = SystemConfig::getInstance()->getAppDataPath();
    QString offlineModel = RuntimeParam::getInstance()->getOfflineModel();
    if(dataPackage.tables.length() > 0){
        QString taskObjectDataKey = dataPackage.tables.at(0).value.at(0)["TaskObjectDataKey"];
        QString strXmlName = writeXMLFile(dataPackage, dataPath + INSPECTION_DIRCTORY);
        qDebug() <<"writeCacheTable  xmlName is " << strXmlName;
        if(strXmlName.isEmpty())
            return false;
        writeCacheTable(action, dataPackage.head.objects, taskObjectDataKey, strXmlName, offlineModel);
    }

    if(dataPackage.binaries.length() > 0){
        for(int i = 0; i < dataPackage.binaries.length(); i++){
            QString binaryFilePath = saveBinaryLocalCache(dataPackage.binaries.at(i), dataPath + INSPECTION_DIRCTORY);
            if(binaryFilePath != ""){
                writeBinaryDBTable(action, dataPackage.binaries.at(i), DBConfigInspection::TABLE_INSPECTIONSUBMIT_BINARY, binaryFilePath, offlineModel);
            } else {
                qDebug() << "saveBinaryLocalCache  return empty";
                continue;
            }
        }
    }
    return true;
}

void InspectionDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    m_nFailedCount = 0;
    if(binaryType){
        m_nBinaryValueList = dbHelper->select(DBConfigInspection::TABLE_INSPECTIONSUBMIT_BINARY, column, " order by OfflineModel ASC");
    } else {
        m_nXmlValueList = dbHelper->select(DBConfigInspection::TABLE_INSPECTIONSUBMIT, column, " order by OfflineModel ASC");
    }
    qDebug() << "InspectionDataLocalCache::submitLocalCache     ";
    commitData(binaryType);
}

void InspectionDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    QString tableName;
    if(binaryType){
        tableName = DBConfigInspection::TABLE_INSPECTIONSUBMIT_BINARY;
    } else {
        tableName = DBConfigInspection::TABLE_INSPECTIONSUBMIT;
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

bool InspectionDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QList<QMap<QString, QString> > valueList;
    if(binaryType)
        valueList = dbHelper->select(DBConfigInspection::TABLE_INSPECTIONSUBMIT_BINARY, column, "");
    else
        valueList = dbHelper->select(DBConfigInspection::TABLE_INSPECTIONSUBMIT, column, "");
    if(valueList.size() > 0)
        return true;
    else
        return false;
}

void InspectionDataLocalCache::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    qDebug() << "InspectionDataLocalCache networkResult submit success " << objects << "  action " << action << "  error is " << error;
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
        QString taskType = m_nXmlValueList.at(0)["TaskType"];
        QString taskObjectDataKey = m_nXmlValueList.at(0)["TaskObjectDataKey"];
        if(offline == "0"){
            qDebug() << "在线情况下发送信号出去给客户端   onResultOfResponse    " << objects << "  action  " << action << "  result " << result << "  actionStr " << actionStr;
            emit onResultOfResponse(objects, actionStr, result, error, map);
        }
        if(result == true){
            //从数据库中删除该条记录
            deleteLocalFile(ID, false);
            if(taskType == INSPECTION_TASK_TYPE_GROUP || taskType == INSPECTION_TASK_TYPE_PRE_GROUP){
                //标注该巡检组已经完成
                markLocalTaskGroupFinished(taskObjectDataKey);
            } else {
                //标注该巡检项已经完成
                markLocalTaskObjectsFinished(taskObjectDataKey);
            }
        } else
            m_nFailedCount++;
        m_nXmlValueList.removeFirst();
        //继续提交下一条
        qDebug() << "networkResult    commitData";
        commitData(false);
    }
}

void InspectionDataLocalCache::commitData(bool binaryType)
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
            qDebug() << "InspectionDataLocalCache m_nXmlValueList.count()== 0  emit onSubmitFinished ";
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

bool InspectionDataLocalCache::writeCacheTable(const QString &action, const QString &objects, const QString &taskObjectDataKey, const QString &strXmlName, const QString &offlineModel)
{
    QMap<QString, QString> fields;
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    QString taskType;
    if(objects == EISM_TaskObject_UpdateOne)
        taskType = INSPECTION_TASK_TYPE_NORMAL;
    else if(objects == EISM_TaskGroupObject_UpdateOne)
        taskType = INSPECTION_TASK_TYPE_GROUP;
    else if(objects == EISM_PRE_TaskObject_UpdateOne)
       taskType = INSPECTION_TASK_TYPE_PRE_NORMAL;
    else if(objects == EISM_PRE_TaskGroupObject_UpdateOne)
       taskType = INSPECTION_TASK_TYPE_PRE_GROUP;
    fields.insert("ID", uuid);
    fields.insert("Action", action);
    fields.insert("TaskObjectDataKey", taskObjectDataKey);
    fields.insert("OfflineModel", offlineModel);
    fields.insert("TaskType", taskType);
    fields.insert("FilePath", strXmlName);
    DBHelper *dbHelper = DBHelper::getInstance();
    bool rv = dbHelper->insert(DBConfigInspection::TABLE_INSPECTIONSUBMIT, fields);
    if(rv){
        qDebug() << "TaskType is " << taskType;
        if(taskType == INSPECTION_TASK_TYPE_GROUP || taskType == INSPECTION_TASK_TYPE_PRE_GROUP){
            markLocalTaskGroupCached(taskObjectDataKey);
        } else {
            markLocalTaskObjectsCached(taskObjectDataKey);
        }
    }
    return rv;
}

void InspectionDataLocalCache::deleteLocalFile(const QString &ID, bool binaryType)
{
    DBHelper *dbHelper = DBHelper::getInstance();
    QStringList column;
    QString  tableName;
    QList<QMap<QString, QString> > valueList;
    if(binaryType){
        tableName = DBConfigInspection::TABLE_INSPECTIONSUBMIT_BINARY;
    } else {
        tableName = DBConfigInspection::TABLE_INSPECTIONSUBMIT;
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

void InspectionDataLocalCache::markLocalTaskObjectsCached(const QString &taskObjectDataKey)
{
    qDebug() << Q_FUNC_INFO << "  taskObjectDatakey is " << taskObjectDataKey;
    QString updateArgs = " where TaskObjectDataKey = '" + taskObjectDataKey + "'";
    QList<QMap<QString, QString> > statusList;
    DBHelper::getInstance()->select("select LocalStatus from ISM_InspectionTaskObject " + updateArgs,  statusList);
    if(statusList.size() > 0 && statusList.at(0)[LOCAL_STATUS] == TASK_STATUS_NO_FINISH){
        QMap<QString, QString> keyValue;
        keyValue[LOCAL_STATUS] = TASK_STATUS_CACHE;
        DBHelper::getInstance()->update("ISM_InspectionTaskObject", keyValue, updateArgs);
        QString strSql = "select TaskDataKey, TaskGroupDataKey from ISM_InspectionTaskObject  where TaskObjectDataKey = '" + taskObjectDataKey + "'";
        QList<QMap<QString, QString> > valueList;
        DBHelper::getInstance()->select(strSql, valueList);
        if(valueList.size() >= 1){
            updateLocalTaskGroupExamObjectCount(valueList.at(0)["TaskGroupDataKey"]);
            checkLocalTaskGroupCached(valueList.at(0)["TaskDataKey"], valueList.at(0)["TaskGroupDataKey"]);
        }
    }
}

void InspectionDataLocalCache::markLocalTaskObjectsFinished(const QString &taskObjectDataKey)
{
    qDebug() << Q_FUNC_INFO << "  taskObjectDataKey  " << taskObjectDataKey;
    QString updateArgs = " where TaskObjectDataKey = '" + taskObjectDataKey + "'";
    QMap<QString, QString> keyValue;
    keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
    DBHelper::getInstance()->update("ISM_InspectionTaskObject", keyValue, updateArgs);
    QString strSql = "select TaskDataKey, TaskGroupDataKey from ISM_InspectionTaskObject  where TaskObjectDataKey = '" + taskObjectDataKey + "'";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    if(valueList.size() >= 1){
        checkLocalTaskGroupFinished(valueList.at(0)["TaskDataKey"], valueList.at(0)["TaskGroupDataKey"]);
    }
}

void InspectionDataLocalCache::markLocalTaskGroupCached(const QString &taskObjectDataKey)
{
    qDebug() << Q_FUNC_INFO << "  taskObjectDatakey is " << taskObjectDataKey;
    QString strSql = "select TaskDataKey, TaskGroupDataKey from ISM_InspectionTaskObject  where TaskObjectDataKey = '" + taskObjectDataKey + "'";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    if(valueList.size() >= 1){
        QString taskGroupDataKey = valueList.at(0)["TaskGroupDataKey"];
        QString taskDataKey = valueList.at(0)["TaskDataKey"];
        QString updateArgs = " where TaskGroupDataKey = '" + taskGroupDataKey + "'";
        QMap<QString, QString> keyValue;
        keyValue[LOCAL_STATUS] = TASK_STATUS_CACHE;
        DBHelper::getInstance()->update("ISM_InspectionTaskGroup", keyValue, updateArgs);
        DBHelper::getInstance()->update("ISM_InspectionTaskObject", keyValue, updateArgs);
        updateLocalTaskGroupExamObjectCount(taskGroupDataKey, true);
        checkLocalTaskCached(taskDataKey);
    }
}

void InspectionDataLocalCache::checkLocalTaskGroupCached(const QString &taskDataKey, const QString &taskGroupDataKey)
{
    QString strSql = "select TaskObjectDataKey from ISM_InspectionTaskObject  where TaskGroupDataKey = '" + taskGroupDataKey + "' and LocalStatus = '0' ";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    qDebug()<< Q_FUNC_INFO<< " taskGroupDataKey " << taskGroupDataKey <<  " valueList is " << valueList.size();
    if(valueList.size() == 0){
        QMap<QString, QString>  keyValue;
        keyValue[LOCAL_STATUS] = TASK_STATUS_CACHE;
        DBHelper::getInstance()->update("ISM_InspectionTaskGroup", keyValue, "where TaskGroupDataKey = '" + taskGroupDataKey + "'");
        checkLocalTaskCached(taskDataKey);
    }
}

void InspectionDataLocalCache::markLocalTaskGroupFinished(const QString &taskObjectDataKey)
{
    QString strSql = "select TaskDataKey, TaskGroupDataKey from ISM_InspectionTaskObject  where TaskObjectDataKey = '" + taskObjectDataKey + "'";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    qDebug() << Q_FUNC_INFO << "  taskObjectDatakey is " << taskObjectDataKey <<" size " <<valueList.size() ;
    if(valueList.size() >= 1){
        QString taskGroupDataKey = valueList.at(0)["TaskGroupDataKey"];
        QString taskDataKey = valueList.at(0)["TaskDataKey"];
        qDebug() << Q_FUNC_INFO << "  taskGroupDataKey is " << taskGroupDataKey <<" taskDataKey " <<taskDataKey;
        QString updateArgs = " where TaskGroupDataKey = '" + taskGroupDataKey + "'";
        QMap<QString, QString> keyValue;
        keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
        DBHelper::getInstance()->update("ISM_InspectionTaskGroup", keyValue, updateArgs);
        DBHelper::getInstance()->update("ISM_InspectionTaskObject", keyValue, updateArgs);
        checkLocalTaskFinished(taskDataKey);
    }
}

void InspectionDataLocalCache::checkLocalTaskGroupFinished(const QString &taskDataKey, const QString &taskGroupDataKey)
{
    QString strSql = "select TaskObjectDataKey from ISM_InspectionTaskObject  where TaskGroupDataKey = '" + taskGroupDataKey + "' and LocalStatus != '2' ";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    qDebug()<< Q_FUNC_INFO<< " taskGroupDataKey " << taskGroupDataKey <<  " valueList is " << valueList.size();
    if(valueList.size() == 0){
        QMap<QString, QString>  keyValue;
        keyValue[LOCAL_STATUS] = TASK_STATUS_FINISH;
        DBHelper::getInstance()->update("ISM_InspectionTaskGroup", keyValue, "where TaskGroupDataKey = '" + taskGroupDataKey + "'");
        checkLocalTaskFinished(taskDataKey);
    }
}

void InspectionDataLocalCache::checkLocalTaskCached(const QString &taskDataKey)
{
    QString strSql = "select TaskGroupDataKey from ISM_InspectionTaskGroup  where TaskDataKey = '" + taskDataKey + "' and LocalStatus = '0' ";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    qDebug()<< Q_FUNC_INFO<< " taskDataKey " << taskDataKey <<  " valueList is " << valueList.size();
    if(valueList.size() == 0){
        QMap<QString, QString>  keyValue;
        keyValue[LOCAL_STATUS] = TASK_STATUS_CACHE;
        DBHelper::getInstance()->update("ISM_InspectionTask", keyValue, "where TaskDataKey = '" + taskDataKey + "'");
    }
}

void InspectionDataLocalCache::checkLocalTaskFinished(const QString &taskDataKey)
{
    QString strSql = "select TaskGroupDataKey from ISM_InspectionTaskGroup where TaskDataKey = '" + taskDataKey + "' and LocalStatus != '2' ";
    QList<QMap<QString, QString> > valueList;
    DBHelper::getInstance()->select(strSql, valueList);
    qDebug()<< Q_FUNC_INFO<< " taskDataKey " << taskDataKey <<  " valueList is " << valueList.size();
    if(valueList.size() == 0){
        DBHelper::getInstance()->deleteRow("ISM_InspectionTask", " where TaskDataKey = '" + taskDataKey + "'");
        DBHelper::getInstance()->deleteRow("ISM_InspectionTaskGroup", " where TaskDataKey = '" + taskDataKey + "'");
        DBHelper::getInstance()->deleteRow("ISM_InspectionTaskObject", " where TaskDataKey = '" + taskDataKey + "'");
    }
}

void InspectionDataLocalCache::updateLocalTaskGroupExamObjectCount(const QString &taskGroupDataKey, bool all )
{
    QString updateArgs = " where TaskGroupDataKey ='" + taskGroupDataKey + "'";
    QList<QMap<QString, QString> > examCount;
    QString strSql = "select ExamObjectCount, TotalObjectCount  from ISM_InspectionTaskGroup " + updateArgs;
    DBHelper::getInstance()->select(strSql, examCount);
    if(examCount.size() > 0){
        QMap<QString, QString> examMap;
        if(all)
            examMap["ExamObjectCount"]  = examMap["TotalObjectCount"];
        else
            examMap["ExamObjectCount"] = QString::number(examCount.at(0)["ExamObjectCount"].toInt() + 1);
        DBHelper::getInstance()->update("ISM_InspectionTaskGroup", examMap, updateArgs);
    }


//        QList<QMap<QString, QString> > examCount;
//        QString strSql = "select count(*) as examNum from ISM_InspectionTaskObject where TaskGroupDataKey = '" + taskGroupDataKey + "' and LocalStatus != '0'";
//        DBHelper::getInstance()->select(strSql, examCount);
//        QString updateArgs = " where TaskGroupDataKey ='" + taskGroupDataKey + "'";
//        QMap<QString, QString> examMap;
//        examMap["ExamObjectCount"] = examCount.at(0)["examNum"];
//        DBHelper::getInstance()->update("ISM_InspectionTaskGroup", examMap, updateArgs);

}

