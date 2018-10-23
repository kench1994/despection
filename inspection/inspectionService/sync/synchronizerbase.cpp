#include "synchronizerbase.h"
#include "../networkcontrol.h"
#include "../common/dbdefine.h"
#include "../DBConfig/dbconfigmanager.h"
#include "../common/bussinessmap.h"
#include <QDebug>
#include <QUuid>

SynchronizerBase::SynchronizerBase()
{
    m_nRunnings = false;
    m_nSyncTaskFailCount = 0;
    m_nSyncTaskSuccessCount = 0;
    m_nSyncTaskTotalCount = 0;
    m_nDbHelper = DBHelper::getInstance();
}

SynchronizerBase::~SynchronizerBase()
{
}

void SynchronizerBase::execSyncTask(const QString &action, const QString &objects, const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO <<  "  m_nRunnings : "<< m_nRunnings;
    if(m_nRunnings){
        m_nActionList.clear();
        m_nRunnings = false;
        emit onSyncFinishedSignal(m_nDataOperationType, MESSAGE_SYNC_ADDTION_FINISHED, m_nSyncAction, m_nSyncObjects, m_nSyncMap);
    }
    syncAddition(action, objects, map);
}

void SynchronizerBase::execSyncPreTask(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO;
    if(NetworkControl::getInstance()->getNetworkConnectStatus() == ""){
        DbusPackage map;
        map.otherMap.insert("objects", dataPackage.head.objects);
        map.otherMap.insert("serverCode", "0");
        map.otherMap.insert("description", "no network");
        emit onResultOfResponse(dataPackage.head.objects, action, false, NETWORK_ERROR_NONETWORK, map);
    }else{
        syncPreTask(action, dataPackage);
    }
}

void SynchronizerBase::syncAddition(const QString &action, const QString &objects, const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(action)
    Q_UNUSED(objects)
    Q_UNUSED(map)
}

void SynchronizerBase::syncPreTask(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(action)
    Q_UNUSED(dataPackage)
}

void SynchronizerBase::sendData(const DataPackage &dataPackage, const QString &action)
{
    QString uuid = createUuid();
    m_nActionList.insert(uuid, action);
    NetworkControl::getInstance()->sendData(dataPackage, uuid);
}

QString SynchronizerBase::createUuid()
{
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    return uuid;
}

bool SynchronizerBase::requestObjs(const QMap<QString, QString> &objIDModifyTimeMap, const QString &taskId)
{
    QStringList objIDs;
    QMap<QString, QString>::const_iterator iter = objIDModifyTimeMap.begin();
    while (iter != objIDModifyTimeMap.end()) {
        QString objID = iter.key();
        if(!objIDs.contains(objID) && isNeedSync(objID, iter.value()) ){
            objIDs.append(objID);
        }
        ++iter;
    }
    qDebug() << Q_FUNC_INFO << " objIDs.count() :" << objIDs.count();
    if(objIDs.count() > 0){
        DataPackage dataPackage;
        dataPackage.head.key = PACKAGE_HEAD_KEY;
        dataPackage.head.objects = EMB_Off_GetEquipmentAndPartsListByJoinIDs;
        dataPackage.head.type = "1";
        dataPackage.para["joinids"] = objIDs.join(",");
        sendData(dataPackage, taskId);
        return true;
    }
    return false;
}

bool SynchronizerBase::isNeedSync(const QString &objID, const QString &modifiedDate){
    QString selectionArg = " where ID = '" + objID + "' and ModifiedDate >= '" + modifiedDate + "'";
    QStringList columns;
    QList<QMap<QString, QString> > valueList;
    valueList = m_nDbHelper->select("EMB_Equipment", columns, selectionArg);
    return (valueList.count() == 0);
}

