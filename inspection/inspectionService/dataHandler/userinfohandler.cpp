#include "userinfohandler.h"
#include "../common/bussinessmap.h"
#include "../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include "../DBConfig/dbconfiguserinfo.h"
#include "datahandlerfactory.h"
#include <QDateTime>
UserinfoHandler::UserinfoHandler()
{
    m_nDataOperationType = BUSINESS_TYPE_USER_INFO;
}

UserinfoHandler::~UserinfoHandler()
{
}

void UserinfoHandler::offLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO;
    QString objects = dataPackage.head.objects;
    QStringList column;
    QList<DataRow> dataRows;
    QString userCode = "";
    QString userID = "";
    DbusPackage dbusPackage;
    if(objects == SDMS_User_Change_Pwd){
        sendNoNetworkResponse(objects, action);
    } else {
        if(objects == User_QueryPhonePrincipal ||
            objects == User_QueryPhonePrincipalByRFIDCode){
            if(objects == User_QueryPhonePrincipal){
                userCode = dataPackage.para.value(USER_CODE);
                dataRows = m_nDbHelper->select(USER_TABLE, column, " where "+ QString(USER_CODE) +"='" + userCode +"'");
            }else if(objects == User_QueryPhonePrincipalByRFIDCode){
                QString RFIDCode = dataPackage.para.value("UserRFIDCode").toLower();
                dataRows = m_nDbHelper->select(USER_TABLE, column, " where " + QString(USER_RFID) + "='" + RFIDCode + "'");
            }
            if(!dataRows.empty()){
                userID =  dataRows.at(0).value(USER_ID);
                userCode = dataRows.at(0).value(USER_CODE);
            }
            QList<DataRow> userInfo;
            m_nDbHelper->select("select UserCode from " + QString(HISTORY_USER_TABLE) + " where UserCode = '" + userCode + "'", userInfo);
            if(userInfo.size() == 0){
                sendNoNetworkResponse(dataPackage.head.objects, action);
                return;
            }

             ///初始化  Head
            dbusPackage.dataPackage.head.objects = dataPackage.head.objects;
            dbusPackage.dataPackage.head.key = dataPackage.head.key;
            dbusPackage.dataPackage.head.type = dataPackage.head.type;
            dbusPackage.dataPackage.head.date = dataPackage.head.date;
            ///初始化　Tablas
            DataTable table0;
            table0.name = USER_TABLE;
            table0.key = USER_ID;
            table0.value.append(dataRows);
            dbusPackage.dataPackage.tables.append(table0);

            dataRows = m_nDbHelper->select(USER_TABLE_APPLICATION, column, " where "+ QString(USER_ID) + "='" + userID +"'");
            DataTable table1;
            table1.name = USER_TABLE_APPLICATION;
            table1.key = USER_ID;
            table1.value.append(dataRows);
            dbusPackage.dataPackage.tables.append(table1);

            dataRows = m_nDbHelper->select(USER_TABLE_MODULE, column, " where "+ QString(USER_ID) + "='" + userID +"'");
            DataTable table2;
            table2.name = USER_TABLE_MODULE;
            table2.key = USER_ID;
            table2.value.append(dataRows);
            dbusPackage.dataPackage.tables.append(table2);
        } else if(objects == User_QueryHistoryUsers_Local){
            QList<DataRow> usersInfo;
            m_nDbHelper->select("select * from " + QString(HISTORY_USER_TABLE) + "  order by LastLoginDate desc ", usersInfo);
            convertToDataPackage(usersInfo, dataPackage.head, HISTORY_USER_TABLE, "UserCode", dbusPackage.dataPackage);
        } else if(objects == User_SetHistoryUsers_Local){
            QString userCode = dataPackage.para["UserCode"];
            QString passwd = dataPackage.para["Password"];
            updateHistoryUsersInfo(userCode, passwd);
        }
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    }
    m_nRequestPkgMap.remove(action);
}

void UserinfoHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() <<Q_FUNC_INFO << dataPackage.head.objects;
    if(dataPackage.head.objects == User_QueryHistoryUsers_Local || dataPackage.head.objects == User_SetHistoryUsers_Local)
        offLine(action ,dataPackage);
    else
        DataHandlerBase::onLine(action, dataPackage);
}

int UserinfoHandler::getItemCount()
{
    return 0;
}

void UserinfoHandler::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    Q_UNUSED(map)
    Q_UNUSED(action)
    Q_UNUSED(objects)
    qDebug() <<Q_FUNC_INFO;
}

void UserinfoHandler::pureOnLine(const QString &action, const DataPackage &dataPackage)
{
    qDebug() << Q_FUNC_INFO << " objects " << dataPackage.head.objects;
    if(dataPackage.head.objects == User_QueryHistoryUsers_Local || dataPackage.head.objects == User_SetHistoryUsers_Local)
        offLine(action ,dataPackage);
    else
        DataHandlerBase::pureOnLine(action, dataPackage);
}

void UserinfoHandler::networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map)
{
    //离线请求
    qDebug() << Q_FUNC_INFO << objects << " result " << result << "  error " << error;
    if(result && error == NETWORK_NO_ERROR){
        if(objects == User_QueryPhonePrincipal ||
                objects == User_QueryPhonePrincipalByRFIDCode){
            handleUserInfo(map.dataPackage, action);
            emit onResultOfResponse(objects, action, result, error, map);
        }else if(objects == SDMS_User_Change_Pwd){
            QMap<QString, QString> &para = m_nRequestPkgMap[action].para;
            QString updateArg = " where " + QString(USER_ID) + "='" + para["UserID"] +"'";
            DataRow dataRow;
            dataRow["Password"] = para["PassWord"];
            dataRow["PasswordKey"] = para["PasswordKey"];
            dataRow["PasswordIV"] = para["PasswordIV"];
            m_nDbHelper->update(USER_TABLE, dataRow, updateArg);
            emit onResultOfResponse(objects, action, result, error, map);
        }
        m_nRequestPkgMap.remove(action);
    } else {
        offLine(action, m_nRequestPkgMap[action]);
    }
}

