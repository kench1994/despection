#include "dbconfigoperation.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
QString DBConfigOperation::TABLE_FAULTREPORTSUBMIT = "EMF_FaultRepair_ImportDetail";
QString DBConfigOperation::TABLE_FAULTREPORTSUBMIT_BINARY = "EMF_FaultRepair_binary_ImportDetail";
DBConfigOperation::DBConfigOperation()
{
    m_nDataOperationType = BUSINESS_TYPE_OPERATION;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/operation_table_info.xml");
}

DBConfigOperation::~DBConfigOperation()
{

}

