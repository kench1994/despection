#ifndef USERINFOHANDLER_H
#define USERINFOHANDLER_H

#include "datahandlerbase.h"
class UserinfoHandler : public DataHandlerBase
{
public:
    UserinfoHandler();
    ~UserinfoHandler();
    virtual void offLine(const QString &action, const DataPackage &dataPackage);
    virtual void onLine(const QString &action, const DataPackage &dataPackage);
    virtual int getItemCount();
    virtual void syncFinished(const QString &action, const QString &objects, const QVariantMap &map);
    virtual void pureOnLine(const QString &action, const  DataPackage &dataPackage);
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
     * @brief handleUserInfo : 处理用户登录返回的数据
     * @param dataPackage : 数据
     * @param action : 网络请求的序列号
     */
    void handleUserInfo(const DataPackage &dataPackage, const QString &action);

    /**
     * @brief updateHistoryUsersInfo : 更新历史登陆用户信息
     * @param userCode : 用户编号
     * @param passwd : 用户密码
     */
    void updateHistoryUsersInfo(const QString &userCode, const QString &passwd);
};

#endif // USERINFOHANDLER_H
