#ifndef NETWORKSENDER_H
#define NETWORKSENDER_H


#include <QThread>
#include "../framework/network/tools/inetworktool.h"
#include "../framework/dataManager/datahandler.h"
#include "pushauthority.h"

class NetworkSender : public QThread
{
    Q_OBJECT

public:
    /**
     * 构造函数
     *
     * @param networkTool 网络处理工具模块
     */
    NetworkSender(INetworkTool *networkTool);

    /**
     * 析构函数
     */
    ~NetworkSender();

    /**
     * 获取初始化标记
     *
     * @return true 初始化完成  false 未完成
     */
    bool isInited();

    /**
     * 设置初始化标记
     *
     * @param val true 初始化完成  false 未完成
     */
    void setInited(bool val);

    /**
     * 发送心跳包
     */
    void sendHeartbeat();

    /**
     * 注销Push
     */
    void unregisterPush();


    /**
        * 取消订阅
        */
    void unsubscribe();

    /**
     * @brief subscribe : 订阅系统消息
     */
    void subscribe();

    /**
     * 退出线程
     */
    void quitTread();

    /**
     * 线程处理函数
     */
    void run();

public slots:

    /**
     * 开始Push会话
     */
    void initPushAuthorityAndKeepAlive();

    /**
     * 发送心跳包 - 内部实现
     */
    void sendHeartbeatInner();

    /**
     * 发送注销Push指令
     */
    void unregisterPushInner();
signals:

    /**
     * 初始化长链接信号
     */
    void initKeepAlive();

    /**
     * 退出线程信号
     */
    void cleanThread();

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
        * 注册信息
        */
       PushAuthority *pushAuthority;

       /**
        * 生成长连接请求指令
        *
        * @return bool
        */
       bool generateKeepAlivePackage(QByteArray &data);

       /**
        * 发送数据
        *
        * @param data 数据
        * @return true 成功 false 失败
        */
       bool sendData(QByteArray &data);

       /**
        * 生成身份注册指令
        *
        * @return bool
        */
       bool generatePushAuthorityPackage(QByteArray &data);

       /**
        * 生成订阅信息包
        *
        * @return bool
        */
       bool generateSubscribePackage(const QString &user, const QString &topic, QByteArray &data);

       /**
        * 生成注销数据包
        *
        * @return bool
        */
       bool generateUnregisterPackage(QByteArray &data);

       /**
        * 生成退订多有订阅指令
        *
        * @return bool
        */
       bool generateUnsubscribePackage(const QString &user, QByteArray &data);

       /**
        * 发送长连接指令
        * 向服务器注册设备ID，以便能够接受业务消息(需要app将设备ID作为updateTocken发送给服务器)
        */
       void initPushAuthorityAndKeepAliveInner();

};

#endif // NETWORKSENDER_H
