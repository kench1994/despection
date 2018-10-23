#include "networkreceiver.h"
#include <QDebug>
#include <QTimer>
#include "push_define.h"
#include "../framework/network/networkheadtool.h"
#include "../framework/config/sdisruntimecontext.h"
#include "../framework/network/tools/networkencryptionutil.h"
NetworkReceiver::NetworkReceiver(INetworkTool *networkTool):QThread(NULL)
{
    this->moveToThread(this);
    this->networkTool = networkTool;
}

NetworkReceiver::~NetworkReceiver()
{
}

bool NetworkReceiver::isInited() {
    return beInited;
}

void NetworkReceiver::setInited(bool val) {
    this->beInited = val;
}

void NetworkReceiver::run() {
    this->dataManager = new DataHandler(this);
    setInited(true);
    emit initComplete();
    startReception();
    this->exec();
}

void NetworkReceiver::startReception() {
    QTimer::singleShot(500, this, SLOT(startReceptionInner()));
}

void NetworkReceiver::startReceptionInner() {
    if(networkTool == NULL) {
        return;
    }
    if(!receiveData()){
        emit error(ERROR_DATA_RECV, "Receive date failed");
        return;
    }
     //开始下次接收 - 保证巡检接收
    startReception();
}

bool NetworkReceiver::receiveData(){
    qDebug() << Q_FUNC_INFO;
    char protocolBuffer[16] = {0};
    if(!networkTool->isConnected()) {
        qDebug() << Q_FUNC_INFO <<" not connect....";
        return false;
    }
    int len = networkTool->recvData(protocolBuffer, 16);
    qDebug() << Q_FUNC_INFO <<"  receive len:" << len;
    if(len < 0 ){
        return false;
    }
    ProtocolHead protocolHead;
    NetworkHeadTool::decodeProtocolHead(protocolHead, protocolBuffer);
    qDebug() << Q_FUNC_INFO <<"  Type =  " << protocolHead.type << "  Length   " << protocolHead.length;
    if(strncmp(protocolHead.mark, NETWORK_PROTOCOL_MARK, strlen(NETWORK_PROTOCOL_MARK)) != 0){
        emit error(ERROR_DATA_PROTOCOL, "Data Package head error");
        return false;
    }else if(protocolHead.type == 8) {
         //success : adjust heartbeat interval
        emit heartbeatResult(true);
        return true;
    } else if(protocolHead.length <= 0) {
        qDebug() <<Q_FUNC_INFO << "   Receive data length = 0";
        return true;
    }

     //ignore remain 32 bytes in security head mode
    int headLen = 4;
    int authype = SDISRuntimeContext::getInstance()->getAuthenticationType();
    if(authype == 0){
        headLen = 16;
    }else if(authype == 1){
        headLen = 48;
    }
    qDebug() <<Q_FUNC_INFO << "    --000----headLen: " << headLen;
    if(headLen - 16 > 0){
        QByteArray headBuf;
        len = networkTool->recvData(headBuf, headLen-16);
        qDebug() << Q_FUNC_INFO << "   ---000----len: " << len;
    }

    QByteArray receivedData;
    len = networkTool->recvData(receivedData, protocolHead.length);
    qDebug() << Q_FUNC_INFO <<"   --111----len: " << len;
    if(len < 0 ){
        return false;
    }

     //Decrypt data
    QByteArray byteResponse;
    if(!NetworkEncryptionUtil::getFinalReceivedData(receivedData, byteResponse)){
        qDebug() << Q_FUNC_INFO <<"    -----Decrypt data fail!!!";
        emit error(ERROR_DATA_PROTOCOL, "Decrypt error");
        return false;
    }

    //parse data
    DataPackage dataPackage;
    bool result = dataManager->decodeData(byteResponse, dataPackage, protocolHead.format, protocolHead.charset);
    if(!result){
        qDebug() << Q_FUNC_INFO << " -Decode error";
        emit error(ERROR_DATA_PROTOCOL, "Decode error");
        return true;
    }
    qDebug() << Q_FUNC_INFO << " -Parse data package over";
    handleData(dataPackage);
    return true;
}

void NetworkReceiver::handleData(const DataPackage &dataPackage) {
    qDebug() << Q_FUNC_INFO << "  dataPackage.head.type " << dataPackage.head.type;
    if(dataPackage.head.type != PUSH_DEX_TYPE) {
        return;
    }
    qDebug() << Q_FUNC_INFO <<" summary.result:  " << dataPackage.summary.result << "   summary.code " << dataPackage.summary.code;
    bool success = dataPackage.summary.result == 1 && dataPackage.summary.code == 100;
    if(dataPackage.head.objects == OBJECT_PUSH_MESSAGE){
        qDebug() << "-------------NetworkReceiver::handleData----------ObjectCreatePush";
        emit handlePushData(dataPackage);
    }else if(dataPackage.head.objects == OBJECT_CREATE_PUSH){
            qDebug() << "-------------NetworkReceiver::handleData----------CreatePush " << success;
    } else if(dataPackage.head.objects == OBJECT_REGISTER_PUSH){
            qDebug() << "-------------NetworkReceiver::handleData----------RegisterPush " << success;
            if(!success)
                emit error(ERROR_DATA_PROTOCOL, "RegisterPush fail");
    } else if(dataPackage.head.objects == OBJECT_SUBSCRIBE_TOPIC){
            qDebug() << "-------------NetworkReceiver::handleData----------SubscribeTopic "  << success;
            if(!success)
                emit error(ERROR_DATA_PROTOCOL, "SubscribeTopic fail");
    }else if(dataPackage.head.objects == OBJECT_UNREGISTER_PUSH){
            qDebug() << "-------------NetworkReceiver::handleData----------UnregisterPush " << success;
            if(!success)
                emit error(ERROR_DATA_PROTOCOL, "UnregisterPush fail");
    }else if(dataPackage.head.objects == OBJECT_UNSUBSCRIBE_ALL){
            qDebug() << "-------------NetworkReceiver::handleData----------UnsubscribeAll " << success;
            if(!success)
                emit error(ERROR_DATA_PROTOCOL, "UnsubscribeAll fail");
    }
}

void NetworkReceiver::quitThread() {
    this->quit();
    networkTool = NULL;
}
