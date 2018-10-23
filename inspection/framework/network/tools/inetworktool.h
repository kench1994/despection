#ifndef INETWORKTOOL_H
#define INETWORKTOOL_H

#include <QString>
#include <QByteArray>

class INetworkTool
{
public:
    virtual bool isConnected() = 0;
    virtual int connectToServer() = 0;
    virtual int connectToServer(QString address, int port) = 0;
    virtual int sendData(QByteArray& data) = 0;
    virtual int sendData(char* data, int size) = 0;
    virtual int recvData(char* data, const int& len) = 0;
    virtual int recvData(QByteArray& data, const int& len) = 0;
    virtual void disconnect() = 0;
    virtual int getTimeout() = 0;
    virtual void setTimeout(int value) = 0;
    virtual bool hasDataIncome() = 0;
};

#endif // INETWORKTOOL_H
