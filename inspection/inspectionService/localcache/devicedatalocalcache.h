#ifndef DEVICEDATALOCALCACHE_H
#define DEVICEDATALOCALCACHE_H
#include "datalocalcachebase.h"

class DeviceDataLocalCache:public DataLocalCacheBase
{
public:
    DeviceDataLocalCache();
    ~DeviceDataLocalCache();
    virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    virtual void submitLocalCache(bool binaryType = false) ;
    virtual void clearCache(bool binaryType = false) ;
    virtual bool hasLocalCache(bool binaryType = false);
    virtual bool isOnlieInterface(const QString &objects);

public slots:
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);

private:

    /** @brief writeCacheTable : 写缓存表
    * @param dataPackage : 提交时的数据包
    * @param strXmlName : 保存的xml文件全路径
    */
    void writeCacheTable(const QString &action,
                       const QString &strXmlName,
                       const QString &offlineModel);

    /**
    * @brief commitData : 提交数据
    */
    void commitData(const bool &binaryType);

    /**
    * @brief deleteLocalFile : 删除缓存
    * @param ID : ID
    */
    void deleteLocalFile(const QString &ID, const bool &binaryType);

    /**
    * 从数据库读取并提交
    */
    void submitXml();

    /**
    * 从数据库读取并提交
    */
    void submitBinary();

    /**
    * 处理xml提交结果
    */
    void resultProcess();
};

#endif // DEVICEDATALOCALCACHE_H
