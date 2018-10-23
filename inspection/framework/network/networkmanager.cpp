#include "networkmanager.h"
#include "networkheadtool.h"
#include "../config/sdisruntimecontext.h"
#include "../common/systemconfig.h"
#include "../common/functiontype.h"
#include "../dataManager/datacrypto.h"
#include <QCryptographicHash>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

#define DBG_TAG "NetworkManager : "

NetworkManager* NetworkManager::pInstance = NULL;

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent)
{
    initialize();
}

NetworkManager::~NetworkManager()
{
}

void NetworkManager::initialize()
{
    connect(&mBinaryManager,
            SIGNAL(error(const QString, const int, const QString)),
            this,
            SIGNAL(binaryError(const QString, const int, const QString)));
    connect(&mBinaryManager,
            SIGNAL(response(const QString, const QString)),
            this,
            SIGNAL(binaryResponse(const QString, const QString)));
    connect(&mBinaryManager,
            SIGNAL(progress(const QString, const int, const int, const int)),
            this,
            SIGNAL(binaryProgress(const QString, const int, const int, const int)));
    connect(&mCommandManager,
            SIGNAL(error(const QString, const QString, const int, const int, const QString)),
            this,
            SIGNAL(cmdError(const QString, const QString, const int, const int, const QString)));
    connect(&mCommandManager,
            SIGNAL(response(const QString, const QString, const QString, const QByteArray, const ProtocolHead)),
            this,
            SLOT(cmdResponseInner(const QString, const QString, const QString, const QByteArray, const ProtocolHead)));
    connect(this,
            SIGNAL(systemParaUpdated(bool,QString,QString,QString,QString,QString,int)),
            this,
            SLOT(updateSystemPara(bool,QString,QString,QString,QString,QString,int)));

}

void NetworkManager::setTarget(const QString& address, const int& port)
{
    qDebug() << "NetworkManager::setTarget   " << address << " port   " << port;
    if(address.isEmpty() || port <= 0) {
        return;
    }
    SDISRuntimeContext::getInstance()->setServerAddress(address);
    SDISRuntimeContext::getInstance()->setServerPort(port);
}

bool NetworkManager::sendCommandRequest(const DataPackage package, const QString action)
{
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    AccessToken token = SDISRuntimeContext::getInstance()->getCurrentToken();
    if(authType == 1 && token.accessKey == 0){
        emit cmdError(package.head.objects, action, NETWORK_ERROR_TOKEN_INVALID_OR_TIMEOUT, 0, "Token haven't initialized yet");
        return false;
    }
    return sendCommandRequestInner(package, action);
}

void NetworkManager::sendBinaryRequest(const QString remotePath, const QString localPath,
                                       const QString action, const QString name, const QString title, const QString description,
                                       const QString seriesKey, bool download) {
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    AccessToken token = SDISRuntimeContext::getInstance()->getCurrentToken();
    if(authType == 1 && token.accessKey == 0){
        emit binaryError(action, NETWORK_ERROR_TOKEN_INVALID_OR_TIMEOUT, "Token haven't initialized yet");
        return;
    }
    mBinaryManager.addTask(action, remotePath, localPath, name, title, description, seriesKey, download);
}

/**
 * 初始化网络令牌（登陆的时候需要调用）
 *
 * @param userName 用户名
 * @param password 密码
 */
void NetworkManager::initAccessToken(const QString &userName, const QString &password)
{
    initUserAccessTokenInner(userName, password, AUTH_USER_TYPE_BUSINESS, ACTION_FETCH_USER_ACCESS_TOKEN);
}

void NetworkManager::initSystemAccessToken()
{
    /**
     * 初始化系统用户Token
     */
    initUserAccessTokenInner(SDISRuntimeContext::getInstance()->getSystemUser(), SDISRuntimeContext::getInstance()->getSystemPassword(), AUTH_USER_TYPE_SERVICE, ACTION_FETCH_SYSTEM_ACCESS_TOKEN);
}

