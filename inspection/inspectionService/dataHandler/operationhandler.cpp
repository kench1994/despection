#include "operationhandler.h"
#include "common/dbhelper.h"
#include "../common/bussinessmap.h"
OperationHandler::OperationHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_OPERATION;
}

OperationHandler::~OperationHandler()
{
}

void OperationHandler::offLine(const QString &action, const DataPackage &dataPackage){
    qDebug() << Q_FUNC_INFO << " action " << action;
    if(dataPackage.head.objects == EMF_FaultRepair_Para || dataPackage.head.objects == EMF_FaultAct_List || dataPackage.head.objects == EMF_FaultRepair_ModelCount){
        DbusPackage resultPkg;
        if(dataPackage.head.objects == EMF_FaultRepair_Para){
            getLocalFaultRepairPara(dataPackage, resultPkg.dataPackage);
        } else if(dataPackage.head.objects == EMF_FaultAct_List){
            QList<DataRow> dataRows;
            convertToDataPackage(dataRows, dataPackage.head, dataPackage.para, "EMF_FaultAct", "", resultPkg.dataPackage);
        } else if(dataPackage.head.objects == EMF_FaultRepair_ModelCount){
            QList<DataRow> dataRows;
            convertToDataPackage(dataRows, dataPackage.head, dataPackage.para, "EMF_FaultRepair", "", resultPkg.dataPackage);
        }
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, resultPkg);
    } else
        sendNoNetworkResponse(dataPackage.head.objects, action);
    m_nRequestPkgMap.remove(action);
}

int OperationHandler::getItemCount()
{
    return -1;
}

void OperationHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(action)
    Q_UNUSED(objects)
    Q_UNUSED(map)
}

void OperationHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    emit onResultOfResponse(objects, action, result, error, map);
    m_nRequestPkgMap.remove(action);
}

bool OperationHandler::getLocalFaultRepairPara(const DataPackage &requestPackage, DataPackage &resultPackage)
{
    QString strSql ="";
    QList<QMap<QString, QString> > valueList;
    QString equipmentID = requestPackage.para["EquipmentID"];
    QString keyCode = requestPackage.para["KeyCode"];
    QString equipmentCode = requestPackage.para["EquipmentCode"];
    QString organiseUnitID = requestPackage.para["OrganiseUnitID"];
    strSql = "select ID, EquipmentCode, (EquipmentName || '[' || Positions || ']')  as EquipmentName, EquipmentModelName, Custodian, "
            "ClassName, ModelName, ManufacturerName as EnterpriseName, OrganiseUnitName as EquipmentOrganiseUnitName, OtherCode, Detaile "
            " FROM EMB_Equipment ";
    if (!equipmentID.isEmpty())
        strSql += "where ID='" + equipmentID + "' AND EquipmentStatus < 6 and OrganiseUnitID= '" + organiseUnitID + "'";
    else if (!keyCode.isEmpty())
        strSql += "where LOWER(RFIDCode)='" + keyCode + "' or BarCode='" + keyCode + "' or QRCode='" + keyCode + "'"
                " AND EquipmentStatus < 6 and OrganiseUnitID= '" + organiseUnitID + "'";
    else if (!equipmentCode.isEmpty())
        strSql += "where EquipmentCode='" + equipmentCode + "' AND EquipmentStatus < 6 and OrganiseUnitID= '" + organiseUnitID + "'";

    bool rv = DBHelper::getInstance()->select(strSql, valueList);
    qDebug() << Q_FUNC_INFO << "strSql is " << strSql;
    qDebug() << Q_FUNC_INFO << valueList;
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestPackage.head, "ISM_InspectionTaskGroup", "TaskGroupDataKey", resultPackage);
    if(resultPackage.tables.at(0).value.length() == 0){
        DataRow value;
        value["ID"] = "";
        value["EquipmentCode"] = equipmentCode;
        value["RFIDCode"] = keyCode;
        value["BarCode"] = keyCode;
        value["EquipmentName"] = "离线虚拟设备";
        value["ModelName"] = "";
        value["EnterpriseName"] = "";
        value["Custodian"] = "";
        resultPackage.tables[0].value.append(value);
    }

    //离线时，故障类型固定，故障原因没有
    DataTable table;
    table.name = "FaultType";
    table.key = "DicItemCode";
    DataRow value1;
    value1["DicItemCode"] = "1";
    value1["Name"] = "运行损坏";
    DataRow value2;
    value2["DicItemCode"] = "2";
    value2["Name"] = "意外损坏";
    DataRow value3;
    value3["DicItemCode"] = "3";
    value3["Name"] = "事故损坏";
    DataRow value4;
    value4["DicItemCode"] = "4";
    value4["Name"] = "其它原因";
    table.value.append(value1);
    table.value.append(value2);
    table.value.append(value3);
    table.value.append(value4);
    resultPackage.tables.append(table);

    DataTable tableReason;
    tableReason.key = "DicItemCode";
    tableReason.name = "FaultReason";
    resultPackage.tables.append(tableReason);
    return true;
}
