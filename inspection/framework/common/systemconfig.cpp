#include "systemconfig.h"
#include "utils.h"
#include <QSettings>
#include <QDebug>
#include <cenvironment.h>
#include <framework/cuiappinfo.h>
#include <framework/cpackageinfo.h>
#include <framework/csystempackagemanager.h>
#include <csystempowermanager.h>
#include <QFile>
#include <QProcess>
#include <QSound>
#include <QSoundEffect>
#include <QTimer>
#include <QUuid>
extern QString gAppInstallPath;
SystemConfig* SystemConfig::m_pSysConf = NULL;
SystemConfig::SystemConfig():
    m_nConnectNetworkType(""),
    m_nIsMessagePage(false),
    m_nPlayTimerRunning(false)
{

    CEnvironment cenv;
    m_nAppDataPath = cenv.dataPath();
    m_nAppInstallPath = gAppInstallPath;
    qDebug() << "cenvironment  data path      " << m_nAppDataPath + "/" + CONFIG_FILE << "  intall path  " << m_nAppInstallPath + "/" + CONFIG_FILE;
    qDebug() <<m_nAppDataPath + "/" + CONFIG_FILE << " exist   is " << QFile::exists(m_nAppDataPath + "/" + CONFIG_FILE);
    qDebug() <<m_nAppInstallPath + "/" + CONFIG_FILE << " exist   is " << QFile::exists(m_nAppInstallPath + "/" + CONFIG_FILE);
    if(!QFile::exists(m_nAppDataPath + "/" + CONFIG_FILE)){
        qDebug() << "QFile::exists(m_nAppDataPath    ";
        QFile::copy(m_nAppInstallPath + "/" + CONFIG_FILE, m_nAppDataPath + "/" + CONFIG_FILE);
        QFile::setPermissions(m_nAppDataPath + "/" + CONFIG_FILE,
                              QFileDevice::ReadOwner|QFileDevice::WriteOwner|QFileDevice::ExeOwner|
                              QFileDevice::ReadGroup|QFileDevice::WriteGroup|QFileDevice::ExeGroup);
    } else {
        QSettings oldSettings(m_nAppDataPath + "/" + CONFIG_FILE, QSettings::IniFormat);
        QSettings newSettings(m_nAppInstallPath + "/" + CONFIG_FILE, QSettings::IniFormat);
        QString oldVersion = oldSettings.value("system/version", "").toString();
        QString newVersion = newSettings.value("system/version", "").toString();
        if(oldVersion < newVersion){
            QFile::remove(m_nAppDataPath + "/" + CONFIG_FILE);
            QFile::copy(m_nAppInstallPath + "/" + CONFIG_FILE, m_nAppDataPath + "/" + CONFIG_FILE);
            QFile::setPermissions(m_nAppDataPath + "/" + CONFIG_FILE,
                                  QFileDevice::ReadOwner|QFileDevice::WriteOwner|QFileDevice::ExeOwner|
                                  QFileDevice::ReadGroup|QFileDevice::WriteGroup|QFileDevice::ExeGroup);
        }
    }

    m_pSettings = new QSettings(m_nAppDataPath + "/" + CONFIG_FILE, QSettings::IniFormat);
    //m_nSystemPowerMng = new CSystemPowerManager();
    //connect(m_nSystemPowerMng, SIGNAL(displayStateChanged()), this, SLOT(displayState()));
    m_nPowerMng = new CPowerManager();
}

SystemConfig* SystemConfig::getInstance(){
    if(m_pSysConf == NULL)
        m_pSysConf = new SystemConfig();
    return m_pSysConf;
}

QString SystemConfig::getSequence()
{
    return getUUID();
}

QString SystemConfig::getUUID(){
    QUuid id = QUuid::createUuid();
    QString strId = id.toString();
    strId.remove(QChar('{'));
    strId.remove(QChar('}'));
    return strId;
}

QString SystemConfig::getOsVersion()
{
    QString path = getOsVersionFilePath();
    QString content;
    bool flag = false;
    QFile fl(path);
    if(fl.open(QIODevice::ReadOnly)){
        QTextStream in(&fl);
        while (!in.atEnd()) {
            content = in.readLine();
            qDebug() << "content is  " << content;
            if(content.indexOf("VERSION_ID") != -1){
                flag = true;
                content = content.right(content.length() - content.indexOf("=") - 1);
                qDebug() << "content is  00000  " << content;
                break;
            }
        }
    }
    if(flag == false)
        content = "";
    return content;
}

QString SystemConfig::getOfflineModel()
{
    return m_pSettings->value("appSystem/offlineModel", "1").toString();
}

void SystemConfig::setOfflineModel(const QString &offlineModel)
{
    m_pSettings->setValue("appSystem/offlineModel", offlineModel);
}

void SystemConfig::setOfflineTaskInitTime(const QString initTime)
{
    m_pSettings->setValue("appSystem/offlineTaskInitTime", initTime);
}