void NetworkManager::resetSecurityParameter(QString code, QString account)
{
    DataPackage data;
    long long time = QDateTime::currentMSecsSinceEpoch()/ 1000;
    /**
     * 这个值应该是1000~2000之间
     */
    int nonce = qrand()%1000 + 1000;
    securityCode = code;

    data.head.type = TYPE_APP_PARA;
    data.head.objects = OBJECT_FETCH_APPLICATION_PARAMETER;
    data.para.insert("AppId", SDISRuntimeContext::getInstance()->getAppId());
    QString signature = generateSignature(time, nonce, code);
    data.para.insert("signature", signature);
    data.para.insert("Timestamp", QString::number(time));
    data.para.insert("Nonce", QString::number(nonce));
    data.para.insert("UserAccount", account);
    sendCommandRequestInner(data, ACTION_FETCH_APP_PARA);
}

void NetworkManager::cmdResponseInner(const QString objects,
                                    const QString type,
                                    const QString action,
                                    const QByteArray data,
                                    const ProtocolHead protocolHead)
{
    qDebug() << " NetworkManager::actionResponse type is " << type;
    DataPackage package;
    bool ret = mDataHandler.decodeData(data, package, protocolHead.format, protocolHead.charset);
    if(!ret) {
        emit cmdError(objects, action, NETWORK_ERROR_DECODE_FAIL, 0, "decode XML fail");
        return;
    }

    if(type == TYPE_REQUEST){
        if(package.summary.code != NETWORK_SERVER_RESULT_COMPLETE &&
                package.summary.code != NETWORK_SERVER_RESULT_HANDLING){
            if(package.summary.code == NETWORK_SERVER_TOKEN_INVALID ||
                    package.summary.code == NETWORK_SERVER_TOKEN_TIMEOUT){
                emit cmdError(objects, action, NETWORK_ERROR_TOKEN_INVALID_OR_TIMEOUT,
                                  package.summary.code, package.summary.description);
            }else{
                emit cmdError(objects, action, NETWORK_ERROR_SERVER_HANDLE,  package.summary.code, package.summary.description);
            }

        }else{
            emit cmdResponse(objects, type, action, package);
        }
    }else if(type == TYPE_COMMIT || type == TYPE_COMMIT_OTHER || type == TYPE_PUSH){
        // no data content
        if(package.summary.code != NETWORK_SERVER_RESULT_COMPLETE &&
                package.summary.code != NETWORK_SERVER_RESULT_HANDLING){
            if(package.summary.code == NETWORK_SERVER_TOKEN_INVALID ||
                    package.summary.code == NETWORK_SERVER_TOKEN_TIMEOUT){
                emit cmdError(objects, action, NETWORK_ERROR_TOKEN_INVALID_OR_TIMEOUT,
                                  package.summary.code, package.summary.description);
            }else{
                emit cmdError(objects, action, NETWORK_ERROR_SERVER_HANDLE,
                                  package.summary.code, package.summary.description);
            }
        }else{
            qDebug() << DBG_TAG << package.summary.description << " - " << package.summary.code;
            QString code = QString("%1").arg(package.summary.code);
            emit cmdResult(objects, type, action, code);
        }
    }else if(type == TYPE_SECURITY){
        if(action == ACTION_FETCH_USER_ACCESS_TOKEN && objects == OBJECT_FETCH_USER_ACCESS_TOKEN) {
            updateCurrentToken(package, false);
        }else if(action == ACTION_FETCH_SYSTEM_ACCESS_TOKEN && objects == OBJECT_FETCH_USER_ACCESS_TOKEN) {
            updateCurrentToken(package, true);
        }
    }else if(type == TYPE_APP_PARA){
        if(action == ACTION_FETCH_APP_PARA){
            updateSystemParameter(package);
        }
    }
}

