#include "syncmanager.h"
#include "../dataprovider.h"
#include "../common/bussinessmap.h"
#include "../networkcontrol.h"
#include <QDebug>
SyncManager* SyncManager::m_pSyncManager = NULL;
SyncManager::SyncManager()
{
    NetworkControl *networkControl = NetworkControl::getInstance();
    connect(networkControl, SIGNAL(sendNetworkResultSignal(QString,QString,bool,int,DbusPackage)),
            this, SLOT(sendNetworkResultSlot(QString,QString,bool,int,DbusPackage)));

    QList<SynchronizerBase * > synchronizerBaseList = m_nSyncFactory.getAllSynchronizer();
    for(int i = 0; i < synchronizerBaseList.size(); i++){
        SynchronizerBase *synchronizerBase = synchronizerBaseList.at(i);
        connect(synchronizerBase, SIGNAL(onSyncFinishedSignal(DataOperationType,BusinessMessageType,QString,QString,QVariantMap)),
                this, SLOT(syncFinished(DataOperationType,BusinessMessageType,QString,QString,QVariantMap)));
        connect(synchronizerBase, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
    }
    DataProvider *provider = DataProvider::getInstance();
    //进入主事件循环
    connect(this, SIGNAL(onSyncFinishedSignal(BusinessMessageType,QString,QString,QVariantMap)),
            provider, SLOT(messageLoopFunction(BusinessMessageType,QString,QString,QVariantMap)), Qt::QueuedConnection);
    connect(this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), provider, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));

}

SyncManager *SyncManager::getInstance()
{
    if(m_pSyncManager == NULL)
        m_pSyncManager = new SyncManager();
    return m_pSyncManager;
}

SyncManager::~SyncManager()
{
    NetworkControl *networkControl = NetworkControl::getInstance();
    disconnect(networkControl, SIGNAL(sendNetworkResultSignal(QString,QString,bool,int,DbusPackage)),
               this, SLOT(sendNetworkResultSlot(QString,QString,bool,int,DbusPackage)));

    QList<SynchronizerBase * > synchronizerBaseList = m_nSyncFactory.getAllSynchronizer();
    for(int i = 0; i < synchronizerBaseList.size(); i++){
        SynchronizerBase *synchronizerBase = synchronizerBaseList.at(i);
        //disconnect(synchronizerBase, SIGNAL(onSyncFinishedSignal(BusinessMessageType,QString,QString,QVariantMap)), this, SIGNAL(onSyncFinishedSignal(BusinessMessageType,QString,QString,QVariantMap)));
        disconnect(synchronizerBase, SIGNAL(onSyncFinishedSignal(DataOperationType,BusinessMessageType,QString,QString,QVariantMap)),
                this, SLOT(syncFinished(DataOperationType,BusinessMessageType,QString,QString,QVariantMap)));
        disconnect(synchronizerBase, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
    }
    DataProvider *provider = DataProvider::getInstance();
    //进入主事件循环
    disconnect(this, SIGNAL(onSyncFinishedSignal(BusinessMessageType,QString,QString,QVariantMap)),
               provider, SLOT(messageLoopFunction(BusinessMessageType,QString,QString,QVariantMap)));
    disconnect(this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), provider, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
}

void SyncManager::syncAddition(const QString &action,  const QString &objects, const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO << " action " << action << "  objects " << objects;
    DataOperationType type = BussinessMap::getInstance()->getBussinessType(objects, BussinessMap::MODULE_TYPE_HANDLER);
    SynchronizerBase *synchronizerBase = m_nSyncFactory.getSynchronizer(type);
    synchronizerBase->execSyncTask(action, objects, map);
}

void SyncManager::syncPreTask(const QString &action, const DataPackage &dataPackage)
{
    QString objects = dataPackage.head.objects;
    qDebug() << Q_FUNC_INFO << " action " << action << "  objects " << objects;
    DataOperationType type = BussinessMap::getInstance()->getBussinessType(objects, BussinessMap::MODULE_TYPE_SYNC);
    SynchronizerBase *synchronizerBase = m_nSyncFactory.getSynchronizer(type);
    synchronizerBase->execSyncPreTask(action, dataPackage);

}

void SyncManager::sendNetworkResultSlot(QString objects, QString action, bool result, int error, DbusPackage map)
{
    DataOperationType type = BussinessMap::getInstance()->getBussinessType(objects, BussinessMap::MODULE_TYPE_SYNC);
    qDebug() << Q_FUNC_INFO  << " objects " << objects << " type " << type;
    if(type == BUSINESS_TYPE_UNKOWN && objects == EMB_Off_GetEquipmentAndPartsListByJoinIDs){
        QList<SynchronizerBase *>  syncingBaseList = m_nSyncFactory.getAllSynchronizer();
        for(int i = 0; i < syncingBaseList.count(); i++)
            syncingBaseList.at(i)->networkResponse(objects, action, result, error, map);
    } else if(type != BUSINESS_TYPE_UNKOWN){
        SynchronizerBase *synchronizerBase = m_nSyncFactory.getSynchronizer(type);
        synchronizerBase->networkResponse(objects, action, result, error, map);
    }
}

void SyncManager::syncFinished(const DataOperationType operationType, const BusinessMessageType &businessMsgType,
                               const QString &action, const QString &objects, const QVariantMap &map)
{
    qDebug() << Q_FUNC_INFO << "   operationType: " << operationType;
    emit onSyncFinishedSignal(businessMsgType, action, objects, map);
}

