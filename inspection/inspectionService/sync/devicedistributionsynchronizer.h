#ifndef DEVICEDISTRIBUTIONSYNCHRONIZER_H
#define DEVICEDISTRIBUTIONSYNCHRONIZER_H

#include "synchronizerbase.h"
class DeviceDistributionSynchronizer : public SynchronizerBase
{
public:
    DeviceDistributionSynchronizer();
    ~DeviceDistributionSynchronizer();
    virtual void syncAddition(const QString &action,  const QString &objects, const QVariantMap &map);
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);

private:
    /**
    * 同步任务
    * @param list 任务列表
    * @param taskType 任务类型
    */
    void synTaskByIds(const QStringList &taskIDs, const QString &userID, const SyncTaskType &taskType);

    /**
    * 获取任务更新时间
    * @param taskID 任务ID
    * @return 上次任务个更新时间
    */
    QString getTaskSyncTime(const QString &userID, const QString &taskID);

    /**
    * 同步任务信息
    * @param taskID 任务ID
    * @param taskType 任务类型
    * @param syncDatetime 任务时间
    * @param action action
    */
    void syncTaskInfoById(const QString &taskID, const QString &taskType, const QString &syncDatetime, const QString &action);


    /**
    * 准备更新数据
    * @param dataPackage 返回的数据
    */
    void handleTasks(const DataPackage &dataPackage, const QString &taskID);

    /**
    * 更新任务表数据
    * @param dataRows 任务信息
    */
    void updateTask(const QList<DataRow> &dataRows);


    /**
    * 更新任务项数据库表
    * @param dataRows dataRows
    */
    void updateEquipment(const QList<DataRow> &dataRows, QMap<QString, QString> &deviceInfo);


    /**
    * 检查任务项是否为空，若为空则删除任务
    * @param taskID 任务ID
    **/
    void checkTask(const QString &taskID, const QString &userID);

    /**
    * 删除已经完成的任务
    *
    * @param taskID 任务ID
    */
    void deleteTask(const QString &taskID, const QString &userID);

    /**
    * @param taskID 任务ID
    * @param result 网络请求的结果
    */
    void resultProcess(const QString &taskID, const bool &result);

    /**
    * 更新用户任务时间表
    * @param userID 用户ID
    * @param taskID 任务ID
    * @param time 时间
    */
    void updateUserTaskTime(const QString &userID, const QString &taskID, const QString &time);

    /**
    * 更新用户业务时间表
    * @param userID 用户ID
    * @param time 时间
    */
    void updateUserTime(const QString &userID, const QString &time);

    /**
    * 更新任务表状态
    * @param taskID 任务ID
    */
    void updateTaskStatus(const QString &taskID, const QString& syncStatus);


};

#endif // DEVICEDISTRIBUTIONSYNCHRONIZER_H
