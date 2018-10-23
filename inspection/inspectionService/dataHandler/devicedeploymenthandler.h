#ifndef DEVICEDEPLOYMENTHANDLER_H
#define DEVICEDEPLOYMENTHANDLER_H
#include "datahandlerbase.h"
class DeviceDeploymentHandler : public DataHandlerBase
{
public:
    DeviceDeploymentHandler();
    ~DeviceDeploymentHandler();
    virtual void onLine(const QString &action, const DataPackage &dataPackage);
    virtual void offLine(const QString &action, const DataPackage &dataPackage);
    virtual int getItemCount();
    virtual void cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID);
    virtual void syncFinished(const QString &action, const QString &objects, const QVariantMap &map);
public slots:
    /**
     * @brief networkResult : 网络请求返回结果函数
     * @param objects : objects
     * @param action : 网络请求的序列号
     * @param result : 网络请求结果；true:成功；false：失败
     * @param error : 网络请求的错误代码；0：无错误；其他：错误代码
     * @param map : 网络请求返回数据
     */
    virtual  void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map);

private:

    /**
     * 处理增量同步任务
     * @param data data
     * @param action action
     */
    void handleTaskPatch(const DataPackage &data, const QString &action);

    /**
    * 删除任务
    * @param value 任务ID列表
    */
    void deleteTask(DataRow &dataRow);

    /**
    * 更新任务表数据
    * @param value 任务信息
    */
    void updateTask(DataRow &dataRow);


    /**
    * 处理任务项列表
    * @param data data
    * @param action action
    */
    void handleEquipmentResponse(DataPackage &data);

    /**
    * 从本地数据库中获取任务列表
    * @param dataPackage dataPackage
    * @return DataPackage
    */
    bool getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage);

    /**
     * 从本地数据库中获取任务项列表
     * @param dataPackage dataPackage
     * @return DataPackage
     */
    bool getLocalEquipments(const DataPackage &requestPackage, DataPackage &resultPackage);
};

#endif // DEVICEDEPLOYMENTHANDLER_H
