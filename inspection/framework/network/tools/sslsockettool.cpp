#include "sslsockettool.h"
#include "../../common/utils.h"
#include "../../config/sdisruntimecontext.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/err.h>
#include <framework/cpackageinfo.h>
#include <framework/csystempackagemanager.h>
#include <QDebug>

#define CACERT "certificate.pem"

#define DBG_TAG "SSLSocketTool >> "

extern QString gAppInstallPath;
SSLSocketTool::SSLSocketTool(){

    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();

    // 我们使用 TLS1.0 or SSL 3.0
    const SSL_METHOD *meth = SSLv3_client_method();
    SSL_CTX *ctx = SSL_CTX_new(meth);
    if(ctx == NULL){
        qDebug() << DBG_TAG << "New SSL CTX Failed!";
    }
    // 要求校验对方证书
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);


    // 加载CA的证书
    QString certPath = gAppInstallPath + "/" + QString(CACERT);
    int ret = SSL_CTX_load_verify_locations(ctx, certPath.toLocal8Bit().data(), NULL);
    if(ret != 1){
        SSL_CTX_free(ctx);
        qDebug() << DBG_TAG << "Load Verify Locations Failed!";
        qDebug() << DBG_TAG << "Reason Error: " << ERR_reason_error_string(ERR_get_error());
        qDebug() << DBG_TAG << "Lib    Error: " << ERR_lib_error_string(ERR_get_error());
    }
    pCTX = ctx;
    bConnected = false;
    iTimeout = 0;
    mSocket = 0;
    pSSL = NULL;
}

SSLSocketTool::~SSLSocketTool(){
    if(isConnected()) {
        disconnect();
    }
    if(pCTX != NULL){
        SSL_CTX_free(pCTX);
        pCTX = NULL;
    }
}

bool SSLSocketTool::hasDataIncome(){
    return false;
}

void SSLSocketTool::setTimeout(int value){
    iTimeout = value;
}

int SSLSocketTool::getTimeout(){
    return iTimeout;
}

void SSLSocketTool::disconnect(){

    QMutexLocker sockLocker(&mSocketMutex);
    bConnected = false;
    if(pSSL != NULL){
        SSL_free(pSSL);
        pSSL = NULL;
    }
    if(mSocket != 0) {
        close(mSocket);
        mSocket = 0;
    }
}

int SSLSocketTool::recvData(QByteArray &data, const int &len) {
    QMutexLocker sockLocker(&mSocketReceiverMutex);
    int tmp = 0;
    int ret = 0;
    data.resize(len);
    char* buf = data.data();
    while(tmp < len){
        int recv;
        /*
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
                break;
            }
            if(!FD_ISSET(mSocket, &rfds)){
                continue;
            }
        }
        */
        recv = SSL_read(pSSL, buf, len - tmp);
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

int SSLSocketTool::recvData(char *data, const int &len){
    QMutexLocker sockLocker(&mSocketReceiverMutex);
    int tmp = 0;
    int ret = 0;
    char* buf = data;
    while(tmp < len){
        int recv;
        /*
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
                break;
            }
            if(!FD_ISSET(mSocket, &rfds)){
                continue;
            }
        }
        */
        recv = SSL_read(pSSL, buf, len - tmp);
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

int SSLSocketTool::sendData(char *data, int len){
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
        sent = SSL_write(pSSL, buf, sendLen);
        if(sent <= 0){
            qDebug() << DBG_TAG << "Send data Failed! sent:" << sent;
            qDebug() << DBG_TAG << "Reason Error: " << ERR_reason_error_string(ERR_get_error());
            qDebug() << DBG_TAG << "Lib    Error: " << ERR_lib_error_string(ERR_get_error());
            ret = sent;
            break;
        }
        buf += sent;
        tmp += sent;
        ret = tmp;
    }
    return ret;
}

int SSLSocketTool::sendData(QByteArray &data){
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
        sent = SSL_write(pSSL, buf, sendLen);
        if(sent <= 0){
            qDebug() << DBG_TAG << "Send data Failed! sent:" << sent;
            qDebug() << DBG_TAG << "Reason Error: " << ERR_reason_error_string(ERR_get_error());
            qDebug() << DBG_TAG << "Lib    Error: " << ERR_lib_error_string(ERR_get_error());
            ret = sent;
            break;
        }
        buf += sent;
        tmp += sent;
        qDebug() << DBG_TAG << "Send : " << tmp;
        ret = tmp;
    }
    return ret;
}

int SSLSocketTool::connectToServer(QString address, int port){

    // Reset Socket
    if(isConnected()) {
        disconnect();
    }

    //创建Socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        qDebug() << DBG_TAG << "Create Socket Failed!";
        return NETWORK_ERROR_SOCKET;
    }
    // Set Paramters
    // TODO : Fetch IP address by address 'www.xxx.com'
    struct sockaddr_in s_add;
    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family = AF_INET;
    s_add.sin_addr.s_addr = inet_addr(address.toLocal8Bit().data());
    s_add.sin_port = htons(port);
    if(::connect(sock,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)) == -1){
        qDebug() << DBG_TAG << "Connect fail! -- " << address;
        return NETWORK_ERROR_SOCKET_CONNECTION;
    }

    if(iTimeout > 0) {
        struct timeval timeout;
        timeout.tv_sec = iTimeout;
        timeout.tv_usec = 0;
        int result = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout.tv_sec,sizeof(struct timeval));
        if (result < 0){
            qDebug() << DBG_TAG << "setsockopt!";
        }
        result = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout.tv_sec,sizeof(struct timeval));
        if (result < 0){
            qDebug() << DBG_TAG << "setsockopt!";
        }
    }

    //将连接付给SSL
    SSL *ssl = SSL_new(pCTX);
    if(ssl == NULL){
        close(sock);
        qDebug() << DBG_TAG << "New SSL Failed!";
        return NETWORK_ERROR_SOCKET;
    }
    int ret = SSL_set_fd(ssl, sock);
    if(ret != 1){
        close(sock);
        SSL_free(ssl);
        qDebug() << DBG_TAG << "SSL Set FD Failed!";
        qDebug() << DBG_TAG << "Reason Error: " << ERR_reason_error_string(ERR_get_error());
        qDebug() << DBG_TAG << "Lib    Error: " << ERR_lib_error_string(ERR_get_error());
        return NETWORK_ERROR_SOCKET;
    }

    ret = SSL_connect(ssl);
    if(ret != 1){
        close(sock);
        SSL_free(ssl);
        qDebug() << DBG_TAG << "SSL Connect Failed!";
        qDebug() << DBG_TAG << "Reason Error: " << ERR_reason_error_string(ERR_get_error());
        qDebug() << DBG_TAG << "Lib    Error: " << ERR_lib_error_string(ERR_get_error());
        return NETWORK_ERROR_SOCKET;
    }
    mSocket = sock;
    pSSL = ssl;
    bConnected = true;
    return NETWORK_NO_ERROR;
}

int SSLSocketTool::connectToServer(){
    qDebug() << DBG_TAG << "connectToServer ";
    QString address = SDISRuntimeContext::getInstance()->getServerAddress();
    int port = SDISRuntimeContext::getInstance()->getServerPort();
    if(address.isEmpty() || port == 0){
        return NETWORK_ERROR_EMPTY_ADDRESS;
    }
    return connectToServer(address, port);
}

bool SSLSocketTool::isConnected(){
    return bConnected;
}
