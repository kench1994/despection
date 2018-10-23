#include "dbconfigstandardization.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
DBConfigStandardization::DBConfigStandardization()
{
    m_nDataOperationType = BUSINESS_TYPE_STANDARDIZATION;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/standardization_table_info.xml");
}

DBConfigStandardization::~DBConfigStandardization()
{

}

