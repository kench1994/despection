#ifndef INVENTORYHANDLER_H
#define INVENTORYHANDLER_H
#include "datahandlerbase.h"
#include <QMap>
class InventoryHandler : public DataHandlerBase
{
public:
    InventoryHandler();
    ~InventoryHandler();
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
    virtual  void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map);
    /**
     * @brief taskHandFunction : 请求离线盘点任务返回数据的处理函数
     * @param objects : objects
     * @param action : 网络请求的序列号
     * @param data : 离线盘点任务返回数据
     */
    void taskHandFunction(const QString &objects, const QString &action, const DataPackage &data);
private:
    /**
     * @brief getLocalTasks : 请求本地盘点任务
     * @param requestPackage : 请求包
     * @param resultPackage : 返回结果包
     * @return : 是否成功
     */
    bool getLocalTasks(const DataPackage &requestPackage, DataPackage &resultPackage);
    /**
     * @brief getLocalTaskObjects : 请求本地盘点任务项列表
     * @param requestPackage : 请求包
     * @param resultPackage : 返回结果包
     * @return : 是否成功
     */
    bool getLocalTaskObjects(const DataPackage &requestPackage, DataPackage &resultPackage);
    /**
     * @brief getLocalObjectPara : 请求本地盘点任务项明细
     * @param requestPackage : 请求包
     * @param resultPackage : 返回结果包
     * @return : 是否成功
     */
    bool getLocalObjectPara(const DataPackage &requestPackage, DataPackage &resultPackage);
private:
    /**
     * @brief objectsHandFunction :根据服务器返回数据修改本地任务项,如果从服务获取的数据中没有该项,则说明已经完成,把本地任务项标注已完成
     * @param action : 网络请求的序列号
     * @param data : 服务获取的数据
     * @return : 是否成功
     */
    void  objectsHandFunction(const QString &action, const DataPackage &data);

    /**
     * @brief markLocalTaskObjectsFinished : 将任务项标注成已完成，然后检查所属任务是否完成
     * @param taskID : 任务ID
     * @param taskObjectList : 任务项ID列表
     * @return : 是否成功
     */
    bool markLocalTaskObjectsFinished(const QString &taskID, const QStringList &taskObjectList);

    /**
     * @brief checkLocalTaskFinished : 检查任务是否完成，如果完成，调用markLocalTaskFinished
     * @param taskID : 任务ID
     */
    void checkLocalTaskFinished(const QString &taskID);
    /**
     * @brief markLocalTaskFinished : 将任务删除，同时将该任务下的项都删除掉
     * @param taskID : 任务ID
     */
    void markLocalTaskFinished(const QString &taskID);
    QString queryLastTime(const QString &userID);
};

#endif // INVENTORYHANDLER_H
