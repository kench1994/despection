#include <QTimer>
#include <QSettings>
#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>
#include "push_define.h"
#include "pushmanager.h"
#include "../framework/common/utils.h"
#include "../framework/common/commondbustypes.h"
#include "../framework/config/sdisruntimecontext.h"
#include "../framework/network/tools/networktoolfactory.h"
#include "../framework/sdissystemenvironment.h"
#include "connectionthread.h"
#include "csystemdevicemanager.h"
#include "cenvironment.h"

PushManager* PushManager::pushManager = NULL;
PushManager *PushManager::getInstance(QObject *parent)
{
    if(pushManager == NULL)
        pushManager = new PushManager(parent);
    return pushManager;
}

PushManager::PushManager(QObject *parent) : QObject(parent)
{
    registerCommonDataTypes();
    isConnecting = false;
    beInit = false;
    beRunning = false;
    networkReceiver = NULL;
    networkSender = NULL;
    m_pNetworkStatus = NetworkStatus::getInstance();
    networkTool = NetworkToolFactory::createTool(SOCKET);
    heartbeatManager = new HeartbeatManager(this);
    loadConfig();
    connect(m_pNetworkStatus, SIGNAL(networkConnectChanged(QString )), this, SLOT(networkStatusChanged(QString)));
}

bool PushManager::isBeRunning() {
    return beRunning;
}

void PushManager::setPushAuthority(const PushAuthority &pushAuthority, const QString &action) {
    qDebug() << Q_FUNC_INFO << "  action " << action;
    this->pushAuthority = pushAuthority;
    this->clientAction = action;
    saveConfig();
}

void PushManager::setPushAuthority(const QString &action)
{
    qDebug() << Q_FUNC_INFO << "  action " << action;
    PushAuthority pushAuthority;
    this->pushAuthority = pushAuthority;
    this->clientAction = action;
    saveConfig();
}

void PushManager::saveConfig() {
    qDebug() << Q_FUNC_INFO;
    CEnvironment env;
    QString dataPath = env.dataPath();
    QSettings *settings = new QSettings(dataPath + "/" + CONFIG_FILE, QSettings::IniFormat);
    QString userID = "";
    QString content = "";
    QMap<QString, QString>::const_iterator iterator;
    for (iterator = pushAuthority.getAuthority().constBegin(); iterator != pushAuthority.getAuthority().constEnd(); iterator++) {
        userID = iterator.key();
        content =iterator.value();
    }
    settings->setValue("push/action", clientAction);
    settings->setValue("push/user", userID);
    settings->setValue("push/content", content);
    if(!address.isEmpty() && port > 0) {
        settings->setValue("network/ip", address);
        settings->setValue("network/port", port);
    }
}

void PushManager::loadConfig() {
    qDebug() << Q_FUNC_INFO;
    CEnvironment env;
    QString dataPath = env.dataPath();
    QSettings *settings = new QSettings(dataPath + "/" + CONFIG_FILE, QSettings::IniFormat);
    QString action = settings->value("push/action", "").toString();
    QString userID = settings->value("push/user","").toString();
    QString content = settings->value("push/content","").toString();
    QString address = settings->value("network/ip", "").toString();
    int port = settings->value("network/port", 0).toInt();
    if(!address.isEmpty() && port > 0) {
        SDISRuntimeContext::getInstance()->setServerAddress(address);
        SDISRuntimeContext::getInstance()->setServerPort(port);
        this->address = address;
        this->port = port;
    }
    if(action.isEmpty()) {
        return;
    }
    pushAuthority.setAuthority(userID, content);
    clientAction = action;
}

bool PushManager::setServerAddress(const QString &host, const int &port) {
    qDebug() << Q_FUNC_INFO;
    bool ret = true;
    if(this->address != host || this->port != port) {
        ret = false;
    }
    this->address = host;
    this->port = port;
    SDISRuntimeContext::getInstance()->setServerAddress(address);
    SDISRuntimeContext::getInstance()->setServerPort(port);
    saveConfig();
    return ret;
}

void PushManager::startService()
{
    QTimer::singleShot(0, this, SLOT(startPush()));
}

void PushManager::connectServer() {
    if(networkTool == NULL) {
        networkTool = NetworkToolFactory::createTool(SOCKET);
    }
    if(networkTool->isConnected()){
        return;
    }
    qDebug() << Q_FUNC_INFO <<" isConnecting:" << isConnecting;
    if(isConnecting)
        return;
    isConnecting = true;
    ConnectionThread *thread = new ConnectionThread(networkTool, address, port);
    connect(thread, SIGNAL(serverConnected(const bool &)), this, SLOT(serverConnectedInner(const bool &)));
    thread->start();
}

void PushManager::startPush() {
    qDebug() << Q_FUNC_INFO;
    if(beRunning || address.isEmpty() || port == 0 || m_pNetworkStatus->getNetworkConnectStatus() == "") {
        return;
    }
    connectServer();
    beRunning = true;
}

