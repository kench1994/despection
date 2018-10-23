#include "networkcommandthread.h"
#include <QTimer>
#include <QDebug>
#include "../../config/sdisruntimecontext.h"
#include "../tools/networkencryptionutil.h"
#include "../networkheadtool.h"
#include "../framework/dataManager/datacrypto.h"
#define DBG_TAG "NetworkCommandThread : "

NetworkCommandThread::NetworkCommandThread(QObject *parent) :
    NetworkThreadBase(parent)
{
    this->moveToThread(this);
    mCurrentAction = "";
    mCurrentObject = "";
    mCurrentType = "";
    running = false;
}

NetworkCommandThread::~NetworkCommandThread()
{
    this->quit();
    cleanup();
    if(!wait(3000)) //Wait until it actually has terminated (max. 3 sec)
    {
        terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
        wait(); //We have to wait again here!
    }
}

void NetworkCommandThread::cleanup() {
    NetworkThreadBase::cleanup();
    mCurrentAction = "";
    mCurrentObject = "";
    mCurrentType = "";
    mId = "";
    running = false;
}

bool NetworkCommandThread::isRunning()
{
    QMutexLocker locker(&mutex);
    return running;
}

void NetworkCommandThread::run()
{
    qDebug() << DBG_TAG << "run - Thread ID : " << QThread::currentThreadId();
    exec();
}

bool NetworkCommandThread::addTask(CommandTask& task)
{
    QMutexLocker locker(&mutex);
    if(running) {
        return false;
    }
    mCurrentAction = task.action;
    mCurrentObject = task.objects;
    mCurrentType = task.type;
    mId = task.id;
    taskInfo = task;
    running = true;
    QTimer::singleShot(0, this, SLOT(doTask()));
    return true;
}

