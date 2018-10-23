#ifndef INVENTORYSYNCHRONIZER_H
#define INVENTORYSYNCHRONIZER_H

#include "synchronizerbase.h"
class InventorySynchronizer : public SynchronizerBase
{
public:
    InventorySynchronizer();
    ~InventorySynchronizer();
    virtual void syncAddition(const QString &action,  const QString &objects, const QVariantMap &map);

public slots:
    /**
    * @brief networkResult : 网络消息接受槽函数
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true : 成功； false：失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果
    */
    virtual  void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);
private:
    /**
     * @brief synTask : 开始同步盘点任务
     * @param taskIdList : 要同步的任务的ID列表
     * @param userId : 用户ID
     * @param syncType : 同步的任务的类型,是新增还是更新任务
     */
    void synTask(const QStringList &taskIdList, const QString &userId, SyncTaskType syncType);
    /**
     * @brief taskDataHandle : 服务器返回的同步的任务数据的处理函数
     * @param action : 请求的action
     * @param dataPackage : 服务器返回的同步的任务数
     */
    void taskDataHandle(const QString &action, const DataPackage &dataPackage);
    /**
     * @brief updateTask : 更新本地任务表
     * @param taskValueList : 任务相关的字段信息
     * @param taskId : 任务ID
     */
    void updateTask(const QList<QMap<QString, QString> > &taskValueList, const QString &taskId);
    /**
     * @brief updateObject : 更新本地盘点任务项表
     * @param objectValueList : 任务项相关字段信息列表
     * @param objectIDList : 任务项相关的设备ID和设备的修改时间映射
     * @param taskId : 任务ID
     */
    void updateObject(const QList<QMap<QString, QString> > &objectValueList,
                      QMap<QString, QString> &objectIDList, const QString &taskId);
    /**
     * @brief updateDic : 更新盘点字典表
     * @param dicValueList : 字典字段信息
     * @param taskId : 任务ID
     */
    void updateDic(const QList<QMap<QString, QString> > &dicValueList, const QString &taskId);
    /**
     * @brief updateTaskSyncStatus : 更新任务表中任务的同步状态(SyncStatus = 1: 同步完成;SyncStatus = 0: 正在同步)
     * @param result : 同步结果
     * @param taskID : 任务ID
     */
    void updateTaskSyncStatus(const QString &taskID, bool result);
    /**
     * @brief resultProcess : 同步进度信息
     * @param action : 序列号
     * @param result : 该次同步结果,true:成功,false:失败
     */
    void resultProcess(const QString &action, bool result);

    /**
     * @brief markLocalTaskObjectsFinished : 将任务项标注成已完成，然后检查所属任务组是否完成
     * @param taskObjectList : 任务项ID列表
     * @return : 是否成功
     */
    bool markLocalTaskObjectsFinished(const QString &taskID, const QStringList &taskObjectList);

    /**
     * @brief checkLocalTaskFinished : 检查任务是否完成，如果完成，调用markLocalTaskFinished
     * @param taskID : 任务ID
     */
    void checkLocalTaskFinished(const QString &taskID);
    /**
     * @brief markLocalTaskFinished : 将任务删除，同时将该任务下的项都删除掉
     * @param taskID : 任务ID
     */
    void markLocalTaskFinished(const QString &taskID);

    /**
     * @brief updateUserTaskTime : 更新用户-任务表中指定任务的同步完成时间
     * @param time : 同步完成时间
     * @param taskId : 任务ID
     */
    void updateUserTaskTime(const QString &time, const QString &taskId);
    /**
     * @brief updateTaskTime : 更新盘点模块此次同步完成的时间
     * @param time : 同步完成的时间
     */
    void updateTaskTime(const QString &time);
    /**
     * @brief getMaxTime : 获取此次同步中所有任务同步时间的最大时间
     * @return : 最大时间
     */
    QString getMaxTime();
    /**
     * @brief getTaskLastSyncTime : 获取指定用户指定任务的上次同步时间
     * @param userID: 用户ID
     * @param taskID : 任务ID
     * @return : 上次同步时间
     */
    QString getTaskLastSyncTime(const QString &userID, const QString &taskID);
};

#endif // INVENTORYSYNCHRONIZER_H
