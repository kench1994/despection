#include "pushinterface.h"
#include <QVariantMap>
#include <QDBusConnection>
#include <QDebug>
#include "push_define.h"
#include "../framework/config/sdisruntimecontext.h"
#include "../framework/sdissystemenvironment.h"
#include "../framework/common/functiontype.h"
#include "./runtimeparam.h"

/**
 * 构造函数
 */
PushInterface::PushInterface(QObject *parent) : QObject(parent)
{
    initialize();
}

/**
 * 初始化函数
 */
void PushInterface::initialize() {

    QDBusConnection::sessionBus().connect(QString(), CLIENT_DBUS_PATH,
                                          CLIENT_DBUS_INTERFACE,
                                          QString("statusUpdated"),
                                          this, SLOT(setStatus(const QString&, const QVariantMap&)));

    QDBusConnection::sessionBus().connect(QString(), SERVICE_DBUS_PATH,
                                          SERVICE_DBUS_INTERFACE,
                                          QString("statusUpdated"),
                                          this, SLOT(setStatus(const QString&, const QVariantMap&)));


    SDISRuntimeContext::getInstance()->setDeviceID(RuntimeParam::getInstance()->getPadId());
    m_pNetworkStatus = NetworkStatus::getInstance();
    connect(m_pNetworkStatus, SIGNAL(networkConnectChanged(QString)), this, SLOT(networkStatus(QString)));
    pushManager = PushManager::getInstance(this);
    connect(pushManager, SIGNAL(handlePushData(const DbusPackage &)), this, SIGNAL(handlePushData(const DbusPackage &)));
    pushManager->startService();
}

/**
 * Push注册处理函数
 */
void PushInterface::setRegisterStatus(const QVariantMap &varMap)
{
    QString intentAction = varMap.value(INTENT_ACTION).toString();
    qDebug() << Q_FUNC_INFO <<" intentAction: " << intentAction;
    if(intentAction == HEARTBEA_ACTION){
        pushManager->onHeartbeat();
    }else if(intentAction == ADDRESS_ACTION){
        QString address = varMap.value(INTENT_EXTRA_ADDRESS).toString();
        int port = varMap.value(INTENT_EXTRA_PORT).toInt();
        if(address != "" && port != 0) {
            bool ret = pushManager->setServerAddress(address, port);
            if(!ret) {
                pushManager->switchAddress();
            }
        }
    }else if(intentAction == REGISTER_PUSH_AUTHORITY_ACTION){
        PushAuthority pushAuthority;
        QMap<QString,QVariant> map = varMap.value(INTENT_EXTRA_AUTHORITY).toMap();
        QMap<QString,QVariant>::const_iterator iterator;
        for(iterator = map.constBegin(); iterator != map.constEnd();iterator++){
            pushAuthority.getAuthority().insert(iterator.key(),iterator.value().toString());
        }
        QString action = varMap.value(INTENT_EXTRA_ACTION).toString();
        pushManager->setPushAuthority(pushAuthority, action);
    }else if(intentAction == REGISTER_PUSH_ACTION){
        QString action = varMap.value(INTENT_EXTRA_ACTION).toString();
        pushManager->setPushAuthority(action);
    }else if(intentAction == UNREGISTER_PUSH_ACTION){
        PushAuthority pushAuthority;
        pushManager->setPushAuthority(pushAuthority, "");
    }else if(intentAction == INIT_TOKEN_ACTION){
        pushManager->switchAddress();
    }
}

/**
 * 网络状态变化函数
 */
void PushInterface::networkStatus(const QString &status)
{
    if(status != ""){
        if(!pushManager->isBeRunning()) {
            pushManager->startService();
        }
    }
}

void PushInterface::setStatus(const QString &functionType, const QVariantMap &map)
{
    if(functionType == FUNCTION_TYPE_STATUS_ADDRESS){
        QString address = map["ip"].toString();
        int port = map["port"].toInt();
        QVariantMap varMap;
        varMap.insert(INTENT_ACTION, ADDRESS_ACTION);
        varMap.insert(INTENT_EXTRA_ADDRESS, address);
        varMap.insert(INTENT_EXTRA_PORT, port);
        setRegisterStatus(varMap);
    }else if(functionType == FUNCTION_TYPE_STATUS_AUTHTYPE){
        int authType = map["authType"].toInt();
        SDISRuntimeContext::getInstance()->setAuthenticationType(authType);
        QVariantMap varMap;
        varMap.insert(INTENT_ACTION, INIT_TOKEN_ACTION);
        setRegisterStatus(varMap);
    }else if(functionType == FUNCTION_TYPE_STATUS_ENCRYPTTYPE){
        int encryptType = map["encryptType"].toInt();
        SDISRuntimeContext::getInstance()->setDataEncryptionType(encryptType);
        QVariantMap varMap;
        varMap.insert(INTENT_ACTION, INIT_TOKEN_ACTION);
        setRegisterStatus(varMap);
    }else if(functionType == FUNCTION_TYPE_STATUS_RESET_PARA_PUSH){
        QString appKey = map["appKey"].toString();
        QString appSecretKey = map["appSecretKey"].toString();
        QString appSecretIV = map["appSecretIV"].toString();
        QString appPublicKey = map["appPublicKey"].toString();
        QString appPrivateKey = map["appPrivateKey"].toString();
        SDISRuntimeContext::getInstance()->setAppKey(appKey);
        SDISRuntimeContext::getInstance()->setAppSecretKey(appSecretKey);
        SDISRuntimeContext::getInstance()->setAppSecretIV(appSecretIV);
        SDISRuntimeContext::getInstance()->setAppPublicKey(appPublicKey);
        SDISRuntimeContext::getInstance()->setAppPrivateKey(appPrivateKey);
        QVariantMap varMap;
        varMap.insert(INTENT_ACTION, INIT_TOKEN_ACTION);
        setRegisterStatus(varMap);
    }
}
