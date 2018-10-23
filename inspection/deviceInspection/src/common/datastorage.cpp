#include "datastorage.h"
#include <QtSql/QSqlQuery>
#include <QUuid>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <csystemnotification.h>//add by chen ruihua
#include <errno.h>
#include <QTimer>
DataStorage* DataStorage::m_pInstance = NULL;

DataStorage::DataStorage(){
    initialize();
}

DataStorage* DataStorage::getInstance(){
    if(m_pInstance == NULL){
        m_pInstance = new DataStorage();
    }
    return m_pInstance;
}

void DataStorage::initialize(){
    SystemConfig *config = SystemConfig::getInstance();
    if(!config) {
        qDebug("SystemConfig::getInstance failed.");
        return;
    }

    m_nMsgCountForShowInList = config->getMsgCountForShowInList();
    m_nSystemMsgCountShowInNotify = config->getSystemMsgCountForShowInNotify();
    m_nBusinessMsgCountShowInNotify = config->getBusinessMsgCountForShowInNotify();
    m_nSystemNotifyKeyQueue.clear();
    m_nBusinessNotifyKeyQueue.clear();
    m_nSystemMessageList.clear();
    m_nBusinessMessageList.clear();
    m_nPendingSystemMessageList.clear();
    m_nPendingBusinessMessageList.clear();
    m_nTimer.setSingleShot(true);
    m_nTimer.setInterval(1500);
    connect(&m_nTimer, SIGNAL(timeout()), this, SLOT(removeMsgFromNotification()));
   //removeAllMessageNotify("com.syberos.deviceInspection");
    m_nDB = QSqlDatabase::addDatabase("QSQLITE");

    m_nDbFullPath = config->getDatabaseFileName();
    if( m_nDbFullPath == ""){
        m_nDbFullPath = MESSAGE_DB_PATH;
        config->setDatabaseFileName(m_nDbFullPath);
    }
    m_nDB.setDatabaseName(m_nDbFullPath);
    if(!m_nDB.open()){
        qDebug() << "failed to open database" << MESSAGE_DB_PATH;
    }
    bool hasTable = false;
    QSqlQuery query;
    qDebug() << "Search Table SQL: SELECT name FROM sqlite_master WHERE type='table';";
    query.exec("SELECT name FROM sqlite_master WHERE type='table';");
    while(query.next()){
        qDebug() << "table name" << query.value(0).toString().trimmed();
        if(query.value(0).toString().trimmed() == MESSAGE_TABLE_NAME){
            hasTable = true;
            break;
        }
    }
    qDebug() << "hasTable = " << hasTable;
    //OperType  MessageType
    if(!hasTable){
        QString sql = QString("CREATE TABLE '%1'('%2' Varchar(200) PRIMARY KEY NOT NULL, "
                              " '%3' Text,'%4' Text, '%5' Datetime, '%6' INTEGER, '%7' "
                              " Varchar(200), '%8' Varchar(200), '%9'  Varchar(200), '%10' "
                              " Varchar(200) , '%11' Varchar(200), '%12' Varchar(200), '%13' "
                              " Varchar(200), '%14' Varchar(200));")
                .arg(MESSAGE_TABLE_NAME)
                .arg(MESSAGE_ID)
                .arg(MESSAGE_MESSAGETITLE)
                .arg(MESSAGE_MESSAGECONTENT)
                .arg(MESSAGE_PUBLISHDATE)
                .arg(MESSAGE_READ_STATUS)
                .arg(MESSAGE_NOTIFY_KEY)
                .arg(MESSAGE_PUBLISHER)
                .arg(MESSAGE_ORGANISEUNITID)
                .arg(MESSAGE_DEPARTMENTID)
                .arg(MESSAGE_DEPARTMENTNAME)
                .arg(MESSAGE_ORGANISEUNITNAME)
                .arg(MESSAGE_OPERTYPE)
                .arg(MESSAGE_MESSAGETYPE);
        qDebug() << "Create Table:" << sql;
        query.exec(sql);
    } else {
        if(setMessageList() == false){
            qDebug() << "get MessageList failed.";
        }
    }
}