void UserinfoHandler::handleUserInfo(const DataPackage &dataPackage, const QString &action)
{
    QList<DataTable> tables = dataPackage.tables;
    if(tables.isEmpty()){
        return;
    }

    QString userID;
    QStringList column;

    m_nDbHelper->beginTransaction();

    /**
    * 用户基本信息
    */
    const QList<DataRow> &dataRows0 = tables.at(0).value; ///获取table中第0个元素的value
    if(!dataRows0.isEmpty()){
        DataRow dataRow = dataRows0[0];
        userID = dataRow[USER_ID];
        dataRow.remove(POST_CODE);
        if(dataRow["UserRFIDCode"] != ""){
            dataRow["RFIDCode"] = dataRow["UserRFIDCode"].toLower();
        }else{
            dataRow["RFIDCode"] = dataRow["RFIDCode"].toLower();
        }
        dataRow.remove("UserRFIDCode");
        QList<DataRow> list = m_nDbHelper->select(USER_TABLE, column, " where "+ QString(USER_ID) + "='"+ userID + "'");
        if(!list.isEmpty()){
            QString oldOrganiseUnitID = list.at(0)["OrganiseUnitIDs"];
            QString oldDepartmentID = list.at(0)["DepartmentIDs"];
            QString newOrganiseUnitID = dataRow["OrganiseUnitIDs"];
            QString newDepartmentID = dataRow["DepartmentIDs"];
            qDebug() << Q_FUNC_INFO << "  oldOrganiseUnitID : " << oldOrganiseUnitID << ",  newOrganiseUnitID : " << newOrganiseUnitID;
            qDebug() << Q_FUNC_INFO << "    oldDepartmentID : " << oldDepartmentID   << ",    newDepartmentID : " << newDepartmentID;
            if(oldOrganiseUnitID != newOrganiseUnitID || oldDepartmentID != newDepartmentID){
                DataHandlerFactory dataHandlerFactory;
                QList<DataHandlerBase * > dataHandlerBaseList = dataHandlerFactory.getAllDataHandler();
                for(int i = 0; i < dataHandlerBaseList.size(); i++){
                    DataHandlerBase *dataHandlerBase = dataHandlerBaseList.at(i);
                    dataHandlerBase->cleaerTask(userID, oldOrganiseUnitID, oldDepartmentID);
                }
            }
            m_nDbHelper->update(USER_TABLE, dataRow, " where " + QString(USER_ID) + "='" + userID + "'");
        }else{
            m_nDbHelper->insert(USER_TABLE, dataRow);
        }
    }

    /**
    * SDMS_Application
    */
    QList<DataRow> dataRows1 = tables.at(1).value; ///获取table中第1个元素的value
    foreach (DataRow dataRow, dataRows1) {
        QList<DataRow> list = m_nDbHelper->select(USER_TABLE_APPLICATION, column, " where " + QString(USER_ID) + "='"+ userID + "'");
        if(!list.isEmpty()){
            m_nDbHelper->update(USER_TABLE_APPLICATION, dataRow, " where " + QString(USER_ID) + "='" + userID + "'");
        }else{
            dataRow.insert(USER_ID, userID);
            m_nDbHelper->insert(USER_TABLE_APPLICATION, dataRow);
        }
    }

    /**
    * 权限模块信息
    */
    QList<DataRow> dataRows2 = tables.at(2).value; ///获取table中第2个元素的value
    m_nDbHelper->deleteRow(USER_TABLE_MODULE, " where " + QString(USER_ID) + "='" + userID + "'");
    foreach (DataRow dataRow, dataRows2) {
        dataRow.insert(DB_ID, SystemConfig::getInstance()->getUUID());
        dataRow.insert(USER_ID, userID);
        m_nDbHelper->insert(USER_TABLE_MODULE, dataRow);
    }

    /**
    * 用户岗位信息
    */
    QList<DataRow> dataRows3 = tables.at(3).value; ///获取table中第3个元素的value
    m_nDbHelper->deleteRow(USER_TABLE_POST, " where " + QString(USER_ID) + "='" + userID + "'");
    foreach (DataRow dataRow, dataRows3) {
        dataRow.insert(USER_ID, userID);
        m_nDbHelper->insert(USER_TABLE_POST, dataRow);
    }
    m_nDbHelper->endTransaction();

    Q_UNUSED(action)
}

void UserinfoHandler::updateHistoryUsersInfo(const QString &userCode, const QString &passwd)
{
    QList<DataRow> userInfo;
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
    DataRow value;
    value["UserCode"] = userCode;
    value["Password"] = passwd;
    value["LastLoginDate"] = currentTime;
    m_nDbHelper->select("select UserCode from " + QString(HISTORY_USER_TABLE) + " where UserCode = '" + userCode + "'", userInfo);
    if(userInfo.size() > 0){
        m_nDbHelper->update(HISTORY_USER_TABLE, value, " where UserCode = '" + userCode + "'");
    } else
        m_nDbHelper->insert(HISTORY_USER_TABLE, value);
}
