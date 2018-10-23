#ifndef SDISRUNTIMECONTEXT_H
#define SDISRUNTIMECONTEXT_H

#include "../common/utils.h"

class SDISRuntimeContext
{
private:
    SDISRuntimeContext();
    static SDISRuntimeContext* pInstance;
    QString mAddress;
    int iPort;
    QString mDeviceID;
    DEXDataFormat protocolFormat;
    DEXCharset protocolCharset;
    QString mUserID;
    int encryptionType; ///<0：无加密；1：普通加密；2：ssl&tls加密；3: ssl2.0加密
    int authenticationType;///< 0:0：普通协议头；1：加密协议头
    AccessToken systemToken;/// 系统用户Token
    AccessToken userToken;///   登录用户Token
    QString appKey;          ///App Key
    QString appSecretKey;    ///加密KEY
    QString appSecretIV;     ///加密IV
    QString appPublicKey;    ///公钥
    QString appPrivateKey;   ///私钥
    QString systemUser;       ///系统用户
    QString systemPassword;   ///系统用户密码
    QString appId;            ///app id
public:
    static SDISRuntimeContext* getInstance();
    void setServerAddress(QString address);
    void setServerPort(int port);
    void setProtocolFormat(DEXDataFormat value);
    void setProtocolCharset(DEXCharset value);
    void setDataEncryptionType(int type);
    void setSystemToken(const AccessToken &token);
    void setUserToken(const AccessToken &token);
    void setAuthenticationType(const int &type);
    void setAppKey(const QString &appKey);
    void setAppSecretKey(const QString &appSecretKey);
    void setAppSecretIV(const QString &appSecretIV);
    void setAppPublicKey(const QString &appPublicKey);
    void setAppPrivateKey(const QString &appPrivateKey);
    void setSystemUser(const QString &user);
    void setSystemPassword(const QString &password);
    void setAppId(const QString &appId);
    QString getServerAddress();
    int getServerPort();
    void setDeviceID(const QString& value);
    QString getDeviceID();
    DEXDataFormat getProtocolFormat();
    DEXCharset getProtocolCharset();
    int getDataEncryptionType();
    int getAuthenticationType();
    AccessToken getCurrentToken(void);
    QString getAppKey();
    QString getAppSecretKey();
    QString getAppSecretIV();
    QString getAppPublicKey();
    QString getAppPrivateKey();
    QString getSystemUser();
    QString getSystemPassword();
    QString getAppId();
};

#endif // SDISRUNTIMECONTEXT_H
