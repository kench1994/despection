#include "userinfodatalocalcache.h"
UserInfoDataLocalCache::UserInfoDataLocalCache()
{
    m_nDataOperationType = BUSINESS_TYPE_USER_INFO;
}

UserInfoDataLocalCache::~UserInfoDataLocalCache()
{

}

bool UserInfoDataLocalCache::addCache(const DataPackage &dataPackage, const QString &action)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(dataPackage);
    Q_UNUSED(action);
    return true;
}

void UserInfoDataLocalCache::submitLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(binaryType);
}

void UserInfoDataLocalCache::clearCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(binaryType);
}

bool UserInfoDataLocalCache::hasLocalCache(bool binaryType)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(binaryType);
    return false;
}
