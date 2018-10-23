#include "dbconfigdevicedeployment.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
DBConfigDeviceDeployment::DBConfigDeviceDeployment()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICE_DEPLOYMENT;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/device_deployment.xml");
}

DBConfigDeviceDeployment::~DBConfigDeviceDeployment()
{
}

