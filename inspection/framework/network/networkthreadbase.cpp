#include "networkthreadbase.h"
#include <QMutexLocker>
#include <QTimer>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <QHostInfo>
#include <QDebug>
#include <QCryptographicHash>
#include "./tools/networktoolfactory.h"
#include "../config/sdisruntimecontext.h"
#define DBG_TAG "NetworkThreadBase : "

NetworkThreadBase::NetworkThreadBase(QObject *parent) :
    QThread(parent)
{
    initialize();
    networkType = SOCKET;
    socket = NetworkToolFactory::createTool(SOCKET);
    sslsocket = NetworkToolFactory::createTool(SSLSOCKET);
    usb = NetworkToolFactory::createTool(USB);
}

bool NetworkThreadBase::isConnected()
{
    bool ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->isConnected();
        }else{
            ret = socket->isConnected();
        }
        break;
    case USB:
        ret = usb->isConnected();
        break;
    default:
        break;
    }
    return ret;
}

int NetworkThreadBase::connectToServer()
{
    int ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->connectToServer();
        }else{
            ret = socket->connectToServer();
        }
        break;
    case USB:
        ret = usb->connectToServer();
        break;
    default:
        break;
    }
    return ret;
}

int NetworkThreadBase::connectToServer(QString address, int port)
{
    int ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->connectToServer(address, port);
        }else{
            ret = socket->connectToServer(address, port);
        }
        break;
    case USB:
        ret = usb->connectToServer(address, port);
        break;
    default:
        break;
    }
    return ret;
}

int NetworkThreadBase::sendData(QByteArray& data)
{
    int ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->sendData(data);
        }else{
            ret = socket->sendData(data);
        }
        break;
    case USB:
        ret = usb->sendData(data);
        break;
    default:
        break;
    }
    return ret;
}

int NetworkThreadBase::sendData(char* data, int len)
{
    int ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->sendData(data, len);
        }else{
            ret = socket->sendData(data, len);
        }
        break;
    case USB:
        ret = usb->sendData(data, len);
        break;
    default:
        break;
    }
    return ret;
}

int NetworkThreadBase::recvData(char* data, const int& len)
{
    int ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->recvData(data, len);
        }else{
            ret = socket->recvData(data, len);
        }
        break;
    case USB:
        ret = usb->recvData(data, len);
        break;
    default:
        break;
    }
    return ret;
}

int NetworkThreadBase::recvData(QByteArray& data, const int& len)
{
    int ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->recvData(data, len);
        }else{
            ret = socket->recvData(data, len);
        }
        break;
    case USB:
        ret = usb->recvData(data, len);
        break;
    default:
        break;
    }
    return ret;
}

void NetworkThreadBase::disconnect()
{
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            sslsocket->disconnect();
        }else{
            socket->disconnect();
        }
        break;
    case USB:
        usb->disconnect();
        break;
    default:
        break;
    }
}

int NetworkThreadBase::getTimeout()
{
    int ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->getTimeout();
        }else{
            ret = socket->getTimeout();
        }
        break;
    case USB:
        ret = usb->getTimeout();
        break;
    default:
        break;
    }
    return ret;
}

void NetworkThreadBase::setTimeout(int value)
{
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            sslsocket->setTimeout(value);
        }else{
            socket->setTimeout(value);
        }
        break;
    case USB:
        usb->setTimeout(value);
        break;
    default:
        break;
    }
}

bool NetworkThreadBase::hasDataIncome()
{
    bool ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    switch (networkType) {
    case SOCKET:
        if(encryptType == 2){
            ret = sslsocket->hasDataIncome();
        }else{
            ret = socket->hasDataIncome();
        }
        break;
    case USB:
        ret = usb->hasDataIncome();
        break;
    default:
        break;
    }
    return ret;
}

void NetworkThreadBase::initialize()
{
}


void NetworkThreadBase::cleanup(){
    if(isConnected()) {
        disconnect();
    }
}




