#ifndef SYNCHRONIZERBASE_H
#define SYNCHRONIZERBASE_H

#include <QObject>
#include <QVariantMap>
#include "../../framework/common/utils.h"
#include "../common/dataoperationtype.h"
#include "../common/dbhelper.h"
class SynchronizerBase : public QObject
{
    Q_OBJECT
public:
    SynchronizerBase();
    ~SynchronizerBase();

    //任务-类型的enum 1 新增 2 修改 3 完成
    enum SyncTaskType{
        SYNC_TASK_UNKOWN,
        SYNC_TASK_ADD,
        SYNC_TASK_UPDATE,
        SYNC_TASK_FINISHED
    };

    /**
     * @brief execSync  : 执行增量同步
     * @param action : action
     * @param objects : objects
     * @param map : 增量同步的数据
     */
    void execSyncTask(const QString &action,  const QString &objects, const QVariantMap &map);

    /**
     * @brief execSyncPreTask : 执行同步未来任务
     * @param action : 序列号
     * @param dataPackage : 请求数据
     */
    void execSyncPreTask(const QString &action,  const DataPackage &dataPackage);

    /**
     * @brief syncAddition  : 增量同步
     * @param action : action
     * @param objects : objects
     * @param map : 增量同步的数据
     */
    virtual void syncAddition(const QString &action, const QString &objects, const QVariantMap &map);

    /**
     * @brief syncPreTask : 同步未来任务
     * @param action : 序列号
     * @param dataPackage : 请求数据
     */
    virtual void syncPreTask(const QString &action, const DataPackage &dataPackage);

    /**
     * @brief sendData : 发送数据包
     * @param dataPackage :  数据包
     * @param action : action
     */
    void sendData(const DataPackage &dataPackage, const QString &action);

    /**
     * @brief requestObjs : 请求和任务相关的设备或者零部件
     * @param objIDModifyTimeMap : 项ID和该项ID对应的modifyDate
     * @param taskId : 任务ID
     */
    bool requestObjs(const QMap<QString, QString> &objIDModifyTimeMap, const QString &taskId);

signals:
    void onSyncFinishedSignal(const DataOperationType &operationType, const BusinessMessageType &businessMsgType,
                              const QString &action, const QString &objects, const QVariantMap &map);

    /**
    * @brief onResultOfResponse : 同步未来任务返回结果信号
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true:成功； false:失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果()
    */
    void onResultOfResponse(QString objects, QString action, bool result, int error, DbusPackage map);

public slots:
    /**
    * @brief networkResponse : 接受manager的网络返回信号
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true : 成功； false：失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果
    */
   void networkResponse(QString objects, QString action, bool result, int error, DbusPackage map);

    /**
    * @brief networkResult : 接受manager的网络返回信号
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true : 成功； false：失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果
    */
   virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);

protected:
    bool m_nRunnings;///<是否正在进行同步
    QString m_nSyncUser; ///< 由dataHandler模块传入同步user，用户
    QString m_nSyncAction; ///< 由dataHandler模块传入同步action，同步完成后回传给dataHandler
    QString m_nSyncObjects; ///< 由dataHandler模块传入同步objects，同步完成后回传给dataHandler
    QVariantMap m_nSyncMap; ///< 需要进行同步的信息，由dataHandler模块传入，同步完成后回传给dataHandler
    int m_nSyncTaskTotalCount; ///<需要同步的任务总数
    int m_nSyncTaskSuccessCount;///<同步成功的任务数
    int m_nSyncTaskFailCount;///<同步失败的任务数
    DataOperationType m_nDataOperationType; ///<业务类型
    QVariantMap m_nSyncTaskInfoMap; ///< 正在同步的任务相关信息的映射<taskId,( 任务同步类型(TaskType)，任务同步时间(TaskSyncTime) )>
    DBHelper *m_nDbHelper; ///<数据库访问对象
    QString m_nUserMaxTime;
    QStringList m_nSyncPreTaskPlanIDs; ///< 正在同步的未来任务的 PlanId
    int m_nSyncPreTaskFailCount;///<同步失败的未来任务数
    QString m_nSyncPreTaskAction; ///< 由app传入同步action
    /**
    * @brief createUuid : 创建一个同步网络序列号(uuid)
    * @return : 网络序列号
    */
    QString createUuid();

private:
    /**
     * @brief isNeedSync : 判断该项是否需要同步
     * @param objID : 项ID
     * @param modifiedDate : 该项的修改时间
     * @return : true:需要同步; false:不需要同步
     */
    bool isNeedSync(const QString &objID, const QString &modifiedDate);
    /**
     * @brief writeDeviceToDB
     * @param dataPackage
     */
    void writeDeviceToDB(const DataPackage &dataPackage);
    QMap<QString, QString> m_nActionList;  ///<网络请求的序列号列表
};

#endif // SYNCHRONIZERBASE_H
