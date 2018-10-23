#ifndef DEVICERECEIVEDATALOCALCACHE_H
#define DEVICERECEIVEDATALOCALCACHE_H

#include "datalocalcachebase.h"
class DeviceReceiveDataLocalCache:public DataLocalCacheBase
{
public:
    DeviceReceiveDataLocalCache();
    ~DeviceReceiveDataLocalCache();
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
    * @param orderCode : 任务单号
    * @param equipmentID : 设备ID
    */
    void writeCacheTable(const QString &action,  const QString &strXmlName, const QString &offlineModel, const QString &orderCode, const QString &equipmentID);

    /**
    * @brief updateTaskInfo : 清理本地与此条记录相关的信息
    * @param orderCode : 任务单号
    * @param equipmentID : 设备ID
    * @param localStatus : 设备项状态
    */
    void updateTaskInfo(const QString &orderCode, const QString &equipmentID, const QString &localStatus);

    /**
    * @brief getEquipmentID :  根据设备码获取设备ID
    * @param type : 设备码类型
    * @param code : 设备码
    * @return 设备ID
    */
    QString getEquipmentID(const QString &type, const QString &code);

    /**
    * @brief hasEquipment : 判断当前任务是否是空的
    * @param orderCode 任务单号
    * @return true 有Equipment false 无Equipment
    */
    bool hasEquipment(const QString &orderCode, const QString &localStatus);

    /**
    * 从数据库读取并提交
    */
    void submitXml();


    /**
    * 提交数据
    */
    void commitData();

    /**
    * 删除缓存
    */
    void deleteLocalFile(const QString &ID);

    /**
    * 处理xml提交结果
    */
    void resultProcess();

};

#endif // DEVICERECEIVEDATALOCALCACHE_H
