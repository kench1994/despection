#include "localcachefactory.h"
#include "datalocalcachebase.h"
LocalCacheFactory::LocalCacheFactory()
{
    m_pDevice = new DeviceDataLocalCache();
    m_pDeviceDeployment = new DeviceDeploymentDataLocalCache();
    m_pDeviceReceive = new DeviceReceiveDataLocalCache();
    m_pInspection = new InspectionDataLocalCache();
    m_pDaily = new DailyLocalCache();
    m_pInventory = new InventoryDataLocalCache();
    m_pOperation = new OperationDataLocalCache();
    m_pStandardization = new StandardizationDataLocalCache();
    m_pUserInfo = new UserInfoDataLocalCache();
    m_pMaintenance = new MaintenanceDataLocalCache();
    m_pRectification = new RectificationLocalCache();
    m_pMessage = new MessageLocalCache();
}

LocalCacheFactory::~LocalCacheFactory()
{
    delete m_pDevice;
    delete m_pDeviceDeployment;
    delete m_pDeviceReceive;
    delete m_pInspection;
    delete m_pDaily;
    delete m_pInventory;
    delete m_pOperation;
    delete m_pStandardization;
    delete m_pUserInfo;
    delete m_pMaintenance;
    delete m_pRectification;
    delete m_pMessage;
}

DataLocalCacheBase * LocalCacheFactory::getLocalCache(DataOperationType operationType){
    DataLocalCacheBase *localCacheBase = NULL;
    switch (operationType) {
    case BUSINESS_TYPE_USER_INFO:
        localCacheBase =  m_pUserInfo;
        break;
    case BUSINESS_TYPE_DEVICES:
        localCacheBase =   m_pDevice;
        break;
    case BUSINESS_TYPE_INSPECTION:
        localCacheBase =   m_pInspection;
        break;
    case BUSINESS_TYPE_DAILY:
        localCacheBase =   m_pDaily;
        break;
    case BUSINESS_TYPE_MAINTENANCE:
        localCacheBase =   m_pMaintenance;
        break;
    case BUSINESS_TYPE_STANDARDIZATION:
        localCacheBase =   m_pStandardization;
        break;
    case BUSINESS_TYPE_INVENTORY:
        localCacheBase =  m_pInventory;
        break;
    case BUSINESS_TYPE_OPERATION:
        localCacheBase = m_pOperation;
        break;
    case BUSINESS_TYPE_DEVICE_RECEIVE:
        localCacheBase = m_pDeviceReceive;
        break;
    case BUSINESS_TYPE_DEVICE_DEPLOYMENT:
        localCacheBase = m_pDeviceDeployment;
        break;
    case BUSINESS_TYPE_RECTIFICATION:
        localCacheBase = m_pRectification;
        break;
    case BUSINESS_TYPE_MESSAGE:
        localCacheBase = m_pMessage;
        break;
    default:
        break;
    }
    return localCacheBase;
}

QList<DataLocalCacheBase *> LocalCacheFactory::getAllLocalCache(){
    QList<DataLocalCacheBase * >  localCacheBaseList;
    localCacheBaseList.append(m_pUserInfo);
    localCacheBaseList.append(m_pDevice);
    localCacheBaseList.append(m_pInspection);
    localCacheBaseList.append(m_pDaily);
    localCacheBaseList.append(m_pMaintenance);
    localCacheBaseList.append(m_pStandardization);
    localCacheBaseList.append(m_pInventory);
    localCacheBaseList.append(m_pOperation);
    localCacheBaseList.append(m_pDeviceReceive);
    localCacheBaseList.append(m_pDeviceDeployment);
    localCacheBaseList.append(m_pRectification);
    localCacheBaseList.append(m_pMessage);
    return localCacheBaseList;
}
