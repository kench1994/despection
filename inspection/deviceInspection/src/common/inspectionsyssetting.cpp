#include "inspectionsyssetting.h"
#include "../../../framework/dataManager/datacrypto.h"
#include "../../../framework/common/functiontype.h"
#include "../../../framework/common/systemconfig.h"
#include "./runtimeparam.h"
#include <QDebug>
#include "common/transmitmanager.h"
#define CONNMAM_SERVICE "net.connman"
#define CONNMAM_PATH "/"
#define CONNMAM_INTERFACE "net.connman.Manager"

#define SIMMAM_SERVICE "org.ofono"
#define SIMMAM_PATH "/ril_0"
#define SIMMAM_INTERFACE "org.ofono.Modem"

//#define CRYPTO_KEY_LENGTH 16
//#define CRYPTO_IV_LENGTH 8
#define CRYPTO_KEY_LENGTH 32
#define CRYPTO_IV_LENGTH 16
InspectionSysSetting::InspectionSysSetting(QObject *parent) :
    QObject(parent)
{
    SystemConfig *sysconf = SystemConfig::getInstance();
    RuntimeParam *runParam = RuntimeParam::getInstance();
    m_nCurrentServerAddr = sysconf->getServerIP();
    m_nCurrentServerPort = sysconf->getServerPort();

    m_nCurrentVersion = sysconf->getVersionInfo();
    m_nCurRFIDScanType = sysconf->getRFIDScanType();
    m_nAuthType = sysconf->getAuthType();
    m_nEncryptType = sysconf->getEncryptType();

    m_nIsShowGPSItem = sysconf->getShowGPSTestItemValue();

    qDebug() << "sysconf->getVersionInfo()" << m_nCurrentVersion;
    QString padId = runParam->getPadId();
    qDebug() << "InspectionSysSetting::InspectionSysSetting  " << m_nCurrentServerAddr << padId;
    connect(&m_nLocateManager,
            SIGNAL(LocateModeChanged(CSystemLocateModeManager::LocateMode)),
            this,
            SLOT(slotLocateModeChanged(CSystemLocateModeManager::LocateMode)));

    connect(&m_nNetWorkManager,
            SIGNAL(cellularEnabledChanged(bool)),
            this,
            SLOT(slotcellularEnabledChanged(bool)));


    connect(&m_nDisplayManager, SIGNAL(brightnessChanged()), this, SLOT(slotBrightnessChanged()));

    QDBusConnection::systemBus().connect(CONNMAM_SERVICE, CONNMAM_PATH, CONNMAM_INTERFACE, "PropertyChanged", this,
                                         SLOT(getConnmamPropertyChanged(const QString&, const QDBusVariant&)));

    QDBusConnection::systemBus().connect(SIMMAM_SERVICE, SIMMAM_PATH,SIMMAM_INTERFACE, "PropertyChanged", this,
                                         SLOT(getSimmamPropertyChanged(const QString&, const QDBusVariant&)));
}

InspectionSysSetting::~InspectionSysSetting()
{

}

//for rfid
QString InspectionSysSetting::getRfidScanType() const
{
    return m_nCurRFIDScanType;
}

void InspectionSysSetting::setRfidScanType(QString rType)
{
    m_nCurRFIDScanType = rType;
    SystemConfig *sysconf = SystemConfig::getInstance();
    sysconf->setRFIDScanType(m_nCurRFIDScanType);
    emit rfidScanTypeChanged(m_nCurRFIDScanType);
}

//for version
QString InspectionSysSetting::getVersionNum() const
{
    return m_nCurrentVersion;
}

void InspectionSysSetting::setVersionNum(QString &vNum)
{
    m_nCurrentVersion = vNum;
    SystemConfig *sysconf = SystemConfig::getInstance();
    sysconf->setVersionInfo(m_nCurrentVersion);
    emit versionNumChanged(m_nCurrentVersion);
}

//for server address
QString InspectionSysSetting::getServerAddr(void) const
{
    return m_nCurrentServerAddr;
}