bool DataStorage::addMessage(const Message& msg)
{
    // TODO : insert new message
    //qDebug() << "add Message time=" << msg.publishDate;

    QSqlQuery query;
    QString sql = QString("INSERT INTO %1 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);")
            .arg(MESSAGE_TABLE_NAME);
    query.prepare(sql);
    query.bindValue(0, msg.uuid);
    query.bindValue(1, msg.messageTitle);
    query.bindValue(2, msg.messageContent);
    query.bindValue(3, msg.publishDate);
    query.bindValue(4, msg.status);

    query.bindValue(5, msg.notifyKey);
    query.bindValue(6, msg.publisher);
    query.bindValue(7, msg.organiseUnitID);
    query.bindValue(8, msg.departmentID);
    query.bindValue(9, msg.departmentName);
    query.bindValue(10, msg.organiseUnitName);
    query.bindValue(11, msg.operType);
    query.bindValue(12, msg.messageType);
    //qDebug() << "add Message SQL:" << sql;
    return query.exec();
}

bool DataStorage::setMessageList(){
    QSqlQuery query;
    QString sql = QString("SELECT * from %1 order by %2 desc;").arg(MESSAGE_TABLE_NAME).arg(MESSAGE_PUBLISHDATE);
    //qDebug() << "get Message: SQL" << sql;
    bool ret = query.exec(sql);
    while (query.next()) {
        Message msg;
        msg.uuid = query.value(0).toString();
        msg.messageTitle = query.value(1).toString();
        msg.messageContent = query.value(2).toString();
        msg.publishDate = query.value(3).toString();
        msg.status = query.value(4).toInt();

        msg.notifyKey = query.value(5).toString();
        msg.publisher = query.value(6).toString();
        msg.organiseUnitID = query.value(7).toString();
        msg.departmentID = query.value(8).toString();
        msg.departmentName = query.value(9).toString();
        msg.organiseUnitName = query.value(10).toString();
        msg.operType = query.value(11).toString();
        msg.messageType = query.value(12).toString();
        if(msg.messageType == "1"){
            m_nSystemMessageList.append(msg);
            if(m_nPendingSystemMessageList.length() < m_nSystemMsgCountShowInNotify)
                m_nPendingSystemMessageList.append(msg);
        }else if(msg.messageType == "2"){
            m_nBusinessMessageList.append(msg);
            if(m_nPendingBusinessMessageList.length() < m_nBusinessMsgCountShowInNotify)
                m_nPendingBusinessMessageList.append(msg);
        }
    }
//    updateLocalNofiyCacheAndNotification();
    return ret;
}

/**
  * @brief updateMessage:更新系统消息到缓存和数据库
  * @return  int: 更新到数据库的记录数
  */
int DataStorage::updateMessage(const QList<QVariant> &list){
    int totalNum = 0;
    qDebug() << "DataStorage::update Message  count " << list.count();
    for (int i = 0; i < list.count(); i++){
        QVariantMap map = list.at(i).toMap();
        QString id = map["MessageID"].toString();
        QString messageType = map["MessageType"].toString();
        //qDebug() << "id   is   " << id;
        if(messageType == "1"){
            int i = 0;
            for (; i < m_nSystemMessageList.size(); ++i){
                if(m_nSystemMessageList.at(i).uuid == id){
                    qDebug() << "m_nSystemMessageList.at(i).uuid == id   "
                             << m_nSystemMessageList.at(i).uuid  << "      " <<id;
                    break;
                }
            }
            if(i < m_nSystemMessageList.size()){
                //uuid相同，说明消息已存在，不必更新
                continue;
            }
            //更新消息列表
            if(m_nSystemMessageList.size() >= m_nMsgCountForShowInList) {
                //消息已满，清除最早的消息；
                int pos = m_nSystemMessageList.size() - 1;
                if(!removeMessage("1", m_nSystemMessageList.at(pos).uuid)){
                    //qDebug() << "remove Message uuid " << m_nSystemMessageList.at(pos).uuid << "failed.";
                    continue;
                }
            }
        }else if(messageType == "2"){
            int i = 0;
            for (; i < m_nBusinessMessageList.size(); ++i){
                if(m_nBusinessMessageList.at(i).uuid == id){
                    //qDebug() << "m_nBusinessMessageList.at(i).uuid == id   " << m_nBusinessMessageList.at(i).uuid  << "      " <<id;
                    break;
                }
            }
            if(i < m_nBusinessMessageList.size()){
                //uuid相同，说明消息已存在，不必更新
                continue;
            }
            //更新消息列表
            if(m_nBusinessMessageList.size() >= m_nMsgCountForShowInList) {
                //消息已满，清除最早的消息；
                int pos = m_nBusinessMessageList.size() - 1;
                if(!removeMessage("2", m_nBusinessMessageList.at(pos).uuid)){
                    //qDebug() << "remove Message uuid " << m_nSystemMessageList.at(pos).uuid << "failed.";
                    continue;
                }
            }
        }else
            continue;
        Message msg;
        msg.uuid = map["MessageID"].toString();
        msg.messageTitle = map["MessageTitle"].toString();
        msg.publisher = map["Publisher"].toString();
        msg.publishDate =  map["PublishDate"].toString();
        msg.messageContent = map["MessageContent"].toString();
        msg.organiseUnitID = map["CompanyID"].toString();
        msg.departmentID = map["DepartmentID"].toString();
        msg.organiseUnitName = map["OrganiseUnitName"].toString();
        msg.departmentName = map["DepartmentName"].toString();
        msg.operType = map["OperType"].toString();
        msg.messageType = map["MessageType"].toString();
        msg.status = 1;

        //添加新消息到数据库和缓存
        if(!addMessage(msg)){
            qDebug() << "addMessage(msg)    error ";
            continue;
        }
        if(messageType == "1"){
            m_nSystemMessageList.insert(0, msg);
            m_nPendingSystemMessageList.insert(0, msg);
        }else if(messageType == "2"){
            m_nBusinessMessageList.insert(0, msg);
            m_nPendingBusinessMessageList.insert(0, msg);
        }
        while(m_nPendingSystemMessageList.length() > m_nSystemMsgCountShowInNotify){
            m_nPendingSystemMessageList.removeLast();
        }
        while(m_nPendingBusinessMessageList.length() > m_nBusinessMsgCountShowInNotify){
            m_nPendingBusinessMessageList.removeLast();
        }
        totalNum++;
    }

    updateLocalNofiyCacheAndNotification();
    SystemConfig *sysConf = SystemConfig::getInstance();
    qDebug() << "totalNum is " << totalNum;
    if(totalNum > 0 && sysConf->getIsMessagePage() == false){
        sysConf->setNewMsgTip(true);
    }
    return totalNum;
}

