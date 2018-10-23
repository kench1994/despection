/*!
* \file systemconfig.h
* \brief 配置文件信息读取和设置模块
*
*配置文件信息读取和设置模块
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/8
*/
#ifndef SYSTEMCONFIG_H
#define SYSTEMCONFIG_H
#include <QObject>
#include <QVariant>
#include <csystempowermanager.h>
#include <cpowermanager.h>
#include <QMediaPlayer>
#include <QSettings>
/*!
  \class SystemConfig
  \inModule 配置文件信息读取和设置模块
  \brief 读取和设置配置文件信息
  \ingroup common
  SystemConfig类提供读取和设置配置文件信息的接口
*/

class SystemConfig:public QObject
{
    Q_OBJECT
public:

    /**
     * @brief  getShowGPSTestItemValue : 获取GPS测试Item显示状态
     * @return bool, GPS测试Item显示状态: true 显示; false 不显示
     */
    Q_INVOKABLE bool getShowGPSTestItemValue();

    /**
     * @brief  getShowGPSTestItemValue : 设置GPS测试Item显示状态
     * @param isShow: bool, GPS测试Item显示状态
     */
    Q_INVOKABLE void setShowGPSTestItemValue(const bool &isShow);

    /**
     * @brief  getAppInstallPath : 获取软件安装路径
     * @return QString, 软件安装路径
     */
    Q_INVOKABLE QString getAppInstallPath();

    /**
     * @brief  getAppDataPath : 获取软件数据路径
     * @return QString, 软件数据路径
     */
    Q_INVOKABLE QString getAppDataPath();

    /**
     * @brief  setServerIP : 设置服务器ip地址
     * @param  ipStr : QString &: 服务器ip地址
     */
    Q_INVOKABLE void setServerIP(const QString &ipStr);

    /**
     * @brief  getServerIP : 获取服务器ip地址
     * @return QString, 服务器ip地址
     */
    Q_INVOKABLE QString getServerIP();

    /**
     * @brief  setServerPort : 设置服务器端口号
     * @param  portStr : QString &: 服务器端口号
     */
    Q_INVOKABLE void setServerPort(const QString &portStr);

    /**
     * @brief  getServerPort : 获取服务器端口号
     * @return QString, 服务器端口号
     */
    Q_INVOKABLE QString getServerPort();

/**
     * @brief getEncryptType 获取数据加密类型
     * @return int 数据加密类型
     */
    int getEncryptType();
    /**
     * @brief setEncryptType 设置数据加密类型
     * @param type 数据加密类型
     */
    void setEncryptType(const int &type);

    /**
     * @brief getAuthType 获取协议头类型
     * @return int 协议头类型
     */
    int getAuthType();


    /**
     * @brief setAuthType 设置协议头类型
     * @param type 协议头类型
     */
    void setAuthType(const int &type);


    /**
     * @brief getAuthType 获取App key
     * @return QString App key
     */
    QString getAppKey();

    /**
     * @brief setAppKey 设置App key
     * @param appKey App key
     */
    void setAppKey(const QString &appKey);

    /**
     * @brief getAppSecretKey 获取加密KEY
     * @return QString 加密KEY
     */
    QString getAppSecretKey();

    /**
     * @brief setAppSecretKey 设置加密KEY
     * @param appSecretKey KEY
     */
    void setAppSecretKey(const QString &appSecretKey);

    /**
     * @brief getAppSecretIV 获取加密IV
     * @return QString 加密IV
     */
    QString getAppSecretIV();

    /**
     * @brief setAppSecretIV 设置加密IV
     * @param appSecretIV IV
     */
    void setAppSecretIV(const QString &appSecretIV);

    /**
     * @brief getAppPublicKey 获取公钥
     * @return QString 公钥
     */
    QString getAppPublicKey();

    /**
     * @brief setAppPublicKey 设置公钥
     * @param appPublicKey 公钥
     */
    void setAppPublicKey(const QString &appPublicKey);

    /**
     * @brief getPrivateKey 获取私钥
     * @return QString 私钥
     */
    QString getPrivateKey();

    /**
     * @brief setAppPriaveKey 设置私钥
     * @param appPrivateKey 私钥
     */
    void setAppPriaveKey(const QString &appPrivateKey);

    /**
     * @brief getSystemUser 获取系统用户
     * @return 用户
     */
    QString getSystemUser();

    /**
     * @brief setSystemUser 设置系统用户
     * @param user 用户
     */
    void setSystemUser(const QString& user);

    /**
     * @brief getSystemPassword 获取系统用户密码
     * @return 密码
     */
    QString getSystemPassword();

    /**
     * @brief setSystemPassword 设置系统用户密码
     * @param password 密码
     */
    void setSystemPassword(const QString& password);

    /**
     * @brief getAppId 获取 app id
     * @return app id
     */
    Q_INVOKABLE QString getAppId();

    /**
     * @brief setAppId 设置 app id
     * @param appId 密码
     */
    void setAppId(const QString& appId);

    /**
     * @brief  setLogoIconPath : 设置Logo图标路径
     * @param  logoIconPath : QString &: Logo图标路径
     */
    Q_INVOKABLE void setLogoIconPath(const QString &logoIconPath);

    /**
     * @brief  getServerPort : 获取Logo图标路径
     * @return QString, Logo图标路径
     */
    Q_INVOKABLE QString getLogoIconPath();

    /**
     * @brief  setUsersFilePath : 设置记录登陆人员的文件的路径
     * @param  fullPath : QString &: 记录登陆人员的文件的路径
     */
    Q_INVOKABLE void setUsersFilePath(const QString &fullPath);

