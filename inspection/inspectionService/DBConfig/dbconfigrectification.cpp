#include "dbconfigrectification.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
DBConfigRectification::DBConfigRectification()
{
    m_nDataOperationType = BUSINESS_TYPE_RECTIFICATION;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/rectification_table_info.xml");
}

DBConfigRectification::~DBConfigRectification()
{

}
