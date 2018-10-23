#include "deviceinspectionworkspace.h"

#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>
#include <framework/csystempackagemanager.h>
#include <cpackageinfo.h>
#include <qcamera.h>
#include "generateForm/generateForm.h"
#include "common/transmitmanager.h"
#include "generallistmodel/generallistmodel.h"
#include "generallistmodel/commonproxymodel.h"
#include "imageProcess/imageproc.h"
#include "common/inspectionsyssetting.h"//add by chen ruihua
#include "imageProcess/decodeworkspace.h"
#include "common/deviceinspectioneventfilter.h"
#include "imageProcess/qtcamera.h"
#include "../../framework/common/systemconfig.h"
#include "./common/runtimeparam.h"
#include <qpa/qplatformnativeinterface.h>
#include "upgrade/upgradeThread.h"
#include "upgrade/upgradeconfig.h"
#include <unistd.h>
#include "theme/themeadaptor.h"
#include "common/nfcmanager.h"
#include "imageProcess/audiorecorder.h"
#include "imageProcess/videorecorder.h"
#include "imageProcess/multimediautils.h"
#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbusconnectioninterface.h>
#include <QtDBus/qdbusmetatype.h>
#include <QtDBus/qdbusreply.h>
#include <QTimer>
#include <QTime>
#include "./common/httprequest.h"
#include "../framework/common/networkstatus.h"
#include <framework/cpackageinfo.h>
#include <framework/csystempackagemanager.h>

QString gAppInstallPath = "";
DeviceInspectionWorkSpace::DeviceInspectionWorkSpace(QObject *parent):CWorkspace(parent),
    m_pViewer(NULL)
{
    CSystemPackageManager mng;
    QSharedPointer<CPackageInfo> pkgInfo = mng.packageInfo("com.syberos.deviceInspection");
    gAppInstallPath = pkgInfo->installPath();
    m_nWindowIsActive = true;
    QTimer::singleShot(0, this, SLOT(initialize()));
}