void DataStorage::updateLocalNofiyCacheAndNotification(){
    qDebug() << "ataStorage::updateLocalNofiyCacheAndNotification() m_nPendingSystemMessageList   "
             << m_nPendingSystemMessageList.size()
             << "  m_nPendingBusinessMessageList   " << m_nPendingBusinessMessageList.size();
    for(int i = m_nPendingSystemMessageList.size() - 1; i >= 0; i--){
        QString notifyKey = updateNotification(m_nPendingSystemMessageList.at(i));
        m_nSystemNotifyKeyQueue.insert(0, notifyKey);
    }
    for(int i = m_nPendingBusinessMessageList.size() - 1; i >= 0 ; i--){
        QString notifyKey = updateNotification(m_nPendingBusinessMessageList.at(i));
        m_nBusinessNotifyKeyQueue.insert(0, notifyKey);
    }
    m_nPendingSystemMessageList.clear();
    m_nPendingBusinessMessageList.clear();
    m_nTimer.start();
}

QString DataStorage::updateNotification(Message msg){
    CSystemNotification mSystemNotification;
    mSystemNotification.setSopId("com.syberos.deviceInspection");
    mSystemNotification.setAppName("设备巡检");
    mSystemNotification.setTitle(msg.departmentName + " " + msg.publisher);
    mSystemNotification.setSubtitle(msg.messageTitle);
    //qDebug() << "mSystemNotification.setDisplayTime   " << msg.publishDate;
    mSystemNotification.setDisplayTime(QDateTime::fromString(msg.publishDate, "yyyy/MM/dd hh:mm:ss"));
    SystemConfig *m_sysConfig = SystemConfig::getInstance();
    QString imgPath = m_sysConfig->getAppInstallPath() + "/res/";
    if(msg.messageType == "1"){
        mSystemNotification.setIconName( imgPath + "systemMsg.png");
        mSystemNotification.setAction(msg.uuid + "_systemMsg");
    }else if(msg.messageType == "2"){
        mSystemNotification.setIconName(imgPath + "businessMsg.png");
        mSystemNotification.setAction(msg.uuid + "_businessMsg");
    }
    //设置通知发送时的震动模式:正常模式。 使用系统的震动设置。 系统启用震动， 通知发送时也震动。否则， 不震动。
    mSystemNotification.setVibrationMode(CNotification::NormalMode);
    //qDebug() << "m_nNotificationManager.sendNotification  " << msg.messageTitle;
    msg.notifyKey = m_nNotificationManager.sendNotification(mSystemNotification);
    return msg.notifyKey;
}

