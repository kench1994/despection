#ifndef INSPECTIONHANDLER_H
#define INSPECTIONHANDLER_H

#include "datahandlerbase.h"
class InspectionHandler : public DataHandlerBase
{
public:
    InspectionHandler();
    ~InspectionHandler();
    virtual void onLine(const QString &action, const DataPackage &dataPackage);
    virtual void offLine(const QString &action, const DataPackage &dataPackage);
    virtual int getItemCount();
    virtual void cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID);
    virtual void syncFinished(const QString &action, const QString &objects, const QVariantMap &map);
public slots:
    /**
     * @brief networkResult : 网络请求返回结果函数
     * @param objects : objects
     * @param action : 网络请求的序列号
     * @param result : 网络请求结果；true:成功；false：失败
     * @param error : 网络请求的错误代码；0：无错误；其他：错误代码
     * @param map : 网络请求返回数据
     */
    virtual  void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map);
private:
    /**
     * @brief handleTaskPatch : 请求任务返回数据的处理函数
     * @param objects : 请求的objects
     * @param action : 请求的action
     * @param data : 返回的数据
     */
    void handleTaskPatch(const QString &objects,  const QString &action, const DataPackage &data);

    /**
     * @brief handleTaskGroupPatch : 请求任务组返回数据的处理函数
     * @param objects : 请求的objects
     * @param action : 请求的action
     * @param data : 返回的数据
     */
    void handleTaskGroupPatch(const QString &objects,  const QString &action, const DataPackage &data);

    /**
     * @brief handleTaskGroupPatch : 请求任务项返回数据的处理函数
     * @param objects : 请求的objects
     * @param action : 请求的action
     * @param data : 返回的数据
     */
    void handleTaskObjectPatch(const QString &objects,  const QString &action, const DataPackage &data);
    /**
     * @brief getLocalTasks : 请求本地巡检任务
     * @param requestPackage : 请求包
     * @param resultPackage : 返回结果包
     * @return : 是否成功
     */
    bool getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage);
    /**
     * @brief getLocalTasks : 请求本地巡检任务组
     * @param requestPackage : 请求包
     * @param resultPackage : 返回结果包
     * @return : 是否成功
     */
    bool getLocalTaskGroup(const DataPackage &requestPackage, DataPackage &resultPackage);
    /**
     * @brief getLocalTaskObject : 请求本地巡检任务项
     * @param requestPackage : 请求包
     * @param resultPackage : 返回结果包
     * @return : 是否成功
     */
    bool getLocalTaskObject(const DataPackage &requestPackage, DataPackage &resultPackage);

    /**
     * @brief getLocalTaskObjectDetail : 请求本地巡检任务项详情
     * @param requestPackage : 请求包
     * @param resultPackage : 返回结果包
     * @return : 是否成功
     */
    bool getLocalTaskObjectDetail(const DataPackage &requestPackage, DataPackage &resultPackage);

    /**
     * @brief checkLocalTaskGroupFinished : 检查本地任务组是否完成，如果该组下的所有项完成，标注该组已经完成
     * @param taskDataKey : 任务ID列表
     * @param taskGroupDataKeyList : 任务组ID列表
     * @return : 是否成功
     */
    bool checkLocalTaskGroupFinished(const QString &taskDataKey, const QStringList &taskGroupDataKeyList);

    /**
     * @brief markLocalTaskGroupFinished : 将任务组标注成已完成，同时将该组下的项标注为完成
     * @param taskDataKey : 任务ID
     * @param taskGroupDataKeyList : 任务组列表
     * @return : 是否成功
     */
    bool markLocalTaskGroupFinished(const QString &taskDataKey, const QStringList &taskGroupDataKeyList);

    /**
     * @brief markLocalTaskObjectsFinished : 将任务项标注成已完成，然后检查所属任务组是否完成
     * @param taskObjectList : 任务项ID列表
     * @return : 是否成功
     */
    bool markLocalTaskObjectsFinished(const QStringList &taskObjectList);
    /**
     * 检查未提交的任务组是否为空，若为空则删除任务,同时删除该任务下的所有任务组
     * @param taskDataKey 任务ID
     **/
    /**
     * @brief checkLocalTaskFinished : 检查任务是否完成，如果完成，调用markLocalTaskFinished
     * @param taskDataKey : 任务ID
     */
    void checkLocalTaskFinished(const QString &taskDataKey);
    /**
     * @brief markLocalTaskFinished : 将任务删除，同时将该任务下的组和项都删除掉
     * @param taskDataKey : 任务ID
     */
    void markLocalTaskFinished(const QString &taskDataKey);

    /**
     * @brief updateLocalTaskGroupExamObjectCount : 更新任务组的已巡检项
     * @param taskGroupDataKeyList : 任务组列表
     */
    void updateLocalTaskGroupExamObjectCount(const QString &taskGroupDataKey, int count, bool all = false);
    /**
     * @brief queryLastTime : 查询上次同步业务的时间
     * @param userID : 用户ＩＤ
     * @return : 上次同步业务的时间
     */
    QString queryLastTime(const QString &userID);
};

#endif // INSPECTIONHANDLER_H
