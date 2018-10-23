#ifndef NETWORKTHREADBASE_H
#define NETWORKTHREADBASE_H

#include "../common/utils.h"
#include "./tools/inetworktool.h"
#include <QObject>
#include <QThread>
#include <QMap>
#include <QByteArray>
#include <QMutex>
#include <QString>

/*!
  \class NetworkConnectionThread
  \inModule 网络模块
  \brief 底层负责数据收发的线程模块
  \ingroup network
  NetworkConnectionThread类实现了发送队列以及关于数据传输的相关操作
*/
class NetworkThreadBase : public QThread
{
    Q_OBJECT
public:
    explicit NetworkThreadBase(QObject *parent = 0);

protected:
    void initialize();
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
    virtual void cleanup();

protected:
    NetworkType networkType;
    INetworkTool* socket;
    INetworkTool* sslsocket;
    INetworkTool* usb;
};

#endif // NETWORKTHREADBASE_H
