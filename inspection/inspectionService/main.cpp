#include <SyberosServiceCache>
#include "service_workspace.h"
#if 1
Q_DECL_EXPORT int main(int argc, char *argv[])
{
    return SYBEROS::SyberosServiceCache::qApplication(argc, argv, new Service_Workspace());
}
#else
#include <QGuiApplication>
#include <QDebug>
#include <QString>
#include <framework/cpackageinfo.h>
#include <framework/csystempackagemanager.h>
#include "workthread.h"
extern QString gAppInstallPath;
Q_DECL_EXPORT int main(int argc, char* argv[]){
    QGuiApplication app(argc, argv);
    CSystemPackageManager mng;
    QSharedPointer<CPackageInfo> pkgInfo = mng.packageInfo("com.syberos.deviceInspection");
    gAppInstallPath = pkgInfo->installPath();

    WorkThread* workThread = new WorkThread();
    workThread->start();
    qDebug() << "QGuiApplication app(argc, argv);";
    app.exec();
}
#endif