void InspectionSysSetting::setServerAddr(QString &address)
{
    //增加判断语句——当前存储的服务器地址与系统设置传入的不符合时(20150706)
    if(m_nCurrentServerAddr != address) {
        m_nCurrentServerAddr = address;
        SystemConfig *sysconf = SystemConfig::getInstance();
        sysconf->setServerIP(m_nCurrentServerAddr);
        QVariantMap map;
        map.insert("ip", m_nCurrentServerAddr);
        map.insert("port", m_nCurrentServerPort.toInt());
        TransmitManager::getInstance()->setStatus(FUNCTION_TYPE_STATUS_ADDRESS, map);
    }
    emit serverAddrChanged(m_nCurrentServerAddr);
}

QString InspectionSysSetting::getServerPort() const{
    return m_nCurrentServerPort;
}

void InspectionSysSetting::setServerPort(const QString &serverPort){
    if(m_nCurrentServerPort != serverPort) {
        m_nCurrentServerPort = serverPort;
        SystemConfig *sysconf = SystemConfig::getInstance();
        sysconf->setServerPort(m_nCurrentServerPort);
        QVariantMap map;
        map.insert("ip", m_nCurrentServerAddr);
        map.insert("port", m_nCurrentServerPort.toInt());
        TransmitManager::getInstance()->setStatus(FUNCTION_TYPE_STATUS_ADDRESS, map);
        emit serverPortChanged(m_nCurrentServerPort);
    }
}

void InspectionSysSetting::setAuthType(const int &type)
{
    if(m_nAuthType != type){
        m_nAuthType = type;
        SystemConfig *sysconf = SystemConfig::getInstance();
        sysconf->setAuthType(m_nAuthType);
        QVariantMap map;
        map.insert("authType", m_nAuthType);
        TransmitManager::getInstance()->setStatus(FUNCTION_TYPE_STATUS_AUTHTYPE, map);
    }
}

void InspectionSysSetting::setEncryptType(const int &type)
{
    if(m_nEncryptType != type){
        m_nEncryptType = type;
        SystemConfig *sysconf = SystemConfig::getInstance();
        sysconf->setEncryptType(m_nEncryptType);
        QVariantMap map;
        map.insert("encryptType", m_nEncryptType);
        TransmitManager::getInstance()->setStatus(FUNCTION_TYPE_STATUS_ENCRYPTTYPE, map);
    }
}

QString InspectionSysSetting::getOfflineMode()
{
    if(m_nOfflineMode.isEmpty())
        m_nOfflineMode = SystemConfig::getInstance()->getOfflineModel();
    return m_nOfflineMode;
}

void InspectionSysSetting::setOfflineMode(const QString &offlineMode)
{
    if(m_nOfflineMode != offlineMode){
        m_nOfflineMode = offlineMode;
        SystemConfig *sysconf = SystemConfig::getInstance();
        sysconf->setOfflineModel(m_nOfflineMode);
        QVariantMap map;
        map.insert("offlineMode", m_nOfflineMode);
        TransmitManager::getInstance()->setStatus(FUNCTION_TYPE_STATUS_OFFLINEMODE, map);
    }
}

//for gps
void InspectionSysSetting::slotLocateModeChanged(CSystemLocateModeManager::LocateMode mode)
{
    QString modeStr = "NoneMode";

    if(mode == CSystemLocateModeManager::SatelliteNetworkMode){
        modeStr = "SatelliteNetworkMode";
    } else if(mode == CSystemLocateModeManager::SatelliteMode){
        modeStr = "SatelliteMode";
    } else if(mode == CSystemLocateModeManager::NetworkMode){
        modeStr = "NetworkMode";
    } else {
        modeStr = "NoneMode";
    }

    emit sglLocateModeChanged(modeStr);
}

