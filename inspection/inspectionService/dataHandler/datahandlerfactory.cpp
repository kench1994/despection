#include "datahandlerfactory.h"
#include "devicedeploymenthandler.h"
#include "devicehandler.h"
#include "devicereceivehandler.h"
#include "inspectionhandler.h"
#include "inventoryhandler.h"
#include "maintenancehandler.h"
#include "operationhandler.h"
#include "standardizationhandler.h"
#include "userinfohandler.h"
#include "messagehandler.h"
#include "updatehandler.h"
#include "homehandler.h"
#include "rectificationhandler.h"
#include "dailytaskhandler.h"
#include "scantaskhandler.h"
#include "knowledge.h"
DataHandlerFactory::DataHandlerFactory()
{
    m_pDeviceDeployment = new DeviceDeploymentHandler();
    m_pDevice = new DeviceHandler();
    m_pDeviceReceive = new DeviceReceiveHandler();
    m_pInspection = new InspectionHandler();
    m_pDaily = new DailyTaskHandler();
    m_pInventory = new InventoryHandler();
    m_pOperation = new OperationHandler();
    m_pStandardization = new StandardizationHandler();
    m_pUserInfo = new UserinfoHandler();
    m_pMaintenance = new MaintenanceHandler();
    m_pMessage = new MessageHandler();
    m_pUpdate = new UpdateHandler();
    m_pHome = new HomeHandler();
    m_pRectification = new RectificationHandler();
    m_pScanTask = new ScanTaskHandler();
    m_pKnowledge = new KnowLedge();

}

DataHandlerFactory::~DataHandlerFactory()
{
    delete m_pDeviceDeployment;
    delete m_pDevice;
    delete m_pDeviceReceive;
    delete m_pInspection;
    delete m_pDaily;
    delete m_pInventory;
    delete m_pOperation;
    delete m_pStandardization;
    delete m_pUserInfo;
    delete m_pMaintenance;
    delete m_pMessage;
    delete m_pUpdate;
    delete m_pHome;
    delete m_pRectification;
    delete m_pScanTask;
    delete m_pKnowledge;
}

DataHandlerBase *DataHandlerFactory::getDataHandler(DataOperationType operationType)
{
    switch (operationType) {
    case BUSINESS_TYPE_USER_INFO:
        return m_pUserInfo;
    case BUSINESS_TYPE_DEVICES:
        return m_pDevice;
    case BUSINESS_TYPE_INSPECTION:
        return m_pInspection;
    case BUSINESS_TYPE_DAILY:
        return m_pDaily;
    case BUSINESS_TYPE_MAINTENANCE:
        return m_pMaintenance;
    case BUSINESS_TYPE_STANDARDIZATION:
        return m_pStandardization;
    case BUSINESS_TYPE_INVENTORY:
        return m_pInventory;
    case BUSINESS_TYPE_OPERATION:
        return m_pOperation;
    case BUSINESS_TYPE_DEVICE_RECEIVE:
        return m_pDeviceReceive;
    case BUSINESS_TYPE_DEVICE_DEPLOYMENT:
        return m_pDeviceDeployment;
    case BUSINESS_TYPE_MESSAGE:
        return m_pMessage;
    case BUSINESS_TYPE_UPDATE:
        return m_pUpdate;
    case BUSINESS_TYPE_HOME:
        return m_pHome;
    case BUSINESS_TYPE_RECTIFICATION:
        return m_pRectification;
    case BUSINESS_TYPE_SCAN:
        return m_pScanTask;
    case BUSINESS_TYPE_KNOWLEDGE:
        return m_pKnowledge;
    default:
        return NULL;
    }
    return NULL;
}

QList<DataHandlerBase *> DataHandlerFactory::getAllDataHandler()
{
    QList<DataHandlerBase * >  dataHandlerBaseList;
    dataHandlerBaseList.append(m_pUserInfo);
    dataHandlerBaseList.append(m_pDevice);
    dataHandlerBaseList.append(m_pInspection);
    dataHandlerBaseList.append(m_pDaily);
    dataHandlerBaseList.append(m_pMaintenance);
    dataHandlerBaseList.append(m_pStandardization);
    dataHandlerBaseList.append(m_pInventory);
    dataHandlerBaseList.append(m_pOperation);
    dataHandlerBaseList.append(m_pDeviceReceive);
    dataHandlerBaseList.append(m_pDeviceDeployment);
    dataHandlerBaseList.append(m_pMessage);
    dataHandlerBaseList.append(m_pUpdate);
    dataHandlerBaseList.append(m_pHome);
    dataHandlerBaseList.append(m_pRectification);
    dataHandlerBaseList.append(m_pScanTask);
    dataHandlerBaseList.append(m_pKnowledge);
    return dataHandlerBaseList;
}

