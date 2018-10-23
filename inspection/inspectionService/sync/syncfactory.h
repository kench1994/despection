#ifndef SYNCFACTORY_H
#define SYNCFACTORY_H
#include "synchronizerbase.h"

class SyncFactory
{
public:
    SyncFactory();
    ~SyncFactory();
    SynchronizerBase * getSynchronizer(DataOperationType operationType);
    QList<SynchronizerBase *> getAllSynchronizer();
 private:
     SynchronizerBase *m_pDevice;
     SynchronizerBase *m_pDeviceDeployment;
     SynchronizerBase *m_pDeviceReceive;
     SynchronizerBase *m_pInspection;
     SynchronizerBase *m_pInventory;
     SynchronizerBase *m_pOperation;
     SynchronizerBase *m_pExamine;
     SynchronizerBase *m_pUserInfo;
     SynchronizerBase *m_pMaintenance;
     SynchronizerBase *m_pDaily;
     SynchronizerBase *m_pRectification;
};

#endif // SYNCFACTORY_H
