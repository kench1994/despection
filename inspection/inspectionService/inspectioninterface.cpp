#include "inspectioninterface.h"
#include "dataprovider.h"
#include "../framework/common/functiontype.h"
#include "../framework/network/networkmanager.h"
#include "../framework/common/systemconfig.h"
#include "./common/runtimeparam.h"
#include "../framework/config/sdisruntimecontext.h"
#include "./common/dbhelper.h"
#include "./DBConfig/dbconfigmanager.h"
#include <csystemnotification.h>
#include <csystemnotificationmanager.h>
#include <QDebug>
#include <QUuid>
InspectionInterface::InspectionInterface(QObject *parent):QObject(parent)
{
    qDebug() << "InspectionInterface::InspectionInterface()";
    registerCommonDataTypes();
    if(RuntimeParam::getInstance()->needCreateDB()){
        qDebug() << "needCreateDb  remove " << DATABASE_PATH;
        QFile fl(DATABASE_PATH);
        fl.remove();
    }
    DBHelper *dbHelper = DBHelper::getInstance();
    //检查数据库中是否有表存在
    if(!dbHelper->checkHasTableExist()){
        DBConfigManager dbConfManager;
        qDebug() << "DBConfigManager dbConfManager";
        dbConfManager.initDBTables();
    } else {
        qDebug() << "has table is db 1111 ";
    }
    QString ip = SystemConfig::getInstance()->getServerIP();
    QString port = SystemConfig::getInstance()->getServerPort();
    NetworkManager *networkManager = NetworkManager::getInstance(NULL);
    networkManager->setTarget(ip, port.toInt());
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    if(authType == 1){
        networkManager->initSystemAccessToken();
    }
    m_pDataProvider = DataProvider::getInstance();
    m_pLocalCacheManager = LocalCacheManager::getInstance();
    m_pDataHandlerManager = DataHandlerManager::getInstance();
    m_pSyncManager = SyncManager::getInstance();
    connect(m_pDataProvider, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)),
            this, SLOT(aaaaaaaaa(QString,QString,bool,int,DbusPackage)));


    QDBusConnection::sessionBus().connect(QString(), CLIENT_DBUS_PATH,
                                          CLIENT_DBUS_INTERFACE,
                                          QString("statusUpdated"),
                                          this, SLOT(setStatus(const QString&, const QVariantMap&)));

    //接收Push进程发送的信号
    QDBusConnection::sessionBus().connect(QString(), PUSH_SERVICE_DBUS_PATH,
                                          PUSH_SERVICE_DBUS_INTERFACE,
                                          QString("handlePushData"),
                                          this,SLOT(handlePushData(const DbusPackage &)));

    CSystemNotificationManager notificationManager;
    notificationManager.removeAllNotifications("com.syberos.deviceInspection");
}

InspectionInterface::~InspectionInterface()
{
    qDebug() << "InspectionInterface::~InspectionInterface()";

    QDBusConnection::sessionBus().disconnect(QString(), CLIENT_DBUS_PATH,
                                          CLIENT_DBUS_INTERFACE,
                                          QString("statusUpdated"),
                                          this, SLOT(setStatus(const QString&, const QVariantMap&)));


    QDBusConnection::sessionBus().disconnect(QString(), PUSH_SERVICE_DBUS_PATH,
                                          PUSH_SERVICE_DBUS_INTERFACE,
                                          QString("handlePushData"),
                                          this,SLOT(handlePushData(const DbusPackage &)));
}

void InspectionInterface::aaaaaaaaa(QString objects, QString action, bool result, int error, DbusPackage map)
{
    qDebug() << Q_FUNC_INFO << " objects " << objects;
    emit responseToClient(objects, action, result, error, map);
}

void InspectionInterface::receiveData(const DataPackage &dataPackage, const QString &action, const QString &functionType){
    qDebug() << Q_FUNC_INFO << "   begin " << dataPackage.head.objects;
    if(functionType == FUNCTION_TYPE_SYNC_ALL){
        //syncAll();
    } else if(functionType == FUNCTION_TYPE_REQUEST) {
        m_pDataHandlerManager->request(dataPackage, action);
    } else if(functionType == FUNCTION_TYPE_SUBMIT) {
        m_pLocalCacheManager->addCache(dataPackage, action);
    }else if(functionType == FUNCTION_TYPE_CLEAR_CACHE_ALL){
        m_pLocalCacheManager->clearLocalCacheAll();
    }else if(functionType == FUNCTION_TYPE_HAS_CACHE){
        m_pLocalCacheManager->hasLocalCache();
    } else if(functionType == FUNCTION_TYPE_SUBMIT_CACHE){
        DataOperationType operationType = BussinessMap::getInstance()->getBussinessType(dataPackage.head.objects, BussinessMap::MODULE_TYPE_LOCALCACHE);
        m_pLocalCacheManager->submitLocalCache(operationType);
    } else if(functionType == FUNCTION_TYPE_CLEAR_CACHE){
        DataOperationType operationType = BussinessMap::getInstance()->getBussinessType(dataPackage.head.objects, BussinessMap::MODULE_TYPE_LOCALCACHE);
        m_pLocalCacheManager->clearLocalCache(operationType);
    } else if(functionType == FUNCTION_TYPE_HAS_TYPE_CACHE){
        DataOperationType operationType = BussinessMap::getInstance()->getBussinessType(dataPackage.head.objects, BussinessMap::MODULE_TYPE_LOCALCACHE);
        m_pLocalCacheManager->hasLocalCache(operationType);
    } else if(functionType == FUNCTION_TYPE_SYNC) {
        m_pSyncManager->syncPreTask(action, dataPackage);
    } else if(functionType == FUNCTION_TYPE_REQUEST_TASK) {
        m_pDataHandlerManager->requestTask(dataPackage, action);
    }
}

