#include "clientinterface.h"
#include "../../../framework/common/commondbustypes.h"
#include <QDebug>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCallWatcher>
#include <QDBusError>
ClientInterface::ClientInterface(QObject *parent):QObject(parent)
{
    registerCommonDataTypes();
    //接收服务进程发送的信号
    QDBusConnection::sessionBus().connect(QString(), SERVICE_DBUS_PATH,
                                          SERVICE_DBUS_INTERFACE,
                                          QString("responseToClient"),
                                          this,SIGNAL(response(QString,QString,bool,int,DbusPackage)));

    QDBusConnection::sessionBus().connect(QString(), SERVICE_DBUS_PATH,
                                          SERVICE_DBUS_INTERFACE,
                                          QString("messageUpdated"),
                                          this,SLOT(messageUpdate(QString)));

}

ClientInterface::~ClientInterface()
{
    QDBusConnection::sessionBus().disconnect(QString(), SERVICE_DBUS_PATH,
                                          SERVICE_DBUS_INTERFACE,
                                          QString("responseToClient"),
                                          this,SIGNAL(response(QString,QString,bool,int,DbusPackage)));

    QDBusConnection::sessionBus().disconnect(QString(), SERVICE_DBUS_PATH,
                                          SERVICE_DBUS_INTERFACE,
                                          QString("messageUpdated"),
                                          this,SLOT(messageUpdate(QString)));
}

bool ClientInterface::sendData(const DataPackage &dataPackage, const QString &action, const QString &functionType)
{
    qDebug() << Q_FUNC_INFO << " action " << action << "  functionType " << functionType;
    QDBusMessage m = QDBusMessage::createMethodCall(SERVICE_DBUS_SERVICE, SERVICE_DBUS_PATH, SERVICE_DBUS_INTERFACE, "receiveData");
    qDebug() << Q_FUNC_INFO << m.errorMessage() << m.type();
    QVariant varPkg, varAction, varFunctionType;
    varPkg.setValue(dataPackage);
    varAction.setValue(action);
    varFunctionType.setValue(functionType);
    m << varPkg <<varAction<<varFunctionType;
    QDBusPendingCall  pendingCall = QDBusConnection::sessionBus().asyncCall(m);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(callFinishedSlot(QDBusPendingCallWatcher*)));

    return true;
}

void ClientInterface::callFinishedSlot(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<> reply = *call;
        if (reply.isError()) {
            qDebug() << Q_FUNC_INFO <<  reply.error().type() << reply.error()     <<"  error " << reply.reply().errorMessage() << reply.reply().type();
            if(reply.error().type() == QDBusError::ServiceUnknown){
                DbusPackage map;
                response("", "", false, DBUS_ERROR_SERVICE_UNKNOWN, map);
            }
        } else {
            qDebug() << Q_FUNC_INFO << "  ok";
        }
        call->deleteLater();
}

void ClientInterface::setStatus(const QString &statusType, const QVariantMap &map)
{
    qDebug() << "ClientInterface::setStatus  " << statusType;
    QDBusMessage m = QDBusMessage::createSignal(CLIENT_DBUS_PATH,  CLIENT_DBUS_INTERFACE, "statusUpdated");
    QVariant varType, varMap;
    varType.setValue(statusType);
    varMap.setValue(map);
    m << varType << varMap;
    QDBusConnection::sessionBus().send(m);
}

void ClientInterface::result(QString objects, QString action, bool result, int error, QVariantMap map)
{
    qDebug() << "ClientInterface::result  " << map;
}
void ClientInterface::resultDbusPackage(QString objects, QString action, bool result, int error, DbusPackage map)
{
    qDebug() << "ClientInterface::resultDbusPackage  " << map.otherMap;
}

void ClientInterface::messageUpdate(QString messageType)
{
    qDebug() << "-------ClientInterface::messageUpdate----------messageType: " << messageType;
    emit messageUpdated(messageType);
}

