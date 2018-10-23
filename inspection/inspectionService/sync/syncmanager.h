#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H
#include "../common/dataoperationtype.h"
#include "synchronizerbase.h"
#include "syncfactory.h"
#include <QObject>
#include <QVariantMap>
#include <QList>
class SyncManager : public QObject
{
    Q_OBJECT
public:
    static SyncManager* getInstance();
    ~SyncManager();
    /**
     * @brief syncAddition : 增量同步
     * @param action : 序列号
     * @param type : 业务类型
     * @param map : 增量同步所需参数
     */
    void syncAddition(const QString &action,  const QString &objects, const QVariantMap &map);

    /**
     * @brief syncPreTask : 未来任务同步
     * @param action : 序列号
     * @param dataPackage : 请求数据
     */
    void syncPreTask(const QString &action,  const DataPackage &dataPackage);

public slots:
    /**
     * @brief sendNetworkResultSlot : 接受网络返回信号的槽函数
     * @param objects : objects
     * @param action : 网络请求的序列号
     * @param result : 网络请求结果；true:成功；false：失败
     * @param error : 网络请求的错误代码；0：无错误；其他：错误代码
     * @param map : 网络请求返回数据
     */
    void sendNetworkResultSlot(QString objects, QString action, bool result, int error, DbusPackage map);
    /**
     * @brief syncFinished
     * @param operationType
     * @param businessMsgType
     * @param action
     * @param objects
     * @param map
     */
    void syncFinished(const DataOperationType operationType, const BusinessMessageType &businessMsgType,
                      const QString &action, const QString &objects, const QVariantMap &map);
signals:
    void onSyncFinishedSignal(const BusinessMessageType businessMsgType, const QString action, const QString objects, QVariantMap map);
    void onResultOfResponse(QString objects, QString action, bool result, int error, DbusPackage map);
private:
    SyncManager();
private:
    static SyncManager* m_pSyncManager;
    SyncFactory m_nSyncFactory;
};

#endif // SYNCMANAGER_H
