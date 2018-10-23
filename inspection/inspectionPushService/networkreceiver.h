#ifndef NETWORKRECEIVER_H
#define NETWORKRECEIVER_H

#include <QThread>
#include "../framework/network/tools/inetworktool.h"
#include "../framework/dataManager/datahandler.h"

class NetworkReceiver:public QThread
{
    Q_OBJECT
public:
    /**
     * 构造函数
     * @param networkTool 网络处理工具模块
     */
    NetworkReceiver(INetworkTool *networkTool);

    /**
     * 析构函数
     */
    ~NetworkReceiver();

    /**
     * 获取初始化标记
     * @return true 初始化完成  false 未完成
     */
    bool isInited();

    /**
     * 设置初始化标记
     * @param val true 初始化完成  false 未完成
     */
    void setInited(bool val);

    /**
     * 开始下次接收
     */
    void startReception();

    /**
     * 退出线程
     */
    void quitThread();


    /**
     * 线程处理函数
     */
    void run();

public slots:

    /**
     * 接收是数据的Runnable
     */
    void startReceptionInner();

signals:
    /**
     * 初始化完成信号
     */
    void initComplete();

    /**
     * 错误信号
     */
    void error(const int &type, const QString &description);

    /**
     * 心跳信号
     */
    void heartbeatResult(const bool &result);

    /**
     * Push消息信号
     */
    void handlePushData(const DataPackage &dataPackage);

private:
    /**
        * 初始化标记
        */
       bool beInited;

       /**
        * 网络操作对象
        */
       INetworkTool *networkTool;

       /**
        * 协议解析模块
        */
       DataHandler *dataManager;

       /**
        * 接收Push信息
        */
       bool receiveData();

       /**
       * 处理数据包
       * @param dataPackage 服务器的数据包
       */
       void handleData(const DataPackage &dataPackage);

};

#endif // NETWORKRECEIVER_H
