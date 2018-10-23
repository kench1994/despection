/*!
* \file main.cpp
* \brief 巡检主文件
*
* Main函数的实现
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#if 1
#include "deviceinspectionworkspace.h"

Q_DECL_EXPORT int main(int argc, char* argv[])
{
    return SYBEROS::SyberosGuiCache::qApplication(argc, argv, new DeviceInspectionWorkSpace());
}
#else
//#include "simpleappWorkspace.h"
//#if defined(OS_SYBEROS)
//#include <cgui_application.h>
//#else
#include <QGuiApplication>
//#endif
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>
#include <qpa/qplatformnativeinterface.h>
#include <framework/csystempackagemanager.h>
#include "generateForm/generateForm.h"
#include "common/transmitmanager.h"
#include "generallistmodel/generallistmodel.h"
#include "generallistmodel/commonproxymodel.h"
#include "imageProcess/imageproc.h"
#include "common/inspectionsyssetting.h"
#include "imageProcess/decodeworkspace.h"
#include "common/deviceinspectioneventfilter.h"
#include "imageProcess/qtcamera.h"
#include "common/nfcmanager.h"
#include "upgrade/upgradeThread.h"
#include "upgrade/upgradeconfig.h"
#include "theme/themeadaptor.h"
#include "./imageProcess/audiorecorder.h"
#include "./imageProcess/videorecorder.h"
#include "./imageProcess/multimediautils.h"
#include "./common/httprequest.h"
#include "../../framework/common/systemconfig.h"
#include "./common/runtimeparam.h"
#include "../framework/common/networkstatus.h"
#include <framework/cpackageinfo.h>
#define MAIN_TAG "main: "
extern QString gAppInstallPath;
Q_DECL_EXPORT int main(int argc, char* argv[])
{

//#if defined(OS_SYBEROS)
    //qDebug() << "1111111111111111111111111";
    //SYBEROS::CGuiApplication app(argc, argv);
//#else
//    qDebug() << "2222222222222222222222222222";
    QGuiApplication app(argc, argv);
//#endif

    //m_view = SYBEROS::SyberosGuiCache::qQuickView();
    CSystemPackageManager mng;
    QSharedPointer<CPackageInfo> pkgInfo = mng.packageInfo("com.syberos.deviceInspection");
    gAppInstallPath = pkgInfo->installPath();
    QQuickView *m_view = new QQuickView();
    SystemConfig *inspectionSystemConfig = SystemConfig::getInstance();
    QQmlEngine::setObjectOwnership(inspectionSystemConfig, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("inspectionSystemConfig", inspectionSystemConfig);

    RuntimeParam *inspectionSystemParam = RuntimeParam::getInstance();
    QQmlEngine::setObjectOwnership(inspectionSystemParam, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("inspectionSystemParam", inspectionSystemParam);
    inspectionSystemParam->setViewer(m_view);

    NetworkStatus *networkStatus = NetworkStatus::getInstance();
    m_view->rootContext()->setContextProperty("networkStatus", networkStatus);

    qDebug() << MAIN_TAG << "---------main    m_viewer  " << m_view;
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    qmlRegisterType<GenerateForm>("com.syberos.generateForm", 1, 0, "GenerateForm");
    qmlRegisterType<ImageProc>("com.syberos.imageproc", 1, 0, "ImageProc");
    qmlRegisterType<InspectionSysSetting>("myInspectionSysSetting", 1, 0, "InspectionSysSetting");//add by chen ruihua
    qmlRegisterType<DecodeWorkSpace>("com.syberos.decodeWorkSpace", 1, 0, "DecodeWorkSpace");
    //qmlRegisterType<GpsManager>("com.syberos.gpsmanager", 1, 0, "GpsManager");

    qRegisterMetaType<DataPackage>("DataPackage");
    qRegisterMetaType<CPackageInfo::PackageStatus> ("PackageStatus");
    qRegisterMetaType<CPackageInfo::PackageError> ("PackageError");
    qRegisterMetaType<ProtocolHead>("ProtocolHead");

    //for MultimediaUtils
    MultimediaUtils *multimediaUtils = MultimediaUtils::getInstance();
    m_view->rootContext()->setContextProperty("multimediaUtils", multimediaUtils);
    //for audioRecorder
    AudioRecorder *multimediaAduioRecorder = new AudioRecorder();
    m_view->rootContext()->setContextProperty("multimediaAduioRecorder", multimediaAduioRecorder);

    //for videoRecorder
    VideoRecorder *multimediaVideoRecorder = new VideoRecorder();
    m_view->rootContext()->setContextProperty("multimediaVideoRecorder", multimediaVideoRecorder);

    // main functions list mode
    ThemeAdaptor * inspectionFuncsItemModel = new ThemeAdaptor(qApp);
    QQmlEngine::setObjectOwnership(inspectionFuncsItemModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("inspectionFuncsItemModel", inspectionFuncsItemModel);

    //for systemMsgs
    TransmitManager *transmitManager =  TransmitManager::getInstance();
    QQmlEngine::setObjectOwnership(transmitManager, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("transmitManager", transmitManager);

    QtCamera *camera = QtCamera::getInstance();
    QQmlEngine::setObjectOwnership(camera, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("QtCamera", camera);
    //for upgrade package

    UpgradeThread * upgradeThread = UpgradeThread::getInstance();
    QQmlEngine::setObjectOwnership(upgradeThread, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("upgradeThread", upgradeThread);
    upgradeThread->start();

    // for NFC instance
    NfcManager* nfcManager = NfcManager::getInstance();
    QQmlEngine::setObjectOwnership(nfcManager, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("nfcManager", nfcManager);

    //for httpRequest
    HttpRequest* httpRequest = new HttpRequest();
    QQmlEngine::setObjectOwnership(httpRequest, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("httpRequest", httpRequest);


    DeviceInspectionEventFilter *deviceInspectionEventFilter = DeviceInspectionEventFilter::getInstance();
    m_view->rootContext()->setContextProperty("deviceInspectionEventFilter", deviceInspectionEventFilter);
    m_view->installEventFilter(deviceInspectionEventFilter);
    //for upgrade package

    m_view->create();
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    //m_view->setTitle("simple-app");

    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    native->setWindowProperty(m_view->handle(), QLatin1String("CATEGORY"), QString("home"));
    native->setWindowProperty(m_view->handle(), "statusBarVisible", "true");
    native->setWindowProperty(m_view->handle(), "statusBarStyle", "transblack");
    qDebug() << MAIN_TAG << "SimpleAppWorkspace constructor finished";
    qDebug () << MAIN_TAG << "-----------HAS_BOOSTER------------";

    //inspectionTask list model
    GeneralListModel *inspectionTaskModel = new GeneralListModel(&app);
    QQmlEngine::setObjectOwnership(inspectionTaskModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("inspectionTaskModel", inspectionTaskModel);
    qDebug() << "GeneralListModel *inspectionTaskModel = new GeneralListModel(&app);";
    //inspectionTaskSuite list model
    GeneralListModel *inspectionTaskSuiteModel = new GeneralListModel(&app);
    QQmlEngine::setObjectOwnership(inspectionTaskSuiteModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("inspectionTaskSuiteModel", inspectionTaskSuiteModel);
    qDebug() << "GeneralListModel *inspectionTaskSuiteModel = new GeneralListModel(&app);";
    //inspectionTaskItem list model
    GeneralListModel *inspectionTaskItemModel = new GeneralListModel(&app);
    QQmlEngine::setObjectOwnership(inspectionTaskItemModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("inspectionTaskItemModel", inspectionTaskItemModel);
    qDebug() << "GeneralListModel *inspectionTaskItemModel = new GeneralListModel(&app);";

    CommonProxyModel *commonProxyModel = new CommonProxyModel();
    QQmlEngine::setObjectOwnership(commonProxyModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("commonProxyModel", commonProxyModel);

    //syetem message list model
    GeneralListModel *systemMessageModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(inspectionTaskModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("systemMessageModel", systemMessageModel);

    //bussiness message list model
    GeneralListModel *businessMessageModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(inspectionTaskModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("businessMessageModel", businessMessageModel);

    //inspectionTask scan list model
    GeneralListModel *scanInspectionModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(scanInspectionModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("scanInspectionModel", scanInspectionModel);

    //maintainTask scan list model
    GeneralListModel *scanMaintainModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(scanMaintainModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("scanMaintainModel", scanMaintainModel);

    //rectificationTask scan list model
    GeneralListModel *scanRectificationModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(scanRectificationModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("scanRectificationModel", scanRectificationModel);

    //oprationTask scan list model
    GeneralListModel *scanOprationModel = new GeneralListModel(qApp);
    QQmlEngine::setObjectOwnership(scanOprationModel, QQmlEngine::CppOwnership);
    m_view->rootContext()->setContextProperty("scanOprationModel", scanOprationModel);

    m_view->showFullScreen();
    qDebug() << "m_view->showFullScreen();   ok  ";
    return app.exec();
}
#endif
