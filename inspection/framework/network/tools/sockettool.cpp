#include "sockettool.h"
#include "../../common/utils.h"
#include "../../config/sdisruntimecontext.h"
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
#include <QThread>

#define DBG_TAG "SocketTool >> "

SocketTool::SocketTool()
{
    iTimeout = NETWORK_TIMEOUT_ASYN;
    mSocket = 0;
    bConnected = false;
}

SocketTool::~SocketTool()
{
    if(isConnected()) {
        disconnect();
    }
}


bool SocketTool::isConnected()
{
    return bConnected;
}

bool SocketTool::parseDNS(const QString& address, QString& result)
{
    if(address.isEmpty()){
        return false;
    }
    QHostInfo info = QHostInfo::fromName(address);
    QList<QHostAddress> results = info.addresses();
    if(results.isEmpty()){
        qDebug() << DBG_TAG << "Coudn't get server's address";
        return false;
    }
    foreach (const QHostAddress& record, results) {
        QString tmp = record.toString();
        if(!tmp.isEmpty()){
            qDebug() << DBG_TAG << "DNS Parse successful - " << tmp;
            result = tmp;
            bDNSParsed = true;
            break;
        }
    }
    return true;
}

int SocketTool::connectToServer()
{
    qDebug() << Q_FUNC_INFO;
    QString address = SDISRuntimeContext::getInstance()->getServerAddress();
    int port = SDISRuntimeContext::getInstance()->getServerPort();
    return connectToServer(address, port);
}

int SocketTool::connectToServer(QString address, int port)
{
    qDebug() << Q_FUNC_INFO << "  address " << address << " port " << port;
    QMutexLocker sockLocker(&mSocketMutex);
    QString ipAddress;
    if(address.isEmpty() || port == 0){
        return NETWORK_ERROR_EMPTY_ADDRESS;
    }
    // Reset Socket
    if(bConnected){
        close(mSocket);
        mSocket = 0;
        bConnected = false;
    }
    // 2rd parse DNS
    if(!bDNSParsed){
        bool ret = parseDNS(address, ipAddress);
        if(!ret){
            // DNS parse failed
            return NETWORK_ERROR_DNS;
        }
    }
    // Create Socket
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mSocket == -1){
        qDebug() << Q_FUNC_INFO << "Create Socket Failed!";
        return NETWORK_ERROR_SOCKET;
    }

    // Set Paramters
    // TODO : Fetch IP address by address 'www.xxx.com'
    struct sockaddr_in s_add;
    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    QByteArray addrBytes = address.toLocal8Bit();
    s_add.sin_addr.s_addr= inet_addr(addrBytes.data());
    s_add.sin_port=htons(port);
    if(::connect(mSocket,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)) == -1)
    {
        qDebug() << DBG_TAG << "Connect fail! -- " << address;
        return NETWORK_ERROR_SOCKET_CONNECTION;
    }
    bConnected = true;
    if(iTimeout > 0) {
        struct timeval timeout;
        timeout.tv_sec = iTimeout;
        timeout.tv_usec = 0;
        int result = setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout.tv_sec,sizeof(struct timeval));
        if (result < 0)
        {
            qDebug() << DBG_TAG << "setsockopt!";
        }
        result = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout.tv_sec,sizeof(struct timeval));
        if (result < 0)
        {
            qDebug() << DBG_TAG << "setsockopt!";
        }
    }
    return NETWORK_NO_ERROR;
}

int SocketTool::sendData(QByteArray& data)
{
    QMutexLocker sockLocker(&mSocketSenderMutex);
    int tmp = 0;
    int ret = 0;
    char* buf = (char*)data.data();
    int len = data.size();
    while(tmp < len){
        int sent;
        int sendLen = 0;
        if((len - tmp) >= 1024){
            sendLen = 1024;
        }else{
            sendLen = len - tmp;
        }
        sent = ::send(mSocket, buf, sendLen, 0);
        if(sent <= 0){
            ret = sent;
            break;
        }
        buf += sent;
        tmp += sent;
        qDebug() << Q_FUNC_INFO << tmp;
        ret = tmp;
    }
    return ret;
}

int SocketTool::sendData(char* data, int len)
{
    QMutexLocker sockLocker(&mSocketSenderMutex);
    int tmp = 0;
    int ret = 0;
    char* buf = (char*)data;
    while(tmp < len){
        int sent;
        int sendLen = 0;
        if((len - tmp) >= 1024){
            sendLen = 1024;
        }else{
            sendLen = len - tmp;
        }
        sent = ::send(mSocket, buf, sendLen, 0);
        if(sent <= 0){
            ret = sent;
            break;
        }
        buf += sent;
        tmp += sent;
        ret = tmp;
    }
    return ret;
}

int SocketTool::recvData(char* data, const int& len)
{
    QMutexLocker sockLocker(&mSocketReceiverMutex);
    int tmp = 0;
    int ret = 0;
    char* buf = data;
    while(tmp < len){
        int recv;
        if(iTimeout > 0) {
            ///< 设置手动超时机制
            fd_set rfds;
            struct timeval tv;
            int retval;
            FD_ZERO(&rfds);
            FD_SET(mSocket, &rfds);
            tv.tv_sec = iTimeout;
            tv.tv_usec = 0;
            retval = select(mSocket + 1, &rfds, NULL, NULL, &tv);
            if (retval <= 0){  ///< Socket Error or Timeout
                ret = -1;
                qDebug() << Q_FUNC_INFO <<"   errno  "<< errno <<"  retval: "<< retval;
                break;
            }
            if(!FD_ISSET(mSocket, &rfds)){
                continue;
            }
        }
        recv = ::recv(mSocket, buf, len - tmp, 0);
        if(recv <= 0){
            ret = recv;
            break;
        }
        buf += recv;
        tmp += recv;
        ret = tmp;
    }
    return ret;
}

int SocketTool::recvData(QByteArray& data, const int& len)
{
    qDebug() << "-------------SocketTool::recvData----------Thread ID  :" << QThread::currentThread();
    QMutexLocker sockLocker(&mSocketReceiverMutex);
    int tmp = 0;
    int ret = 0;
    data.resize(len);
    char* buf = data.data();
    while(tmp < len){
        int recv;
        if(iTimeout > 0) {
            ///< 设置手动超时机制
            fd_set rfds;
            struct timeval tv;
            int retval;
            FD_ZERO(&rfds);
            FD_SET(mSocket, &rfds);
            tv.tv_sec = iTimeout;
            tv.tv_usec = 0;
            retval = select(mSocket + 1, &rfds, NULL, NULL, &tv);
            if (retval <= 0){  ///< Socket Error or Timeout
                ret = -1;
                qDebug() << Q_FUNC_INFO <<"   errno  "<< errno <<"  retval: "<< retval;
                break;
            }
            if(!FD_ISSET(mSocket, &rfds)){
                continue;
            }
        }
        recv = ::recv(mSocket, buf, len - tmp, 0);
        if(recv <= 0){
            ret = recv;
            break;
        }
        buf += recv;
        tmp += recv;
        ret = tmp;
    }
    return ret;
}

void SocketTool::disconnect()
{
    QMutexLocker sockLocker(&mSocketMutex);
    bConnected = false;
    if(mSocket != 0) {
        close(mSocket);
        mSocket = 0;
    }
}

int SocketTool::getTimeout()
{
    return iTimeout;
}

void SocketTool::setTimeout(int value)
{
    iTimeout = value;
}

bool SocketTool::hasDataIncome()
{
    return false;
}
