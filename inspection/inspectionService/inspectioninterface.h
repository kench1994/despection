#ifndef INSPECTIONINTERFACE_H
#define INSPECTIONINTERFACE_H
#include <QObject>
#include "../framework/common/commondbustypes.h"
#include "dataprovider.h"
#include "../framework/common/utils.h"
#include "dataHandler/datahandlermanager.h"
#include "localcache/localcachemanager.h"
#include "sync/syncmanager.h"
/**
 * @brief The InspectionInterface class : service的接口类，接受app请求
 */
class InspectionInterface:public QObject
{
    Q_OBJECT
        // 定义Interface名称为"com.syberos.inspectionService.interface"
        Q_CLASSINFO("D-Bus Interface", SERVICE_DBUS_INTERFACE)
public:
    InspectionInterface(QObject *parent = 0);
    ~InspectionInterface();
signals:
    /**
     * @brief responseToClient : 发送给app的信号
     * @param objects :业务编码
     * @param action : 序列号
     * @param result : true : 成功； false：失败
     * @param error : 错误代码
     * @param map : 返回的结果
     */
    void responseToClient(QString objects, QString action, bool result, int error, DbusPackage map);

    /**
     * @brief messageUpdated : 发送给app的信号
     * @param messgeType : 消息类型
     */
    void messageUpdated(QString messgeType);
public slots:
    /**
     * @brief aaaaaaaaa : 接受各个模块的信号，然后发送responseToClient信号
     * @param objects
     * @param action
     * @param result
     * @param error
     * @param map
     */
    void aaaaaaaaa(QString objects, QString action, bool result, int error, DbusPackage map);
    /**
     * @brief receiveData : 接受app请求
     * @param dataPackage : 请求的数据
     * @param action : 序列号
     * @param functionType : 请求类型
     */
    void receiveData(const DataPackage &dataPackage, const QString &action, const QString &functionType);
    /**
     * @brief setStatus : app设置给service的参数, 比如服务器地址，登陆状态等
     * @param functionType : 设置的参数类型
     * @param map : 参数
     */
    void setStatus(const QString &functionType, const QVariantMap map);

    /**
     * @brief dbusPackage : Push数据
     */
    void handlePushData(const DbusPackage &dbusPackage);

private:
    DataProvider *m_pDataProvider; ///< 数据提供类
    LocalCacheManager *m_pLocalCacheManager;
    DataHandlerManager *m_pDataHandlerManager;
    SyncManager *m_pSyncManager;
    QList<QString> m_nBusinessNotifyKeyQueue;  ///< 已经添加到通知栏的业务消息的notifyKey列表
};

#endif // INSPECTIONINTERFACE_H
