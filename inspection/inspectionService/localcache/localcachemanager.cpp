#include "localcachemanager.h"
#include "datalocalcachebase.h"
#include "../../framework/common/convertdata.h"
#include <QDebug>
#include "../dataprovider.h"
#include "../networkcontrol.h"
#include "../../framework/config/sdisruntimecontext.h"
#include "../common/runtimeparam.h"
LocalCacheManager* LocalCacheManager::m_PLocalCacheManager = NULL;
LocalCacheManager::LocalCacheManager()
{
    m_nIsSubmitting = false;
    m_nIsSubmittingBinary = false;
    m_nSubmittingBusiness = BUSINESS_TYPE_UNKOWN;
    m_pSubmittingLocalCache = NULL;
    m_pSubmittingBinaryLocalCache = NULL;

    NetworkControl *networkControl = NetworkControl::getInstance();
    connect(networkControl, SIGNAL(sendNetworkResultSignal(QString,QString,bool,int,DbusPackage)), this, SLOT(sendNetworkResultSlot(QString,QString,bool,int,DbusPackage)));
    connect(networkControl, SIGNAL(networkConnectChanged(QString)), this, SLOT(networkConnectChanged(QString)));

    QList<DataLocalCacheBase * > localCacheBaseList = m_nLocalCacheFactory.getAllLocalCache();
    for(int i = 0; i < localCacheBaseList.size(); i++){
        DataLocalCacheBase *localCacheBase = localCacheBaseList.at(i);
        connect(localCacheBase, SIGNAL(onSubmitStart(DataOperationType, bool)), this, SLOT(submitStart(DataOperationType, bool)));
        connect(localCacheBase, SIGNAL(onSubmitStop(DataOperationType, bool)), this, SLOT(submitStop(DataOperationType, bool)));
        connect(localCacheBase, SIGNAL(onSubmitProgress(DataOperationType, int, const QString &, bool)), this, SLOT(submitProgress(DataOperationType, int, const QString &, bool)));
        connect(localCacheBase, SIGNAL(onSubmitFinished(DataOperationType, bool, bool)), this, SLOT(submitFinished(DataOperationType, bool, bool)));
        connect(localCacheBase, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
    }
    DataProvider *provider = DataProvider::getInstance();
    connect(this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), provider, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
}

LocalCacheManager *LocalCacheManager::getInstance()
{
    if(m_PLocalCacheManager == NULL)
        m_PLocalCacheManager = new LocalCacheManager();
    return m_PLocalCacheManager;
}

LocalCacheManager::~LocalCacheManager()
{
    NetworkControl *networkControl = NetworkControl::getInstance();
    disconnect(networkControl, SIGNAL(sendNetworkResultSignal(QString,QString,bool,int,DbusPackage)), this, SLOT(sendNetworkResultSlot(QString,QString,bool,int,DbusPackage)));
    disconnect(networkControl, SIGNAL(networkConnectChanged(QString)), this, SLOT(networkConnectChanged(QString)));

    QList<DataLocalCacheBase * > localCacheBaseList = m_nLocalCacheFactory.getAllLocalCache();
    for(int i = 0; i < localCacheBaseList.size(); i++){
        DataLocalCacheBase *localCacheBase = localCacheBaseList.at(i);
        disconnect(localCacheBase, SIGNAL(onSubmitStart(DataOperationType ,bool)), this, SLOT(submitStart(DataOperationType ,bool)));
        disconnect(localCacheBase, SIGNAL(onSubmitStop(DataOperationType ,bool)), this, SLOT(submitStop(DataOperationType ,bool)));
        disconnect(localCacheBase, SIGNAL(onSubmitProgress(DataOperationType, int, const QString & ,bool)), this, SLOT(submitProgress(DataOperationType, int, const QString & ,bool)));
        disconnect(localCacheBase, SIGNAL(onSubmitFinished(DataOperationType, bool ,bool)), this, SLOT(submitFinished(DataOperationType, bool ,bool)));
        disconnect(localCacheBase, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
    }
    DataProvider *provider = DataProvider::getInstance();
    disconnect(this, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)), provider, SIGNAL(onResultOfResponse(QString,QString,bool,int,DbusPackage)));
}