void NetworkManager::updateSystemPara(bool success, QString appKey, QString appSecretKey, QString appSecretIV, QString appPublicKey, QString appPrivateKey, int errorNo)
{
    if(success){

        SystemConfig *sysConf = SystemConfig::getInstance();
        sysConf->setAppKey(appKey);
        sysConf->setAppSecretKey(appSecretKey);
        sysConf->setAppSecretIV(appSecretIV);
        sysConf->setAppPublicKey(appPublicKey);
        sysConf->setAppPriaveKey(appPrivateKey);

        SDISRuntimeContext::getInstance()->setAppKey(appKey);
        SDISRuntimeContext::getInstance()->setAppSecretKey(appSecretKey);
        SDISRuntimeContext::getInstance()->setAppSecretIV(appSecretIV);
        SDISRuntimeContext::getInstance()->setAppPublicKey(appPublicKey);
        SDISRuntimeContext::getInstance()->setAppPrivateKey(appPrivateKey);

        int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
        if(authType == 1){
            initSystemAccessToken();
        }

        QDBusMessage m = QDBusMessage::createSignal(SERVICE_DBUS_PATH,  SERVICE_DBUS_INTERFACE, "statusUpdated");
        QVariant varType, varMap;
        QVariantMap map;
        varType.setValue(QString(FUNCTION_TYPE_STATUS_RESET_PARA_PUSH));
        map.insert("appKey", appKey);
        map.insert("appSecretKey", appSecretKey);
        map.insert("appSecretIV", appSecretIV);
        map.insert("appPublicKey", appPublicKey);
        map.insert("appPrivateKey", appPrivateKey);
        varMap.setValue(map);
        m << varType << varMap;
        QDBusConnection::sessionBus().send(m);

    }
}

void NetworkManager::initUserAccessTokenInner(const QString &userName,
                                              const QString &password,
                                              const QString &authUserType,
                                              const QString &action)
{
    DataPackage data;
    long long time = QDateTime::currentMSecsSinceEpoch()/ 1000;
    /**
     * 这个值应该是1000~2000之间
     */
    int nonce = qrand()%1000 + 1000;
    data.head.type = TYPE_SECURITY;
    data.head.objects = OBJECT_FETCH_USER_ACCESS_TOKEN;
    data.para.insert("AppId", SDISRuntimeContext::getInstance()->getAppId());
    QString signature = generateSignature(time, nonce, SDISRuntimeContext::getInstance()->getAppKey());
    data.para.insert("signature", signature);
    data.para.insert("Timestamp", QString::number(time));
    data.para.insert("Nonce", QString::number(nonce));
    data.para.insert("UserAccount", userName);
    data.para.insert("PassWord", password);
    data.para.insert("AuthUserType", authUserType);
    sendCommandRequestInner(data, action);
}

bool NetworkManager::sendCommandRequestInner(const DataPackage package, const QString action)
{
    QByteArray data;
    if(!mDataHandler.encodeData(package, data)) {
        qDebug() << DBG_TAG << "encode XML fail";
        return false;
    }
    CommandTask task;
    task.action = action;
    task.data = data;
    task.disposing = false;
    task.objects = package.head.objects;
    task.type = package.head.type;
    mCommandManager.addTask(task);
    return true;
}

NetworkManager* NetworkManager::getInstance(QObject* parent)
{
    if(pInstance == NULL){
        pInstance = new NetworkManager(parent);
    }
    return pInstance;
}

void NetworkManager::setDeviceID(const QString& value)
{
    mDataHandler.setDeviceID(value);
    SDISRuntimeContext::getInstance()->setDeviceID(value);
}

//升序排列
static bool cmp(const QString &s1, const QString &s2){
    return s1 < s2;
}

QString NetworkManager::generateSignature(const long long &time, const int &nonce, const QString &appKey) {
    QStringList values;
    values.append(appKey);
    values.append(QString::number(time));
    values.append(QString::number(nonce));
    qSort(values.begin(), values.end(), cmp);
    QByteArray md5str = values.join("").toLocal8Bit();
    QByteArray md5sum = QCryptographicHash::hash(md5str, QCryptographicHash::Md5);
    QString md5Ret(md5sum.toHex());
    qDebug() <<"----------generateSignature------------md5Ret: " << md5Ret;
    return md5Ret.toUpper();
}

