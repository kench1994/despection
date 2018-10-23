#ifndef SSLSOCKETTOOL_H
#define SSLSOCKETTOOL_H
#include "inetworktool.h"
#include <openssl/ssl.h>
#include <QMutex>

class SSLSocketTool: public INetworkTool
{
public:
    SSLSocketTool();
    ~SSLSocketTool();
    virtual bool isConnected();
    virtual int connectToServer();
    virtual int connectToServer(QString address, int port);
    virtual int sendData(QByteArray& data);
    virtual int sendData(char* data, int len);
    virtual int recvData(char* data, const int& len);
    virtual int recvData(QByteArray& data, const int& len);
    virtual void disconnect();
    virtual int getTimeout();
    virtual void setTimeout(int value);
    virtual bool hasDataIncome();

private:
    QMutex mSocketMutex; // socket线程锁
    QMutex mSocketSenderMutex; // socket线程锁
    QMutex mSocketReceiverMutex; // socket线程锁
    bool bConnected;  // socket连接状态
    int iTimeout;
    SSL* pSSL;  // SSL对象
    int mSocket;  // socket对象
    SSL_CTX* pCTX;
};

#endif // SSLSOCKETTOOL_H
