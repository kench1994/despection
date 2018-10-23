#include "runtimeparam.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include "systemconfig.h"
#include "utils.h"
#include <errno.h>
RuntimeParam * RuntimeParam::m_pSysParam = NULL;
RuntimeParam::RuntimeParam(QObject *parent):QObject(parent)
{
    m_nLogin = false;
}

RuntimeParam* RuntimeParam::getInstance(){
    if(m_pSysParam == NULL)
        m_pSysParam = new RuntimeParam();
    return m_pSysParam;
}

QQuickView* RuntimeParam::getViewer(){
    return m_pViewer;
}

void RuntimeParam::setViewer(QQuickView *viewer){
    m_pViewer = viewer;
}

QString RuntimeParam::getPadId(){
    if(m_nImei.isEmpty()){
        CSystemDeviceManager sysDeviceMng;
        m_nImei = sysDeviceMng.imei(0);
        if(m_nImei.isEmpty())
            m_nImei = "1111111111";
    }
    return m_nImei;
}

void RuntimeParam::setUserValueMap(const QVariantMap &userMap)
{
    m_nUserMap = userMap;
}

QVariantMap RuntimeParam::getUserMap(){
    return m_nUserMap;
}

void RuntimeParam::setLoginUserMsg(const QMap<QString, QString> &userMsg)
{
    qDebug() <<"------RuntimeParam::setLoginUserMsg---------userMsg:" << userMsg;
    m_nLoginUserMsg = userMsg;
}

QMap<QString, QString> RuntimeParam::getLoginUserMsg() const
{
    return m_nLoginUserMsg;
}


QString RuntimeParam::getLoginUserMsgByKey(const QString &key) const
{
    if(m_nLoginUserMsg.contains(key))
        return m_nLoginUserMsg[key];
    return "";
}

QString RuntimeParam::getUserId()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["UserID"].toString();
    return "";
}

QString RuntimeParam::getUserCode()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["UserCode"].toString();
    return "";
}

QString RuntimeParam::getUserName()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["UserName"].toString();
    return "";
}

QString RuntimeParam::getOrganiseUnitIDs()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["OrganiseUnitIDs"].toString();
    return "";
}

QString RuntimeParam::getOrganiseUnitCodes()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["OrganiseUnitCodes"].toString();
    return "";
}

QString RuntimeParam::getOrganiseUnitNames(){
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["OrganiseUnitNames"].toString();
    return "";
}

QString RuntimeParam::getDepartmentIDs()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["DepartmentIDs"].toString();
    return "";
}

QString RuntimeParam::getDepartmentCodes()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["DepartmentCodes"].toString();
    return "";
}

QString RuntimeParam::getDepartmentNames()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["DepartmentNames"].toString();
    return "";
}

QString RuntimeParam::getUserProperty(const QString &key){
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()[key].toString();
    return "";
}

void RuntimeParam::setLoginStatus(bool login)
{
    m_nLogin = login;
}

bool RuntimeParam::getLoginStatus() const
{
    return m_nLogin;
}

void RuntimeParam::setUserPermission(const QString& module, bool value)
{
    m_nUserPermissions[module] = value;
}

bool RuntimeParam::checkUserPermission(const QString& module)
{
    bool ret = m_nUserPermissions.value(module, false);
    qDebug() << "checkUserPermission : " << module << ":" << ret;
    return ret;
}

void RuntimeParam::setModuleID(const QString& module, const QString& id)
{
    m_nModuleID[module] = id;
}

void RuntimeParam::setUserPermissionData(const QString &module, const QVariantMap &dataMap){
    m_nModuleData[module] = dataMap;
}

QString RuntimeParam::getUserPermissionData(const QString &module, const QString &key){
    QVariantMap map = m_nModuleData[module];
    if(map.isEmpty())
        return "";
    else
        return map[key].toString();
}

void RuntimeParam::clearUserPermission()
{
    m_nUserPermissions.clear();
}


QString RuntimeParam::getModuleID(const QString& module)
{
    QString ret = m_nModuleID.value(module, QString(""));
    return ret;
}

bool RuntimeParam::needCreateDB()
{
    return SystemConfig::getInstance()->needCteateDB();
}

bool RuntimeParam::setHistoryUser(const QStringList& userList){

    QString userFullPath = SystemConfig::getInstance()->getUserFilePath();
    if(userFullPath == "") {
        userFullPath = USER_HISTORY_CACHE;
        SystemConfig::getInstance()->setUsersFilePath(userFullPath);
    }
    QFile data(userFullPath);
    if (data.open(QFile::WriteOnly)){
        QTextStream out(&data);
        for(int i = 0; i < userList.size(); i++){
            QString tmp = userList.at(i);
            if(tmp.right(1) != "\n"){
                tmp += "\n";
            }
            out << tmp;
        }
        out.flush();
        data.close();
        sync();
    }else{
        return false;
    }
    return true;
}

QStringList RuntimeParam::getHistoryUser(){
    QStringList list;
    QString userFullPath = SystemConfig::getInstance()->getUserFilePath();
    if(userFullPath == "") {
        userFullPath = USER_HISTORY_CACHE;
        SystemConfig::getInstance()->setUsersFilePath(userFullPath);
    }
    QFile data(userFullPath);
    if (data.open(QFile::ReadOnly)) {
        QTextStream out(&data);
        QString tmp;
        do {
            tmp = out.readLine();
            if(!tmp.isEmpty() && tmp != "\n"){
                list.append(tmp);
            }
        } while (!tmp.isNull());
        data.close();
        sync();
    }else{
        qDebug() << "data.open error " << strerror(errno);
    }
    return list;
}

void RuntimeParam::removeUserHistory(){
    QString userFullPath = SystemConfig::getInstance()->getUserFilePath();
    if(userFullPath == "") {
        userFullPath = USER_HISTORY_CACHE;
        SystemConfig::getInstance()->setUsersFilePath(userFullPath);
    }
    QFile data(userFullPath);
    data.remove();
}

QString RuntimeParam::getAppId()
{
    return SystemConfig::getInstance()->getAppId();
}

void RuntimeParam::setOfflineModel(const QString &offlineModel)
{
    m_nOfflineModel = offlineModel;
}

QString RuntimeParam::getOfflineModel()
{
    if(m_nOfflineModel.isEmpty())
        m_nOfflineModel = SystemConfig::getInstance()->getOfflineModel();
    return m_nOfflineModel;
}
