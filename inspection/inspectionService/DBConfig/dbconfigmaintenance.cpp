#include "dbconfigmaintenance.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
DBConfigMaintenance::DBConfigMaintenance()
{
    m_nDataOperationType = BUSINESS_TYPE_MAINTENANCE;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/maintenance_table_info.xml");
}

DBConfigMaintenance::~DBConfigMaintenance()
{

}

