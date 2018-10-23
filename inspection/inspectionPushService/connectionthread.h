#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H
#include <QThread>
#include "../framework/common/utils.h"
#include "../framework/network/tools/inetworktool.h"

/**
 * 用来异步连接网络的类
 */
class ConnectionThread : public QThread
{
    Q_OBJECT
public:

    /**
     * 构造函数
     * @param networkTool 网络对象
     */
     ConnectionThread(INetworkTool *networkTool, const QString &address, const int &port);

     /**
      * 析构函数
      */
     ~ConnectionThread();

     /**
      * run函数
      */
     void run();

signals:
    void serverConnected(const bool &result);

private:

    /**
     * 网络传输工具
     */
    INetworkTool *networkTool;

    /**
     * 服务器地址
     */
    QString address;

    /**
     * 服务器端口
     */
    int port;
    /**
     * @brief generateHead:生成head
     * @param headBuf
     * @param length
     */
    void generateHead(QByteArray &headBuf, const int &length);

    /**
     * @brief generatePackage:生成获取Token数据包
     * @param dataPackage:数据包
     */
    void generatePackage(DataPackage &dataPackage);

    /**
     * @brief updateToken:更新Token
     * @param dataPackage:Token数据
     * @return
     */
    bool updateToken(const DataPackage &dataPackage);

    /**
     * @brief initAccessToken:初始化Token
     */
    bool initAccessToken();
};

#endif // CONNECTIONTHREAD_H