bool LocalCacheManager::addCache(const DataPackage &dataPackage, const QString &action)
{
    DataOperationType type = BussinessMap::getInstance()->getBussinessType(dataPackage.head.objects, BussinessMap::MODULE_TYPE_LOCALCACHE);
    DataLocalCacheBase* localCacheBase = m_nLocalCacheFactory.getLocalCache(type);
    if(localCacheBase == NULL){
        qDebug() << "没有发现相关业务缓存";
        DbusPackage map;
        map.otherMap.insert("objects", dataPackage.head.objects);
        map.otherMap.insert("serverCode", "0");
        map.otherMap.insert("description", "没有发现相关业务缓存");
        emit onResultOfResponse(dataPackage.head.objects, action, false, BUSINESS_ERROR_NO_BUSINESS, map);
        return false;
    }
    //判断是否在线模式
    if((localCacheBase->isOnlieInterface(dataPackage.head.objects) || RuntimeParam::getInstance()->getOfflineModel() == "0")
            && NetworkControl::getInstance()->getNetworkConnectStatus() == ""){
        sendNoNetworkResponse(dataPackage.head.objects, action);
        return true;
    }
    localCacheBase->addCache(dataPackage, action);
    qDebug() <<"ocalCacheBase->addCache   ok  " << action;
    m_nBinaryFailList.removeOne(type);
    m_nXmlFailList.removeOne(type);
    //离线模式下，把数据缓存到本地，然后就发送信号给app. 纯在线接口除外,纯在线接口必须等发送完数据后返回结果给app
    if(RuntimeParam::getInstance()->getOfflineModel() != "0" && !(localCacheBase->isOnlieInterface(dataPackage.head.objects)) ){
        DbusPackage map;
        map.otherMap["objects"] = dataPackage.head.objects;
        emit onResultOfResponse(dataPackage.head.objects, action, true, 0, map);
    }
    submitLocalCache(type);
    return true;
}

void LocalCacheManager::submitLocalCacheAll()
{
    QList<DataLocalCacheBase * > localCacheBaseList = m_nLocalCacheFactory.getAllLocalCache();
    bool hasXmlFileSubmit = false;
    //普通xml文件提交优先
    int i = 0;
    for(; i < localCacheBaseList.count(); i++){
        DataLocalCacheBase *tmpBase = localCacheBaseList.at(i);
        if(tmpBase->hasLocalCache() && !(m_nXmlFailList.contains(tmpBase->getDataOperationType())) ){
            qDebug() << "tmpBase->hasLocalCache() xml  submitLocalCache";
            hasXmlFileSubmit = true;
            submitLocalCache(tmpBase->getDataOperationType());
            break;
        }
    }
    if(i == localCacheBaseList.count()){
        qDebug() << "All xml localcache has submit";
    }
    //如果没有xml文件，开始提交二进制文件
    if(!hasXmlFileSubmit){
        for(i = 0; i < localCacheBaseList.count(); i++){
            DataLocalCacheBase *tmpBase = localCacheBaseList.at(i);
            if(tmpBase->hasLocalCache(true) && !(m_nBinaryFailList.contains(tmpBase->getDataOperationType())) ){
                qDebug() << "tmpBase->hasLocalCache() binaries  submitLocalCache";
                submitLocalCache(tmpBase->getDataOperationType(), true);
                break;
            }
        }
        if(i == localCacheBaseList.count()){
            qDebug() << "All binary localcache has submit";
        }
    }
}

void LocalCacheManager::submitLocalCache(DataOperationType operationType, bool binaryType )
{
    //判断是否有网
    QString netWorkType = NetworkControl::getInstance()->getNetworkConnectStatus();
    if(netWorkType == ""){
        qDebug() << "submit LocalCache  netWorkType  == empty return";
        return;
    }

    qDebug() << "m_nIsSubmitting is " << m_nIsSubmitting << "   m_nIsSubmittingBinary is " << m_nIsSubmittingBinary;
    DataLocalCacheBase* localCacheBase = m_nLocalCacheFactory.getLocalCache(operationType);
    if(!binaryType && localCacheBase->hasLocalCache() && m_nIsSubmitting == false){
        qDebug() << "localCacheBase->submitLocalCache(); 11111111111111  " << m_pSubmittingLocalCache;
        m_pSubmittingLocalCache = localCacheBase;
        m_pSubmittingLocalCache->submitLocalCache();
        m_nIsSubmitting = true;
    } else if(binaryType &&  localCacheBase->hasLocalCache(true) && m_nIsSubmittingBinary == false){
        qDebug() << "localCacheBase->submitLocalCache(true);  11111111111111 " << m_pSubmittingBinaryLocalCache;
        m_pSubmittingBinaryLocalCache = localCacheBase;
        m_pSubmittingBinaryLocalCache->submitLocalCache(true);
        m_nIsSubmittingBinary = true;
    }
}

void LocalCacheManager::clearLocalCacheAll()
{
    QList<DataLocalCacheBase * > localCacheBaseList = m_nLocalCacheFactory.getAllLocalCache();
    for(int i = 0; i < localCacheBaseList.count(); i++){
        localCacheBaseList.at(i)->clearCache();
    }
}

