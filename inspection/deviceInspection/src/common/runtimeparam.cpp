#include "runtimeparam.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include "../../../framework/common/utils.h"
#include <errno.h>
#include <csystemdevicemanager.h>
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

void RuntimeParam::setLoginUserInfo(const QVariantMap &userMap)
{
    m_nUserMap = userMap;
    m_nLogin = true;
}

QVariantMap RuntimeParam::getUserMap(){
    return m_nUserMap;
}

QString RuntimeParam::getUserId()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["UserID"].toString();
    return "";
}

QString RuntimeParam::getOrganiseUnitIDs()
{
    if(!m_nUserMap.empty())
        return m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()["OrganiseUnitIDs"].toString();
    return "";
}

QString RuntimeParam::getUserPropertyByKey(const QString &key){
    QString value;
    if(!m_nUserMap.empty()){
        value =  m_nUserMap["tables"].toList().at(0).toMap()["value"].toList().at(0).toMap()[key].toString();
    }
    qDebug() << Q_FUNC_INFO << " key " << key << "  value is " << value;
    return value;
}

bool RuntimeParam::getLoginStatus() const
{
    return m_nLogin;
}

bool RuntimeParam::checkUserPermission(const QString& module)
{
    const QList < QVariant > &userPermission = m_nUserMap["tables"].toList().at(2).toMap()["value"].toList();
    bool permission = false;
    for(int i = 0; i < userPermission.size(); i++){
        if(userPermission[i].toMap()["ShortName"] == module){
            permission = true;
            break;
        }
    }
    return permission;
}

QString RuntimeParam::getUserPermissionData(const QString &module, const QString &key){
    const QList < QVariant > &userPermission = m_nUserMap["tables"].toList().at(2).toMap()["value"].toList();
    for(int i = 0; i < userPermission.size(); i++){
        if(userPermission[i].toMap()["ShortName"] == module){
            return userPermission[i].toMap()[key].toString();
        }
    }
    return "";
}

void RuntimeParam::clearLoginUserInfo()
{
    m_nUserMap.clear();
    m_nLogin = false;
}
