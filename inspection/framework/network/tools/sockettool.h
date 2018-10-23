#ifndef SOCKETTOOL_H
#define SOCKETTOOL_H

#include "inetworktool.h"
#include <QMutex>

class SocketTool : public INetworkTool
{
public:
    SocketTool();
    ~SocketTool();
    bool isConnected();
    int connectToServer();
    int connectToServer(QString address, int port);
    int sendData(QByteArray& data);
    int sendData(char* data, int len);
    int recvData(char* data, const int& len);
    int recvData(QByteArray& data, const int& len);
    void disconnect();
    int getTimeout();
    void setTimeout(int value);
    bool hasDataIncome();

private:
    bool parseDNS(const QString& address, QString& result);
private:
    QMutex mSocketMutex; // socket线程锁
    QMutex mSocketSenderMutex; // socket线程锁
    QMutex mSocketReceiverMutex; // socket线程锁
    bool bConnected;  // socket连接状态
    int iTimeout;
    int mSocket;  // socket对象
    bool bDNSParsed;  // DNS解析标志
};

#endif // SOCKETTOOL_H
