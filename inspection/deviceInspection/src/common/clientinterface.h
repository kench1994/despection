#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QObject>
#include <QVariantMap>
#include "../../../framework/common/utils.h"
class QDBusPendingCallWatcher;
class ClientInterface : public QObject
{
    Q_OBJECT
        // 定义Interface名称为"com.syberos.inspectionClient.interface"
        Q_CLASSINFO("D-Bus Interface", CLIENT_DBUS_INTERFACE)
public:
    ClientInterface(QObject *parent = 0);
    ~ClientInterface();
    bool sendData(const DataPackage &dataPackage, const QString &action, const QString &functionType);
    void setStatus(const QString &statusType, const QVariantMap &map);
public slots:
    void result(QString objects, QString action, bool result, int error, QVariantMap map);
    void resultDbusPackage(QString objects, QString action, bool result, int error, DbusPackage map);
    void messageUpdate(QString messageType);
    void callFinishedSlot(QDBusPendingCallWatcher *call);
signals:
    void response(QString objects, QString action, bool result, int error, DbusPackage map);
    void messageUpdated(QString operType);
};

#endif // CLIENTINTERFACE_H
