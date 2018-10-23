#include "networksender.h"
#include <QTimer>
#include <QDebug>
#include "push_define.h"
#include "../framework/config/sdisruntimecontext.h"
#include "../framework/network/networkheadtool.h"
#include "./runtimeparam.h"
#include "../framework/network/tools/networkencryptionutil.h"
#include "../framework/common/systemconfig.h"

/**
 * 构造函数
 *
 * @param networkTool 网络处理工具模块
 */
NetworkSender::NetworkSender(INetworkTool *networkTool)//:QThread(NULL)
{
    qDebug() << Q_FUNC_INFO <<"   --Thread ID  :" << QThread::currentThread();
    this->moveToThread(this);
    this->beInited = false;
    this->networkTool = networkTool;
    this->pushAuthority = NULL;
    connect(this, SIGNAL(initKeepAlive()), this, SLOT(initPushAuthorityAndKeepAlive()));
}

/**
 * 析构函数
 */
NetworkSender::~NetworkSender()
{
}

bool NetworkSender::isInited() {
    return beInited;
}

void NetworkSender::setInited(bool val) {
    this->beInited = val;
}

void NetworkSender::run() {
    this->dataManager = new DataHandler(this);
    setInited(true);
    emit initKeepAlive();
    emit initComplete();
    exec();
}

void NetworkSender::initPushAuthorityAndKeepAlive() {
    initPushAuthorityAndKeepAliveInner();
    subscribe();
}

void NetworkSender::initPushAuthorityAndKeepAliveInner() {
    qDebug() << Q_FUNC_INFO << " Thread ID  :" << QThread::currentThread();
    QByteArray data;
    //生成长连接请求指令
    bool result = generateKeepAlivePackage(data);
    if (!result) {
        emit error(ERROR_DATA_ENCODE, "Send Hello failed!");
        return;
    }
    //发送长连接指令
    if (!sendData(data)) {
        emit error(ERROR_DATA_SEND, "Send Hello failed!");
        return;
    }
}

bool NetworkSender::generateKeepAlivePackage(QByteArray &data) {
    qDebug() << Q_FUNC_INFO << " PushKey is " << SDISRuntimeContext::getInstance()->getDeviceID();
    DataPackage dataPackage;
    dataPackage.head.type = PUSH_DEX_TYPE;
    dataPackage.head.objects = OBJECT_CREATE_PUSH;
    dataPackage.para.insert("PushKey", RuntimeParam::getInstance()->getPadId());
    dataPackage.para.insert("InnerConect",  "");
    return dataManager->encodeData(dataPackage, data);
}

bool NetworkSender::sendData(QByteArray &data) {
    QByteArray sentData;
    if(!NetworkEncryptionUtil::getFinalSentData(data, sentData)){
        return false;
    }
    ProtocolHead head;
    memcpy(head.mark, NETWORK_PROTOCOL_MARK, sizeof(head.mark));
    head.charset = SDISRuntimeContext::getInstance()->getProtocolCharset();
    head.format = SDISRuntimeContext::getInstance()->getProtocolFormat();
    head.mainVersion = NETWORK_PROTOCOL_MAIN_VERSION;
    head.subVersion = NETWORK_PROTOCOL_SUB_VERSION;
    head.result = 0;
    head.type = QString(TYPE_PUSH).toInt();
    head.statusCode = 0;
    head.length = sentData.length();

    bool encrypt = false;
    bool emptyToken = false;
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    if(authType == 1){//secert
        encrypt = true;
    }else if(authType == 0){ //normal
        emptyToken = true;
    }
    char protocolBuffer[48] = {0};
    int len = NetworkHeadTool::generateProtocolHead(head, encrypt, emptyToken, protocolBuffer);
    QByteArray headBuf;
    headBuf.append(protocolBuffer, len);

    if (!networkTool->isConnected()) {
        return false;
    }
    networkTool->sendData(headBuf);
    networkTool->sendData(sentData);
    return true;
}

void NetworkSender::sendHeartbeat() {
    QTimer::singleShot(0, this, SLOT(sendHeartbeatInner()));
}

void NetworkSender::sendHeartbeatInner() {
    qDebug() << Q_FUNC_INFO;
     //send heartbeat package
    ProtocolHead head;
    memcpy(head.mark, NETWORK_PROTOCOL_MARK, sizeof(head.mark));
    head.charset = UTF8;
    head.format = DEXML;
    head.mainVersion = NETWORK_PROTOCOL_MAIN_VERSION;
    head.subVersion = NETWORK_PROTOCOL_SUB_VERSION;
    head.result = 1;
    head.type = QString(TYPE_HEART).toInt();
    head.statusCode = 0;
    head.length = 0;

    char protocolBuffer[64] = {0} ;
    int len = NetworkHeadTool::generateProtocolHead(head, false, true, protocolBuffer);
    QByteArray headBuf;
    headBuf.append(protocolBuffer, len);
    int result = networkTool->sendData(headBuf);
    qDebug() << Q_FUNC_INFO <<" ::sendHeartbeatInner------------------result: " << result;
    if (result <= 0) {
       //failed : notify upper module fail message
        emit heartbeatResult(false);
    }
}

