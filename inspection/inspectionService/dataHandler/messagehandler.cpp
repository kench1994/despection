#include "messagehandler.h"
#include "common/dbhelper.h"
#include "../../framework/common/systemconfig.h"
#include "../common/runtimeparam.h"
#include <csystemnotification.h>
#include <csystemnotificationmanager.h>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

MessageHandler::MessageHandler()
{
    m_nMessageCount = 0;
    m_nDataOperationType = BUSINESS_TYPE_MESSAGE;
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::offLine(const QString &action, const DataPackage &dataPackage){
    qDebug()<<Q_FUNC_INFO << "  action " << action;
    if(dataPackage.head.objects == "CMDS_MessageList"){
        m_nMessageCount = 0;
        CSystemNotificationManager notificationManager;
        notificationManager.removeAllNotifications("com.syberos.deviceInspection");
        DbusPackage dbusPackage;
        getLocalMessageList(dataPackage, dbusPackage.dataPackage);
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    } else if(dataPackage.head.objects == "Module_Query_MessageCounts_ForAndroid"){
        DbusPackage dbusPackage;
        bool rv = getLocalUnreadMessageCount(dataPackage, dbusPackage.dataPackage);
        if(rv)
            emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
        else
            emit onResultOfResponse(dataPackage.head.objects, action, false, DATABASE_ERROR_SELECT_FAIL, dbusPackage);
    } else if(dataPackage.head.objects == "CMDS_Message_Delete") {
        DbusPackage dbusPackage;
        deleteLocalMessages(dataPackage);
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    } else if(dataPackage.head.objects == "CMDS_Message_Update"){
        DbusPackage dbusPackage;
        updateLocalMessages(dataPackage);
        emit onResultOfResponse(dataPackage.head.objects, action, true, NETWORK_NO_ERROR, dbusPackage);
    } else if(dataPackage.head.objects == "CMDS_MessageList_Push"){
        QList<DataTable> tables = dataPackage.tables;
        if(tables.size() > 0 && tables.at(0).value.size() > 0){
            QList<DataRow> dataRows = tables.at(0).value;
            savePushMessageToDB(dataRows);
            sendNotification(dataRows.count());
        }
    }
    m_nRequestPkgMap.remove(action);
}

void MessageHandler::pureOnLine(const QString &action, const DataPackage &dataPackage)
{
    offLine(action, dataPackage);
}

void MessageHandler::onLine(const QString &action, const DataPackage &dataPackage)
{
    offLine(action, dataPackage);
}

bool MessageHandler::getLocalMessageList(const DataPackage &requestDataPackage, DataPackage &resultDataPackage)
{
    QList<DataRow > valueList;
    QString userID = requestDataPackage.para.value("UserID");
    QString operType = requestDataPackage.para.value("OperType");
    bool rv = getLocalMessageList(operType, userID, 0, valueList);
    if(!rv)
        return false;
    convertToDataPackage(valueList, requestDataPackage.head, "Message_Info", "MessageID", resultDataPackage);
    return true;
}

bool MessageHandler::getLocalUnreadMessageCount(const DataPackage &requestDataPackage, DataPackage &resultDataPackage)
{
    QString strSql = "select count(*) as MessageCount from Message_Info  where ReadStatus = '0'";
    qDebug() << Q_FUNC_INFO  << strSql;
    QList<DataRow > valueList;
    bool rv =  DBHelper::getInstance()->select(strSql, valueList);
    if(rv == false)
        return false;
    convertToDataPackage(valueList, requestDataPackage.head, "Message_Info", "MessageID", resultDataPackage);
    return true;
}

bool MessageHandler::getLocalMessageList(const QString &operType, const QString &userID,  const int &maxCount, QList<DataRow> &valueList)
{
    Q_UNUSED(userID)
    QString strSql;
    if(operType == "Notify"){
        strSql = "select * from Message_Info  where operType = 'Notify' order by PublishDate desc";
    } else {
        strSql = "select * from Message_Info where operType !='Notify'  order by PublishDate desc";
    }
    if(maxCount > 0){
        strSql += " limit " + QString::number(maxCount);
    }
    qDebug() << Q_FUNC_INFO <<"   strSql: " << strSql;
    return DBHelper::getInstance()->select(strSql, valueList);
}

bool MessageHandler::deleteLocalMessages(const DataPackage &requestDataPackage)
{
    QString messageIDs = requestDataPackage.para["MessageIDs"];
    bool rv = false;
    if(!messageIDs.isEmpty()){
        messageIDs.replace(",", "','");
        messageIDs = QString("'") + messageIDs + QString("'");
        DBHelper *dbHelper = DBHelper::getInstance();
        QString sql = "delete from  Message_Info where MessageID in (" + messageIDs + ")";
        rv = dbHelper->execSQL(sql);
    }
    return rv;
}

bool MessageHandler::updateLocalMessages(const DataPackage &requestDataPackage)
{
    QStringList messageIDList = requestDataPackage.para["MessageIDs"].split(",");
    QStringList readStatusList = requestDataPackage.para["ReadStatuss"].split(",");
    bool rv = false;
    if(messageIDList.count() > 0 && readStatusList.count() > 0 && messageIDList.count() == readStatusList.count()){
        DBHelper *dbHelper = DBHelper::getInstance();
        dbHelper->beginTransaction();
        for(int i = 0; i < messageIDList.count(); i++){
            QString messageID = messageIDList.at(i);
            QString readStatus = readStatusList.at(i);
            QMap<QString, QString> value;
            value.insert("ReadStatus", readStatus);
            rv = dbHelper->update("Message_Info", value, " where MessageID='" + messageID + "'");
            if(rv == false){
                break;
            }
        }
        if(rv){
            dbHelper->endTransaction();
        } else
            dbHelper->endTransaction(false);
    }

    return rv;
}

QString MessageHandler::sendNotification(const int msgCount){
    CSystemNotificationManager notificationManager;
    if(m_nNotifyKey != ""){
        notificationManager.removeOneNotification(m_nNotifyKey);
    }
    CSystemNotification systemNotification;
    systemNotification.setSopId("com.syberos.deviceInspection");
    systemNotification.setAppName("设备巡检");
    systemNotification.setTitle("巡检消息");
    m_nMessageCount += msgCount;
    QString notifyStr = QString("收到") + QString::number(m_nMessageCount) + QString("条消息");
    qDebug() << Q_FUNC_INFO << "  notifyStr is   " << notifyStr;
    systemNotification.setSubtitle(notifyStr);
    //systemNotification.setDisplayTime(QDateTime::fromString(publishDate, "yyyy/MM/dd hh:mm:ss"));
    //systemNotification.setIconName(imgPath + "businessMsg.png");
    systemNotification.setAction(SystemConfig::getInstance()->getUUID() + "_Msg");
    //设置通知发送时的震动模式:正常模式。 使用系统的震动设置。 系统启用震动， 通知发送时也震动。否则， 不震动。
    systemNotification.setVibrationMode(CNotification::NormalMode);
    m_nNotifyKey = notificationManager.sendNotification(systemNotification);
    return m_nNotifyKey;
}

void MessageHandler::savePushMessageToDB(QList<DataRow> &values)
{
    DBHelper *dbHelper = DBHelper::getInstance();
    dbHelper->beginTransaction();
    QStringList column;
    foreach (DataRow value, values) {
        QString messageID = value.value("MessageID");
        QString operType = value.value("OperType");
        QString content = value.value("Content");
        QString appId = value.value("AppId");
        if(appId != SystemConfig::getInstance()->getAppId()){
            continue;
        }
        if(operType != "Notify"){
            int index = content.lastIndexOf("Descript");
            if(index != -1){
                QStringList contents = content.mid(index).split("=");
                if(contents.length() > 1 && contents[1].length() > 5) {
                    value.insert("Content", contents[1].mid(1, contents[1].length() - 4));
                } else {
                   value.insert("Content", "");
                }
            }
        }
        QList<DataRow > list = dbHelper->select("Message_Info", column, " where MessageID = '"+ messageID + "'");
        if(!list.isEmpty()){
            dbHelper->update("Message_Info", value, " where MessageID='" + messageID + "'");
        }else{
            value.insert("ReadStatus", "0");
            dbHelper->insert("Message_Info", value);
        }
    }
    dbHelper->endTransaction();
    QDBusMessage m = QDBusMessage::createSignal(SERVICE_DBUS_PATH,  SERVICE_DBUS_INTERFACE, "messageUpdated");
    QString operType = values.at(0)["OperType"];
    m << operType;
    QDBusConnection::sessionBus().send(m);
}
