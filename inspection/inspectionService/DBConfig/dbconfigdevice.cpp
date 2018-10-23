#include "dbconfigdevice.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
DBConfigDevice::DBConfigDevice()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICES;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/device_table_info.xml");
}

DBConfigDevice::~DBConfigDevice()
{

}

