#ifndef EXAMINESYNCHRONIZER_H
#define EXAMINESYNCHRONIZER_H

#include "synchronizerbase.h"
class ExamineSynchronizer : public SynchronizerBase
{
public:
    ExamineSynchronizer();
    ~ExamineSynchronizer();
    virtual void syncAddition(const QString &action,  const QString &objects, const QVariantMap &map);
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
    * 请求任务详细数据
    * @param taskDateKey 任务DateKey
    * @param userID 用户ID
    * @param type  任务属性 1 新增 2 修改
    */
    void syncTaskInfoById(const QString &taskDateKey, const QString &userID, const QString &type);

    /**
    * 准备更新数据
    * @param dataPackage 返回的数据
    */
    void handleTasks(const DataPackage &dataPackage, const QString &taskDataKey);

    /**
    * 更新任务表数据
    * @param dataRows 数据
    */
    void updateTaskTable(const QList<DataRow> &dataRows);

    /**
    * 更新任务对象表数据
    * @param values 数据
    */
    void updateObjectTable(const QList<DataRow> &dataRows);

    /**
    * 更新任务子对象（Item）表数据
    * @param values 数据
    */
    void updateItemTable(const QList<DataRow> &dataRows);

    /**
    * 更新检查点表数据
    * @param values 数据
    */
    void updateAreaTable(const QList<DataRow> &dataRows);

    /**
    * 更新组织机构表数据
    * @param values 数据
    */
    void updateOrganizationTable(const QList<DataRow> &dataRows);

    /**
    * 判断当前任务是否存在
    * @param taskDataKey 任务DataKey
    * @return true 存在 false 不存在
    */
    bool isTaskExisted(const QString &taskDataKey);

    /**
    * 判断当前Object是否存在
    * @param objectDataKey 对象DataKey
    * @return true 存在 false 不存在
    */
    bool isObjectExisted(const QString &objectDataKey);

    /**
    * 判断当前item否存在
    * @param itemDataKey ItemDataKey
    * @return true 存在 false 不存在
    */
    bool isItemExisted(const QString &itemDataKey);

    /**
    * 判断当前区域点否存在
    * @param areaID 区域点ID
    * @return true 存在 false 不存在
    */
    bool isAreaExisted(const QString &areaID);

    /**
    * 判断当前组织机构信息否存在
    * @param areaID 区域点ID
    * @return true 存在 false 不存在
    */
    bool isOrganizationExisted(const QString &organiseUnitID);

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
};

#endif // EXAMINESYNCHRONIZER_H