QString SystemConfig::getOfflineTaskInitTime()
{
    return m_pSettings->value("appSystem/offlineTaskInitTime", "1970-01-01 00:00:00").toString();
}

QString SystemConfig::getXmlVersion()
{
    return m_pSettings->value("appSystem/XmlVersion", DEX_VERSION).toString();
}

QString SystemConfig::getProtocolVersion()
{
    return m_pSettings->value("appSystem/ProtocolVersion", PROTOCOL_VERSION).toString();
}

int SystemConfig::getEncryptType()
{
    return m_pSettings->value("appSystem/EncryptType", 0).toInt();
}

void SystemConfig::setEncryptType(const int &type)
{
    return m_pSettings->setValue("appSystem/EncryptType", type);
}

int SystemConfig::getAuthType()
{
    return m_pSettings->value("appSystem/AuthType", 0).toInt();
}

void SystemConfig::setAuthType(const int &type)
{
    return m_pSettings->setValue("appSystem/AuthType", type);
}

QString SystemConfig::getAppKey()
{
    return m_pSettings->value("appSystem/AppKey", APP_KEY).toString();
}

void SystemConfig::setAppKey(const QString &appKey)
{
    return m_pSettings->setValue("appSystem/AppKey", appKey);
}

QString SystemConfig::getAppSecretKey()
{
    return m_pSettings->value("appSystem/AppSecretKey", APP_SECRET_KEY).toString();
}

void SystemConfig::setAppSecretKey(const QString &appSecretKey)
{
    return m_pSettings->setValue("appSystem/AppSecretKey", appSecretKey);
}

QString SystemConfig::getAppSecretIV()
{
    return m_pSettings->value("appSystem/AppSecretIV", APP_SECRET_IV).toString();
}

void SystemConfig::setAppSecretIV(const QString &appSecretIV)
{
    return m_pSettings->setValue("appSystem/AppSecretIV", appSecretIV);
}

QString SystemConfig::getAppPublicKey()
{
    return m_pSettings->value("appSystem/AppPublicKey", APP_PUBLIC_KEY).toString();
}

void SystemConfig::setAppPublicKey(const QString &appPublicKey)
{
    return m_pSettings->setValue("appSystem/AppPublicKey", appPublicKey);
}

QString SystemConfig::getPrivateKey()
{
    return m_pSettings->value("appSystem/AppPrivateKey", APP_PRIVATE_KEY).toString();
}

void SystemConfig::setAppPriaveKey(const QString &appPrivateKey)
{
    return m_pSettings->setValue("appSystem/AppPrivateKey", appPrivateKey);
}

QString SystemConfig::getSystemUser()
{
    return m_pSettings->value("appSystem/systemUser", SYSTEM_USER_ACCOUNT).toString();
}

void SystemConfig::setSystemUser(const QString &user)
{
    return m_pSettings->setValue("appSystem/systemUser", user);
}

QString SystemConfig::getSystemPassword()
{
    return m_pSettings->value("appSystem/systemPassword", SYSTEM_USER_PASSWORD).toString();
}

void SystemConfig::setSystemPassword(const QString &password)
{
    return m_pSettings->setValue("appSystem/systemPassword", password);
}

QString SystemConfig::getAppId()
{
    return m_pSettings->value("appSystem/appId", APP_ID).toString();
}

void SystemConfig::setAppId(const QString &appId)
{
    return m_pSettings->setValue("appSystem/appId", appId);
}

void SystemConfig::displayState()
{
    int state = m_nSystemPowerMng->displayState();
    qDebug() << "m_nSystemPowerMng.displayState()   " << state;
    emit displayStateChanged(state);
}

void SystemConfig::soundPlayState(QMediaPlayer::State state)
{
    qDebug()<<"SystemConfig::soundPlayState  " << state;
    if(state == QMediaPlayer::StoppedState){
        qDebug()<<"m_nPowerMng 555555555555556666 releasewakeclock";
        m_nPowerMng->releaseWakelock();
    }
}

bool SystemConfig::getShowGPSTestItemValue()
{
    return m_pSettings->value("appSystem/showGPSTestItem", "false").toBool();
}

void SystemConfig::setShowGPSTestItemValue(const bool &isShow)
{
    m_pSettings->setValue("appSystem/showGPSTestItem", isShow);
}

QString SystemConfig::getAppInstallPath(){
    return m_nAppInstallPath;
}

QString SystemConfig::getAppDataPath()
{
    return m_nAppDataPath;
}

void SystemConfig::setServerIP(const QString &ipStr){
    m_pSettings->setValue("network/ip", ipStr);
}

QString SystemConfig::getServerIP(){
    return m_pSettings->value("network/ip", "0.0.0.0").toString();
}

void SystemConfig::setServerPort(const QString &portStr){
    m_pSettings->setValue("network/port", portStr);
}

