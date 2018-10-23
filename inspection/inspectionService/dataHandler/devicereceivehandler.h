#ifndef DEVICERECEIVEHANDLER_H
#define DEVICERECEIVEHANDLER_H

#include "datahandlerbase.h"
class DeviceReceiveHandler : public DataHandlerBase
{
public:
    DeviceReceiveHandler();
    ~DeviceReceiveHandler();
    virtual void onLine(const QString &action, const DataPackage &dataPackage);
    virtual void offLine(const QString &action, const DataPackage &dataPackage);
    virtual int getItemCount();
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
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map);

private:
    /**
    * 处理任务差异数据
    * @param action action
    * @param data 任务差异化数据
    */
    void handleTaskPatch(const DataPackage &data, const QString &action);

    /**
    * 处理任务数据请求
    * @param action 上层Action
    * @param data  请求数据
    */
    void handleTaskListResponse(const QString &action, const DbusPackage &map);

    /**
    * 处理设备数据请求
    * @param action 上层Action
    * @param data  请求数据
    */
    void handleEquipmentListResponse(const QString &action, const DataPackage &dataPackage);

    /**
    * 标记已经完成的任务项
    *
    * @param idList idList
    */
    void updateEquipmentToComplete(QStringList idList);

    /**
    * 检查任务项的组是否为空，若为空则删除任务
    * @param taskID 任务ID
    **/
    void checkTask(const QString &taskID, const QString &userID);

    /**
    * 删除已经完成的任务
    *
    * @param taskID 任务ID
    */
    void deleteTask(const QString &taskID, const QString &userID);

    /**
    * 处理任务设备明细表单详细数据
    * @param action 上层action
    * @param data  数据
    */
    void handleEquipmentParaResponse(const QString &action, const DbusPackage &map);

    /**
    * 请求本地任务数据
    *
    * @param requestPackage 请求数据包
    * @param action action
    */
    void requestLocalTaskList(const DataPackage &requestPackage, const QString &action);

    /**
    * 获取本地任务信息
    *
    * @param requestPackage 请求数据包
    * @param resultPackage 本地查询结果
    */
    void getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage);

    /**
    * 请求本地设备
    *
    * @param requestPackage 数据包
    * @param action      action
    */
    void requestLocalEquipmentList(const DataPackage &requestPackage, const QString &action);

    /**
    * 获取本地设备项
    *
    * @param requestPackage 数据包
    * @param resultPackage 本地查询结果
    */
    void getLocalEquipmentList(const DataPackage &requestPackage, DataPackage &resultPackage);

    /**
    * 请求设备项详情
    *
    * @param requestPackage 数据包
    * @param action      action
    */
    void requestLocalEquipmentDetail(const DataPackage &requestPackage, const QString &action);

    /**
    * 获取设备项详情
    *
    * @param requestPackage 数据包
    * @param DataPackage 本地查询结果
    */
    void getEquipmentDetail(const DataPackage &requestPackage, DataPackage &resultPackage);

};

#endif // DEVICERECEIVEHANDLER_H