QString InspectionSysSetting::getLocateMode(void)
{
    QString modeStr = "NoneMode";
    qDebug() << "getLocateMode m_nLocateManager.getLocateMode(); ";
    CSystemLocateModeManager::LocateMode mode = m_nLocateManager.getLocateMode();
    qDebug() << "getLocateMode m_nLocateManager.getLocateMode(); ok";
    if(mode == CSystemLocateModeManager::SatelliteNetworkMode) {
        modeStr = "SatelliteNetworkMode";
    } else if(mode == CSystemLocateModeManager::SatelliteMode){
        modeStr = "SatelliteMode";
    } else if(mode == CSystemLocateModeManager::NetworkMode){
        modeStr = "NetworkMode";
    } else {
        modeStr = "NoneMode";
    }
    return modeStr;
}

bool InspectionSysSetting::setLocateMode(QString modeStr)
{
    qDebug() << "setLocateMode m_nLocateManager.getLocateMode();";
    CSystemLocateModeManager::LocateMode mode = m_nLocateManager.getLocateMode();
    qDebug() << "setLocateMode m_nLocateManager.getLocateMode(); ok";
    if(mode == CSystemLocateModeManager::SatelliteNetworkMode){
        if(modeStr.compare("NoneMode", Qt::CaseInsensitive) == 0){
            mode = CSystemLocateModeManager::NetworkMode;
        }
    } else if(mode == CSystemLocateModeManager::SatelliteMode) {
        if(modeStr.compare("NoneMode", Qt::CaseInsensitive) == 0) {
            mode = CSystemLocateModeManager::NoneMode;
        }
    } else if(mode == CSystemLocateModeManager::NetworkMode) {
        if(modeStr.compare("SatelliteMode", Qt::CaseInsensitive) == 0){
            mode = CSystemLocateModeManager::SatelliteNetworkMode;
        }
    } else {
        if(modeStr.compare("SatelliteMode", Qt::CaseInsensitive) == 0){
            mode = CSystemLocateModeManager::SatelliteMode;
        }
    }
    qDebug() << "m_nLocateManager.setLocateMode(mode);";
    bool flag = m_nLocateManager.setLocateMode(mode);
    qDebug() << "m_nLocateManager.setLocateMode(mode); ok";
    return flag;
}
QStringList InspectionSysSetting::setPassword(QString passwd){
    qDebug()<<"    begin  set passwd";
    const int CRYPTO_MAX_LENGTH = 1024;
    unsigned char buf[CRYPTO_MAX_LENGTH];
    int outLen = 0;
    memset(buf,0,sizeof(buf));
    unsigned char iv[CRYPTO_IV_LENGTH]={0} ;//加密的初始化向量
    unsigned char key[CRYPTO_KEY_LENGTH]={0};
    memset(key,0,sizeof(key));
    memset(iv,0,sizeof(iv));
    char *tmp = "0123456789ABCDEFGHIJKLMNOPQRSTUVWZYZ";
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    for(int i=0;i<CRYPTO_KEY_LENGTH;i++) {
        int ir = qrand()%strlen(tmp);
        key[i] = tmp[ir];
        if(i< CRYPTO_IV_LENGTH)
           iv[i] = tmp[ir];
    }
    QByteArray bytekey ((const char *)key,CRYPTO_KEY_LENGTH);
    QString out_key = bytekey.toBase64();
    QByteArray byteiv((const char *)iv,CRYPTO_IV_LENGTH);
    QString out_iv =  byteiv.toBase64();
    QByteArray pwdByteArray = passwd.toUtf8();
    bool ret = DataCrypto::encrypt((const char*)pwdByteArray.data(),
                                   pwdByteArray.size(),
                                   buf, &outLen,
                                   key,
                                   iv,
                                   AES_CBC_256_MODE);
    if(!ret)
        qDebug()<<"Failed";
    QByteArray pwd((const char*)buf,outLen);
    QString out_pwd = pwd.toBase64();
    qDebug() << "passwd = " << out_pwd  << "   key is  "<<out_key<<" iv  is   "<<out_iv;
    QStringList list;
    list.append(out_pwd);
    list.append(out_key);
    list.append(out_iv);
    return list;

}

