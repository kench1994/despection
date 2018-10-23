#include "datahandlermanager.h"
#include "datahandlerbase.h"
#include <QDebug>
#include "../../framework/network/networkmanager.h"
#include "../networkcontrol.h"
#include "../dataprovider.h"
DataHandlerManager* DataHandlerManager::m_pDataHandlerManager = NULL;
DataHandlerManager::DataHandlerManager()
{
    NetworkControl *networkControl = NetworkControl::getInstance();
    connect(networkControl, SIGNAL(sendNetworkResultSignal(QString,QString,bool,int,DbusPackage)), this, SLOT(sendNetworkResultSlot(QString,QString,bool,int,DbusPackage)));

    QList<DataHandlerBase * > dataHandlerBaseList = m_nDataHandlerFactory.getAllDataHandler();
    for(int i = 0; i < dataHandlerBaseList.size(); i++){
        DataHandlerBase *dataHandlerBase = dataHandlerBaseList.at(i);
        connect(dataHandlerBase, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
        connect(dataHandlerBase, SIGNAL(onBeginSyncSignal(BusinessMessageType,QString,QString,QVariantMap)),
                this, SIGNAL(onBeginSyncSignal(BusinessMessageType,QString,QString,QVariantMap)));
    }

    DataProvider *provider = DataProvider::getInstance();
    connect(this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), provider, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
    //进入各主功能模块之间的事件循环
    connect(this, SIGNAL(onBeginSyncSignal(BusinessMessageType,QString,QString,QVariantMap)),
            provider, SLOT(messageLoopFunction(BusinessMessageType,QString,QString,QVariantMap)), Qt::QueuedConnection);
}

DataHandlerManager *DataHandlerManager::getInstance()
{
    if(m_pDataHandlerManager == NULL)
        m_pDataHandlerManager = new DataHandlerManager();
    return m_pDataHandlerManager;
}

DataHandlerManager::~DataHandlerManager()
{
    NetworkControl *networkControl = NetworkControl::getInstance();
    disconnect(networkControl, SIGNAL(sendNetworkResultSignal(QString,QString,bool,int,DbusPackage)), this, SLOT(sendNetworkResultSlot(QString,QString,bool,int,DbusPackage)));

    QList<DataHandlerBase * > dataHandlerBaseList = m_nDataHandlerFactory.getAllDataHandler();
    for(int i = 0; i < dataHandlerBaseList.size(); i++){
        DataHandlerBase *dataHandlerBase = dataHandlerBaseList.at(i);
        disconnect(dataHandlerBase, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
        disconnect(dataHandlerBase, SIGNAL(onBeginSyncSignal(QString,QVariantMap)), this, SIGNAL(onBeginSyncSignal(QString,QVariantMap)));
    }

    DataProvider *provider = DataProvider::getInstance();
    disconnect(this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), provider, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
    disconnect(this, SIGNAL(onBeginSyncSignal(BusinessMessageType,QString,QVariantMap)), provider, SLOT(messageLoopFunction(BusinessMessageType,QString,QVariantMap)));
}

void DataHandlerManager::request(const DataPackage &dataPackage, const QString &action)
{
    DataOperationType type = BussinessMap::getInstance()->getBussinessType(dataPackage.head.objects, BussinessMap::MODULE_TYPE_HANDLER);
    DataHandlerBase *dataHandlerBase = m_nDataHandlerFactory.getDataHandler(type);
    qDebug() << Q_FUNC_INFO <<"  objects is " << dataPackage.head.objects << " type is " << type;
    dataHandlerBase->request(action, dataPackage);
}

void DataHandlerManager::requestTask(const DataPackage &dataPackage, const QString &action)
{
    DataOperationType type = BussinessMap::getInstance()->getBussinessType(dataPackage.head.objects, BussinessMap::MODULE_TYPE_HANDLER);
    DataHandlerBase *dataHandlerBase = m_nDataHandlerFactory.getDataHandler(type);
    dataHandlerBase->requestTask(action, dataPackage);
}

void DataHandlerManager::syncFinished(const QString &action, const QString &objects, const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO <<" action " << action << " objects " << objects;
    DataOperationType type = BussinessMap::getInstance()->getBussinessType(objects, BussinessMap::MODULE_TYPE_HANDLER);
    DataHandlerBase *dataHandlerBase = m_nDataHandlerFactory.getDataHandler(type);
    dataHandlerBase->syncFinished(action, objects, map);
}

void DataHandlerManager::sendNetworkResultSlot(QString objects, QString action, bool result, int error, DbusPackage map)
{
    qDebug() << Q_FUNC_INFO <<" action " << action << " objects " << objects;
    DataHandlerBase *dataHandlerBase = NULL;
    if(objects == "binary"){
        //只有更新模块能接受二进制信号
        dataHandlerBase = m_nDataHandlerFactory.getDataHandler(BUSINESS_TYPE_UPDATE);
    } else {
        DataOperationType type = BussinessMap::getInstance()->getBussinessType(objects, BussinessMap::MODULE_TYPE_HANDLER);
        if(type == BUSINESS_TYPE_UNKOWN && objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
            QList<DataHandlerBase *>  handlerBaseList = m_nDataHandlerFactory.getAllDataHandler();
            for(int i = 0; i < handlerBaseList.count(); i++)
                handlerBaseList.at(i)->networkResponse(objects, action, result, error, map);
        }else if(type != BUSINESS_TYPE_UNKOWN){
            dataHandlerBase = m_nDataHandlerFactory.getDataHandler(type);
        }
    }
    if(dataHandlerBase)
        dataHandlerBase->networkResponse(objects, action, result, error, map);
}