void NetworkCommandThread::doTask()
{
    qDebug() << DBG_TAG << "run - Thread ID : " << QThread::currentThreadId();
    int ret;
    ret = connectToServer();
    if(ret != NETWORK_NO_ERROR) {
        emit error(mId, mCurrentObject, mCurrentAction, ret, "Connect fail");
        cleanup();
        return;
    }

    QByteArray sentData;
    bool result = false;
    if(mCurrentObject == OBJECT_FETCH_APPLICATION_PARAMETER){
        result = NetworkEncryptionUtil::getFinalSentData(taskInfo.data, sentData, true);
    }else{
        result = NetworkEncryptionUtil::getFinalSentData(taskInfo.data, sentData);
    }
    if(!result){
        qDebug() << DBG_TAG << "Encrypt Data fail";
        emit error(mId, mCurrentObject, mCurrentAction, NETWORK_ERROR_ENCODE_FAIL, "Encrypt fail");
        cleanup();
        return;
    }

    ProtocolHead head;
    memcpy(head.mark, NETWORK_PROTOCOL_MARK, sizeof(head.mark));
    head.charset = SDISRuntimeContext::getInstance()->getProtocolCharset();
    head.format = SDISRuntimeContext::getInstance()->getProtocolFormat();
    head.mainVersion = NETWORK_PROTOCOL_MAIN_VERSION;
    head.subVersion = NETWORK_PROTOCOL_SUB_VERSION;
    head.result = 0;
    head.statusCode = 0;

    int len = 0;
    char protocolBuffer[48] = {0};
    QByteArray headBuf;
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    bool encrypt = false;
    bool emptyToken = false;
    if(authType == 1){
        encrypt = true;
    }else if(authType == 0){
        emptyToken = true;
    }
    if(mCurrentObject == OBJECT_FETCH_USER_ACCESS_TOKEN) {
        head.length = sentData.size();
        head.type = QString(TYPE_SECURITY).toInt();
        len = NetworkHeadTool::generateProtocolHead(head, encrypt, true, protocolBuffer);
    }else if(mCurrentObject == OBJECT_FETCH_APPLICATION_PARAMETER){
        head.length = sentData.size();
        head.type = QString(TYPE_APP_PARA).toInt();
        len = NetworkHeadTool::generateProtocolHead(head, encrypt, true, protocolBuffer);
    }else if(mCurrentType == TYPE_PUSH){
        head.length = sentData.size();
        head.type = QString(TYPE_PUSH).toInt();
        len = NetworkHeadTool::generateProtocolHead(head, encrypt, emptyToken, protocolBuffer);
    }else{
        head.type = taskInfo.type.toInt();
        head.length = sentData.size();
        len = NetworkHeadTool::generateProtocolHead(head, encrypt, emptyToken, protocolBuffer);
    }
    qDebug() << "-----------NetworkCommandThread::doTask-------send-----head len: "<< len;
    headBuf.append(protocolBuffer, len);

    ret = sendData(headBuf);
    if(ret < 0) {
        emit error(mId, mCurrentObject, mCurrentAction, NETWORK_ERROR_SEND_FAIL, "Send head fail");
        cleanup();
        return;
    }
    ret = sendData(sentData);
    if(ret < 0) {
        emit error(mId, mCurrentObject, mCurrentAction, NETWORK_ERROR_SEND_FAIL, "Send data fail");
        cleanup();
        return;
    }

    int responseLength = 0;
    ProtocolHead protocolHead;
    memset(&protocolHead, 0, sizeof(ProtocolHead));
    int headLen = 4;
    if(authType == 0){
        headLen = 16;
    }else if(authType == 1){
        headLen = 48;
    }
    memset((void *)protocolBuffer, 0, sizeof(protocolBuffer));
    ret = recvData(protocolBuffer, headLen);
    qDebug() << "-----------NetworkCommandThread::doTask-------recv-----head len: "<< headLen;
    NetworkHeadTool::decodeProtocolHead(protocolHead, protocolBuffer);
    if(strncmp(protocolHead.mark, NETWORK_PROTOCOL_MARK, strlen(NETWORK_PROTOCOL_MARK)) != 0){
        qDebug() << Q_FUNC_INFO << "emit  error111";
        emit error(mId, mCurrentObject, mCurrentAction, NETWORK_ERROR_RECV_FAIL, "Head protocol error");
        cleanup();
        return;
    }
    responseLength = protocolHead.length;

    qDebug() <<"-------NetworkCommandThread::doTask----000--------ret: " << ret;
    if(ret < 0) {
        qDebug() << Q_FUNC_INFO << "emit  error222";
        emit error(mId, mCurrentObject, mCurrentAction, NETWORK_ERROR_RECV_FAIL, "Recv head fail");
        cleanup();
        return;
    }

    QByteArray data;
    ret = recvData(data, responseLength);
    qDebug() <<"-------NetworkCommandThread::doTask----responseLength: " << responseLength;
    qDebug() <<"-------NetworkCommandThread::doTask-----111-------ret: " << ret;
    if(ret < 0) {
        qDebug() << Q_FUNC_INFO << "emit  error333";
        emit error(mId, mCurrentObject, mCurrentAction, NETWORK_ERROR_RECV_FAIL, "Recv data fail");
        cleanup();
        return;
    }

    /**
     * Decrypt data if it's necessary
     */
    QByteArray receivedData;
    if(mCurrentObject == OBJECT_FETCH_APPLICATION_PARAMETER){
        result = NetworkEncryptionUtil::getFinalReceivedData(data, receivedData, true);
    }else{
        result = NetworkEncryptionUtil::getFinalReceivedData(data, receivedData);
    }
    if(!result){
        qDebug() << DBG_TAG << "Decrypt Data fail";
        emit error(mId, mCurrentObject, mCurrentAction, NETWORK_ERROR_DECODE_FAIL, "Decrypt fail");
        cleanup();
        return;
    }
    emit response(mId, mCurrentObject, mCurrentType, mCurrentAction, receivedData, protocolHead);
    cleanup();
}
