#ifndef LOCALCACHEFACTORY_H
#define LOCALCACHEFACTORY_H
#include <QList>
#include "../common/dataoperationtype.h"
#include "devicedatalocalcache.h"
#include "devicedeploymentdatalocalcache.h"
#include "devicereceivedatalocalcache.h"
#include "inspectiondatalocalcache.h"
#include "inventorydatalocalcache.h"
#include "operationdatalocalcache.h"
#include "standardizationdatalocalcache.h"
#include "userinfodatalocalcache.h"
#include "maintenancedatalocalcache.h"
#include "rectificationlocalcache.h"
#include "messagelocalcache.h"
#include "dailylocalcache.h"
class LocalCacheFactory
{
public:
    LocalCacheFactory();
    ~LocalCacheFactory();
   DataLocalCacheBase * getLocalCache(DataOperationType operationType);
   QList<DataLocalCacheBase *> getAllLocalCache();
private:
    DataLocalCacheBase *m_pDevice;
    DataLocalCacheBase *m_pDeviceDeployment;
    DataLocalCacheBase *m_pDeviceReceive;
    DataLocalCacheBase *m_pInspection;
    DataLocalCacheBase *m_pDaily;
    DataLocalCacheBase *m_pInventory;
    DataLocalCacheBase *m_pOperation;
    DataLocalCacheBase *m_pStandardization;
    DataLocalCacheBase *m_pUserInfo;
    DataLocalCacheBase *m_pMaintenance;
    DataLocalCacheBase *m_pRectification;
    DataLocalCacheBase *m_pMessage;
};

#endif // LOCALCACHEFACTORY_H
