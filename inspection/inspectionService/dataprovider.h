#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H
#include "../framework/network/networkmanager.h"
#include "../framework/common/commondbustypes.h"
#include "common/dataoperationtype.h"
#include <QObject>
class DataHandlerManager;
class LocalCacheManager;
/**
 * @brief The DataProvider class : 数据提供类，service收到app的请求后交由该类处理
 */
class DataProvider : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getInstance : 获取该类单例
     * @return : 该类单例
     */
    static DataProvider* getInstance();
    ~DataProvider();

public slots:
     /**
      * @brief messageLoopFunction : 消息循环槽函数，处理各个模块manager之间的交互
      * @param action : 动作序列号
      * @param map : 传递数据
      */
     void messageLoopFunction(const BusinessMessageType businessMsgType, const QString action, const QString objects, QVariantMap map);

     /**
      * @brief networkStatusChanged : 网络链接状态变更槽函数
      * @param status : 网络状态
      */
     void networkStatusChanged(QString status);

signals:
     /**
      * @brief onResultOfResponse : 接受各个功能模块返回的信号，发送给app
      * @param objects : 业务编码
      * @param action : 序列号
      * @param result : true:成功； false:失败
      * @param error : 错误代码;０:无错误
      * @param map : 返回的结果
      */
     void onResultOfResponse(QString objects, QString action, bool result, int error, DbusPackage map);

     /**
      * @brief messageLoopFunction : 消息循环信号，处理各个模块manager之间的交互
      * @param action : 动作序列号
      * @param map : 传递数据
      */
     void messageLoopSignal(const QString action, DbusPackage map);
private:
     DataProvider(QObject *prent = 0);
     static DataProvider* m_pDataProvider; ///< 单例对象
    LocalCacheManager *m_pLocalCacheManager; ///< 缓存管理模块
    DataHandlerManager *m_pDataHandlerManager; ///< 数据处理管理模块
    NetworkManager *m_pNetworkManager; ///< 网络操作模块
};

#endif // DATAPROVIDER_H
