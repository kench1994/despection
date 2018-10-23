#ifndef DATAHANDLERFACTORY_H
#define DATAHANDLERFACTORY_H
#include "datahandlerbase.h"
#include "../common/dataoperationtype.h"
class DataHandlerFactory
{
public:
    DataHandlerFactory();
    ~DataHandlerFactory();
    DataHandlerBase * getDataHandler(DataOperationType operationType);
    QList<DataHandlerBase *> getAllDataHandler();
 private:
     DataHandlerBase *m_pDevice;
     DataHandlerBase *m_pDeviceDeployment;
     DataHandlerBase *m_pDeviceReceive;
     DataHandlerBase *m_pInspection;
     DataHandlerBase *m_pDaily;
     DataHandlerBase *m_pInventory;
     DataHandlerBase *m_pOperation;
     DataHandlerBase *m_pStandardization;
     DataHandlerBase *m_pUserInfo;
     DataHandlerBase *m_pMaintenance;
     DataHandlerBase *m_pMessage;
     DataHandlerBase *m_pUpdate;
     DataHandlerBase *m_pHome;
     DataHandlerBase *m_pRectification;
     DataHandlerBase *m_pScanTask;
     DataHandlerBase *m_pKnowledge;
};

#endif // DATAHANDLERFACTORY_H
