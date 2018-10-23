#include "dbconfigfactory.h"
#include "dbconfiguserinfo.h"
#include "dbconfigdevice.h"
#include "dbconfiginspection.h"
#include "dbconfigmaintenance.h"
#include "dbconfigstandardization.h"
#include "dbconfiginventory.h"
#include "dbconfigoperation.h"
#include "dbconfigdevicereceive.h"
#include "dbconfigdevicedeployment.h"
#include "dbconfigrectification.h"
#include "dbconfigmessage.h"
#include "dbconfighomeinfo.h"
#include "dbconfigdaily.h"
DBconfigFactory::DBconfigFactory()
{
    m_pUserInfo = new DBConfigUserInfo();
    m_pDevice = new DBConfigDevice();
    m_pInspection = new DBConfigInspection();
    m_pDaily = new DBConfigDaily();
    m_pMaintenance = new DBConfigMaintenance();
    m_pStandardization = new DBConfigStandardization();
    m_pInventory = new DBConfigInventory();
    m_pOperation = new DBConfigOperation();
    m_pDeviceReceive = new DBConfigDeviceReceive();
    m_pDeviceDeployment = new DBConfigDeviceDeployment();
    m_pRectification = new DBConfigRectification();
    m_pMessage = new DBConfigMessage();
    m_pHomeInfo = new DBConfigHomeInfo();
}
DBconfigFactory::~DBconfigFactory()
{
    delete m_pUserInfo;
    delete m_pDevice;
    delete m_pInspection;
    delete m_pDaily;
    delete m_pMaintenance;
    delete m_pStandardization;
    delete m_pInventory;
    delete m_pOperation;
    delete m_pDeviceReceive;
    delete m_pDeviceDeployment;
    delete m_pRectification;
    delete m_pMessage;
    delete m_pHomeInfo;
}

DBConfigBase *DBconfigFactory::getDBConfig(const DataOperationType &type){
    DBConfigBase *dbBase = NULL;
    switch (type) {
    case BUSINESS_TYPE_USER_INFO:
        dbBase =  m_pUserInfo;
        break;
    case BUSINESS_TYPE_DEVICES:
        dbBase =   m_pDevice;
        break;
    case BUSINESS_TYPE_INSPECTION:
        dbBase =   m_pInspection;
        break;
    case BUSINESS_TYPE_DAILY:
        dbBase =   m_pDaily;
        break;
    case BUSINESS_TYPE_MAINTENANCE:
        dbBase =   m_pMaintenance;
        break;
    case BUSINESS_TYPE_STANDARDIZATION:
        dbBase =   m_pStandardization;
        break;
    case BUSINESS_TYPE_INVENTORY:
        dbBase =  m_pInventory;
        break;
    case BUSINESS_TYPE_OPERATION:
        dbBase = m_pOperation;
        break;
    case BUSINESS_TYPE_DEVICE_RECEIVE:
        dbBase = m_pDeviceReceive;
        break;
    case BUSINESS_TYPE_DEVICE_DEPLOYMENT:
        dbBase = m_pDeviceDeployment;
        break;
    case BUSINESS_TYPE_RECTIFICATION:
        dbBase = m_pRectification;
        break;
    case BUSINESS_TYPE_MESSAGE:
        dbBase =  m_pMessage;
        break;
    case BUSINESS_TYPE_HOME:
        dbBase =  m_pHomeInfo;
        break;
    default:
        break;
    }
    return dbBase;
}

QList<DBConfigBase * > DBconfigFactory::getAllDBConfig(){
    QList<DBConfigBase * >  configBaseList;
    configBaseList.append(m_pUserInfo);
    configBaseList.append(m_pDevice);
    configBaseList.append(m_pInspection);
    configBaseList.append(m_pDaily);
    configBaseList.append(m_pMaintenance);
    configBaseList.append(m_pStandardization);
    configBaseList.append(m_pInventory);
    configBaseList.append(m_pOperation);
    configBaseList.append(m_pDeviceReceive);
    configBaseList.append(m_pDeviceDeployment);
    configBaseList.append(m_pRectification);
    configBaseList.append(m_pMessage);
    configBaseList.append(m_pHomeInfo);
    return configBaseList;
}