QString SystemConfig::getServerPort(){
    return m_pSettings->value("network/port", "0").toString();
}

void SystemConfig::setLogoIconPath(const QString &logoIconPath){
    m_pSettings->setValue("appSystem/logoIconPath", logoIconPath);
}

QString SystemConfig::getLogoIconPath(){
    return m_pSettings->value("appSystem/logoIconPath", "").toString();
}

void SystemConfig::setValue(const QString &key, const QVariant &value){
    m_pSettings->setValue(key, value);
}

QString SystemConfig::getVersionInfo()
{
    return m_pSettings->value("system/version", "").toString();
}

void SystemConfig::setVersionInfo(const QString &versionNum)
{
    m_pSettings->setValue("system/version", versionNum);
}

QString SystemConfig::getRFIDScanType()
{
    return m_pSettings->value("system/rfidType", "").toString();//HF | UHF
}

void SystemConfig::setRFIDScanType(const QString &nType)
{
    qDebug() << "SystemConfig::setRFIDScanType  " << nType;
    m_pSettings->setValue("system/rfidType", nType);
}

QVariant SystemConfig::getValue(const QString &key){
    return m_pSettings->value(key, QVariant());
}

QVariant SystemConfig::getValue(const QString &key, QVariant defaultValue)
{
    return m_pSettings->value(key, defaultValue);
}

void SystemConfig::setUsersFilePath(const QString &fullPath)
{
    m_pSettings->setValue("appSystem/usersFilePath", fullPath);
}

QString SystemConfig::getUserFilePath()
{
    return m_pSettings->value("appSystem/usersFilePath", "").toString();
}

void SystemConfig::setCurrentTheme(const QString &value)
{
    m_pSettings->setValue("system/theme",value);
    m_pSettings->sync();
}

QString SystemConfig::getCurrentTheme() const
{
    return m_pSettings->value("system/theme", "").toString();
}

bool SystemConfig::checkSysTime(QString time){
    QDateTime currenttime = QDateTime::currentDateTime();
    QDateTime localtime;
    localtime = QDateTime::fromString(time,"yyyy/MM/dd hh:mm:ss.zzz");
    QString strTime = localtime.toString("yyyy-MM-dd hh:mm:ss");
    if(currenttime < localtime){
        qDebug()<<"---------------------modify time";
        QString cmd = "date -s \"" +strTime + "\"";
        QProcess process;
        int result = process.execute(cmd);
        qDebug()<<"----------------result is "<<result<<cmd;
        if(!result)
            return true;
        else
            return false;
    }
    return false;
}

QString SystemConfig::getOsVersionFilePath(){
    return m_pSettings->value("system/osVersionFilePath", "/usr/etc/syberos-release").toString();
}

void SystemConfig::setOsVersionFilePath(const QString &filePath){
    m_pSettings->setValue("system/osVersionFilePath", filePath);
}

void SystemConfig::setClientPriority(const bool &priority){
    m_pSettings->setValue("system/clientPriority", priority);
}

bool SystemConfig::getClientPriority(){
    return m_pSettings->value("system/clientPriority", "false").toBool();
}

void SystemConfig::releaseWakeClockWaitTime(){
    qDebug() << "SystemConfig::soundPlayTimer    m_nPowerMng.releaseWakelock()  3333333333366 play";
    m_nPowerMng->releaseWakelock();
    m_nPlayTimerRunning = false;
}

void SystemConfig::soundPlayTimer(){
    qDebug() << "SystemConfig::soundPlayTimer   play sound 3333333333366 play";
    QSound::play(m_nAppInstallPath + "/res/sound.wav");
//    device_set_leds_color(0x00ff0000);
//    device_set_leds_blink(1);
}

void SystemConfig::setNewMsgTip(bool on)
{
    if(on && m_nPlayTimerRunning == false){
        m_nPlayTimerRunning = true;
        qDebug() << "SystemConfig::setNewMsgTip    m_nPowerMng.accquireWakelock()  88888888888888866 play";
        m_nPowerMng->accquireWakelock();
        QTimer::singleShot(3000, this, SLOT(soundPlayTimer()));
        QTimer::singleShot(10000, this, SLOT(releaseWakeClockWaitTime()));
    } /*else if(on == false){
        device_set_leds_color(0x00000000);
        device_set_leds_blink(0);
    }*/
//    if(on && m_pMediaPlayer->state() == QMediaPlayer::StoppedState ){
//        m_nPowerMng.accquireWakelock();
//        qDebug() << "m_nPowerMng.accquireWakelock()  3333333333366 play";
//        m_pMediaPlayer->play();
//        //device_set_leds_color(0x00ff0000);
//        //device_set_leds_blink(1);
//    } else if(on == false){
//        //device_set_leds_color(0x00000000);
//       // device_set_leds_blink(0);
//    }
}
