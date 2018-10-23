#ifndef DEVICEDEPLOYMENTSYNCHRONIZER_H
#define DEVICEDEPLOYMENTSYNCHRONIZER_H

#include "synchronizerbase.h"
class DeviceDeploymentSynchronizer : public SynchronizerBase
{
public:
    DeviceDeploymentSynchronizer();
    ~DeviceDeploymentSynchronizer();
    virtual void syncAddition(const QString &action,  const QString &objects, const QVariantMap &map);
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);
private:
    /**
    * 同步任务
    * @param list 任务列表
    */
    void synTaskByIds(const QStringList &taskIDs, const QString &userID);

    /**
    * 获取任务更新时间
    * @param taskID 任务ID
    * @return 上次任务个更新时间
    */
    QString getTaskSyncTime(const QString &userID, const QString &taskID);

    /**
    * 同步任务信息
    * @param taskID 任务ID
    * @param syncDatetime 任务时间
    * @param action action
    */
    void syncTaskInfoById(const QString &taskID, const QString &syncDatetime, const QString &action);

    /**
    * 更新任务表数据
    * @param dataRows 任务信息
    */
    void updateTask(const QList<DataRow> &dataRows);

    /**
    * 组或任务项更新数据库表
    * @param dataRows 任务项信息
    * @param deviceInfo 设备修改日期信息
    */
    void updateChild(const QList<DataRow> &dataRows, QMap<QString, QString> &deviceInfo);

    /**
     * 更新任务表状态
     * @param taskID 任务ID
     */
    void updateTaskStatus(const QString &taskID, const QString& syncStatus);

    /**
    * @param syncType 同步类型
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

};

#endif // DEVICEDEPLOYMENTSYNCHRONIZER_H