void LocalCacheManager::clearLocalCache(DataOperationType type)
{
    DataLocalCacheBase* localCacheBase = m_nLocalCacheFactory.getLocalCache(type);
    localCacheBase->clearCache();
}

bool LocalCacheManager::hasLocalCache()
{
    bool hasCache = false;
    QList<DataLocalCacheBase * > localCacheBaseList = m_nLocalCacheFactory.getAllLocalCache();
    for(int i = 0; i < localCacheBaseList.count(); i++){
        hasCache =  localCacheBaseList.at(i)->hasLocalCache();
        if(hasCache)
            break;
    }
    return hasCache;
}

bool LocalCacheManager::hasLocalCache(DataOperationType type)
{
    DataLocalCacheBase* localCacheBase = m_nLocalCacheFactory.getLocalCache(type);
    return localCacheBase->hasLocalCache();
}

bool LocalCacheManager::hasBinaryLocalCache(DataOperationType type)
{
    DataLocalCacheBase* localCacheBase = m_nLocalCacheFactory.getLocalCache(type);
    return localCacheBase->hasLocalCache(true);
}

bool LocalCacheManager::hasBinaryLocalCache()
{
    bool hasCache = false;
    QList<DataLocalCacheBase * > localCacheBaseList = m_nLocalCacheFactory.getAllLocalCache();
    for(int i = 0; i < localCacheBaseList.count(); i++){
        hasCache =  localCacheBaseList.at(i)->hasLocalCache(true);
        if(hasCache)
            break;
    }
    return hasCache;
}

void LocalCacheManager::sendNetworkResultSlot(QString objects, QString action, bool result, int error, DbusPackage map)
{
    if(objects == "binary"){
        if(m_pSubmittingBinaryLocalCache){
            qDebug() << Q_FUNC_INFO <<" binary  " << m_pSubmittingBinaryLocalCache->getDataOperationType();
            m_pSubmittingBinaryLocalCache->networkResponse(objects, action, result, error, map);
        }
    } else if(m_pSubmittingLocalCache){
        DataOperationType type = BussinessMap::getInstance()->getBussinessType(objects, BussinessMap::MODULE_TYPE_LOCALCACHE);
        if(type != BUSINESS_TYPE_UNKOWN){
            DataLocalCacheBase* localCacheBase = m_nLocalCacheFactory.getLocalCache(type);
            if(localCacheBase == m_pSubmittingLocalCache){
                qDebug() << Q_FUNC_INFO <<" xml  " << m_pSubmittingLocalCache->getDataOperationType();
                m_pSubmittingLocalCache->networkResponse(objects, action, result, error, map);
            }
        } else {
            qDebug() << Q_FUNC_INFO << "type != BUSINESS_TYPE_UNKOWN  objects is " << objects;
        }
    }
}

void LocalCacheManager::submitProgress(DataOperationType type, int progress, const QString &description, bool binaryType)
{
    qDebug() << Q_FUNC_INFO << "type " << type << "  progress " << progress << "  description " << description << "  binaryType " << binaryType;
}

void LocalCacheManager::submitFinished(DataOperationType type, bool result, bool binaryType)
{
    qDebug() << Q_FUNC_INFO << "type " << type << "  result " << result << "  binaryType " << binaryType;
    if(result == false){
        if(binaryType)
            m_nBinaryFailList.append(type);
        else
            m_nXmlFailList.append(type);
    }
    //该模块提交完成，设置为NULL
    if(binaryType){
        m_pSubmittingBinaryLocalCache = NULL;
        m_nIsSubmittingBinary = false;
    } else {
        m_pSubmittingLocalCache = NULL;
        m_nIsSubmitting = false;
    }
    submitLocalCacheAll();
    qDebug() << Q_FUNC_INFO << "   end";
}

void LocalCacheManager::submitStop(DataOperationType type, bool binaryType)
{
    qDebug() << Q_FUNC_INFO << "type " << type << "  binaryType " << binaryType;
}

void LocalCacheManager::submitStart(DataOperationType type, bool binaryType)
{
    qDebug() << Q_FUNC_INFO << "type " << type << "  binaryType " << binaryType;
}

void LocalCacheManager::networkConnectChanged(const QString &networkType)
{
    qDebug() << "LocalCacheManager::networkConnectChanged " << networkType;
    if(networkType != ""){
        m_nXmlFailList.clear();
        m_nBinaryFailList.clear();
        submitLocalCacheAll();
    }
}

void LocalCacheManager::sendNoNetworkResponse(const QString &objects, const QString &action)
{
    DbusPackage map;
    map.otherMap.insert("objects", objects);
    map.otherMap.insert("serverCode", "0");
    map.otherMap.insert("description", "no network");
    emit onResultOfResponse(objects, action, false, NETWORK_ERROR_NONETWORK, map);
}

