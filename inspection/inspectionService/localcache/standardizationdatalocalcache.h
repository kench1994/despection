#ifndef STANDARDIZATIONDATALOCALCACHE_H
#define STANDARDIZATIONDATALOCALCACHE_H

#include "datalocalcachebase.h"
class StandardizationDataLocalCache : public DataLocalCacheBase
{
public:
    StandardizationDataLocalCache();
    ~StandardizationDataLocalCache();
    virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    virtual void submitLocalCache(bool binaryType = false) ;
    virtual void clearCache(bool binaryType = false) ;
    virtual bool hasLocalCache(bool binaryType = false);
public slots:
    virtual  void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);
private:
    /**
    * @brief writeCacheTable : 写xml缓存表
    * @param action : action
    * @param strXmlName 保存的xml文件全路径
    * @param offlineModel : 离线模式标识
    * @param taskDataKey : taskDataKey
    * @param taskObjectDataKey : taskObjectDataKey
    * @param taskItemDataKey : taskItemDataKey
    */
    void writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel, const QString &taskDataKey, const QString &taskObjectDataKey, const QString &taskItemDataKey);

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
    void deleteLocalFile(const QString &ID, const bool &binaryType);

    /**
    * 清理本地与此条记录相关的信息
    * @param taskDataKey  任务DataKey
    * @param taskObjectDataKey  对象DataKey
    * @param taskItemDataKey  对象子Item的DataKey
    * @param localStatus  localStatus
    */
    void updateTaskInfo(const QString &taskDataKey,
                        const QString &taskObjectDataKey,
                        const QString &taskItemDataKey,
                        const QString &localStatus);

    /**
    * 判断当前对象是否是空的
    * @param objectDataKey 对象DataKey
    * @param localStatus  localStatus
    * @return true 有Item false 无Item
    */
    bool hasItem(const QString &objectDataKey, const QString &localStatus);

    /**
    * 判断当前任务是否是空的
    * @param taskDataKey 任务DataKey
    * @param localStatus  localStatus
    * @return true 有Object false 无Object
    */
    bool hasObject(const QString &taskDataKey, const QString &localStatus);

    /**
    * 获取任务ID
    * @param taskItemDataKey 对象ID
    */
    QString getTaskDataKey(const QString &taskItemDataKey);

    /**
    * 获取组ID
    * @param taskItemDataKey 对象ID
    */
    QString getTaskObjectDataKey(const QString &taskItemDataKey);

    /**
    * 更新本地数据库对应的任务信息
    * @param taskItemDataKey  ItemDataKey
    */
    void modifyTaskStatus(const QString &taskItemDataKey, const QString &taskObjectDataKey);

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

};
#endif // STANDARDIZATIONDATALOCALCACHE_H