bool NetworkSender::generatePushAuthorityPackage(QByteArray &data) {
    DataPackage dataPackage;
    dataPackage.head.type = PUSH_DEX_TYPE;
    dataPackage.head.objects = OBJECT_REGISTER_PUSH;
    dataPackage.para.insert("AppId", SystemConfig::getInstance()->getAppId());
    dataPackage.para.insert("PushKey",  RuntimeParam::getInstance()->getPadId());
    qDebug() << Q_FUNC_INFO << "  PushKey is " << RuntimeParam::getInstance()->getPadId();
    return dataManager->encodeData(dataPackage, data);
}

bool NetworkSender::generateSubscribePackage(const QString &user, const QString &topic, QByteArray &data) {
    DataPackage dataPackage;
    dataPackage.head.type = PUSH_DEX_TYPE;
    dataPackage.head.objects = OBJECT_SUBSCRIBE_TOPIC;
    dataPackage.para.insert("UserAccount",  user);
    dataPackage.para.insert("TopicIds",  topic);
    return dataManager->encodeData(dataPackage, data);
}

bool NetworkSender::generateUnregisterPackage(QByteArray &data) {
    DataPackage dataPackage;
    dataPackage.head.type = PUSH_DEX_TYPE;
    dataPackage.head.objects = OBJECT_UNREGISTER_PUSH;
    dataPackage.para.insert("AppRegId",  SDISRuntimeContext::getInstance()->getDeviceID());
    return dataManager->encodeData(dataPackage, data);
}

bool NetworkSender::generateUnsubscribePackage(const QString &user, QByteArray &data) {
    DataPackage dataPackage;
    dataPackage.head.type = PUSH_DEX_TYPE;
    dataPackage.head.objects = OBJECT_UNSUBSCRIBE_ALL;
    dataPackage.para.insert("UserAccount",  user);
    return dataManager->encodeData(dataPackage, data);
}

/**
 * 注销Push
 */
void NetworkSender::unregisterPush() {
    QTimer::singleShot(0, this, SLOT(unregisterPushInner()));
}

void NetworkSender::unregisterPushInner() {
    qDebug() << Q_FUNC_INFO;
    QByteArray data;
    bool resulst = generateUnregisterPackage(data);
    if (!resulst) {
        emit error(ERROR_DATA_ENCODE, "Send Unregister failed!");
        return;
    }
    if (!sendData(data)) {
        emit error(ERROR_DATA_SEND, "Send Unregister failed!");
        return;
    }
}

void NetworkSender::unsubscribe() {
    if (pushAuthority == NULL) {
        return;
    }
    QMap<QString, QString>::const_iterator iterator;
    for (iterator = pushAuthority->getAuthority().constBegin(); iterator != pushAuthority->getAuthority().constEnd(); iterator++) {
        /**
          * 生成订阅主题指令
          */
        QByteArray data;
        bool result = generateUnsubscribePackage(iterator.key(), data);
        if (!result) {
            emit error(ERROR_DATA_ENCODE, "Send Unsubscribe failed!");
            return;
        }

        /**
          * 发送取消订阅主题指令
          */
        if (!sendData(data)) {
            emit error(ERROR_DATA_SEND, "Send Unsubscribe failed!");
            return;
        }
        qDebug() << "-------------NetworkSender::unsubscribe----------sent Unsubscribe user: " << iterator.key() ;
    }
}

void NetworkSender::subscribe() {
    qDebug() << Q_FUNC_INFO;
     //生成注册信息指令
    QByteArray data;
    bool result = generatePushAuthorityPackage(data);
    if (!result) {
        emit error(ERROR_DATA_ENCODE, "Send Register failed!");
        return;
    }

    //发送注册信息
    if (!sendData(data)) {
        emit error(ERROR_DATA_SEND, "Send Register failed!");
        return;
    }
#if 0
    QMap<QString, QString>::const_iterator iterator;
    for (iterator = pushAuthority->getAuthority().constBegin(); iterator != pushAuthority->getAuthority().constEnd(); iterator++) {
         //生成订阅主题指令
        bool result = generateSubscribePackage(iterator.key(), iterator.value(), data);
        if (!result) {
            emit error(ERROR_DATA_ENCODE, "Send Subscribe failed!");
            return;
        }
         //发送订阅主题指令
        if (!sendData(data)) {
            emit error(ERROR_DATA_SEND, "Send Subscribe failed!");
            return;
        }
        qDebug() << "-------------NetworkSender::subscribe----------sent subscribe user : " << iterator.key();
    }
#endif
}

void NetworkSender::quitTread() {
    this->quit();
    networkTool = NULL;
}

