#include <QFile>
#include <QSettings>

#include "upgradeconfig.h"


UpgradeConfig * UpgradeConfig::m_pUpgradeConf = NULL;

UpgradeConfig::UpgradeConfig(const QString &configFullPath)
{
    m_pSetting = new QSettings(configFullPath, QSettings::IniFormat);
}

UpgradeConfig* UpgradeConfig::getInstance(const QString & configFullPath)
{
    if(QFile::exists(configFullPath) == false)
    {
        return NULL;
    }
    if(m_pUpgradeConf == NULL)
        m_pUpgradeConf = new UpgradeConfig(configFullPath);
    return m_pUpgradeConf;
}

QString UpgradeConfig::getAPPName()
{
    return m_pSetting->value("app/name","").toString();
}

QString UpgradeConfig::getVersion()
{
    return m_pSetting->value("common/version","").toString();
}



QString UpgradeConfig::getChangeLogFullPath()
{
    return m_pSetting->value("common/changeLogFile","").toString();
}

QString UpgradeConfig::getMd5Code()
{
    return m_pSetting->value("common/md5","").toString();
}

QString UpgradeConfig::getPackageType()
{
    return m_pSetting->value("common/packageType","").toString();
}

QString UpgradeConfig::getPriorVersion()
{
    return m_pSetting->value("app/priorVersion","").toString();
}

QString UpgradeConfig::isClearAppData()
{
    return m_pSetting->value("app/clearData","").toString();
}






