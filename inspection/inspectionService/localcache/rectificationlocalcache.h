#ifndef RECTIFICATIONLOCALCACHE_H
#define RECTIFICATIONLOCALCACHE_H

#include "datalocalcachebase.h"
class RectificationLocalCache : public DataLocalCacheBase
{
public:
    RectificationLocalCache();
    ~RectificationLocalCache();
    virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    virtual void submitLocalCache(bool binaryType = false) ;
    virtual void clearCache(bool binaryType = false) ;
    virtual bool hasLocalCache(bool binaryType = false);

public slots:
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);

private:

    /** @brief writeCacheTable : 写缓存表
    * @param dataPackage : 提交时的数据包
    * @param strXmlName : 保存的xml文件全路径
    */
    void writeCacheTable(const QString &action,
                         const QString &strXmlName,
                         const QString &offlineModel,
                         const QString &orderID,
                         const QString &taskStatus);
    /**
    * @brief submitXml : 提交xml数据
    */
    void submitXml();

    /**
    * @brief resultProcess :  xml数据提交后的处理
    */
    void resultProcess();

    /**
    * @brief submitBinary : 提交binary数据
    */
    void submitBinary();

    /**
    * @brief commitData : 提交数据
    * @param binaryType : xml或binary标识
    */
    void commitData(bool binaryType);

    /**
    * @brief deleteLocalFile : 删除缓存
    * @param ID : ID
    * @param binaryType : xml或binary标识
    */
    void deleteLocalFile(const QString &ID, bool binaryType);

    /**
    * @brief modifyTaskStatus
    * @param orederID
    * @param taskStatus : 标识是整改任务还是整改确认
    */
    void modifyTaskStatus(const QString &orederID, const QString &taskStatus);

    /**
    * @brief deleteTask : 删除任务
    * @param orederID : 任务ID
    * @param taskStatus : 标识是整改任务还是整改确认
    */
    void deleteTask(const QString &orederID, const QString &taskStatus);


};

#endif // RECTIFICATIONLOCALCACHE_H
