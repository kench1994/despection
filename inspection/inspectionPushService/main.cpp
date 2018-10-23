#include <SyberosServiceCache>
#include "push_service_workspace.h"
#if 1
Q_DECL_EXPORT int main(int argc, char *argv[])
{
    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new PushService_Workspace());
}
#else
#include <QDebug>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include "push_define.h"
#include "pushinterface.h"
#include <QGuiApplication>
Q_DECL_EXPORT int main(int argc, char* argv[]){
    QGuiApplication app(argc, argv);

    QDBusConnection bus = QDBusConnection::sessionBus();
    PushInterface* interface = new PushInterface();
    // 在session bus上注册名为"com.syberos.inspectionService"的service
    if (!bus.registerService(PUSH_SERVICE_DBUS_SERVICE)) {
        qDebug() << "-------------PushService_Workspace::PushService_Workspace---------------" << __FILE__ <<__LINE__ << " dbus registerService error: " << bus.lastError().message();
        exit(1);
    }
    if (!bus.registerObject(PUSH_SERVICE_DBUS_PATH, interface, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)){
        qDebug() << "-------------PushService_Workspace::PushService_Workspace---------------" << __FILE__ <<__LINE__ << " dbus registerObject error: " << bus.lastError().message();
        exit(2);
    }
    app.exec();
}
#endif