void SynchronizerBase::writeDeviceToDB(const DataPackage &dataPackage)
{
    if(dataPackage.tables.count() > 0){
        m_nDbHelper->beginTransaction();

        //获取本地数据库设备表中的字段
        DBConfigManager dbConfigMng;
        QStringList deviceFieldNameList, partFieldNameList;
        dbConfigMng.getTableFieldNameList(BUSINESS_TYPE_DEVICES, "EMB_Equipment", deviceFieldNameList);
        dbConfigMng.getTableFieldNameList(BUSINESS_TYPE_DEVICES, "EMB_Equipment_Part", partFieldNameList);

        //设备表
        QStringList column;
        QString selection;
        QString deviceID;
        QList<DataRow> dataRows;
        column.append("ID");
        const QList<QMap<QString, QString> > &deviceValueList = dataPackage.tables.at(0).value;
        for(int i = 0; i < deviceValueList.count(); i++){
            QMap<QString, QString> value;
            //字段过滤，删除本地数据库设备表没有的字段
            for(int j = 0; j < deviceFieldNameList.count(); j++){
                QString fieldName = deviceFieldNameList.at(j);
                if(deviceValueList.at(i).contains(fieldName))
                    value[fieldName] = deviceValueList.at(i)[fieldName];
            }
            QString RFIDCode = value["RFIDCode"];
            if(RFIDCode != ""){
                DataRow updateRow;
                updateRow["RFIDCode"] = "";
                selection = " where RFIDCode = '" + RFIDCode + "'";
                m_nDbHelper->update("EMB_Equipment", updateRow, selection);
            }
            QString BarCode = value["BarCode"];
            if(BarCode != ""){
                selection = " where BarCode = '" + BarCode + "'";
                DataRow updateRow;
                updateRow["BarCode"] = "";
                m_nDbHelper->update("EMB_Equipment", value, selection);
            }
            value["SyncStatus"] = "1";
            deviceID = value[DB_ID];
            selection = " where " + QString(DB_ID) + "='" + deviceID + "'";
            dataRows = m_nDbHelper->select("EMB_Equipment", column, selection);
            if(dataRows.isEmpty()){
                m_nDbHelper->insert("EMB_Equipment", value);
            }else{
                m_nDbHelper->update("EMB_Equipment", value, selection);
            }
        }

        //部件表
        const QList<QMap<QString, QString> > &partValueList = dataPackage.tables.at(1).value;
        for(int i = 0; i < partValueList.count(); i++){
            QMap<QString, QString> value;
            //字段过滤，删除本地数据库部件表没有的字段
            for(int j = 0; j < partFieldNameList.count(); j++){
                QString fieldName = partFieldNameList.at(j);
                if(partValueList.at(i).contains(fieldName))
                    value[fieldName] = partValueList.at(i)[fieldName];
            }
            deviceID = value[DB_ID];
            selection = " where " + QString(DB_ID) + "='" + deviceID + "'";
            dataRows = m_nDbHelper->select("EMB_Equipment_Part", column, selection);
            if(dataRows.isEmpty()){
                m_nDbHelper->insert("EMB_Equipment_Part", value);
            }else{
                m_nDbHelper->update("EMB_Equipment_Part", value, selection);
            }
        }
        m_nDbHelper->endTransaction();
    }
}

void SynchronizerBase::networkResponse(QString objects, QString action, bool result, int error, DbusPackage map)
{
    qDebug() <<Q_FUNC_INFO << " objects " << objects << "  action  " << action << " result " << result << "  error " << error;
    QString requestAction = m_nActionList[action];
    if(requestAction == ""){
        qDebug() << "!m_nActionList.contains(action)  return";
        return;
    }
    m_nActionList.remove(action);
    if(objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs &&
            result == true &&
            error == NETWORK_NO_ERROR){
        //写设备表
        writeDeviceToDB(map.dataPackage);
    }
    networkResult(objects, requestAction, result, error, map);
}

void SynchronizerBase::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map)
{
    Q_UNUSED(objects)
    Q_UNUSED(action)
    Q_UNUSED(result)
    Q_UNUSED(error)
    Q_UNUSED(map)
    qDebug() << Q_FUNC_INFO;
}
