#ifndef OPERATIONDATALOCALCACHE_H
#define OPERATIONDATALOCALCACHE_H

#include "datalocalcachebase.h"
class OperationDataLocalCache:public DataLocalCacheBase
{
public:
    OperationDataLocalCache();
    ~OperationDataLocalCache();
    virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    virtual void submitLocalCache(bool binaryType = false) ;
    virtual void clearCache(bool binaryType = false) ;
    virtual bool hasLocalCache(bool binaryType = false);
    virtual bool isOnlieInterface(const QString &objects);
public slots:
    virtual  void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);
private:
  /** @brief writeCacheTable : 写缓存表
     * @param dataPackage : 提交时的数据包
     * @param strXmlName : 保存的xml文件全路径
     */
    bool writeCacheTable(const QString &action, const QString &strXmlName, const QString &offlineModel, const QString &taskID);
    void commitData(bool binaryType);
    void deleteLocalFile(const QString &ID, bool binaryType);
};

#endif // OPERATIONDATALOCALCACHE_H