QString InspectionSysSetting::getPassword(const QString &strpasswd, const QString &strkey, const QString &striv) const
{
    const int CRYPTO_MAX_LENGTH = 1024;
    QByteArray pwd = QByteArray::fromBase64(strpasswd.toLatin1());
    QByteArray key = QByteArray::fromBase64(strkey.toLatin1());
    QByteArray iv = QByteArray::fromBase64(striv.toLatin1());

    unsigned char buf[1024] = {0};
    int outLen = 0;
    bool ret = DataCrypto::decrypt((const unsigned char *)pwd.data(),
                                   buf,
                                   pwd.size(),
                                   outLen,
                                   (unsigned char *)key.data(),
                                   (unsigned char *)iv.data(),
                                   AES_CBC_256_MODE);
    qDebug() << "test Crypto " << ret;
    if(!ret){
        qDebug() << "Fail!";
    }
    memset(buf + outLen, 0, CRYPTO_MAX_LENGTH - outLen);
    QString out1 = QString::fromUtf8((const char *)buf);
    qDebug() << "passwd  QString::fromUtf8   is    " <<out1;
    return out1;
}

int InspectionSysSetting::getEncryptType()
{
    return m_nEncryptType;
}

int InspectionSysSetting::getAuthType()
{
    return m_nAuthType;
}

void InspectionSysSetting::setWakeLock(const bool& value)
{
    if(value){
        qDebug() << "setWakeLock m_nPowerManager.accquireWakelock(); ";
        m_nPowerManager.accquireWakelock();
        qDebug() << "setWakeLock m_nPowerManager.accquireWakelock(); ok";
    }else{
        qDebug() << "setWakeLock m_nPowerManager.releaseWakelock(); ";
        m_nPowerManager.releaseWakelock();
        qDebug() << "setWakeLock m_nPowerManager.releaseWakelock(); ok";
    }
}

bool InspectionSysSetting::getGPSTestItemShowStatus()
{
    qDebug() << " InspectionSysSetting::getGPSTestItemShowStatus()----------ret:" << m_nIsShowGPSItem;
    return m_nIsShowGPSItem;
}

void InspectionSysSetting::setKeyPara(QString account, QString  code)
{
    QVariantMap map;
    map.insert("account", account);
    map.insert("code", code);
    TransmitManager::getInstance()->setStatus(FUNCTION_TYPE_STATUS_RESET_PARA_SERVICE, map);
}

void InspectionSysSetting::reBootDevice(void)
{
    m_nPowerManager.reboot();
}

void InspectionSysSetting::shutDownDevice(void)
{
    m_nPowerManager.shutdown();
}

int InspectionSysSetting::getMaximumBrightness(void)
{
    qDebug() << "InspectionSysSetting::getMaximumBrightness ";
    int nMax = m_nDisplayManager.maximumBrightness();
    qDebug() << "InspectionSysSetting::getMaximumBrightness  ok  " << nMax;
    return nMax;
}

int InspectionSysSetting::getBrightness(void)
{
    qDebug() << "InspectionSysSetting::getBrightness ";
    int nBrightValue = m_nDisplayManager.brightness();
    qDebug() << "InspectionSysSetting::getBrightness  ok  " << nBrightValue;
    return nBrightValue;
}

void InspectionSysSetting::setBrightness(int nValue)
{
    qDebug() << "InspectionSysSetting::setBrightness ";
    m_nDisplayManager.setBrightness(nValue);
    qDebug() << "InspectionSysSetting::setBrightness  ok  " << nValue;
}

bool InspectionSysSetting::isAirplaneMode()
{
    qDebug() << "InspectionSysSetting::isAirplaneMode ";
    bool ret = m_nNetWorkManager.isAirplaneModeOn();
    qDebug() << "----------m_nNetWorkManager.isAirplaneModeOn()----------ret:" << ret;
    return ret;
}

