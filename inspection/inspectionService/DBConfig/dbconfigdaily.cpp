#include "dbconfigdaily.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
QString DBConfigDaily::TABLE_DAILYSUBMIT = "ISM_Plan_ImportDetail";
QString DBConfigDaily::TABLE_DAILYSUBMIT_BINARY = "ISM_Plan_Binary_ImportDetail";
DBConfigDaily::DBConfigDaily()
{
    m_nDataOperationType = BUSINESS_TYPE_DAILY;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/daily_table_info.xml");
}
DBConfigDaily::~DBConfigDaily()
{
}

