#ifndef NETWORKCONTROL_H
#define NETWORKCONTROL_H
#include "../framework/network/networkmanager.h"
#include "../framework/common/networkstatus.h"
#include <cnetworkmanager.h>
class NetworkControl : public QObject
{
    Q_OBJECT
public:
    static NetworkControl *getInstance();
    ~NetworkControl();
    void sendData(const DataPackage &dataPackage, const QString &action, bool binaryType = false);
    /**
     * @brief getNetworkConnectStatus : 网络链接状态
     * @return : "wifi":wlan； "cellular"：移动网络；"":无网络
     */
    QString getNetworkConnectStatus();
signals:
    /**
     * @brief sendResultSignal : 接受网络模块信号，将发送给各个功能模块
     * @param objects : 业务编码
     * @param action : 序列号
     * @param result : true:成功； false:失败
     * @param error : 错误代码;０:无错误
     * @param map : 返回的结果
     */
    void sendNetworkResultSignal(QString objects, QString action, bool result, int error, DbusPackage map);
    /**
     * @brief networkConnectChanged : 网络链接状态变更信号
     * @param networkType : 网络链接状态
     */
    void networkConnectChanged(QString networkType);
public slots:
    /**
     * @brief errorOccured : 接受网络的错误信号
     * @param objects : 业务编码
     * @param action : 序列号
     * @param error : 错误代码;０:无错误
     * @param serverCode : server端的错误代码
     * @param description : 详情描述
     */
    void errorOccured(const QString& objects, const QString& action, const int& error, const int& serverCode, const QString& description);

    /**
     * @brief response : 连接网络请求返回信号
     * @param objects : 业务编码
     * @param type : 网络动作类型；１：请求数据；２：提交数据；０：下载二进制
     * @param action : 序列号
     * @param package : 请求返回的数据
     */
    void response(const QString& objects, const QString& type, const QString& action,
                        const DataPackage& package);
    /**
     * @brief result : 网络提交数据返回的信号
     * @param objects : 业务编码
     * @param type : 网络动作类型；１：请求数据；２：提交数据；０：下载二进制
     * @param action : 序列号
     * @param result : true:成功； false:失败
     */
    void result(const QString& objects, const QString& type, const QString& action, const QString& result);
    /**
     * @brief binaryProgress : 二进制数据下载或上传进度信号
     * @param action : 序列号
     * @param length : 下载或者上传文件长度
     * @param progress :已经下载或者上传长度
     * @param status : 当前状态
     */
    void binaryProgress(const QString action, const int length,  const int progress, const int status);
    /**
     * @brief binaryResponse : 二进制数据下载或上传完成后的信号
     * @param action : 序列号
     * @param target : 文件路径
     */
    void binaryResponse(const QString action, const QString target);
    /**
     * @brief binaryError : 二进制数据下载或上传发生错误的信号
     * @param action : 序列号
     * @param error : 错误代码;０:无错误
     * @param description : 详情描述
     */
    void binaryError(const QString action, const int error, const QString description);

private:
    NetworkControl();
    static NetworkControl *m_pNetworkControl;
    NetworkManager *m_pNetworkManager; ///< 网络操作模块
    QString m_nConnectNetworkType;///< 链接网络类型
    NetworkStatus *m_pNetworkStatus;
};

#endif // NETWORKCONTROL_H