void NetworkManager::updateCurrentToken(const DataPackage &data, const bool &system) {
    AccessToken token;
    memset((void*)&token, 0, sizeof(AccessToken));
    if(data.summary.code != NETWORK_SERVER_RESULT_COMPLETE
            && data.summary.code != NETWORK_SERVER_RESULT_HANDLING) {
        qDebug() <<"---------updateCurrentToken------------Update token fail : " << data.summary.code;
        if(system) {
            SDISRuntimeContext::getInstance()->setSystemToken(token);
        }else{
            SDISRuntimeContext::getInstance()->setUserToken(token);
        }
        emit accessTokenUpdated(false, system, data.summary.code, data.summary.description, NETWORK_NO_ERROR);
        return;
    }
    QString fieldAccount = "UserAccount";
    QString fieldAccessKey = "AccessKey";
    QString fieldAccessToken = "AccessToken";
    QString fieldTokenTimes = "TokenTimes";
    if(data.tables.isEmpty()) {
        return;
    }
    DataTable table = data.tables.at(0);
    QList<DataRow> dataRows = table.value;
    if(dataRows.isEmpty()) {
        return;
    }
    DataRow dataRow = dataRows.at(0);
    QString account = dataRow.value(fieldAccount);
    QString accessKey = dataRow.value(fieldAccessKey);
    QString accessToken = dataRow.value(fieldAccessToken);
    QString tokenTimes = dataRow.value(fieldTokenTimes);
    token.accessKey = accessKey.toLongLong();
    token.accessToken = accessToken.toLongLong();
    token.deadLine = tokenTimes.toLongLong();
    memcpy((void *)&(token.account), (void *)(account.toLocal8Bit().data()), account.length());
    qDebug() <<"---------updateCurrentToken--------------token.accessKey : " << token.accessKey;
    qDebug() <<"---------updateCurrentToken------------token.accessToken : " << token.accessToken;
    if(system) {
        SDISRuntimeContext::getInstance()->setSystemToken(token);
    }else{
        SDISRuntimeContext::getInstance()->setUserToken(token);
    }
    emit accessTokenUpdated(true, system, 0, "", NETWORK_NO_ERROR);
}

