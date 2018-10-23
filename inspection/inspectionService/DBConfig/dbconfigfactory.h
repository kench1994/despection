#ifndef DBCONFIGFACTORY_H
#define DBCONFIGFACTORY_H
#include "dbconfigbase.h"
#include "../common/dataoperationtype.h"
class DBconfigFactory
{
public:
    DBconfigFactory();
    ~DBconfigFactory();
   DBConfigBase *getDBConfig(const DataOperationType &type);
   QList<DBConfigBase *> getAllDBConfig();
private:
   DBConfigBase *m_pUserInfo;
   DBConfigBase *m_pDevice;
   DBConfigBase *m_pInspection;
   DBConfigBase *m_pDaily;
   DBConfigBase *m_pMaintenance;
   DBConfigBase *m_pStandardization;
   DBConfigBase *m_pInventory;
   DBConfigBase *m_pOperation;
   DBConfigBase *m_pDeviceReceive;
   DBConfigBase *m_pDeviceDeployment;
   DBConfigBase *m_pRectification;
   DBConfigBase *m_pMessage;
   DBConfigBase *m_pHomeInfo;
};

#endif // DBCONFIGFACTORY_H
