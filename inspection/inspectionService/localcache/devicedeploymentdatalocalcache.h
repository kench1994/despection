#ifndef DEVICEDEPLOYMENTDATALOCALCACHE_H
#define DEVICEDEPLOYMENTDATALOCALCACHE_H

#include "datalocalcachebase.h"
class DeviceDeploymentDataLocalCache:public DataLocalCacheBase
{
public:
    DeviceDeploymentDataLocalCache();
    ~DeviceDeploymentDataLocalCache();
    virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    virtual void submitLocalCache(bool binaryType = false) ;
    virtual void clearCache(bool binaryType = false) ;
    virtual bool hasLocalCache(bool binaryType = false);

public slots:
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);

private:

    /**
    * @brief writeCacheTable : 写缓存表
    * @param action : action
    * @param orderCode : 任务单号
    * @param strXmlName : 保存的xml文件全路径
    * @param offlineModel : true:离线模式；false:在线模式
    */
    void writeCacheTable(const QString &action,
                         const QString &orderCode,
                         const QString &strXmlName,
                         const QString &offlineModel,
                         const QString &status);
    /**
    * 更新调拨任务表
    * param allocationID 任务ID
    */
    void writeTaskTable(const QString &orderCode, const QString &status);

    /**
    * 提交数据
    */
    void commitData();

    /**
    * 删除缓存
    */
    void deleteLocalFile(const QString &ID);

    /**
    * 从数据库读取并提交
    */
    void submitXml();

    /**
    * 处理xml提交结果
    */
    void resultProcess();
};

#endif // DEVICEDEPLOYMENTDATALOCALCACHE_H
