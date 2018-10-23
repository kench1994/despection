#include "service_workspace.h"
#include <QDebug>
#include <framework/cpackageinfo.h>
#include <framework/csystempackagemanager.h>

QString gAppInstallPath = "";
Service_Workspace::Service_Workspace()
    : CWorkspace(),m_pWorkThread(NULL)
{
    CSystemPackageManager mng;
    QSharedPointer<CPackageInfo> pkgInfo = mng.packageInfo("com.syberos.deviceInspection");
    gAppInstallPath = pkgInfo->installPath();
    m_pWorkThread = new WorkThread();
    m_pWorkThread->start();
}

void Service_Workspace::onLaunchComplete(Option option, const QStringList &params)
{
    switch (option) {
        case CWorkspace::HOME:
            qDebug() << "-------------appservice_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by HOME";
            break;
        case CWorkspace::URL:
            qDebug() << "-------------appservice_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by URL";
            break;
        case CWorkspace::EVENT:
            qDebug() << "-------------appservice_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by EVENT";
            break;
        case CWorkspace::DOCUMENT:
            qDebug() << "-------------ServerWorkspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by DOCUMENT";
            break;
        case CWorkspace::MULTIDOCS:
            qDebug() << "-------------appservice_Workspace::onLaunchComplete---------------" << __FILE__ <<__LINE__ << " start by MULTIDOCS";
            break;
        default:
            break;
    }
    Q_UNUSED(params);
}

void Service_Workspace::open(const QStringList &params)
{
    qDebug() << "-------------appservice_Workspace::open---------------" << __FILE__ <<__LINE__ << " start by open";
    Q_UNUSED(params);
}

