#ifndef RECTIFICATIONSYNCHRONIZER_H
#define RECTIFICATIONSYNCHRONIZER_H
#include "synchronizerbase.h"
class RectificationSynchronizer : public SynchronizerBase
{
public:
    RectificationSynchronizer();
    ~RectificationSynchronizer();
    virtual void syncAddition(const QString &action,  const QString &objects, const QVariantMap &map);
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);

private:

    /**
    * 同步任务
    * @param orderIDs 任务列表
    */
    void synTaskByIds(const QStringList &orderIDs, const QString &userID, const QString &taskStatus);

    /**
    * 获取任务更新时间
    * @param orderID 任务ID
    * @param taskStatus  标识是整改任务还是整改确认
    * @return 上次任务个更新时间
    */
    QString getTaskSyncTime(const QString &userID, const QString &orderID, const QString& taskStatus);

    /**
    * 同步任务信息
    * @param orderID 任务ID
    * @param syncDatetime 任务时间
    * @param action action
    */
    void syncTaskInfoById(const QString &orderID, const QString &syncDatetime, const QString &action, const QString &taskStatus);

    /**
    * @brief handleTasks  准备更新数据
    * @param dataPackage  返回的数据
    * @param action action
    * @param taskStatus  标识是整改任务还是整改确认
    */
    void handleTasks(const DataPackage &dataPackage, const QString &action, const QString &taskStatus);

    /**
    * 更新任务表数据
    * @param dataRows 任务信息
    */
    void updateTask(const QList<DataRow> &dataRows);

    /**
     * 更新任务表状态
     * @param orderID 任务ID
     */
    void updateTaskStatus(const QString &orderID, const QString& syncStatus);

    /**
    * @param orderID 任务ID
    * @param result 网络请求的结果
    * @param taskStatus 标识是整改任务还是整改确认
    */
    void resultProcess(const QString &orderID, const bool &result, const QString &taskStatus);

    /**
    * 更新用户任务时间表
    * @param userID 用户ID
    * @param orderID 任务ID
    * @param syncTime 时间
    * @param taskStatus 标识是整改任务还是整改确认
    */
    void updateUserTaskTime(const QString &userID, const QString &orderID, const QString &syncTime, const QString &taskStatus);

    /**
    * 更新用户业务时间表
    * @param userID 用户ID
    * @param syncTime 时间
    */
    void updateUserTime(const QString &userID, const QString &syncTime, const QString& taskStatus);

    QString m_nTaskStatus;      ///标识是整改任务还是整改确认

};

#endif // RECTIFICATIONSYNCHRONIZER_H
