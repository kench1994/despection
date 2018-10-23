#ifndef HOMEHANDLER_H
#define HOMEHANDLER_H

#include "datahandlerbase.h"
#include <QMap>
class HomeHandler : public DataHandlerBase
{
public:
    HomeHandler();
    ~HomeHandler();
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
     * @brief handleItemCounts :
     * @param dataPackage :
     */
    void handleItemCounts(const DataPackage &dataPackage);

    /**
     * @brief getLocalItemCounts : 本地获取所有业务任务数量及未完成的任务数
     * @param dataPackage : 请求数据
     * @param action : 网络请求的序列号
     */
    void getLocalItemCounts(const DataPackage &dataPackage, DataPackage &resultPackage);

    /**
     * @brief moduleInfo : 获取模块信息
     * @param module : 模块名称
     * @param map : 模块信息
     */
    void moduleInfo(const QString &module, QMap<QString, QString> &map);
    /**
     * @brief getTotalCount : 获取所有业务任务个数
     * @param userID : 用户ID
     * @return 业务任务个数
     */
    int getTotalCount(const QString& userID);

    /**
     * @brief getModuleItemCount : 获取业务任务个数
     * @param moduleID : 业务ID
     * @param userID : 用户ID
     * @return 所有业务任务个数
     */
    int getModuleItemCount(const QString &userID, const QString& moduleID);

    /**
     * @brief initModules : 初始化业务模块map表
     * @param modules : 模块map表
     */
    void initModules(QMap<DataOperationType, QMap<QString,QString> > &modules);
};

#endif // HOMEHANDLER_H
