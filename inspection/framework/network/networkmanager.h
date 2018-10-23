#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include "../common/utils.h"
#include "../dataManager/datahandler.h"
#include "./binary/binarymanager.h"
#include "./command/commandmanager.h"

/*!
  \class NetworkManager
  \inModule 网络模块
  \brief 网络模块的对外从接口和管理对象
  \ingroup network
  NetworkManager类为上层提供了发送以及接受数据包的接口
*/
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    virtual ~NetworkManager();
    /**
     * @brief sendCommandRequest:发送数据包至服务器
     * @param package    : DataPackage 数据包结构体引用
     * @param action  : QString 上层自定义的动作名，数据包返回后会一并带回
     */
    bool sendCommandRequest(const DataPackage package, const QString action);
    /**
     * @brief sendBinaryRequest:发送二进制数据请求包至服务器
     * @param url    : DataPackage 数据包结构体引用
     * @param action  : QString 上层自定义的动作名，数据包返回后会一并带回
     * @param target  : QString 上层自定义的动作名，数据包返回后会一并带回
     * @return  null
     */
    void sendBinaryRequest(const QString remotePath, const QString localPath, const QString action, const QString name,
                           const QString title, const QString description, const QString seriesKey, bool download);
    /**
     * 初始化网络令牌（登陆的时候需要调用）
     * @param userName 用户名
     * @param password 密码
     */
    void initAccessToken(const QString &userName, const QString &password);

    /**
     * 初始化系统用户网络令牌
     */
    void initSystemAccessToken();

    /**
     * 重置安全参数（Key，向量等……）
     *
     * @param code 授权码
     * @param account 账号
     */
    void resetSecurityParameter(QString code, QString account);

    /**
     * 生成签名字符串
     * @param time UTC秒数
     * @param nonce 随机值
     * @return 签名字符串
     */
    QString generateSignature(const long long &time, const int &nonce, const QString& appKey);

    /**
     * 更新本地Token等信息
     * @param data 数据
     * @param system true系统用户 false业务用户
     */
    void updateCurrentToken(const DataPackage &data, const bool &system);

    /**
     * 更新系统安全配置参数
     * @param data 服务器返回的数据
     */
    void updateSystemParameter(const DataPackage &data);

    /**
     * @brief setTarget:设置服务器配置
     * @param address  : QString  服务器地址
     * @param port  : int      服务器端口
     */
    void setTarget(const QString& address, const int& port);
    /**
     * @brief getInstance:获取网络管理对象
     * @param parent : QObject* 改对象的父对象
     * @return  NetworkManager*, 非空 获取成功, NULL 获取失败
     */
    static NetworkManager* getInstance(QObject* parent);
    /**
      * @brief setDeviceID: 设置设备ID
      * @param value    : const Qstring& 设备ID
      */
    void setDeviceID(const QString& value);

private:
    explicit NetworkManager(QObject *parent = 0);
    /**
     * @brief initialize:类初始化函数
     */
    void initialize();

