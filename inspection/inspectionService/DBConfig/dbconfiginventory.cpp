#include "dbconfiginventory.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
QString DBConfigInventory::TABLE_INVENTORYSUBMIT = "EMI_Inventory_ImportDetail";
QString DBConfigInventory::TABLE_INVENTORYSUBMIT_BINARY = "EMI_Inventory_Binary_ImportDetail";
DBConfigInventory::DBConfigInventory()
{
    m_nDataOperationType = BUSINESS_TYPE_INVENTORY;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/inventory_table_info.xml");
}

DBConfigInventory::~DBConfigInventory()
{

}

