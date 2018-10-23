#ifndef USBTOOL_H
#define USBTOOL_H

#include "inetworktool.h"
#include <QMutex>

class USBTool : public INetworkTool
{
public:
    USBTool();
    ~USBTool();
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
    QMutex mMutex; // socket线程锁
    bool bConnected;  // socket连接状态
    int iTimeout;
};

#endif // USBTOOL_H