void InspectionInterface::setStatus(const QString &functionType, const QVariantMap map)
{
    qDebug() << " InspectionInterface::setStatus " << functionType;
    RuntimeParam *pRunParam = RuntimeParam::getInstance();
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    //设置服务器地址
    if(functionType == FUNCTION_TYPE_STATUS_ADDRESS){
        QString address = map["ip"].toString();
        int port = map["port"].toInt();
        NetworkManager::getInstance(NULL)->setTarget(address, port);
        if(authType == 1){
            AccessToken token;
            memset((void*)&token, 0, sizeof(AccessToken));
            SDISRuntimeContext::getInstance()->setSystemToken(token);
            SDISRuntimeContext::getInstance()->setUserToken(token);
            NetworkManager::getInstance(NULL)->initSystemAccessToken();
        }
    } else if(functionType == FUNCTION_TYPE_STATUS_LOGIN){
        //设置登陆状态
        bool login = map["login"].toBool();
        pRunParam->setLoginStatus(login);
        if(authType == 1){
            if(login){
                QString userName = map["user"].toString();
                QString password = map["password"].toString();
                NetworkManager::getInstance(NULL)->initAccessToken(userName, password);
            }else{
                AccessToken token;
                memset((void*)&token, 0, sizeof(AccessToken));
                SDISRuntimeContext::getInstance()->setUserToken(token);
            }
        }
    } else if(functionType == FUNCTION_TYPE_STATUS_USERINFO){
        //设置登陆用户信息
        QMap<QString, QString> userMsg;
        QVariantMap::const_iterator iter = map.begin();
        while (iter != map.end() ) {
            userMsg[iter.key()] = iter.value().toString();
            ++iter;
        }
        pRunParam->setLoginUserMsg(userMsg);
    }else if(functionType == FUNCTION_TYPE_STATUS_PERMISSION){
        //设置权限
        QString shortName = map["ShortName"].toString();
        bool permission = map["Permission"].toBool();
        if(shortName == ""){
            pRunParam->clearUserPermission();
        }else {
            pRunParam->setUserPermission(shortName, permission);
        }
    }else if(functionType == FUNCTION_TYPE_STATUS_AUTHTYPE){
        int authType = map["authType"].toInt();
        SDISRuntimeContext::getInstance()->setAuthenticationType(authType);
        if(authType == 1){
            NetworkManager::getInstance(NULL)->initSystemAccessToken();
        }
    }else if(functionType == FUNCTION_TYPE_STATUS_ENCRYPTTYPE){
        int encryptType = map["encryptType"].toInt();
        SDISRuntimeContext::getInstance()->setDataEncryptionType(encryptType);
        if(authType == 1){
            NetworkManager::getInstance(NULL)->initSystemAccessToken();
        }
    }else if(functionType == FUNCTION_TYPE_STATUS_RESET_PARA_SERVICE){
        QString account = map["account"].toString();
        QString code = map["code"].toString();
        NetworkManager::getInstance(NULL)->resetSecurityParameter(code, account);
    }else if(functionType == FUNCTION_TYPE_STATUS_OFFLINEMODE){
        RuntimeParam::getInstance()->setOfflineModel(map["offlineMode"].toString());
    }else if(functionType == FUNCTION_TYPE_CLEAR_DB){
        DBConfigManager *dbManager = new DBConfigManager();
        dbManager->clearDBTables();
    }
}

void InspectionInterface::handlePushData(const DbusPackage &dbusPackage)
{
    qDebug() << Q_FUNC_INFO;
    DataPackage dataPackage = dbusPackage.dataPackage;
    QList<DataTable> tables = dataPackage.tables;
    if(!tables.isEmpty()){
        QList<DataRow> dataRows = tables.at(0).value;
        if(dataRows.isEmpty())
            return;
        dataPackage.head.objects = "CMDS_MessageList_Push";
        QString action = QUuid::createUuid().toString().replace("{", "").replace("}", "");
        m_pDataHandlerManager->request(dataPackage, action);
    }
}
