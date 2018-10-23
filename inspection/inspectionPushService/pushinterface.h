#ifndef PUSHINTERFACE_H
#define PUSHINTERFACE_H

#include <QObject>
#include "push_define.h"
#include "../framework/common/networkstatus.h"
#include "pushmanager.h"

class PushInterface : public QObject
{
    Q_OBJECT
    // 定义Interface名称为"com.syberos.inspectionPushService.interface"
    Q_CLASSINFO("D-Bus Interface", PUSH_SERVICE_DBUS_INTERFACE)
public:

    /**
     * 构造函数
     */
    explicit PushInterface(QObject *parent = 0);

signals:
    void handlePushData(const DbusPackage &dbusPackage);

public slots:
    /**
     * Push注册处理函数
     */
    void setRegisterStatus(const QVariantMap &varMap);

    /**
     * 网络状态变化函数
     */
    void networkStatus(const QString &status);

    void setStatus(const QString& functionType, const QVariantMap& map);

private:

    /**
     * Push管理对象
     */
    PushManager *pushManager;///<Push管理对象

    /**
     * 监控网络状态对象
     */
    NetworkStatus *m_pNetworkStatus; ///<监控网络状态对象

    /**
     * 初始化函数
     */
    void initialize();


};

#endif // PUSHINTERFACE_H
