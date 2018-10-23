#ifndef MAINTENANCEHANDLER_H
#define MAINTENANCEHANDLER_H

#include "datahandlerbase.h"
class MaintenanceHandler : public DataHandlerBase
{
public:
    MaintenanceHandler();
    ~MaintenanceHandler();
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
    * 获取任务基本信息 同步第一步
    *
    * @param head head
    * @param action action
    */
    void requestTasksPatch(const DataHead &head, const QString &action);

    /**
    * 将服务器返回数据进行处理，跟本地数据进行合并
    *
    * @param objects objects
    * @param action action
    * @param dataPackage dataPackage
    */
    void handleTasksPatch(const QString &objects, const QString &action, DataPackage &dataPackage);

    /**
    * 删除无效的未来任务
    * @param taskDataKeys  任务列表
    */
    void deletePreTask(const QStringList &taskDataKeys, const QString &userID);

    /**
    * 获取本地任务信息
    *
    * @param requestPackage 请求数据包
    * @param resultPackage 本地查询结果
    */
    void getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage);

    /**
    * 请求本地任务数据
    *
    * @param requestPackage 请求数据包
    * @param action action
    */
    void requestLocalTaskList(const DataPackage &requestPackage, const QString &action);

    /**
    * 将任务请求结果发送给app
    *
    * @param action action
    * @param data   data
    */
    void handleTaskListResponse(const QString &action, const DbusPackage &map);

    /**
     * 将group请求结果发送给app
     *
     * @param action action
     * @param data   data
     */
    void handleTaskGroupResponse(const QString &action, const DataPackage &dataPackage);

    /**
    * 删除已经完成的对象
    *
    * @param groupDataKeyList groupDataKeyList
    */
    void updateGroupToComplete(const QStringList &groupDataKeyList);

    /**
    * 按任务项更新完成进度
    *
    * @param dataPackage dataPackage
    * @param deleteList  deleteList
    */
    void updateExamObjectCount(const DataPackage &dataPackage, const QStringList &deleteList);

    /**
    * 请求本地group数据
    *
    * @param dataPackage 请求数据包
    * @param action      action
    */
    void requestLocalGroupList(const DataPackage &requestPackage, const QString &action);

    /**
    * 获取本地group数据
    *
    * @param requestPackage 请求数据包
    * @param resultPackage 本地查询结果
    */
    void getLocalTaskGroup(const DataPackage &requestPackage, DataPackage &resultPackage);


    /**
    * object请求返回处理 和本地数据库进行合并
    *
    * @param action action
    * @param data   data 服务器返回数据包
    */
    void handleGroupObjectResponse(const QString &action, const DataPackage &dataPackage);

    /**
    * 标记已经完成的任务项
    *
    * @param objectDataKeyList objectDataKeyList
    */
    void updateObjectToComplete(const QStringList &objectDataKeyList);

    /**
    * 按对象更新完成进度
    *
    * @param count count
    * @param taskGroupDataKey taskGroupDataKey
    */
    void updateExamObjectCount(const int &count, const QString &taskGroupDataKey);


    /**
    * 请求本地object数据
    *
    * @param dataPackage 请求数据包
    * @param action      action
    */
    void requestLocalGroupObjectList(const DataPackage &requestPackage, const QString &action);

    /**
    * 获取本地object数据
    *
    * @param requestPackage 请求数据包
    * @param resultPackage 本地查询结果
    */
    void getLocalGroupObject(DataPackage requestPackage, DataPackage &resultPackage);

    /**
    * object请求返回处理 和本地数据库进行合并
    *
    * @param action action
    * @param data   data 服务器返回数据包
    */
    void handleTaskObjectResponse(const QString &action, const DataPackage &dataPackage);

    /**
    * 请求本地object数据
    *
    * @param dataPackage 请求数据包
    * @param action      action
    */
    void requestLocalTaskObjectList(const DataPackage &requestPackage, const QString &action);

    /**
    * 获取本地object数据
    *
    * @param requestPackage 请求数据包
    * @param resultPackage 本地查询结果
    */
    void getLocalTaskObject(DataPackage requestPackage, DataPackage &resultPackage);


    /**
    * 返回object详情给app
    *
    * @param action action
    * @param map map
    */
    void handleTaskObjectDetailResponse(const QString &action, const DbusPackage &map);

    /**
    * 请求object详情
    *
    * @param requestPackage 数据包
    * @param action      action
    */
    void requestLocalTaskObjectDetail(const DataPackage &requestPackage, const QString &action);

    /**
    * 获取object 详情
    *
    * @param requestPackage 数据包
    * @param DataPackage 本地查询结果
    */
    void getTaskObjectDetail(const DataPackage &requestPackage, DataPackage &resultPackage);

    /**
    * 删除已经完成的任务
    *
    * @param taskDataKey taskDataKey
    */
    void deleteTask(const QString &taskDataKey, const QString &userID);

    /**
    * 检查任务项的组是否为空，若为空则删除任务
    * @param taskDataKey 任务ID
    **/
    void checkTask(const QString &taskDataKey, const QString &userID);

    /**
    * 检查组的任务项是否为空，若为空则删除组
    * @param taskGroupDataKey 组ID
    **/
    void checkGroup(const QString &taskGroupDataKey);
};

#endif // MAINTENANCEHANDLER_H
