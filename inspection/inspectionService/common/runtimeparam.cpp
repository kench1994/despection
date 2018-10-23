#include "runtimeparam.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include "../../framework/common/systemconfig.h"
#include "../../framework/common/utils.h"
#include <QSettings>
#include <errno.h>
RuntimeParam * RuntimeParam::m_pSysParam = NULL;
RuntimeParam::RuntimeParam()
{
    m_nLogin = false;
}

RuntimeParam* RuntimeParam::getInstance(){
    if(m_pSysParam == NULL)
        m_pSysParam = new RuntimeParam();
    return m_pSysParam;
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

void RuntimeParam::clearUserPermission()
{
    m_nUserPermissions.clear();
}

bool RuntimeParam::needCreateDB()
{
    //return SystemConfig::getInstance()->needCteateDB();
    bool createDb = false;
    QString appDataPath = SystemConfig::getInstance()->getAppDataPath();
    QString appInstallPath = SystemConfig::getInstance()->getAppInstallPath();
    if(!QFile::exists(appDataPath + "/" + SERVICE_CONFIG_FILE)){
        QFile::copy(appInstallPath + "/" + SERVICE_CONFIG_FILE, appDataPath + "/" + SERVICE_CONFIG_FILE);
        QFile::setPermissions(appDataPath + "/" + SERVICE_CONFIG_FILE,
                              QFileDevice::ReadOwner|QFileDevice::WriteOwner|QFileDevice::ExeOwner|
                              QFileDevice::ReadGroup|QFileDevice::WriteGroup|QFileDevice::ExeGroup);
       QSettings serviceSettings(appDataPath + "/" + SERVICE_CONFIG_FILE, QSettings::IniFormat);
       serviceSettings.setValue("dataBase/createDB", false);
       createDb = true;
    } else {
        QSettings installSettings(appInstallPath + "/" + SERVICE_CONFIG_FILE, QSettings::IniFormat);
        QSettings dataSettings(appDataPath + "/" + SERVICE_CONFIG_FILE, QSettings::IniFormat);
        QString installVersion = installSettings.value("dataBase/version", "").toString();
        QString dataVersion = dataSettings.value("dataBase/version", "").toString();
        bool installCreateDb = installSettings.value("dataBase/createDB", "").toBool();
        if(installVersion > dataVersion && installCreateDb){
            QFile::remove(appDataPath + "/" + SERVICE_CONFIG_FILE);
            QFile::copy(appInstallPath + "/" + SERVICE_CONFIG_FILE, appDataPath + "/" + SERVICE_CONFIG_FILE);
            QFile::setPermissions(appDataPath + "/" + SERVICE_CONFIG_FILE,
                                  QFileDevice::ReadOwner|QFileDevice::WriteOwner|QFileDevice::ExeOwner|
                                  QFileDevice::ReadGroup|QFileDevice::WriteGroup|QFileDevice::ExeGroup);
            QSettings newSettings(appDataPath + "/" + SERVICE_CONFIG_FILE, QSettings::IniFormat);
            newSettings.setValue("dataBase/createDB", false);
            createDb = true;
        }
    }
    qDebug() << Q_FUNC_INFO << createDb;
    return createDb;
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
