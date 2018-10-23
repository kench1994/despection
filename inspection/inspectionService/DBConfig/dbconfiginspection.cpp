#include "dbconfiginspection.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
QString DBConfigInspection::TABLE_INSPECTIONSUBMIT = "ISM_Inspection_ImportDetail";
QString DBConfigInspection::TABLE_INSPECTIONSUBMIT_BINARY = "ISM_Inspection_Binary_ImportDetail";
DBConfigInspection::DBConfigInspection()
{
    m_nDataOperationType = BUSINESS_TYPE_INSPECTION;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/inspection_table_info.xml");
}

DBConfigInspection::~DBConfigInspection()
{

}

