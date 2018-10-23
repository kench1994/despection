#include "push_service_workspace.h"
#include <QDebug>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include "push_define.h"
#include "pushinterface.h"
#include "../framework/config/sdisruntimecontext.h"
#include <framework/cpackageinfo.h>
#include <framework/csystempackagemanager.h>

QString gAppInstallPath = "";

/**
 * 构造函数
 */
PushService_Workspace::PushService_Workspace()
    : CWorkspace()
{
    CSystemPackageManager mng;
    QSharedPointer<CPackageInfo> pkgInfo = mng.packageInfo("com.syberos.deviceInspection");
    gAppInstallPath = pkgInfo->installPath();

    QDBusConnection bus = QDBusConnection::sessionBus();

    interface = new PushInterface(this);
    // 在session bus上注册名为"com.syberos.inspectionService"的service
    if (!bus.registerService(PUSH_SERVICE_DBUS_SERVICE)) {
        qDebug() << "-------------PushService_Workspace::PushService_Workspace---------------" << __FILE__ <<__LINE__ << " dbus registerService error: " << bus.lastError().message();
        exit(1);
    }
    if (!bus.registerObject(PUSH_SERVICE_DBUS_PATH, interface, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)){
        qDebug() << "-------------PushService_Workspace::PushService_Workspace---------------" << __FILE__ <<__LINE__ << " dbus registerObject error: " << bus.lastError().message();
        exit(2);
    }
}

void PushService_Workspace::onLaunchComplete(Option option, const QStringList &params)
{
    switch (option) {
    case CWorkspace::HOME:
        qDebug() << "-------------PushService_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by HOME params:" << params;
        break;
    case CWorkspace::URL:
        qDebug() << "-------------PushService_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by URL";
        break;
    case CWorkspace::EVENT:
        qDebug() << "-------------PushService_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by EVENT params:" << params;
        break;
    case CWorkspace::DOCUMENT:
        qDebug() << "-------------PushService_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by DOCUMENT";
        break;
    case CWorkspace::MULTIDOCS:
        qDebug() << "-------------PushService_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by MULTIDOCS";
        break;
    default:
        break;
    }
    Q_UNUSED(params);
}

void PushService_Workspace::open(const QStringList &params)
{
    qDebug() << "-------------PushService_Workspace::open---------------" << __FILE__ <<__LINE__ << " start by open params:" << params;
    if(params.size() == 2 && params.at(0) == "inspectionPushService"){
        QDBusMessage m = QDBusMessage::createMethodCall(PUSH_SERVICE_DBUS_SERVICE,
                                                        PUSH_SERVICE_DBUS_PATH,
                                                        PUSH_SERVICE_DBUS_INTERFACE,
                                                        SET_REGISTER_STATUS);
        QVariantMap varMap;
        varMap.insert(INTENT_ACTION, HEARTBEA_ACTION);
        m  << varMap;
        QDBusConnection::sessionBus().asyncCall(m);
    }
}