bool InspectionSysSetting::hasSIM()
{
    QDBusInterface *sim_interface = new QDBusInterface("org.ofono",
                                                       "/ril_0",
                                                       "org.ofono.Modem",
                                                       QDBusConnection::systemBus());

    if( !( sim_interface->isValid() ) )
    {
        qDebug()<<"gps_interface->is  unValid())";
        delete sim_interface;
        qDebug() << "----------InspectionSysSetting::hasSIM()----------" << false;
        return false;
    }

    QDBusReply<QVariantMap> reply = sim_interface->call("GetProperties");
    if ( reply.isValid() )
    {
        qDebug() << "QDBusMessage::ReplyMessage get homemnc ok ";
        if( reply.value().value( QStringLiteral("Simpresent") ).toInt() == 1)
        {
            qDebug() << "QDBusMessage::ReplyMessage get homemnc ok Simpresent 1";
            delete sim_interface;
            qDebug() << "----------InspectionSysSetting::hasSIM()----------" << true;
            return true;
        } else {
            qDebug() << "QDBusMessage::ReplyMessage get homemnc ok Simpresent "
                     << reply.value().value( QStringLiteral("Simpresent") ).toInt();
            delete sim_interface;
            qDebug() << "----------InspectionSysSetting::hasSIM()----------" << false;
            return false;
        }
    } else {
        qDebug() << "QDBusMessage::ReplyMessage get homemnc fail  " << reply.error();
    }
    delete sim_interface;
    qDebug() << "----------InspectionSysSetting::hasSIM()----------" << false;
    return false;
}

bool InspectionSysSetting::isCellularConnected()
{
    qDebug() << "InspectionSysSetting::isCellularConnected() ";
    bool ret = m_nNetWorkManager.cellularEnabled();
    qDebug() << "----------m_nNetWorkManager.cellularEnabled()----------ret:" << ret;
    return ret;
}

bool InspectionSysSetting::setCellularEnabled(const bool flag)
{
    bool ret = m_nNetWorkManager.setCellularEnabled(flag);
    qDebug() << "----------m_nNetWorkManager.setCellularEnabled(flag)----------flag:" << flag << "----------ret:" << ret;
    return ret;
}

void InspectionSysSetting::slotcellularEnabledChanged(bool flag)
{
    qDebug() << "----------slotcellularEnabledChanged----------flag:" << flag;
    emit sglcellularEnabledChanged(flag);
}

void InspectionSysSetting::slotBrightnessChanged()
{
    int nCurBrightValue = m_nDisplayManager.brightness();
    emit sglBrightnessChanged(nCurBrightValue);
}

void InspectionSysSetting::getConnmamPropertyChanged(const QString &name, const QDBusVariant &value)
{
    bool bOfflineFlag;
    qDebug() << "InspectionSysSetting::getSimmamPropertyChanged: name---" << name << " -value---- " << value.variant().toBool();
    if(name == "offlineMode")
    {
        bOfflineFlag = value.variant().toBool() ;
        qDebug() << "----------getConnmamPropertyChanged:offlineProperty----------offlineMode:" <<  bOfflineFlag;

        bool bSimFlag = hasSIM();
        qDebug() << "----------getConnmamPropertyChanged----------sglSim_OfflinePropertyChanged(bOfflineFlag, bSimFlag) " << bOfflineFlag << bSimFlag;
        sglSim_OfflinePropertyChanged(bOfflineFlag, bSimFlag);
    }
}

void InspectionSysSetting::getSimmamPropertyChanged(const QString &name, const QDBusVariant &value)
{
    bool bSimFlag = false;
    qDebug() << "InspectionSysSetting::getSimmamPropertyChanged: name---" << name << " -value---- " << value.variant().toInt();
    if(name == "Simpresent")
    {
        if(value.variant().toInt() == 1){
            qDebug() << "----------getSimmamPropertyChanged: simProperty----------hasSim" ;
            bSimFlag = true;
        } else if(value.variant().toInt() == 0){
            qDebug() << "----------getSimmamPropertyChanged: simProperty----------noSim" ;
            bSimFlag = false;
        }

        bool bOfflineFlag = isAirplaneMode();
        qDebug() << "----------getConnmamPropertyChanged----------sglSim_OfflinePropertyChanged(bOfflineFlag, bSimFlag) " << bOfflineFlag << bSimFlag;
        sglSim_OfflinePropertyChanged(bOfflineFlag, bSimFlag);
    }
}
