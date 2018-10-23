#include "dataprovider.h"
#include <QDebug>
#include "../framework/common/functiontype.h"
#include "../framework/common/convertdata.h"
#include "../framework/config/sdisruntimecontext.h"
#include "sync/syncmanager.h"
#include "../framework/common/networkstatus.h"
#include "dataHandler/datahandlermanager.h"
DataProvider* DataProvider::m_pDataProvider = NULL;
DataProvider* DataProvider::getInstance(){
    if(m_pDataProvider == NULL)
        m_pDataProvider = new DataProvider();
    return m_pDataProvider;
}

DataProvider::DataProvider(QObject *parent):QObject(parent)
{
    NetworkStatus *networkStatus = NetworkStatus::getInstance();
    connect(networkStatus, SIGNAL(networkConnectChanged(QString )), this, SLOT(networkStatusChanged(QString)));
}

DataProvider::~DataProvider()
{
}

void DataProvider::messageLoopFunction(const BusinessMessageType businessMsgType, const QString action, const QString objects, QVariantMap map)
{
    qDebug() <<Q_FUNC_INFO << " msgtype " << businessMsgType << "  action " << action  <<"  objects " << objects;
    switch (businessMsgType) {
    case MESSAGE_SUBMIT_CACHE_ALL:
        qDebug() << "MESSAGE_SUBMIT_CACHE_ALL";
        break;
    case MESSAGE_SYNC_STEP_BY_STEP:
        qDebug() << "MESSAGE_SYNC_STEP_BY_STEP";
        break;
    case MESSAGE_SYNC_ADDTION:{
        qDebug() << "MESSAGE_SYNC_ADDTION";
        SyncManager *syncManager = SyncManager::getInstance();
        syncManager->syncAddition(action, objects, map);
    }
        break;
    case MESSAGE_DEVICE_SYNCED:
        qDebug() << "MESSAGE_DEVICE_SYNCED";
        break;
    case MESSAGE_SYNC_ADDTION_FINISHED:{
        qDebug() << "MESSAGE_SYNC_ADDTION_FINISHED";
        DataHandlerManager *dataHandlerManager = DataHandlerManager::getInstance();
        dataHandlerManager->syncFinished(action, objects, map);
    }
        break;
    default:
        break;
    }
}

void DataProvider::networkStatusChanged(QString status)
{
    if(status != ""){
        int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
        if(authType == 1){
            NetworkManager *networkManager = NetworkManager::getInstance(NULL);
            networkManager->initSystemAccessToken();
        }
    }
}
