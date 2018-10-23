/*!
* \file inspectionsyssetting.h
* \brief 系统设置模块
*
*系统设置模块
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/8
*/
#ifndef INSPECTIONSYSSETTING_H
#define INSPECTIONSYSSETTING_H

#include <QObject>
#include "../../../framework/common/utils.h"
#include <QtDBus/QtDBus>
#include <csystemlocatemodemanager.h>
#include <csystempowermanager.h>
#include <cnetworkmanager.h>
#include <cdisplaysettings.h>
#include <csystemdevicemanager.h>

/*!
  \class    InspectionSysSetting
  \inModule 系统设置模块
  \brief    系统设置模块的对外接口和管理对象
  \ingroup  common
  InspectionSysSetting类为系统设置相关的设置项提供控制接口
*/
class InspectionSysSetting : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString versionNum READ getVersionNum WRITE setVersionNum NOTIFY versionNumChanged)
    Q_PROPERTY(QString serverAddr READ getServerAddr WRITE setServerAddr NOTIFY serverAddrChanged)
    Q_PROPERTY(QString serverPort READ getServerPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QString rfidScanType READ getRfidScanType WRITE setRfidScanType NOTIFY rfidScanTypeChanged)
    Q_PROPERTY(int authType READ getAuthType WRITE setAuthType NOTIFY authTypeChanged)
    Q_PROPERTY(int encryptType READ getEncryptType WRITE setEncryptType NOTIFY encryptTypeChanged)

public:

    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    explicit InspectionSysSetting(QObject *parent = 0);
    ~InspectionSysSetting();

    /**
     * @brief   getRfidScanType : 获取rfidtype的值
     * @return  QString, rfidtype的当前值
     */
    Q_INVOKABLE QString getRfidScanType() const;

    /**
     * @brief   setRfidScanType : 设置 rfidtype 的值
     * @param   QString : rType 用户设置的rfidType
     */
    Q_INVOKABLE void setRfidScanType(QString rType);

    /**
     * @brief   getVersionNum : 获取 version number
     * @return  QString, 当前程序的版本号
     */
    QString getVersionNum() const;

    /**
     * @brief   setVersionNum : 设置程序的版本号
     * @param   QString : vNum 更新程序后的版本号
     */
    void setVersionNum(QString& vNum);

    /**
     * @brief   getServerAddr : 获取属性 serverAddr 的值
     * @return  QString, 属性serverAddr 的当前值
     */
    QString getServerAddr() const;

    /**
     * @brief   setServerAddr : 设置属性 serverAddr 的值
     * @param   QString : serverAddr 用户设置的服务器地址
     */
    void setServerAddr(QString& serverAddr);

    /**
     * @brief   getServerPort : 获取服务器端口
     * @return  QString： 服务器端口
     */
    QString getServerPort() const;

    /**
     * @brief   setServerPort : 设置服务器端口
     * @param   QString : 服务器端口
     */
    void setServerPort(const QString& serverPort);

    /**
     * @brief setAuthType 设置协议头类型
     * @param type 协议头类型
     */
    void setAuthType(const int &type);

    /**
     * @brief setEncryptType 设置数据加密类型
     * @param type 数据加密类型
     */
    void setEncryptType(const int &type);

    /**
     * @brief   getOfflineMode : 获取工作模式
     * @return  QString, 当前的离线工作模式
     */
    Q_INVOKABLE QString getOfflineMode();

    /**
     * @brief setOfflineMode : 设置离线模式
     * @param offlineMode : ０：纯在线；１：离线；２：纯离线
     */
    Q_INVOKABLE void setOfflineMode(const QString &offlineMode);

    /**
     * @brief   getLocateMode : 获取OS当前的定位模式
     * @return  QString, OS当前的定位模式
     */
    Q_INVOKABLE QString getLocateMode();

    /**
     * @brief   setLocateMode : 设置OS的定位模式
     * @param   QString : modeStr 用户设置的定位模式:GPS 开/关
     * @return  bool, false 错误, true 成功
     */
    Q_INVOKABLE bool setLocateMode(QString modeStr);

    /**
     * @brief   getMaximumBrightness : 获取OS可设置的最大亮度值
     * @return  int, OS可设置的最大亮度值
     */
    Q_INVOKABLE int getMaximumBrightness();

    /**
     * @brief   getBrightness : 获取OS当前的亮度值
     * @return  int, OS当前的亮度值
     */
    Q_INVOKABLE int getBrightness();

    /**
     * @brief   setBrightness : 设置OS的亮度值
     * @param   int : value 用户设置的亮度值
     */
    Q_INVOKABLE void setBrightness(int nValue);

    /**
     * @brief   isAirplaneMode : 获取飞行模式开关状态
     * @return  bool, true 开启;false 关闭
     */
   Q_INVOKABLE bool isAirplaneMode();

    /**
     * @brief   hasSIM : 查询有无SIM卡
     * @return  bool, true 有卡;false 无卡
     */
    Q_INVOKABLE bool hasSIM();

    /**
     * @brief   isCellularConnected : 判断OS的移动网络是否处于连接状态
     * @return  bool, 移动网络的连接状态: 连接/未连接
     */
    Q_INVOKABLE bool isCellularConnected(void);

    /**
     * @brief   setCellularEnabled : 设置OS的移动网络的连接状态
     * @param   bool : 用户设置的移动网络连接状态: 开/关
     * @return  bool, 执行返回值:  成功/失败
     */
    Q_INVOKABLE bool setCellularEnabled(const bool flag);

    /**
     * @brief   reBootDevice : 控制系统重启
     */
    Q_INVOKABLE void reBootDevice();

    /**
     * @brief   shutDownDevice : 控制系统关闭
     */
    Q_INVOKABLE void shutDownDevice();

    /**
     * @brief getPassword : 解密密码
     * @param passwd : DataPackage 键值
     * @param key : QString 键值
     * @param iv : 管理中心下发的密钥
     * @return  QString, 解密密码
     */
    Q_INVOKABLE QString getPassword( const QString &strpasswd, const QString &strkey, const QString &striv ) const;

    /**
     * @brief getEncryptType 获取数据加密类型
     * @return int 数据加密类型
     */
    Q_INVOKABLE int getEncryptType();

    /**
     * @brief getAuthType 获取协议头类型
     * @return int 协议头类型
     */
    Q_INVOKABLE int getAuthType();

    Q_INVOKABLE void setWakeLock(const bool& value);

    Q_INVOKABLE QStringList setPassword(QString passwd);
    /**
     * @brief   getGPSTestItemShowStatus : 获取GPS测试Item显示状态
     * @return  bool, GPS测试Item显示/不显示
     */
    Q_INVOKABLE bool getGPSTestItemShowStatus();

    /**
     * @brief   setAccount : 设置属性 account 的值
     * @param   QString : account 申请账号
     * @param   QString : code 许可证号
     */
    Q_INVOKABLE void setKeyPara(QString account, QString code);