    /**
     * @brief  getUserFilePath : 获取记录登陆人员的文件的路径
     * @return QString, 记录登陆人员的文件的路径
     */
    Q_INVOKABLE QString getUserFilePath();

    /**
     * @brief  getValue : 获取自由存储值
     * @param  key : QString &: 自由存储值的key
     */
    Q_INVOKABLE QVariant getValue(const QString &key);
    /**
     * @brief getValue : 获取自由存储值
     * @param key : 自由存储值的key
     * @param defaultValue : 默认值
     * @return : 自由存储值
     */
    QVariant getValue(const QString &key, QVariant defaultValue);

    /**
     * @brief  setValue : 存储自由存储值
     * @param  key : QString &: 自由存储值的key
     * @param  value : QString &: 自由存储值的value
     */
    Q_INVOKABLE void setValue(const QString &key, const QVariant &value);

    /**
     * @brief  getVersionInfo : 获取版本信息
     * @return QString, 版本信息
     */
    Q_INVOKABLE QString getVersionInfo();

    /**
     * @brief  setVersionInfo : 设置版本信息
     * @param  versionNum : QString &: 版本信息
     */
    Q_INVOKABLE void setVersionInfo(const QString &versionNum);

    /**
     * @brief  getRFIDScanType : 获取RFID类型
     * @return QString, RFID类型
     */
    Q_INVOKABLE QString getRFIDScanType();

    /**
     * @brief  setRFIDScanType : 设置RFID类型
     * @param  nType : QString &: RFID类型
     */
    Q_INVOKABLE void setRFIDScanType(const QString &nType);

    /**
     * @brief  setCurrentTheme : 设置当前主题
     * @param  value : QString &: 当前主题
     */
    Q_INVOKABLE void setCurrentTheme(const QString &value);

    /**
     * @brief  getCurrentTheme : 获取当前主题
     * @return QString, 当前主题
     */
    Q_INVOKABLE QString getCurrentTheme() const;

    Q_INVOKABLE bool checkSysTime(QString time);

    /**
     * @brief setOsVersionFilePath:设置系统版本文件路径
     * @param filePath:系统版本文件路径
     */
    Q_INVOKABLE void setOsVersionFilePath(const QString &filePath);
    /**
     * @brief getOsVersionFilePath:获取系统版本文件路径
     * @return QString: 系统版本文件路径
     */
    Q_INVOKABLE QString getOsVersionFilePath();

    /**
     * @brief setPriority:设置客户端配置文件的优先级，
     * @param priority:客户端配置文件是否优先
     */
    Q_INVOKABLE void setClientPriority(const bool &priority);
    /**
     * @brief getOsVersionFilePath:获取系统版本文件路径
     * @return QString: 系统版本文件路径
     */
    Q_INVOKABLE bool getClientPriority();

    Q_INVOKABLE void setNewMsgTip(bool on);

    /**
     * @brief   getInstance:获取单例对象
     * @return  SystemConfig*, 非NULL 成功, NULL 失败
     */
    static SystemConfig *getInstance();

    /**
     * @brief   getSequence:获取序列
     * @return  QString, 序列
     */
    Q_INVOKABLE QString getSequence();

    Q_INVOKABLE QString getUUID();

    /**
     * @brief getOsVersion: 获取系统版本号
     * @return QString:  系统版本号
     */
    Q_INVOKABLE QString getOsVersion();

    /**
     * @brief getOfflineModel : 获取离线模式
     * @return  : "0"：纯在线; "1" 离线；"2": 纯离线
     */
    Q_INVOKABLE QString getOfflineModel();
    /**
     * @brief setOfflineModel : 设置离线模式
     */
    Q_INVOKABLE void setOfflineModel(const QString &offlineModel);

    /**
     * @brief setOfflineTaskInitTime : 设置离线模式下任务同步初始化时间
     * @param initTime : 任务同步初始化时间
     */
    void setOfflineTaskInitTime(const QString initTime);

    /**
     * @brief getOfflineTaskInitTime : 获取离线模式下任务同步初始化时间
     * @return  : 任务同步初始化时间
     */
    QString getOfflineTaskInitTime();

    /**
     * @brief getXmlVersion : 获取XML版本
     * @return : XML版本
     */
    QString getXmlVersion();

    /**
     * @brief getProtolcalVersion : 获取传输协议版本
     * @return : 传输协议版本
     */
    QString getProtocolVersion();

public slots:
    /**
     * @brief   displayState : 显示状态变更触发的槽函数
     */
    void displayState();

    /**
     * @brief   soundPlayState : 声音播放槽函数，播放完成后释放wakeclock
     */
    void soundPlayState(QMediaPlayer::State state);
    void releaseWakeClockWaitTime();
    void soundPlayTimer();

signals:

    /**
     * @brief displayStateChanged : 显示状态变更信号
     * @param displayState : 0 on, 1 off  2 dimmed
     */
    void displayStateChanged(int displayState);

private:

    /**
     * @brief 构造函数
     */
    SystemConfig();

private:
    QSettings *m_pSettings;///< 存取系统设置的对象
    static SystemConfig *m_pSysConf;///< 对应单例模型的对象
    QString m_nAppInstallPath; ///< 软件安装路径
    QString m_nAppDataPath;///< 软件数据路径
    QString m_nConnectNetworkType;///< 链接网络类型
    CSystemPowerManager *m_nSystemPowerMng;///< 获取显示状态的对象
    CPowerManager *m_nPowerMng;///< wakeclock
    bool m_nIsMessagePage;///< 是否在消息页面
    QMediaPlayer *m_pMediaPlayer;
    bool  m_nPlayTimerRunning;
};

#endif // SYSTEMCONFIG_H
