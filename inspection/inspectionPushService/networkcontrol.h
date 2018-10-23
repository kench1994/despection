#ifndef NETWORKCONTROL_H
#define NETWORKCONTROL_H

#include <QObject>
#include "cnetworkmanager.h"

class NetworkControl : public QObject
{
    Q_OBJECT
public:

    /**
     * 单例函数
     */
    static NetworkControl *getInstance(QObject *parent = 0);

    /**
     * 析构函数
     */
    ~NetworkControl();

    /**
     * @brief getNetworkConnectStatus : 网络链接状态
     * @return : "wifi":wlan； "cellular"：移动网络；"":无网络
     */
    QString getNetworkConnectStatus();

signals:
    /**
     * @brief networkConnectChanged : 网络链接状态变更信号
     * @param networkType : 网络链接状态
     */
    void networkConnectChanged(QString networkType);

public slots:
    /**
     * @brief slotNetworkStatusChanged : 网络链接状态变更槽函数
     * @param flag
     * @param type : 网络类型
     */
    void slotNetworkStatusChanged(bool flag, CNetworkManager::NetworkType type);

private:
    /**
     * 构造函数
     */
    NetworkControl(QObject *parent = 0);

    /**
     * 单例对象
     */
    static NetworkControl *m_pNetworkControl;

    /**
     * 链接网络类型
     */
    QString m_nConnectNetworkType;

    /**
     * 移动网络管理的对象
     */
    CNetworkManager m_nNetWorkManager;
};

#endif // NETWORKCONTROL_H