signals:
    /**
     * @brief cmdError:错误信号，当错误发生的时候会触发
     * @param objects    : QString  发生错误包头中的Objects
     * @param action  : QString    上层自定义的动作名，在SendPackage中指定
     * @param error  : int      错误编号
     * @param serverCode : int  服务器错误编号
     * @param description ： QString 服务器错误信息
     */
    void cmdError(const QString objects, const QString action, const int error, const int serverCode, const QString description);
    /**
     * @brief taskResponse:请求的服务器数据包返回，对应Type为3
     * @param objects    : QString  发生错误包头中的Objects
     * @param type    : QString  请求数据包头中的类型，详情见接口文档
     * @param action  : QString  上层自定义的动作名，在SendPackage中指定
     * @param package  : DataPackage      返回的数据包
     */
    void cmdResponse(const QString objects, const QString type, const QString action, const DataPackage package);
    /**
     * @brief cmdResult:服务器对于客户端指令的回应，对应Type为2或者4
     * @param objects    : QString  发生错误包头中的Objects
     * @param type    : QString  请求数据包头中的类型，详情见接口文档
     * @param action  : QString  上层自定义的动作名，在SendPackage中指定
     * @param result  : QString  服务器返回的结果
     */
    void cmdResult(const QString objects, const QString type, const QString action, const QString result);
    /**
     * @brief binaryError:错误信号，当错误发生的时候会触发
     * @param action  : QString    上层自定义的动作名，在SendPackage中指定
     * @param error  : int      错误编号
     * @param description ： QString 服务器错误信息
     */
    void binaryError(const QString action, const int error, const QString description);
    /**
     * @brief binaryResponse : 二进制请求的服务器数据包返回，对应Type为3
     * @param action : 上层自定义的动作名，在SendPackage中指定
     * @param target : 目标文件路径（上层传入）
     */
    void binaryResponse(const QString action, const QString target);

    /**
     * @brief binaryProgress : 二进制包的传输的进度信息
     * @param action : 上层自定义的动作名，在SendPackage中指定
     * @param length :
     * @param progress :
     * @param status :
     */
    void binaryProgress(const QString action, const int length,
                        const int progress, const int status);

    /**
     * Token更新完成时间
     * @param success  true 成功 false 失败
     * @param system   true 系统用户 false 业务用户
     * @param code     code编码
     * @param description 描述
     */
    void accessTokenUpdated(bool success, bool system, int code, QString description, int errorNo);
    
    /**
     * 系统安全参数更新完成
     * @param success true 成功 false 失败
     * @param appKey  appKey
     * @param appSecretKey appSecretKey
     * @param appSecretIV appSecretIV
     * @param appPublicKey appPublicKey
     * @param appPrivateKey appPrivateKey
     * @param errorNo 错误编码
     */
    void systemParaUpdated(bool success, QString appKey, QString appSecretKey, QString appSecretIV, QString appPublicKey, QString appPrivateKey, int errorNo);

protected slots:
    /**
     * @brief actionResponse:服务器返回的请求处理槽函数
     * @param objects    : QString  发生错误包头中的Objects
     * @param type    : QString  请求数据包头中的类型，详情见接口文档
     * @param action  : QString  上层自定义的动作名，在SendPackage中指定
     * @param taskData  : QString  服务器返回数据流
     */
    void cmdResponseInner(const QString objects, const QString type, const QString action, const QByteArray data, const ProtocolHead protocolHead);

    /**
     * 系统安全参数更新完成
     * @param success true 成功 false 失败
     * @param appKey  appKey
     * @param appSecretKey appSecretKey
     * @param appSecretIV appSecretIV
     * @param appPublicKey appPublicKey
     * @param appPrivateKey appPrivateKey
     * @param errorNo 错误编码
     */
    void updateSystemPara(bool success, QString appKey, QString appSecretKey, QString appSecretIV, QString appPublicKey, QString appPrivateKey, int errorNo);
    
private:
    CommandManager mCommandManager;
    BinaryManager mBinaryManager; // 负责2进制数据的收发
    DataHandler mDataHandler;  // 负责数据流转义等处理的对象
    static NetworkManager* pInstance; //对应单例模型的对象
    QString securityCode;

    /**
     * 初始化网络令牌（内部函数）
     * @param userName 用户名
     * @param password 密码
     * @param action action
     */
    void initUserAccessTokenInner(const QString &userName,
                                  const QString &password,
                                  const QString &authUserType,
                                  const QString &action);

    /**
     * @brief sendCommandRequestInner:发送数据包至服务器
     * @param package : DataPackage 数据包结构体引用
     * @param action  : QString 上层自定义的动作名，数据包返回后会一并带回
     */
    bool sendCommandRequestInner(const DataPackage package, const QString action);
};

#endif // NETWORKMANAGER_H
