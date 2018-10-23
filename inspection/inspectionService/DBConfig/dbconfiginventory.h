#ifndef DBCONFIGINVENTORY_H
#define DBCONFIGINVENTORY_H

#include "dbconfigbase.h"
class DBConfigInventory:public DBConfigBase
{
public:
    DBConfigInventory();
    ~DBConfigInventory();
    static QString TABLE_INVENTORYSUBMIT;
    static QString TABLE_INVENTORYSUBMIT_BINARY;
};

#endif // DBCONFIGINVENTORY_H
