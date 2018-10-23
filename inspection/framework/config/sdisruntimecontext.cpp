#include "sdisruntimecontext.h"
#include "../common/systemconfig.h"
#include <csystemdevicemanager.h>
#include <QDebug>
SDISRuntimeContext* SDISRuntimeContext::pInstance = NULL;

SDISRuntimeContext::SDISRuntimeContext()
{
    protocolFormat = DEXML;
    protocolCharset = UTF8;
    encryptionType = SystemConfig::getInstance()->getEncryptType();
    authenticationType = SystemConfig::getInstance()->getAuthType();
    mAddress = SystemConfig::getInstance()->getServerIP();
    iPort = SystemConfig::getInstance()->getServerPort().toInt();
    appKey = SystemConfig::getInstance()->getAppKey();
    appSecretKey = SystemConfig::getInstance()->getAppSecretKey();
    appSecretIV = SystemConfig::getInstance()->getAppSecretIV();
    appPrivateKey = SystemConfig::getInstance()->getPrivateKey();
    appPublicKey = SystemConfig::getInstance()->getAppPublicKey();
    systemUser = SystemConfig::getInstance()->getSystemUser();
    systemPassword = SystemConfig::getInstance()->getSystemPassword();
    appId = SystemConfig::getInstance()->getAppId();
    memset((void*)&userToken, 0, sizeof(AccessToken));
    memset((void*)&systemToken, 0, sizeof(AccessToken));
}

SDISRuntimeContext* SDISRuntimeContext::getInstance()
{
    if(pInstance == NULL) {
        pInstance = new SDISRuntimeContext();
    }
    return pInstance;
}

void SDISRuntimeContext::setServerAddress(QString address)
{
    mAddress = address;

}

void SDISRuntimeContext::setServerPort(int port)
{
    iPort = port;
}

void SDISRuntimeContext::setProtocolFormat(DEXDataFormat value)
{
    protocolFormat = value;
}

void SDISRuntimeContext::setProtocolCharset(DEXCharset value)
{
    protocolCharset = value;
}

QString SDISRuntimeContext::getServerAddress()
{
    return mAddress;
}

int SDISRuntimeContext::getServerPort()
{
    return iPort;
}

void SDISRuntimeContext::setDeviceID(const QString &value)
{
    mDeviceID = value;
}



QString SDISRuntimeContext::getDeviceID()
{
    if(mDeviceID.isEmpty()){
        CSystemDeviceManager sysDeviceMng;
        mDeviceID = sysDeviceMng.imei(0);
        if(mDeviceID.isEmpty())
            mDeviceID = "1111111111";
    }
    return mDeviceID;
}

DEXDataFormat SDISRuntimeContext::getProtocolFormat()
{
    return protocolFormat;
}

DEXCharset SDISRuntimeContext::getProtocolCharset()
{
    return protocolCharset;
}

int SDISRuntimeContext::getDataEncryptionType()
{
    return encryptionType;
}

int SDISRuntimeContext::getAuthenticationType()
{
    return authenticationType;
}

AccessToken SDISRuntimeContext::getCurrentToken()
{
    if(userToken.accessKey != 0){
        return userToken;
    }else{
        return systemToken;
    }
}

QString SDISRuntimeContext::getAppKey()
{
    return appKey;
}

QString SDISRuntimeContext::getAppSecretKey()
{
    return appSecretKey;
}

QString SDISRuntimeContext::getAppSecretIV()
{
    return appSecretIV;
}

QString SDISRuntimeContext::getAppPublicKey()
{
    return appPublicKey;
}

QString SDISRuntimeContext::getAppPrivateKey()
{
    return appPrivateKey;
}

QString SDISRuntimeContext::getSystemUser()
{
    return systemUser;
}

QString SDISRuntimeContext::getSystemPassword()
{
    return systemPassword;
}

QString SDISRuntimeContext::getAppId()
{
    return appId;
}

void SDISRuntimeContext::setDataEncryptionType(int type)
{
    encryptionType = type;
}

void SDISRuntimeContext::setSystemToken(const AccessToken &token)
{
    systemToken = token;
}

void SDISRuntimeContext::setUserToken(const AccessToken &token)
{
    userToken = token;
}

void SDISRuntimeContext::setAuthenticationType(const int &type)
{
    authenticationType = type;
}

void SDISRuntimeContext::setAppKey(const QString &appKey)
{
    this->appKey = appKey;
}

void SDISRuntimeContext::setAppSecretKey(const QString &appSecretKey)
{
    this->appSecretKey = appSecretKey;
}

void SDISRuntimeContext::setAppSecretIV(const QString &appSecretIV)
{
    this->appSecretIV = appSecretIV;
}

void SDISRuntimeContext::setAppPublicKey(const QString &appPublicKey)
{
    this->appPrivateKey = appPublicKey;
}

void SDISRuntimeContext::setAppPrivateKey(const QString &appPrivateKey)
{
    this->appPrivateKey = appPrivateKey;
}

void SDISRuntimeContext::setSystemPassword(const QString &password)
{
    this->systemPassword = password;
}

void SDISRuntimeContext::setAppId(const QString &appId)
{
    this->appId = appId;
}

void SDISRuntimeContext::setSystemUser(const QString &user)
{
    this->systemUser = user;
}
