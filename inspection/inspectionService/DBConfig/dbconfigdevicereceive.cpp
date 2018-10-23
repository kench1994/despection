#include "dbconfigdevicereceive.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
DBConfigDeviceReceive::DBConfigDeviceReceive()
{
    m_nDataOperationType = BUSINESS_TYPE_DEVICE_RECEIVE;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/devicereceive_table_info.xml");
}

DBConfigDeviceReceive::~DBConfigDeviceReceive()
{

}

