#ifndef MAINTENANCESYNCHRONIZER_H
#define MAINTENANCESYNCHRONIZER_H

#include "synchronizerbase.h"
class MaintenanceSynchronizer : public SynchronizerBase
{
public:
    MaintenanceSynchronizer();
    ~MaintenanceSynchronizer();

    virtual void syncAddition(const QString &action, const QString &objects, const QVariantMap &map);
    virtual void syncPreTask(const QString &action, const DataPackage &dataPackage);
public slots:
    void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);

private:
    /**
    * 同步任务
    * @param taskDataKeys 任务列表
    * @param userID 用户ID
    * @param taskType 任务状态
    */
    void synTaskByIds(const QStringList &taskDataKeys, const QString &userID, const SyncTaskType &taskType);

    /**
    * 获取任务更新时间
    * @param taskDataKey 任务ID
    * @param userID 用户ID
    * @return 上次任务个更新时间
    */
    QString getTaskSyncTime(const QString &userID, const QString &taskDataKey);

    /**
    * 同步任务信息
    * @param taskDataKey 任务ID
    * @param syncDatetime 任务时间
    * @param userID 用户ID
    * @param taskType 任务状态
    */
    void syncTaskInfoById(const QString &taskDataKey, const QString &userID, const QString &syncDatetime, const QString &taskType);

    /**
    * 准备更新数据
    * @param dataPackage 返回的数据
    */
    void handleTasks(const DataPackage &dataPackage, const QString &taskDataKey);

    /**
    * 更新任务表数据
    * @param dataRows 任务信息
    */
    void updateTask(const QList<DataRow> &dataRows);

    /**
    * 更新组数据库表
    * @param dataRows dataRows
    */
    void updateGroup(const QList<DataRow> &dataRows);

    /**
    * 组或任务项更新数据库表
    * @param dataRows dataRows
    */
    void updateItem(const QList<DataRow> &dataRows, QMap<QString, QString> &deviceInfo);

    /**
    * @param taskDataKey 任务ID
    * @param result 网络请求的结果
    */
    void resultProcess(const QString &taskDataKey, const bool &result);

    /**
    * 更新用户任务时间表
    * @param userID 用户ID
    * @param taskDataKey 任务ID
    * @param time 时间
    */
    void updateUserTaskTime(const QString &userID, const QString &taskDataKey, const QString &time);

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
    void updateTaskStatus(const QString &taskDataKey, const QString& syncStatus);

    /**
    * 删除已经完成的任务
    *
    * @param taskDataKey taskDataKey
    */
    void deleteTask(const QString &taskDataKey, const QString &userID);

    /**
    * 删除已经完成的组
    *
    * @param taskGroupDataKey taskGroupDataKey
    */
    void updateGroupToComplete(const QString &taskGroupDataKey);

    /**
    * 检查任务项的组是否为空，若为空则删除任务
    * @param taskDataKey 任务ID
    **/
    void checkTask(const QString &taskDataKey, const QString &userID);

    /**
    * 检查组的任务项是否为空，若为空则删除组
    * @param taskGroupDataKey 组ID
    **/
    void checkGroup(const QString &taskGroupDataKey);


    /**
    * 获取预生成任务
    * @param dataPackage dataPackage
    */
    void handlePreTaskPlanIDs(const DataPackage &dataPackage);

    /**
    * 预生成任务数据处理
    * @param dataPackage dataPackage
    * @param planID planID
    */
    void handlePreTasks(const DataPackage &dataPackage, const QString &planID);

    /**
    * 处理预生成任务返回结果
    * @param planID 任务方案ID
    * @param result 返回结果 成功 失败
    */
    void preTaskResultProcess(const QString &planID, const bool &result);

    /**
    * 删除任务
    * @param taskDataKeyList 任务ID列表
    */
    void deleteTask(const QStringList &taskDataKeyList);

    /**
    * 更新任务表状态
    * @param planID 任务planID
    */
    void updateTaskStatusByPlanID(const QString &planID, const QString &syncStatus);

};

#endif // MAINTENANCESYNCHRONIZER_H
