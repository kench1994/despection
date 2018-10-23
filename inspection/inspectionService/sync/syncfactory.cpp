#include "syncfactory.h"
#include "devicedeploymentsynchronizer.h"
#include "devicedistributionsynchronizer.h"
#include "devicesynchronizer.h"
#include "examinesynchronizer.h"
#include "homesynchronizer.h"
#include "inspectionsynchronizer.h"
#include "inventorysynchronizer.h"
#include "maintenancesynchronizer.h"
#include "operationsynchronizer.h"
#include "examinesynchronizer.h"
#include "userinfosynchronizer.h"
#include "dailysynchronizer.h"
#include "rectificationsynchronizer.h"
SyncFactory::SyncFactory()
{
    m_pDeviceDeployment = new DeviceDeploymentSynchronizer();
     m_pDevice = new DeviceSynchronizer();
    m_pDeviceReceive = new DeviceDistributionSynchronizer();
    m_pInspection = new InspectionSynchronizer();
    m_pDaily = new DailySynchronizer();
    m_pInventory = new InventorySynchronizer();
    m_pOperation = new OperationSynchronizer();
    m_pExamine = new ExamineSynchronizer();
    m_pUserInfo = new UserInfoSynchronizer();
    m_pMaintenance = new MaintenanceSynchronizer();
    m_pRectification = new RectificationSynchronizer();
}

SyncFactory::~SyncFactory()
{

}

SynchronizerBase *SyncFactory::getSynchronizer(DataOperationType operationType)
{
    SynchronizerBase *synchronizerBase = NULL;
    switch (operationType) {
    case BUSINESS_TYPE_USER_INFO:
        synchronizerBase =  m_pUserInfo;
        break;
    case BUSINESS_TYPE_DEVICES:
        synchronizerBase =   m_pDevice;
        break;
    case BUSINESS_TYPE_INSPECTION:
        synchronizerBase =   m_pInspection;
        break;
    case BUSINESS_TYPE_DAILY:
        synchronizerBase =   m_pDaily;
        break;
    case BUSINESS_TYPE_MAINTENANCE:
        synchronizerBase =   m_pMaintenance;
        break;
    case BUSINESS_TYPE_STANDARDIZATION:
        synchronizerBase =   m_pExamine;
        break;
    case BUSINESS_TYPE_INVENTORY:
        synchronizerBase =  m_pInventory;
        break;
    case BUSINESS_TYPE_OPERATION:
        synchronizerBase = m_pOperation;
        break;
    case BUSINESS_TYPE_DEVICE_RECEIVE:
        synchronizerBase = m_pDeviceReceive;
        break;
    case BUSINESS_TYPE_DEVICE_DEPLOYMENT:
        synchronizerBase = m_pDeviceDeployment;
        break;
    case BUSINESS_TYPE_RECTIFICATION:
        synchronizerBase = m_pRectification;
        break;
    default:
        break;
    }
    return synchronizerBase;
}

QList<SynchronizerBase *> SyncFactory::getAllSynchronizer()
{
    QList<SynchronizerBase * >  synchronizerBaseList;
    synchronizerBaseList.append(m_pUserInfo);
    synchronizerBaseList.append(m_pDevice);
    synchronizerBaseList.append(m_pInspection);
    synchronizerBaseList.append(m_pDaily);
    synchronizerBaseList.append(m_pMaintenance);
    synchronizerBaseList.append(m_pExamine);
    synchronizerBaseList.append(m_pInventory);
    synchronizerBaseList.append(m_pOperation);
    synchronizerBaseList.append(m_pDeviceReceive);
    synchronizerBaseList.append(m_pDeviceDeployment);
    synchronizerBaseList.append(m_pRectification);
    return synchronizerBaseList;
}

