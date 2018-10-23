#ifndef RECTIFICATIONHANDLER_H
#define RECTIFICATIONHANDLER_H

#include "datahandlerbase.h"
class RectificationHandler : public DataHandlerBase
{
public:
    RectificationHandler();
    ~RectificationHandler();
    virtual void onLine(const QString &action, const DataPackage &dataPackage);
    virtual void offLine(const QString &action, const DataPackage &dataPackage);
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
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map);

private:

    /**
    * 处理增量同步任务
    * @param data data
    * @param action action
    */
    void handleTaskPatch(const DataPackage &data, const QString &action, const QString &taskStatus);

    /**
    * 从本地数据库中获取任务列表
    * @param dataPackage dataPackage
    * @return DataPackage
    */
    bool getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage, const QString &taskStatus);

    /**
    * 获取服务器本次返回的数据中，serverDate 的最大值
    * @param list 服务器返回的任务中，serverDate 的链表
    * @return 最大的serverDate
    */
    QString getMaxTime(const QStringList &syncTimeList);

    /**
    * 修改业务同步时间
    * @param time 同步成功时的时间（服务器返回的最后一个任务同步完成的时间)
    */
    void updateSyncTime(const QString &syncTime, const QString &taskStatus, const QString & userID);

    /**
    * 从本地数据库中获取任务项列表
    * @param dataPackage dataPackage
    * @return DataPackage
    */
    bool getLocalProcDetail(const DataPackage &requestPackage, DataPackage &resultPackage);

    QString m_nTaskStatus; ///任务的状态，标识是整改任务还是整改确认
};

#endif // RECTIFICATIONHANDLER_H
