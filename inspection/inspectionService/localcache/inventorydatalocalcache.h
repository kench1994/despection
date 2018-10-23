#ifndef INVENTORYDATALOCALCACHE_H
#define INVENTORYDATALOCALCACHE_H

#include "datalocalcachebase.h"
class InventoryDataLocalCache :public DataLocalCacheBase
{
public:
    InventoryDataLocalCache();
    ~InventoryDataLocalCache();
    virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    virtual void submitLocalCache(bool binaryType = false) ;
    virtual void clearCache(bool binaryType = false) ;
    virtual bool hasLocalCache(bool binaryType = false);
public slots:
virtual  void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);
private:
    /** @brief writeCacheTable : 写盘点提交数据到缓存表
     * @param action : 数据提交的网络序列号
     * @param strXmlName : 保存的xml文件全路径
     * @param offlineModel : true:离线模式;false:在线模式
     */
    void writeCacheTable(const QString &action,   const QString &taskID,  const QString &inventoryId,
                         const QString &strXmlName, const QString &offlineModel);
    /**
     * @brief commitData : 提交数据到服务器
     * @param binaryType : 是否时二进制文件
     */
    void commitData(bool binaryType);
    /**
     * @brief deleteLocalFile : 任务提交成功后，清除本地缓存数据
     * @param ID : 提交任务缓存表中的主键
     * @param binaryType : 是否时二进制文件
     */
    void deleteLocalFile(const QString &ID, bool binaryType);
    /**
     * @brief checkTaskFinished : 检查该任务下的所有任务项是否全部提交缓存
     * @param taskId : 任务ID
     */
    void checkTaskFinished(const QString &taskId);
    /**
     * @brief checkLocalTaskCached : 检查该任务下的所有缓存提交任务项是否全部提交到服务器
     * @param taskId : 任务ID
     */
    void checkLocalTaskCached(const QString &taskId);
    /**
     * @brief markLocalTaskObjectsFinished : 标注该任务下的该任务项已经提交到服务器
     * @param taskId : 任务ID
     * @param inventoryId : 任务项ID
     */
    void markLocalTaskObjectsFinished(const QString &taskId, const QString &inventoryId);
    /**
     * @brief markLocalTaskObjectsCached : 标注该任务下的该任务项已经提交到缓存
     * @param taskId : 任务ID
     * @param inventoryId : 任务项ID
     */
    void markLocalTaskObjectsCached(const QString &taskId, const QString &inventoryId);
};

#endif // INVENTORYDATALOCALCACHE_H
