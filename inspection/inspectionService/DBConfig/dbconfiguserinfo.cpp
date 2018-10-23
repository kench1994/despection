#include "dbconfiguserinfo.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>

DBConfigUserInfo::DBConfigUserInfo()
{
    m_nDataOperationType = BUSINESS_TYPE_USER_INFO;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/userinfo.xml");
}

DBConfigUserInfo::~DBConfigUserInfo()
{
}

