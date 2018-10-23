#ifndef MAINTENANCEDATALOCALCACHE_H
#define MAINTENANCEDATALOCALCACHE_H

#include "datalocalcachebase.h"
class MaintenanceDataLocalCache : public DataLocalCacheBase
{
public:
    MaintenanceDataLocalCache();
    ~MaintenanceDataLocalCache();
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
    * @param strXmlName : 保存的xml文件全路径
    * @param offlineModel : true:离线模式；false:在线模式
    * @param taskDataKey : taskDataKey
    * @param taskGroupDataKey : taskGroupDataKey
    * @param taskObjectDataKey : taskObjectDataKey
    * @return : 写表成功
    */
    void writeCacheTable(const QString &action,
                         const QString &strXmlName,
                         const QString &offlineModel,
                         const QString &taskDataKey,
                         const QString &taskGroupDataKey,
                         const QString &taskObjectDataKey);

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
    * 清理本地与此条记录相关的信息
    * @param taskDataKey  任务DataKey
    * @param taskGroupDataKey  对象子Item的DataKey
    * @param taskObjectDataKey  对象DataKey
    * @param localStatus  localStatus
    */
    void updateTaskInfo(const QString &taskDataKey,
                        const QString &taskGroupDataKey,
                        const QString &taskObjectDataKey,
                        const QString &localStatus);

    /**
    * 判断当前组是否是空的
    * @param groupDataKey 组DataKey
    * @return true 有Object false 无Object
    */
    bool hasObject(const QString &groupDataKey, const QString &localStatus);

    /**
    * 判断当前任务是否是空的
    * @param taskDataKey 任务DataKey
    * @return true 有Group false 无Group
    */
    bool hasGroup(const QString &taskDataKey, const QString &localStatus);


    /**
    * 获取任务ID
    * @param taskObjectDataKey 对象ID
    */
    QString getTaskDataKey(const QString &taskObjectDataKey);

    /**
    * 获取组ID
    * @param taskObjectDataKey 对象ID
    */
    QString getTaskGroupDataKey(const QString &taskObjectDataKey);

    /**
    * 更新本地数据库对应的任务信息
    * @param taskObjectDataKey  ObjectDataKey
    * @param taskGroupDataKey  GroupDataKey
    */
    void modifyTaskStatus(const QString &taskObjectDataKey, const QString &taskGroupDataKey);

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

#endif // MAINTENANCEDATALOCALCACHE_H

