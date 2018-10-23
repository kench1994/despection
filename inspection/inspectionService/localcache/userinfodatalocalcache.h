#ifndef USERINFODATALOCALCACHE_H
#define USERINFODATALOCALCACHE_H

#include "datalocalcachebase.h"
class UserInfoDataLocalCache : public DataLocalCacheBase
{
public:
    UserInfoDataLocalCache();
    ~UserInfoDataLocalCache();
    virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    virtual void submitLocalCache(bool binaryType = false) ;
    virtual void clearCache(bool binaryType = false) ;
    virtual bool hasLocalCache(bool binaryType = false);
};

#endif // USERINFODATALOCALCACHE_H
