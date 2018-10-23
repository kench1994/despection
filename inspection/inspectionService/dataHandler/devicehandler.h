#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "datahandlerbase.h"
class DeviceHandler : public DataHandlerBase
{
public:
    DeviceHandler();
    ~DeviceHandler();
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
};

#endif // DEVICEHANDLER_H