void DataStorage::removeMsgFromNotification(){
    QStringList notifyKeyList = m_nSystemNotificationWatcher.updateIds("com.syberos.deviceInspection",
                                                                      "com.syberos.simpleappui");
    if(notifyKeyList.count() > m_nSystemMsgCountShowInNotify + m_nBusinessMsgCountShowInNotify){
        for(int i = m_nSystemMsgCountShowInNotify; i < m_nSystemNotifyKeyQueue.size(); i++){
            m_nNotificationManager.removeOneNotification(m_nSystemNotifyKeyQueue.at(i));
        }
        for(int i = m_nBusinessMsgCountShowInNotify; i < m_nBusinessNotifyKeyQueue.size(); i++){
            m_nNotificationManager.removeOneNotification(m_nBusinessNotifyKeyQueue.at(i));
        }

        while(m_nSystemNotifyKeyQueue.length() > m_nSystemMsgCountShowInNotify)
            m_nSystemNotifyKeyQueue.removeLast();
        while(m_nBusinessNotifyKeyQueue.length() > m_nBusinessMsgCountShowInNotify)
            m_nBusinessNotifyKeyQueue.removeLast();
    }
}

QVariantMap DataStorage::getMessageByIndex(const QString &messageType, const int &index){
    QVariantMap msgMap;
    Message msg;
    if(messageType == "1"){
        if(index < 0 || index >= m_nSystemMessageList.count()){
            qDebug() << "getMessageTitle: para failed.";
            return msgMap;
        }
        msg = m_nSystemMessageList.at(index);
    }else {
        if(index < 0 || index >= m_nBusinessMessageList.count()){
            qDebug() << "getMessageTitle: para failed.";
            return msgMap;
        }
        msg = m_nBusinessMessageList.at(index);
    }
    msgMap["uuid"] = msg.uuid;
    msgMap["messageTitle"] = msg.messageTitle;
    msgMap["messageContent"] = msg.messageContent;
    msgMap["publishDate"] = msg.publishDate;
    msgMap["status"] = msg.status;
    msgMap["notifyKey"] = msg.notifyKey;
    msgMap["publisher"] = msg.publisher;
    msgMap["organiseUnitID"] = msg.organiseUnitID;
    msgMap["departmentID"] = msg.departmentID;
    msgMap["departmentName"] = msg.departmentName;
    msgMap["organiseUnitName"] = msg.organiseUnitName;
    msgMap["operType"] = msg.operType;
    msgMap["messageType"] = msg.messageType;
    return msgMap;
}

QVariantMap DataStorage::getMessageByUuid(const QString &messageType, const QString &uuid){
    QVariantMap msgMap;
    Message msg;
    if(messageType == "1"){
        int i = 0;
        for(i=0; i<m_nSystemMessageList.size(); i++){
            if(m_nSystemMessageList.at(i).uuid == uuid){
                break;
            }
        }
        if(i == m_nSystemMessageList.size()){
            return msgMap;
        }
        msg = m_nSystemMessageList.at(i);
    }else{
        int i = 0;
        for(i=0; i<m_nBusinessMessageList.size(); i++){
            if(m_nBusinessMessageList.at(i).uuid == uuid){
                break;
            }
        }
        if(i == m_nBusinessMessageList.size()){
            return msgMap;
        }
        msg = m_nBusinessMessageList.at(i);
    }
    msgMap["uuid"] = msg.uuid;
    msgMap["messageTitle"] = msg.messageTitle;
    msgMap["messageContent"] = msg.messageContent;
    msgMap["publishDate"] = msg.publishDate;
    msgMap["status"] = msg.status;
    msgMap["notifyKey"] = msg.notifyKey;
    msgMap["publisher"] = msg.publisher;
    msgMap["organiseUnitID"] = msg.organiseUnitID;
    msgMap["departmentID"] = msg.departmentID;
    msgMap["departmentName"] = msg.departmentName;
    msgMap["organiseUnitName"] = msg.organiseUnitName;
    msgMap["operType"] = msg.operType;
    msgMap["messageType"] = msg.messageType;
    return msgMap;
}

bool DataStorage::removeMessage(const QString &messageType, QString uuid){
    QSqlQuery query;
    //qDebug() << "bool DataStorage::removeMessage  messageTYpe is   " << messageType;
    QString sql = QString("DELETE FROM %1 WHERE %2='%3';")
            .arg(MESSAGE_TABLE_NAME)
            .arg(MESSAGE_ID)
            .arg(uuid);
    //qDebug() << "bool DataStorage::removeMessage  sql is    " << sql;
    if(!query.exec(sql)){
        qDebug() << "query.exec  error  return";
        return false;
    }
    return removeMessageFromList(messageType, uuid);
}

