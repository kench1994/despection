#include "dbconfighomeinfo.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>

DBConfigHomeInfo::DBConfigHomeInfo()
{
    m_nDataOperationType = BUSINESS_TYPE_HOME;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/homeinfo.xml");
}

DBConfigHomeInfo::~DBConfigHomeInfo()
{
}