signals:

    /**
     * @brief sglLocateModeChanged : 定位模式变更信号
     * @param modeStr : 定位模式
     */
    void sglLocateModeChanged(QString modeStr);

    /**
     * @brief sglcellularEnabledChanged : 移动网络状态变更信号
     * @param flag : 移动网络开关状态
     */
    void sglcellularEnabledChanged(bool flag);

    /**
     * @brief sglBrightnessChanged : OS亮度值变更信号
     * @param nCurBrightnessValue : OS当前亮度值
     */
    void sglBrightnessChanged(int nCurBrightnessValue);

    /**
     * @brief sglSim_OfflinePropertyChanged : 有/无simCard (offline:open/close) 变更信号
     * @param bFlag : true/false (有卡/无卡)/(open/close)
     */
    void sglSim_OfflinePropertyChanged(bool bOfflineFlag, bool bSimFlag);

    /**
     * @brief serverAddrChanged: IP地址变化信号
     * @param serverAddress：IP地址
     */
    void serverAddrChanged(const QString &serverAddress);

    /**
     * @brief serverPortChanged: 端口变化信号
     * @param serverPort：端口
     */
    void serverPortChanged(const QString &serverPort);

    /**
     * @brief versionNumChanged: 版本号变化信号
     * @param versionNumber： 版本号
     */
    void versionNumChanged(const QString &versionNumber);

    /**
     * @brief rfidScanTypeChanged:RFID扫描类型变化信号
     * @param rfidScanType：RFID扫描类型
     */
    void rfidScanTypeChanged(const QString &rfidScanType);

    /**
     * @brief authTypeChanged:协议头类型变化信号
     * @param type:协议头类型
     */
    void authTypeChanged(const int &type);

    /**
     * @brief encryptTypeChanged:数据加密类型变化信号
     * @param type:数据加密类型
     */
    void encryptTypeChanged(const int &type);

public slots:

    /**
     * @brief   slotLocateModeChanged : OS底层定位模式变更触发的槽函数
     * @param   mode : CSystemLocateModeManager::LocateMode  OS底层当前的定位模式
     */
    void slotLocateModeChanged(CSystemLocateModeManager::LocateMode mode);

    /**
     * @brief   slotcellularEnabledChanged : OS底层网络连接模式变更触发的槽函数
     * @param   enabled : bool  网络连接是否可用
     */
    void slotcellularEnabledChanged(bool enabled);

    /**
     * @brief   slotBrightnessChanged : OS底层亮度值变更触发的槽函数
     */
    void slotBrightnessChanged();

    /**
     * @brief   getConnmamPropertyChanged : 飞行模式属性值变更触发的槽函数
     * @param   name : wait for: offline
     * @param   value : true/false (offline/online)
     */
    void getConnmamPropertyChanged(const QString &name, const QDBusVariant &value);

    /**
     * @brief   getSimmamPropertyChanged : SIM属性值变更触发的槽函数
     * @param   name : wait for: Simpresent
     * @param   value : 1/0 (有卡/无卡)
     */
    void getSimmamPropertyChanged(const QString &name, const QDBusVariant &value);


private:
    bool m_nIsShowGPSItem;///< 负责存放系统设置页面GPS-test项的显示状态
    QString m_nCurRFIDScanType;///<  负责存放系统当前的RFID扫描类型
    QString m_nCurrentVersion;///<  负责存放系统当前的版本号
    QString m_nCurrentServerAddr;///<  负责存放系统当前所设置的服务器地址
    QString m_nCurrentServerPort;///<  负责存放系统当前所设置的服务器端口
    int m_nAuthType;///<  负责存放系统当前所设置的协议头类型
    int m_nEncryptType;///<  负责存放系统当前所设置的数据加密类型
    QString m_nOfflineMode; ///<  负责存放系统当前是否启用离线模式

    CSystemLocateModeManager m_nLocateManager;///<  负责打开/关闭GPS的对象
    CSystemPowerManager m_nPowerManager;///<  负责系统重启、关机的对象
    CSystemDisplaySettings m_nDisplayManager;///<  负责亮度控制的对象
    CNetworkManager m_nNetWorkManager;///<  负责移动网络管理的对象
    CSystemDeviceManager m_nSysDevManager;///<  负责查询系统平台相关信息及状态的对象
};

#endif // INSPECTIONSYSSETTING_H