bool DataStorage::removeMessageFromList(const QString &messageType, const QString uuid){
    if(messageType == "1"){
        int i = 0;
        for(; i < m_nSystemMessageList.size(); i++){
            if(m_nSystemMessageList.at(i).uuid == uuid){
                break;
            }
        }
        if(i >= m_nSystemMessageList.size()){
            qDebug() << "system   uuid " << uuid << "not exist.";
            return false;
        }
        qDebug() << "m_nSystemMessageList.removeAt   i is  " << i << "  size is " << m_nSystemMessageList.size();
        m_nSystemMessageList.removeAt(i);
        qDebug() << "system uuid" << uuid << "remove success.";
        return true;
    }else {
        int i = 0;
        for(; i < m_nBusinessMessageList.size(); i++){
            if(m_nBusinessMessageList.at(i).uuid == uuid){
                break;
            }
        }

        if(i >= m_nBusinessMessageList.size()){
            qDebug() << "business uuid " << uuid << "not exist.";
            return false;
        }
        //qDebug() << "m_nBusinessMessageList.removeAt   i is  " << i << "  size is " << m_nBusinessMessageList.size();
        m_nBusinessMessageList.removeAt(i);
        //qDebug() << "business uuid" << uuid << "remove success.";
        return true;
    }

}
int DataStorage::getMessageCount(const QString &messageType){
    if(messageType == "1")
        return m_nSystemMessageList.size();
    else
        return m_nBusinessMessageList.size();
}

bool DataStorage::setMessageStatus(const QString &messageType, const QString uuid, int status){
    int i = 0;
    if(messageType == "1"){
        for(i=0; i<m_nSystemMessageList.size(); i++) {
            if(m_nSystemMessageList.at(i).uuid == uuid){
                break;
            }
        }

        if(i == m_nSystemMessageList.size() || (status != 1 && status != 0)){
            qDebug() << "setMessageStatus: uuid para failed.";
            return false;
        }
    }else{
        for(i=0; i<m_nBusinessMessageList.size(); i++) {
            if(m_nBusinessMessageList.at(i).uuid == uuid){
                break;
            }
        }

        if(i == m_nBusinessMessageList.size() || (status != 1 && status != 0)){
            qDebug() << "setMessageStatus: uuid para failed.";
            return false;
        }
    }


    QSqlQuery query;
    QString sql = QString("UPDATE %1 SET %2=%3 WHERE %4='%5';")
            .arg(MESSAGE_TABLE_NAME)
            .arg(MESSAGE_READ_STATUS)
            .arg(status)
            .arg(MESSAGE_ID)
            .arg(uuid);
    qDebug() << "setMessageStatus SQL:" << sql;

    if(!query.exec(sql)){
        qDebug() << "setMessageStatus exec sql failed.";
        return false;
    }
    if(messageType == "1"){
        Message msg;
        msg = m_nSystemMessageList.takeAt(i);
        msg.status = status;
        m_nSystemMessageList.insert(i,msg);
    }else {
        Message msg;
        msg = m_nBusinessMessageList.takeAt(i);
        msg.status = status;
        m_nBusinessMessageList.insert(i,msg);
    }
    return true;
}

bool DataStorage::removeMessageNotifyByKey(const QString &notifyKey){
    bool ret = m_nNotificationManager.removeOneNotification(notifyKey);
    return ret;
}

bool DataStorage::removeAllMessageNotify(const QString &sopId){
    bool ret = m_nNotificationManager.removeAllNotifications(sopId);
    return ret;
}

//升序排列
static bool cmp(const QVariant &s1, const QVariant &s2){
    return s1.toMap()["PublishDate"].toString() < s2.toMap()["PublishDate"].toString();
}

void DataStorage::recvMessage(QVariant mapList, bool isRecved){
    QList <QVariant> list = mapList.toList();
    qDebug() <<"DataStorage::recv Message  " << mapList.toList().length();
    qSort(list.begin(), list.end(), cmp);
    qDebug() << "recv Message: isRecved:" << isRecved;
//    for(int i = 0; i < list.size(); i++)
//        qDebug() << "DataStorage::recvMessage-----------   " << list.at(i).toMap()["PublishDate"].toString();
    if(isRecved == false){
        //未收到消息
        emit sendUpdateFlage(-1);
        return;
    }
    int newMsgCount = updateMessage(list);
    qDebug() << "recv Message: newMsgCount = " << newMsgCount;
    emit sendUpdateFlage(newMsgCount);
}
