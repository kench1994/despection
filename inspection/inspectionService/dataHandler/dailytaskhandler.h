#ifndef DAILYTASKHANDLER_H
#define DAILYTASKHANDLER_H

#include "datahandlerbase.h"
class DailyTaskHandler : public DataHandlerBase
{
public:
    DailyTaskHandler();
    ~DailyTaskHandler();
    virtual void onLine(const QString &action, const DataPackage &dataPackage);
    virtual void offLine(const QString &action, const DataPackage &dataPackage);
    virtual int getItemCount();
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
     * @brief markLocalTaskFinished : 将任务删除，同时将该任务下的组和项都删除掉
     * @param taskDataKey : 任务ID
     */
    void markLocalTaskFinished(const QStringList &taskDataKeyList);

    /**
     * @brief queryLastTime : 查询上次同步业务的时间
     * @param userID : 用户ＩＤ
     * @return : 上次同步业务的时间
     */
    QString queryLastTime(const QString &userID);
};


#endif // DAILYTASKHANDLER_H
