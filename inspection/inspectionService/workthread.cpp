#include "workthread.h"
#include "./common/dataoperationtype.h"
#include <unistd.h>
WorkThread::WorkThread(QObject *parent) :
    QThread(parent)
{
    this->moveToThread(this);
}

void WorkThread::run()
{
    qRegisterMetaType<DataPackage>("DataPackage");
    qRegisterMetaType<ProtocolHead>("ProtocolHead");
    qRegisterMetaType<BusinessMessageType>("BusinessMessageType");
    // 用于建立到session bus的连接, 在session bus上注册名为"com.syberos.inspectionService"的service
    QDBusConnection bus = QDBusConnection::sessionBus();
    while (!bus.registerService(SERVICE_DBUS_SERVICE)) {
        qDebug() << "service dbus registerService error" <<bus.lastError().message();
        usleep(100000);
    }
    m_pInspectionInterface = new InspectionInterface();
    bus.registerObject(SERVICE_DBUS_PATH, m_pInspectionInterface, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
    exec();
}
