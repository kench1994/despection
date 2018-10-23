#ifndef MESSAGELOCALCACHE_H
#define MESSAGELOCALCACHE_H

#include "datalocalcachebase.h"
class MessageLocalCache : public DataLocalCacheBase
{
public:
    MessageLocalCache();
    ~MessageLocalCache();
    virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    virtual void submitLocalCache(bool binaryType = false) ;
    virtual void clearCache(bool binaryType = false) ;
    virtual bool hasLocalCache(bool binaryType = false);
public slots:
    virtual  void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);
private:
  /** @brief writeCacheTable : 写缓存表
     * @param dataPackage : 提交时的数据包
     * @param strXmlName : 保存的xml文件全路径
     */
    void writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel);
    void commitData();
    void deleteLocalFile(const QString &ID);
};

#endif // MESSAGELOCALCACHE_H