void DeviceInspectionWorkSpace::initialize(){
    qWarning() << "DeviceInspectionWorkSpace::initialize()1111 " << QTime::currentTime();
    m_pViewer = SyberosGuiCache::qQuickView();
    SystemConfig *inspectionSystemConfig = SystemConfig::getInstance();
    QQmlEngine::setObjectOwnership(inspectionSystemConfig, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("inspectionSystemConfig", inspectionSystemConfig);

    RuntimeParam *inspectionSystemParam = RuntimeParam::getInstance();
    m_pViewer->rootContext()->setContextProperty("inspectionSystemParam", inspectionSystemParam);
    inspectionSystemParam->setViewer(m_pViewer);

    NetworkStatus *networkStatus = NetworkStatus::getInstance();
    m_pViewer->rootContext()->setContextProperty("networkStatus", networkStatus);

    qmlRegisterType<GenerateForm>("com.syberos.generateForm", 1, 0, "GenerateForm");
    qmlRegisterType<ImageProc>("com.syberos.imageproc", 1, 0, "ImageProc");
    qmlRegisterType<InspectionSysSetting>("myInspectionSysSetting", 1, 0, "InspectionSysSetting");//add by chen ruihua
    qmlRegisterType<DecodeWorkSpace>("com.syberos.decodeWorkSpace", 1, 0, "DecodeWorkSpace");
    //qmlRegisterType<GpsManager>("com.syberos.gpsmanager", 1, 0, "GpsManager");

    qRegisterMetaType<DataPackage>("DataPackage");
    qRegisterMetaType<CPackageInfo::PackageStatus> ("PackageStatus");
    qRegisterMetaType<CPackageInfo::PackageError> ("PackageError");
    qRegisterMetaType<ProtocolHead>("ProtocolHead");

    m_pViewer->create();
    QObject::connect(m_pViewer, SIGNAL(activeChanged()), this, SLOT(setWindowActiveFlag()));

    // main functions list mode
    ThemeAdaptor * inspectionFuncsItemModel = new ThemeAdaptor(qApp);
    QQmlEngine::setObjectOwnership(inspectionFuncsItemModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("inspectionFuncsItemModel", inspectionFuncsItemModel);

    //for systemMsgs
//    qDebug() << "start inspectionService111111111111111";
//    QStringList serviceList;
//    serviceList.append("startService");
//    qApp->runService("inspectionService", serviceList);
//    qDebug() << "start inspectionPushService111111111111111";
//    QStringList pushServiceList;
//    pushServiceList.append("startPushService");
//    qApp->runService("inspectionPushService", pushServiceList);
    TransmitManager *transmitManager = TransmitManager::getInstance();
    QQmlEngine::setObjectOwnership(transmitManager, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("transmitManager", transmitManager);

    //for MultimediaUtils
    MultimediaUtils *multimediaUtils = MultimediaUtils::getInstance();
    m_pViewer->rootContext()->setContextProperty("multimediaUtils", multimediaUtils);

    //for audioRecorder
    AudioRecorder *multimediaAduioRecorder = new AudioRecorder();
    m_pViewer->rootContext()->setContextProperty("multimediaAduioRecorder", multimediaAduioRecorder);

    //for videoRecorder
    VideoRecorder *multimediaVideoRecorder = new VideoRecorder();
    m_pViewer->rootContext()->setContextProperty("multimediaVideoRecorder", multimediaVideoRecorder);

    DeviceInspectionEventFilter *deviceInspectionEventFilter = DeviceInspectionEventFilter::getInstance();
    QQmlEngine::setObjectOwnership(deviceInspectionEventFilter, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("deviceInspectionEventFilter", deviceInspectionEventFilter);
    m_pViewer->installEventFilter(deviceInspectionEventFilter);

    QtCamera *camera = QtCamera::getInstance();
    qDebug() << "QtCamera *camera = QtCamera::getInstance() ok";
    QQmlEngine::setObjectOwnership(camera, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("QtCamera", camera);
    //for upgrade package

    UpgradeThread * upgradeThread = UpgradeThread::getInstance();
    QQmlEngine::setObjectOwnership(upgradeThread, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("upgradeThread", upgradeThread);
    upgradeThread->start();

    // initialize NFC component
    NfcManager* nfcManager = NfcManager::getInstance();
    QQmlEngine::setObjectOwnership(nfcManager, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("nfcManager", nfcManager);

    //for httpRequest
    HttpRequest* httpRequest = new HttpRequest();
    QQmlEngine::setObjectOwnership(httpRequest, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("httpRequest", httpRequest);

    qWarning() << "m_pViewer->setSource1111 " << QTime::currentTime();
    m_pViewer->setSource(QUrl("qrc:/qml/main.qml"));
    qWarning() << "m_pViewer->setSource2222 " << QTime::currentTime();
    m_pViewer->setTitle("DeviceInspection");
    m_pViewer->showFullScreen();
    qWarning() << "m_pViewer->setSource shouwFullScreen " << QTime::currentTime();
    QObject::connect(m_pViewer->engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    //inspectionTask list model
    GeneralListModel *inspectionTaskModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(inspectionTaskModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("inspectionTaskModel", inspectionTaskModel);

    //inspectionTaskSuite list model
    GeneralListModel *inspectionTaskSuiteModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(inspectionTaskSuiteModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("inspectionTaskSuiteModel", inspectionTaskSuiteModel);

    //inspectionTaskItem list model
    GeneralListModel *inspectionTaskItemModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(inspectionTaskItemModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("inspectionTaskItemModel", inspectionTaskItemModel);
    qWarning() << "DeviceInspectionWorkSpace::initialize() ok  " << QTime::currentTime();

    CommonProxyModel *commonProxyModel = new CommonProxyModel();
    QQmlEngine::setObjectOwnership(commonProxyModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("commonProxyModel", commonProxyModel);

    //syetem message list model
    GeneralListModel *systemMessageModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(inspectionTaskModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("systemMessageModel", systemMessageModel);

    //bussiness message list model
    GeneralListModel *businessMessageModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(inspectionTaskModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("businessMessageModel", businessMessageModel);

//    if(category == "home"){
//        QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
//        native->setWindowProperty(m_pViewer->handle(), QLatin1String("CATEGORY"), QString("home"));
//    }
    /*
    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    native->setWindowProperty(m_pViewer->handle(), QLatin1String("CATEGORY"), QString("home"));
    native->setWindowProperty(m_pViewer->handle(), "statusBarVisible", "true");
    native->setWindowProperty(m_pViewer->handle(), "statusBarStyle", "transblack");
    */

    //inspectionTask scan list model
    GeneralListModel *scanInspectionModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(scanInspectionModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("scanInspectionModel", scanInspectionModel);

    //maintainTask scan list model
    GeneralListModel *scanMaintainModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(scanMaintainModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("scanMaintainModel", scanMaintainModel);

    //rectificationTask scan list model
    GeneralListModel *scanRectificationModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(scanRectificationModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("scanRectificationModel", scanRectificationModel);

    //oprationTask scan list model
    GeneralListModel *scanOprationModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(scanOprationModel, QQmlEngine::CppOwnership);
    m_pViewer->rootContext()->setContextProperty("scanOprationModel", scanOprationModel);

}

void DeviceInspectionWorkSpace::setWindowActiveFlag(){
    qDebug() << "DeviceInspectionWorkSpace::setWindowActiveFlag()  " << m_pViewer->isActive();
    m_nWindowIsActive = m_pViewer->isActive();
}


void DeviceInspectionWorkSpace::onLaunchComplete(Option option, const QStringList &params)
{
    qDebug() <<"------------------"<<__FILE__ << __LINE__
            << "DeviceInspectionWorkSpace::onLaunchComplete, params:    "
            << params << "    PID{}===PPPPPPP===============++++++"<< getpid();
    switch (option) {
    case CWorkspace::HOME:
    case CWorkspace::EVENT:{
        if(params.size() >= 1){
            QString arg = params.at(0);
            if(arg.indexOf("Msg") != -1){
                qDebug() << "m_nWindowIsActive  is   " << m_nWindowIsActive  << " params is " << params;
                    entryMsgDetail(arg);
            }
        }
//        QDBusMessage m = QDBusMessage::createMethodCall("com.syberos.compositor", "/com/syberos/compositor",
//                                                        "com.syberos.compositor.CompositorInterface",
//                                                        QStringLiteral("hideBootSplash"));
//        QDBusConnection::systemBus().call(m);

        qDebug() << "---------- case CWorkspace::HOME  88888888888888888888888  :----------" << option;
    }
        break;
    case CWorkspace::URL:
        qDebug() << "----------case CWorkspace::URL:----------" << option;
        break;
    case CWorkspace::DOCUMENT:
        qDebug() << "----------case CWorkspace::DOCUMENT:----------" << option;
        break;
    default:
        qDebug() << "----------default:----------" << option;
        break;
    }
}

void DeviceInspectionWorkSpace::open(const QStringList &params){
    qDebug() <<"------------------"<<__FILE__ << __LINE__<< "DeviceInspectionWorkSpace::open, params:" << params ;
    if(params.size() > 1){
        QString arg = params.at(1);
        if(arg.indexOf("Msg") != -1){
            qDebug() << "m_nWindowIsActive  is   " << m_nWindowIsActive  << " params is " << params;
            //if(m_nWindowIsActive)
                entryMsgDetail(arg);
        }
    }
}

void DeviceInspectionWorkSpace::entryMsgDetail(QString uuid){
    QQuickItem *rootObject = m_pViewer->rootObject();
    QMetaObject::invokeMethod(rootObject, "entryMsgDetail", Qt::DirectConnection, Q_ARG(QVariant, uuid));
}
