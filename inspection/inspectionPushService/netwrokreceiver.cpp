#include "networkreceiver.h"
#include <QDebug>
#include <QTimer>
#include "push_define.h"

NetworkReceiver::NetworkReceiver()
{

}

/**
 * 构造函数
 * @param networkTool 网络处理工具模块
 */
public NetworkReceiver(INetworkTool networkTool, PushListener listener) {
    this.networkTool = networkTool;
    this.listener = listener;
    this.dataHandler = DataFactory.getDataManager();
}

/**
 * 设置监听器
 * @param listener 监听器
 */
void NetworkReceiver::setListener(PushListener listener) {
    this->listener = listener;
}

/**
 * 获取初始化标记
 * @return true 初始化完成  false 未完成
 */
bool NetworkReceiver::isInited() {
    return beInited;
}

/**
 * 设置初始化标记
 * @param val true 初始化完成  false 未完成
 */
void NetworkReceiver::setInited(bool val) {
    this->beInited = val;
}


/**
 * 线程处理函数
 */
void NetworkReceiver::run() {
    if(beExited){
        cleanup();
        return;
    }
    setInited(true);
    if(listener != NULL) {
        listener.onInitComplete();
    }
    startReception();
    exec();
}

/**
 * 清除Looper并退出
 */
void NetworkReceiver::cleanup(){
    if(listener != NULL) {
        listener = NULL;
    }
    networkTool = NULL;
    quit();
}


void NetworkReceiver::receptionProcess() {
    /**
     * 开始接收数据
     */
    if(networkTool == NULL) {
        return;
    }

    if(!receiveData() && listener != NULL){
        listener.onError(ERROR_DATA_RECV, "Receive date failed");
        return;
    }

    /**
     * 开始下次接收 - 保证巡检接收
     */
    startReception();
}

void NetworkReceiver::exitReceiver()
{
    beExited = true;
}

/**
 * 开始下次接收
 */
void NetworkReceiver::startReception() {
    QTimer::singleShot(500, this, SLOT(receptionProcess()));
}

/**
 * 退出线程
 */
void NetworkReceiver::quitReceiver() {
    QTimer::singleShot(500, this, SLOT(exitReceiver()));
}

/**
 * 接收Push信息
 */
bool NetworkReceiver::receiveData(){
    qDebug() << "--------------------NetwrokReceiver::receiveData-------------";
//    QByteArray head = new QByteArray();
    byte[] head = new byte[16];
    try {
        if(networkTool == null) {
            qDebug() << "--------------------NetwrokReceiver::receiveData-------------warnning-------";
        }
        if(!networkTool.isConnected()) {
            return false;
        }
        networkTool.recvData(head, 16);
    } catch (NetworkException e) {
        if(e.error == NetworkErrorDef.NETWORK_ERROR_TIMEOUT) {
            return true;
        }else {
            e.printStackTrace();
            return false;
        }
    }
    NetworkProtocolHead protocolHead = NetworkProtocolHead.fromData(head);
    LogUtil.i(tag(), "Type = " + String.valueOf(protocolHead.typeDex)
            + " Length = " + String.valueOf(protocolHead.length));
    if (protocolHead == null || !protocolHead.mark.equals(NetworkDef.NETWORK_PROTOCOL_MARK)) {
        if(listener != null) {
            listener.onError(PushDef.ErrorProtocol, "Data Package head error");
        }
        return false;
    }else if(protocolHead.typeDex == 8) {
        /**
         * success : adjust heartbeat interval
         */
        if(listener != null) {
            listener.onHeartbeatResult(true);
        }
        return true;
    }else if(protocolHead.length <= 0) {
        LogUtil.w(tag(), "Receive data length = 0");
        return true;
    }
    byte[] byteResponse = new byte[protocolHead.length];
    try {
        if(!networkTool.isConnected()) {
            return false;
        }
        networkTool.recvData(byteResponse, protocolHead.length);
    } catch (NetworkException e) {
        e.printStackTrace();
        return false;
    }

    DataPackage dataPackage = new DataPackage();
    LogUtil.i(tag(), "Parse data package");
    /**
     * parse data
     */
    if(!dataHandler.decodeData(byteResponse, protocolHead, dataPackage)){
        if(listener != null) {
            listener.onError(PushDef.ErrorProtocol, "Decode error");
        }
        return true;
    }
    LogUtil.i(tag(), "Parse data package over");

    /**
     * handle data
     */
    handleData(dataPackage);
    return true;
}


