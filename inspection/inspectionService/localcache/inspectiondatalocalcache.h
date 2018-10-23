#ifndef INSPECTIONDATALOCALCACHE_H
#define INSPECTIONDATALOCALCACHE_H

#include "datalocalcachebase.h"
class InspectionDataLocalCache:public DataLocalCacheBase
{
public:
    InspectionDataLocalCache();
    ~InspectionDataLocalCache();
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
    bool writeCacheTable(const QString &action, const QString &objects, const QString &taskObjectDataKey, const QString &strXmlName, const QString &offlineModel);
    void commitData(bool binaryType);
    void deleteLocalFile(const QString &ID, bool binaryType);

    /**
     * @brief markLocalTaskObjectsCached : 标注该任务项已经提交到缓存
     * @param taskObjectDataKey : 任务项ID
     */
    void markLocalTaskObjectsCached(const QString &taskObjectDataKey);

    /**
     * @brief markLocalTaskObjectsFinished : 标注该任务项已经提交到服务器
     * @param taskObjectDataKey : 任务项ID
     */
    void markLocalTaskObjectsFinished(const QString &taskObjectDataKey);

    /**
     * @brief markLocalTaskGroupCached : 标注该任务组已经提交到缓存,该函数由提交对象组时调用
     * @param taskObjectDataKey : 任务项ID, 根据任务项ID查询出所属的组
     */
    void markLocalTaskGroupCached(const QString &taskObjectDataKey);
    /**
     * @brief checkLocalTaskGroupCached : 检查该任务组下的所有任务项是否全部提交缓存
     * @param taskDataKey : 任务ID
     * @param taskGroupDataKey : 任务组ID
     */
    void checkLocalTaskGroupCached(const QString &taskDataKey, const QString &taskGroupDataKey);
    /**
     * @brief markLocalTaskGroupFinished : 标注任务组下的所有任务项是否全部提交到服务器,该函数由提交对象组时调用
     * @param taskObjectDataKey : 任务项ID, 根据任务项ID查询出所属的组
     */
    void markLocalTaskGroupFinished(const QString &taskObjectDataKey);
    /**
     * @brief checkLocalTaskGroupFinished : 检查该任务组下的所有任务项是否全部提交到服务器,
     * @param taskDataKey :任务ID
     * @param taskGroupDataKey : 任务组ID
     */
    void checkLocalTaskGroupFinished(const QString &taskDataKey, const QString &taskGroupDataKey);
    /**
     * @brief checkLocalTaskCached : 检查该任务下的所有任务项是否全部提交缓存
     * @param taskDataKey : 任务ID
     */
    void checkLocalTaskCached(const QString &taskDataKey);
    /**
     * @brief markLocalTaskFinished : 检查该任务下的所有任务项是否全部提交到服务器,如果完成,将任务删除,同时将该任务下的组和项都删除掉
     * @param taskDataKey : 任务ID
     */
    void checkLocalTaskFinished(const QString &taskDataKey);

     /**
      * @brief updateLocalTaskGroupExamObjectCount : 更新任务组的已巡检项
      * @param taskGroupDataKeyList : 任务组列表
      * @param all : true:将该组进度标注已经完成; false:进度加１
      */
     void updateLocalTaskGroupExamObjectCount(const QString &taskGroupDataKey, bool all = false);
};

#endif // INSPECTIONDATALOCALCACHE_H