void NetworkManager::updateSystemParameter(const DataPackage &data)
{
    if(data.summary.code == NETWORK_SERVER_CODE_INVALID ||
            data.summary.code == NETWORK_SERVER_COMMAND_TIMEOUT) {
        qDebug() <<"------------updateSystemParameter---------Reset app para error : "  << data.summary.code;
        emit systemParaUpdated(false, "", "", "", "", "", NETWORK_SERVER_COMMAND_TIMEOUT);
        return;
    }
    DataTable table = data.tables.at(0);
    QList<DataRow> dataRows = table.value;
    if(dataRows.size() <= 0) {
        emit systemParaUpdated(false, "", "", "", "", "", NETWORK_ERROR_DECODE_FAIL);
        return;
    }
    DataRow dataRow = dataRows.at(0);
    QString appKeyBase64 = dataRow.value("AppKey");
    QString appSecretKeyBase64 = dataRow.value("AppSecretKey");
    QString appSecretIVBase64 = dataRow.value("AppSecretIV");
    QString appPrivateKeyBase64 = dataRow.value("AppPrivateKey");
    QString appPublicKeyBase64 = dataRow.value("AppPublicKey");


    unsigned char in[1024] = {0};
    unsigned char out[1024] = {0};
    int inLen = 0;
    int outLen = 0;
    QByteArray keyBytes = securityCode.toLocal8Bit();
    QByteArray ivBytes = securityCode.toLocal8Bit();
    unsigned char *devKey = (unsigned char *)keyBytes.data();
    unsigned char *decIV = (unsigned char *)ivBytes.data();
    QByteArray temp = QByteArray::fromBase64(appKeyBase64.toLocal8Bit());
    inLen = temp.size();
    memcpy((void *)in, (void *)(temp.data()), inLen);
    bool ret = DataCrypto::decrypt((const unsigned char *)in,
                                   out,
                                   inLen,
                                   outLen,
                                   devKey,
                                   decIV,
                                   AES_CBC_128_MODE, false);
    if(!ret){
        emit systemParaUpdated(false, "", "", "", "", "", NETWORK_ERROR_DECODE_FAIL);
        return;
    }
    QString appKey = QString::fromUtf8((const char *)out, outLen);
    qDebug() <<"-----------appKey: " << appKey;

    memset((void *)in, 0, sizeof(in));
    memset((void *)out, 0, sizeof(out));
    temp = QByteArray::fromBase64(appSecretKeyBase64.toLocal8Bit());
    inLen = temp.size();
    memcpy((void *)in, (void *)(temp.data()), inLen);
    ret = DataCrypto::decrypt((const unsigned char *)in,
                              out,
                              inLen,
                              outLen,
                              devKey,
                              decIV,
                              AES_CBC_128_MODE, false);
    if(!ret){
        emit systemParaUpdated(false, "", "", "", "", "", NETWORK_ERROR_DECODE_FAIL);
        return;
    }
    QString appSecretKey = QString::fromUtf8((const char *)out, outLen);
    qDebug() <<"-----------appSecretKey: " << appSecretKey;

    memset((void *)in, 0, sizeof(in));
    memset((void *)out, 0, sizeof(out));
    temp = QByteArray::fromBase64(appSecretIVBase64.toLocal8Bit());
    inLen = temp.size();
    memcpy((void *)in, (void *)(temp.data()), inLen);
    ret = DataCrypto::decrypt((const unsigned char *)in,
                              out,
                              inLen,
                              outLen,
                              devKey,
                              decIV,
                              AES_CBC_128_MODE, false);
    if(!ret){
        emit systemParaUpdated(false, "", "", "", "", "", NETWORK_ERROR_DECODE_FAIL);
        return;
    }
    QString appSecretIV = QString::fromUtf8((const char *)out, outLen);
    qDebug() <<"-----------appSecretIV: " << appSecretIV;

    QString appPublicKey = "";
    if(appPublicKeyBase64 != ""){
        memset((void *)in, 0, sizeof(in));
        memset((void *)out, 0, sizeof(out));
        temp = QByteArray::fromBase64(appPublicKeyBase64.toLocal8Bit());
        inLen = temp.size();
        memcpy((void *)in, (void *)(temp.data()), inLen);
        ret = DataCrypto::decrypt((const unsigned char *)in,
                                  out,
                                  inLen,
                                  outLen,
                                  devKey,
                                  decIV,
                                  AES_CBC_128_MODE, false);
        if(!ret){
            emit systemParaUpdated(false, "", "", "", "", "", NETWORK_ERROR_DECODE_FAIL);
            return;
        }
        appPublicKey = QString::fromUtf8((const char *)out, outLen);
    }

    QString appPrivateKey = "";
    if(appPrivateKeyBase64 != ""){
        memset((void *)in, 0, sizeof(in));
        memset((void *)out, 0, sizeof(out));
        temp = QByteArray::fromBase64(appPrivateKeyBase64.toLocal8Bit());
        inLen = temp.size();
        memcpy((void *)in, (void *)(temp.data()), inLen);
        ret = DataCrypto::decrypt((const unsigned char *)in,
                                  out,
                                  inLen,
                                  outLen,
                                  devKey,
                                  decIV,
                                  AES_CBC_128_MODE, false);
        if(!ret){
            emit systemParaUpdated(false, "", "", "", "", "", NETWORK_ERROR_DECODE_FAIL);
            return;
        }
        appPrivateKey = QString::fromUtf8((const char *)out, outLen);
    }
    emit systemParaUpdated(true, appKey, appSecretKey, appSecretIV, appPublicKey, appPrivateKey, NETWORK_NO_ERROR);
}