/**
 * 处理数据包
 * @param dataPackage 服务器的数据包
 */
private void handleData(DataPackage dataPackage) {
    LogUtil.i(tag(), "begin handle data");
    if(!dataPackage.head.getType().equals(PushDef.PushDexType)) {
        return;
    }
    switch (dataPackage.head.objects) {
        case PushDef.ObjectPushMessage:
            LogUtil.i(tag(), "ObjectCreatePush");
            if(listener != null) {
                listener.onPushData(dataPackage);
            }
            break;
        case PushDef.ObjectCreatePush:
            LogUtil.i(tag(), "ObjectCreatePush - " + String.valueOf(dataPackage.summary.getResult()) +
                        " - " + String.valueOf(dataPackage.summary.getCode()));
            if (dataPackage.summary.getResult() == 1 && dataPackage.summary.getCode() == 100) {
                LogUtil.i(tag(), "CreatePush success");
            } else {
                LogUtil.i(tag(), "CreatePush fail");
                if(listener != null) {
                    //listener.onError(PushDef.ErrorProtocol, "CreatePush fail");
                }
            }
            break;
        case PushDef.ObjectRegisterPush:
            LogUtil.i(tag(), "ObjectRegisterPush - " + String.valueOf(dataPackage.summary.getResult()) +
                    " - " + String.valueOf(dataPackage.summary.getCode()));
            if (dataPackage.summary.getResult() == 1 && dataPackage.summary.getCode() == 100) {
                LogUtil.i(tag(), "RegisterPush success");
            } else {
                if(listener != null) {
                    listener.onError(PushDef.ErrorProtocol, "RegisterPush fail");
                }
            }
            break;
        case PushDef.ObjectSubscribeTopic:
            LogUtil.i(tag(), "ObjectSubscribeTopic - " + String.valueOf(dataPackage.summary.getResult()) +
                    " - " + String.valueOf(dataPackage.summary.getCode()));
            if (dataPackage.summary.getResult() == 1 && dataPackage.summary.getCode() == 100) {
                LogUtil.i(tag(), "SubscribeTopic success");
            } else {
                if(listener != null) {
                    listener.onError(PushDef.ErrorProtocol, "SubscribeTopic fail");
                }
            }
            break;
        case PushDef.ObjectUnregisterPush:
            LogUtil.i(tag(), "ObjectUnregisterPush - " + String.valueOf(dataPackage.summary.getResult()) +
                    " - " + String.valueOf(dataPackage.summary.getCode()));
            if (dataPackage.summary.getResult() == 1 && dataPackage.summary.getCode() == 100) {
                LogUtil.i(tag(), "UnregisterPush success");
            } else {
                if(listener != null) {
                    listener.onError(PushDef.ErrorProtocol, "UnregisterPush fail");
                }
            }
            break;
        case PushDef.ObjectUnsubscribeAll:
            LogUtil.i(tag(), "ObjectUnsubscribeAll - " + String.valueOf(dataPackage.summary.getResult()) +
                    " - " + String.valueOf(dataPackage.summary.getCode()));
            if (dataPackage.summary.getResult() == 1 && dataPackage.summary.getCode() == 100) {
                LogUtil.i(tag(), "UnsubscribeAll success");
            } else {
                if(listener != null) {
                    listener.onError(PushDef.ErrorProtocol, "UnsubscribeAll fail");
                }
            }
            break;
    }
}