void PushManager::stopPush(){
    qDebug() << Q_FUNC_INFO << "   beRunning  " << beRunning;
    if(!beRunning) {
        return;
    }
     //destroy sender and receiver
    if(networkSender != NULL) {
        networkSender->quitTread();
        networkSender = NULL;
    }
    if(networkReceiver != NULL) {
        networkReceiver->quitThread();
        networkReceiver = NULL;
    }
    if(networkTool != NULL) {
        networkTool->disconnect();
        networkTool = NULL;
    }
    beInit = false;
    beRunning = false;
}

void PushManager::initCompleteInner()
{
    qDebug() << Q_FUNC_INFO << "  beInit " << beInit;
    if (networkSender == NULL || networkReceiver == NULL || beInit) {
        return;
    }
    beInit = networkSender->isInited() && networkReceiver->isInited();
    if (beInit) {
        subscribePush();
    }
}

void PushManager::onHeartbeat() {
    qDebug() << Q_FUNC_INFO;
    heartbeatManager->startAlarm();
    if(!isBeRunning()) {
        startPush();
        return;
    }
    if(networkSender == NULL || !networkSender->isInited()) {
        return;
    }
    networkSender->sendHeartbeat();
}

void PushManager::restartPush(){
    stopPush();
    startService();
}

void PushManager::heartbeatResultInner(const bool &result) {
    /**
     * if heartbeat sent fail then disconnect socket and cleanup sender & receiver
     * stop heartbeat manager
     */
    if(!result) {
        if(m_pNetworkStatus->getNetworkConnectStatus() != ""){
            /**
             * If network disconnected, that means sent heartbeat error due to network state.
             * We should reset heartbeat interval.
             */
            heartbeatManager->resetInterval();
        }else{
            /**
             * If network connected, that means sent heartbeat error due to heartbeat interval is too long.
             * We should adjust heartbeat interval.
             */
            heartbeatManager->adjust(false);
        }
        qDebug() << Q_FUNC_INFO << " false";
        restartPush();
    }else{
        heartbeatManager->adjust(true);
    }
}

void PushManager::onErrorInner(const int &type, const QString &description) {
    qDebug() << Q_FUNC_INFO <<"  type:" << type <<",  description: " <<description;
    if(type == ERROR_DATA_RECV) {
        if(m_pNetworkStatus->getNetworkConnectStatus() != "" && heartbeatManager->getInterval() !=  HEARTBEA_INTERVAL_MIN) {
            heartbeatManager->adjust(false);
            restartPush();
        }else{
            stopPush();
            heartbeatManager->resetInterval();
        }
    } else {
        stopPush();
    }
}

void PushManager::serverConnectedInner(const bool &result) {
    qDebug() << Q_FUNC_INFO << "   success " << result;
    isConnecting = false;
    if(result) {
        if(isBeRunning()) {
            networkSender = new NetworkSender(networkTool);
            connect(networkSender, SIGNAL(initComplete()), this, SLOT(initCompleteInner()));
            connect(networkSender, SIGNAL(error(const int &, const QString &)), this, SLOT(onErrorInner(const int &, const QString &)));
            connect(networkSender, SIGNAL(heartbeatResult(const bool &)), this, SLOT(heartbeatResultInner(const bool &)));
            networkReceiver = new NetworkReceiver(networkTool);
            connect(networkReceiver, SIGNAL(initComplete()), this, SLOT(initCompleteInner()));
            connect(networkReceiver, SIGNAL(error(const int &, const QString &)), this, SLOT(onErrorInner(const int &, const QString &)));
            connect(networkReceiver, SIGNAL(heartbeatResult(const bool &)), this, SLOT(heartbeatResultInner(const bool &)));
            connect(networkReceiver, SIGNAL(handlePushData(const DataPackage &)), this, SLOT(handlePushDataInner(const DataPackage &)));
            networkSender->start();
            networkReceiver->start();
            heartbeatManager->start();
        } else {
            if(networkTool != NULL) {
                networkTool->disconnect();
                networkTool = NULL;
            }
        }
    }else{
        stopPush();
    }
}

void PushManager::switchAddress(){
    QTimer::singleShot(0, this, SLOT(restartPush()));
}

void PushManager::switchAuthority() {
//        if(this.pushAuthority == NULL) {
////            networkSender->unregisterPush();
//            stopService();
//        }else {
//            restartPush();
//        }
}

void PushManager::stopService() {
    QTimer::singleShot(0, this, SLOT(stopPush()));
}

void PushManager::handlePushDataInner(const DataPackage &dataPackage) {
    qDebug() << Q_FUNC_INFO;
    /**
     * handle datapackage
     * Message Push - register to notification panel
     */
    DbusPackage dbusPackage;
    dbusPackage.dataPackage = dataPackage;
    emit handlePushData(dbusPackage);
}

void PushManager::subscribePush() {
//    networkSender->initPushAuthorityAndKeepAlive();
}

void PushManager::networkStatusChanged(QString status)
{
    if(status != ""){
        connectServer();
    }
}

