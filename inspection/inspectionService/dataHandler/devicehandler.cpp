#include "devicehandler.h"
#include "../common/runtimeparam.h"

/**
 * 组装 DataPackage 中的表名
 */
#define TABLE_NAME_OF_DATA_PACKAGE "EMB_Equipment"

DeviceHandler::DeviceHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICES;
}

DeviceHandler::~DeviceHandler()
{
}

void DeviceHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO;
    if(dataPackage.head.objects == Equipment_QueryOne){
        QString keyCode = dataPackage.para.value("KeyCode");
        QString equipmentID = dataPackage.para.value("EquipmentID");
        QString equipmentCode = dataPackage.para.value("EquipmentCode");
        QString organiseUnitID = dataPackage.para.value("OrganiseUnitID");
        QString strSql = "select ID, EquipmentCode, (EquipmentName || \"[\" || Positions || \"]\")  as EquipmentName, EquipmentModelName, Custodian, "
                "ClassName, ModelName, ManufacturerName, OrganiseUnitName, OtherCode, Detaile, EquipmentSeries, BuyDate"
                " from EMB_Equipment ";
        if(equipmentID != "")
            strSql += " where ID='" + equipmentID + "' AND EquipmentStatus < 6 and OrganiseUnitID= '" + organiseUnitID + "'";
        else if(keyCode != "")
            strSql += " where  '" + keyCode.toLower() + "' in (LOWER(RFIDCode), LOWER(BarCode), LOWER(QRCode)) AND EquipmentStatus < 6 and OrganiseUnitID= '" + organiseUnitID + "'";
        else if (equipmentCode != "")
            strSql += " where EquipmentCode='" + equipmentCode + "' AND EquipmentStatus < 6 and OrganiseUnitID= '" + organiseUnitID + "'";
        else
            strSql += " limit 0";
        qDebug() << Q_FUNC_INFO << " strSql  " << strSql;
        QList<DataRow> dataRows;
        m_nDbHelper->select(strSql, dataRows);
        DbusPackage dbusPackage;
        convertToDataPackage(dataRows, dataPackage.head, dataPackage.para , TABLE_NAME_OF_DATA_PACKAGE, DB_ID, dbusPackage.dataPackage);
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    }else if(dataPackage.head.objects == Equipment_QueryList_ByName){
        QString equipmentName = dataPackage.para.value("EquipmentName");
        QString modelName = dataPackage.para.value("ModelName");
        QString equipmentCode = dataPackage.para.value("EquipmentCode");
        QString organiseUnitID = dataPackage.para.value("OrganiseUnitIDs");
        QString strSql = "select ID, EquipmentName, EquipmentCode, EquipmentSeries, EquipmentModelName, Positions, ClassName, ModelName,"
                "ManufacturerName from EMB_Equipment "
                "where EquipmentStatus < 6 "
                "and (EquipmentName like '%" + equipmentName + "%' or '' = '" + equipmentName + "' ) " +
                "and (ModelName like '%" + modelName + "%' or '' = '" + modelName + "' ) " +
                "and (EquipmentCode like '%" + equipmentCode + "%' or '' = '" + equipmentCode + "') " +
                "and OrganiseUnitID='" + organiseUnitID + "'";
        QList<DataRow> dataRows;
        m_nDbHelper->select(strSql, dataRows);
        DbusPackage dbusPackage;
        convertToDataPackage(dataRows, dataPackage.head, dataPackage.para , TABLE_NAME_OF_DATA_PACKAGE, DB_ID, dbusPackage.dataPackage);
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    } else if(dataPackage.head.objects == Equipment_ConRFID_Query_ByRFIDCode){
        QString keyCode = dataPackage.para.value("KeyCode");
        QString equipmentID = dataPackage.para.value("EquipmentID");
        QString equipmentCode = dataPackage.para.value("EquipmentCode");
        QString strSql = "select ID, EquipmentCode, (EquipmentName || \"[\" || Positions || \"]\")  as EquipmentName, EquipmentModelName, Custodian, "
                "ClassName, ModelName, ManufacturerName, OrganiseUnitName, OtherCode, Detaile, EquipmentSeries, BuyDate"
                         " from EMB_Equipment ";
        if(equipmentID != "")
            strSql += "where ID='" + equipmentID + "' and EquipmentStatus < 6 ";
        else if(keyCode != "")
            strSql += " where  '" + keyCode.toLower() + "' in (LOWER(RFIDCode), LOWER(BarCode), LOWER(QRCode)) and EquipmentStatus < 6";
        else if (equipmentCode != "")
            strSql += "where EquipmentCode='" + equipmentCode + "' and EquipmentStatus < 6 ";
        else
            strSql += " limit 0";

        QList<DataRow> dataRows;
        m_nDbHelper->select(strSql, dataRows);
        DbusPackage dbusPackage;
        convertToDataPackage(dataRows, dataPackage.head, TABLE_NAME_OF_DATA_PACKAGE, DB_ID, dbusPackage.dataPackage);
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    }
    m_nRequestPkgMap.remove(action);
}

int DeviceHandler::getItemCount()
{
    return 0;
}

void DeviceHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    Q_UNUSED(action)
    Q_UNUSED(objects)
    qDebug() << Q_FUNC_INFO;
}

void DeviceHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result  && error == NETWORK_NO_ERROR){
        emit onResultOfResponse(objects, action, result, error, map);
        m_nRequestPkgMap.remove(action);
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}
