#ifndef PUSHMANAGER_H
#define PUSHMANAGER_H

#include <QObject>
#include "../framework/network/tools/inetworktool.h"
#include "pushauthority.h"
#include "networkreceiver.h"
#include "networksender.h"
#include "../framework/common/networkstatus.h"
#include "heartbeatmanager.h"

class PushManager : public QObject
{
    Q_OBJECT
public:
    /**
     * 单例函数
     */
    static PushManager *getInstance(QObject* parent);

    /**
     * 设置服务器参数.
     * @param host 地址
     * @param port 端口
     * @return 返回是否修改
     */
    bool setServerAddress(const QString &host, const int &port);


    /**
     * 启动服务
     */
    void startService();

    /**
     * 停止服务
     */
    void stopService();

    /**
     * 切换网络的回调
     */
    void switchAddress();

    /**
     * 切换Push订阅
     */
    void switchAuthority();

    /**
      * 心跳包时间到的触发函数
      */
    void onHeartbeat();

    /**
      * 设置订阅信息
      * @param pushAuthority 订阅信息
      */
    void setPushAuthority(const PushAuthority &pushAuthority, const QString &action);

    /**
      * 设置订阅信息
      * @param action 订阅信息
      */
    void setPushAuthority(const QString &action);

    /**
      * 获取运行状态
      * @return true 运行 false 非运行
      */
    bool isBeRunning();

signals:
    void handlePushData(const DbusPackage &dbusPackage);

public slots:
    /**
     * 开始接收Push
     */
    void startPush();

    /**
      * 重启Push服务
      */
    void restartPush();

    /**
      * 停止Push
      */
    void stopPush();

    /**
      * 初始化模块
      */
    void initCompleteInner();

    /**
      * 服务器连接后续处理
      * @param result true 成功 false 失败
      */
    void serverConnectedInner(const bool &result);

    /**
      * 心跳包结果处理函数
      * @param success true 成功 false 失败
      */
    void heartbeatResultInner(const bool &result);

    /**
      * 错误内部处理
      * @param type  错误类型
      * @param description  错误描述
      */
    void onErrorInner(const int &type, const QString &description);

    /**
      * 处理服务器发送的PushData
      * @param data PushData
      */
    void handlePushDataInner(const DataPackage &dataPackage);

    /**
     * @brief networkStatusChanged : 网络链接状态变更槽函数
     * @param status : 网络状态
     */
    void networkStatusChanged(QString status);

protected:

    /**
      * 服务器地址
      */
    QString address;

    /**
      * 服务器端口
      */
    int port;

private:
    /**
      * 构造函数
      */
    PushManager(QObject *parent = 0);

   /**
      * 保存注册信息
      */
    void saveConfig();

    /**
      * 获取上次注册信息
      */
    void loadConfig();

    /**
      * 连接Push服务器
      */
    void connectServer();


    /**
      * 开始订阅
      */
    void subscribePush();
    static PushManager* pushManager; ///<单例对象
    NetworkStatus *m_pNetworkStatus; ///<网络状态获取对象
    INetworkTool *networkTool; ///<网络操作对象
    NetworkSender *networkSender;///<发送软件包用的连接
    NetworkReceiver *networkReceiver;///<接收软件包用的连接
    bool beInit;///<线程初始化标志
    bool beRunning;///<运行态标志
    HeartbeatManager *heartbeatManager; ///<心跳时间管理模块
    PushAuthority pushAuthority;///<Push订阅信息
    QString clientAction;///<客户端接收用的action
    bool isConnecting;
};

#endif // PUSHMANAGER_H
