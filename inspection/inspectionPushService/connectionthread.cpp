#include "connectionthread.h"
#include <QDebug>
#include <QCryptographicHash>
#include "../framework/dataManager/datahandler.h"
#include "../framework/config/sdisruntimecontext.h"
#include "../framework/network/tools/networkencryptionutil.h"
#include "../framework/network/networkheadtool.h"
/**
 * 构造函数
 * @param networkTool 网络对象
 */
ConnectionThread::ConnectionThread(INetworkTool *networkTool,
                                   const QString &address,
                                   const int &port)
{
    qDebug() << "---------------ConnectionThread::ConnectionThread---------------";
    this->moveToThread(this);
    this->networkTool = networkTool;
    this->address = address;
    this->port = port;
}

/**
 * 析构函数
 */
ConnectionThread::~ConnectionThread()
{
}

/**
 * Calls the <code>run()</code> method of the Runnable object the receiver
 * holds. If no Runnable is set, does nothing.
 *
 * @see Thread#start
 */
void ConnectionThread::run() {
    this->networkTool->setTimeout(0);
    qDebug() << "------------------ConnectionThread::run---------------Connect " << address << "  --  " << port;
    int result = networkTool->connectToServer(address, port);
    qDebug() << "------------------ConnectionThread::run---------------Connect   result: " << result;

    if(result != NETWORK_NO_ERROR){
        emit serverConnected(false);
    }else{
        int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
        if(authType == 1){
            if(initAccessToken()){
               emit serverConnected(true);
            }else{
               emit serverConnected(false);
            }
        }else{
           emit serverConnected(true);
        }
    }
}

//升序排列
static bool cmp(const QString &s1, const QString &s2){
    return s1 < s2;
}

void ConnectionThread::generateHead(QByteArray &headBuf, const int &length){
    ProtocolHead head;
    memcpy(head.mark, NETWORK_PROTOCOL_MARK, sizeof(head.mark));
    head.charset = SDISRuntimeContext::getInstance()->getProtocolCharset();
    head.format = SDISRuntimeContext::getInstance()->getProtocolFormat();
    head.mainVersion = NETWORK_PROTOCOL_MAIN_VERSION;
    head.subVersion = NETWORK_PROTOCOL_SUB_VERSION;
    head.result = 0;
    head.statusCode = 0;
    head.length = length;
    head.type = QString(TYPE_SECURITY).toInt();
    bool encrypt = false;
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    if(authType == 1){
        encrypt = true;
    }
    char protocolBuffer[48] = {0};
    int len = NetworkHeadTool::generateProtocolHead(head, encrypt, true, protocolBuffer);
    headBuf.append(protocolBuffer, len);
}

void ConnectionThread::generatePackage(DataPackage &dataPackage){
    long long time = QDateTime::currentMSecsSinceEpoch()/ 1000;
    /**
     * 这个值应该是1000~2000之间
     */
    int nonce = qrand()%1000 + 1000;
    QStringList values;
    values.append(SDISRuntimeContext::getInstance()->getAppKey());
    values.append(QString::number(time));
    values.append(QString::number(nonce));
    qSort(values.begin(), values.end(), cmp);
    QByteArray md5str = values.join("").toLocal8Bit();
    QByteArray md5sum = QCryptographicHash::hash(md5str, QCryptographicHash::Md5);
    QString signature = QString(md5sum.toHex()).toUpper();
    dataPackage.head.type = TYPE_SECURITY;
    dataPackage.head.objects = OBJECT_FETCH_USER_ACCESS_TOKEN;
    dataPackage.para.insert("AppId", SDISRuntimeContext::getInstance()->getAppId());
    dataPackage.para.insert("signature", signature);
    dataPackage.para.insert("Timestamp", QString::number(time));
    dataPackage.para.insert("Nonce", QString::number(nonce));
    dataPackage.para.insert("UserAccount", SDISRuntimeContext::getInstance()->getSystemUser());
    dataPackage.para.insert("PassWord", SDISRuntimeContext::getInstance()->getSystemPassword());
    dataPackage.para.insert("AuthUserType", AUTH_USER_TYPE_SERVICE);
}

bool ConnectionThread::updateToken(const DataPackage &dataPackage){
    AccessToken token;
    if(dataPackage.summary.code != NETWORK_SERVER_RESULT_COMPLETE
            && dataPackage.summary.code != NETWORK_SERVER_RESULT_HANDLING) {
         SDISRuntimeContext::getInstance()->setSystemToken(token);
         return false;
    }
    QString fieldAccount = "UserAccount";
    QString fieldAccessKey = "AccessKey";
    QString fieldAccessToken = "AccessToken";
    QString fieldTokenTimes = "TokenTimes";
    if(dataPackage.tables.isEmpty()) {
        return false;
    }
    DataTable table = dataPackage.tables.at(0);
    QList<DataRow> dataRows = table.value;
    if(dataRows.isEmpty()) {
        return false;
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
    SDISRuntimeContext::getInstance()->setSystemToken(token);
    return true;
}

bool ConnectionThread::initAccessToken(){
    qDebug() << Q_FUNC_INFO;
    DataPackage sentDackage;
    generatePackage(sentDackage);

    DataHandler dataHandler;
    QByteArray srcData;
    if(!dataHandler.encodeData(sentDackage, srcData)) {
        return false;
    }

    QByteArray sentData;
    if(!NetworkEncryptionUtil::getFinalSentData(srcData, sentData)){
        return false;
    }

    QByteArray sentHeadBuf;
    generateHead(sentHeadBuf, sentData.length());
    networkTool->sendData(sentHeadBuf);
    networkTool->sendData(sentData);
    int ret = 0, len = 0;
    ProtocolHead recvHead;
    memset(&recvHead, 0, sizeof(ProtocolHead));
    int headLen = 4;
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    if(authType == 0){
        headLen = 16;
    }else if(authType == 1){
        headLen = 48;
    }
    char recvProtocolBuffer[48] = {0};
    ret = networkTool->recvData(recvProtocolBuffer, headLen);
    NetworkHeadTool::decodeProtocolHead(recvHead, recvProtocolBuffer);
    if(strncmp(recvHead.mark, NETWORK_PROTOCOL_MARK, strlen(NETWORK_PROTOCOL_MARK)) != 0){
        return false;
    }
    len = recvHead.length;
    if(ret < 0) {
        return false;
    }

    QByteArray srcRecvData;
    ret = networkTool->recvData(srcRecvData, len);
    if(ret < 0) {
        return false;
    }

    QByteArray recvData;
    if(!NetworkEncryptionUtil::getFinalReceivedData(srcRecvData, recvData)){
        qDebug() << "--------initAccessToken------getFinalReceivedData fail----";
        return false;
    }

    DataPackage revcPackage;
    if(!dataHandler.decodeData(recvData, revcPackage, recvHead.format, recvHead.charset)){
        qDebug() << "--------initAccessToken------decodeData fail-------------";
        return false;
    }

    return updateToken(revcPackage);
}
