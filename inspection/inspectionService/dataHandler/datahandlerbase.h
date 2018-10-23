/*!
* \file datahandlerbase.h
* \brief 数据请求基类
*
*数据请求基类
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef DATAHANDLERBASE_H
#define DATAHANDLERBASE_H
#include <QStringList>
#include "../../framework/common/utils.h"
#include "../common/dataoperationtype.h"
#include "../common/bussinessmap.h"
#include "../common/dbhelper.h"
#include "../common/dbdefine.h"
#include <QVariantMap>
#include <QDebug>
/*!
  \class DataHandlerBase
  \inModule 数据请求基类
  \brief 数据请求基类
  \ingroup dataHandler
  DataHandlerBase 数据请求基类
*/
class DataHandlerBase : public QObject
{
    Q_OBJECT
public:
    DataHandlerBase();
    ~DataHandlerBase();
    /**
     * @brief request : 接受app请求的函数
     * @param action : 请求序列号
     * @param dataPackage : 请求包
     */
    void request(const QString &action, const DataPackage &dataPackage);
    /**
     * @brief requestTask : 接受app同步离线任务请求函数
     * @param action : 请求序列号
     * @param dataPackage : 请求包
     */
    void requestTask(const QString &action, const DataPackage &dataPackage);

    /**
     * @brief pureOnLine : 纯在线请求函数
     * @param action : 请求序列号
     * @param dataPackage : 请求包
     */
    virtual void pureOnLine(const QString &action, const  DataPackage &dataPackage);
    /**
     * @brief onLine : 混合模式请求函数
     * @param action : 请求序列号
     * @param dataPackage : 请求包
     */
    virtual void onLine(const QString &action, const DataPackage &dataPackage);

    /**
     * @brief offLine : 离线请求函数，或者混合模式下没网情况下请求函数
     * @param action : 请求序列号
     * @param dataPackage : 请求包
     */
    virtual void offLine(const QString &action, const DataPackage &dataPackage);
    /**
     * @brief getItemCount : 获取模块未完成任务数量
     * @return : 模块未完成任务数量
     */
    virtual int getItemCount();
    /**
     * @brief cleaerTask : 清除本地任务
     * @param userID : 用户ID
     * @param organiseUnitID  : 组织结构ID
     * @param departmentID : 部门ID
     */
    virtual void cleaerTask(const QString &userID, const QString &organiseUnitID, const QString &departmentID);
    /**
     * @brief syncFinished : 接收任务同步完成槽函数
     * @param action : 请求序列号
     * @param objects : 业务编号
     * @param map : 返回参数
     */
    virtual void syncFinished(const QString &action, const QString &objects, const QVariantMap &map);
    /**
     * @brief sendData : 发送数据到服务器
     * @param dataPackage : 发送数据包
     * @param action : 请求序列号
     * @param binaryType : 发送的数据是否时二进制
     * @return : 网络请求序列号
     */
    QString sendData(const DataPackage &dataPackage, const QString &action, const bool &binaryType = false);
    /**
     * @brief getPostCodesByUser : 根据用户ID获取岗位代码
     * @param userID : 用户ID
     * @return : 岗位代码
     */
    QStringList getPostCodesByUser(const QString &userID);

    /**
     * @brief getUsersByPostCode : 根据岗位代码获取用户ID列表
     * @param postCode : 岗位代码
     * @return : 用户ID列表
     */
    QStringList getUsersByPostCode(const QString &postCode);

    /**
     * @brief convertToDataPackage : 数据包转换，将key-value数据转换成dataPackage
     * @param keyValue : 键值对列表
     * @param dataHead : dataPackage的头
     * @param tableName : 表名
     * @param tableKeyID : 表名的Key
     * @param dataPackage : 返回的dataPackage
     */
    void convertToDataPackage(const QList<QMap<QString, QString> > &keyValue,
                              const DataHead &dataHead, const QString &tableName,
                              const QString &tableKeyID, DataPackage &dataPackage);
    /**
     * @brief convertToDataPackage : 数据包转换，将key-value数据转换成dataPackage
     * @param keyValue : 键值对列表
     * @param dataHead : dataPackage的头
     * @param para : 返回的参数
     * @param tableName : 表名
     * @param tableKeyID : 表名的Key
     * @param dataPackage : 返回的dataPackage
     */
    void convertToDataPackage(const QList<QMap<QString, QString> > &keyValue,
                              const DataHead &dataHead, const QMap<QString, QString> &para, const QString &tableName,
                              const QString &tableKeyID, DataPackage &dataPackage);
    /**
     * @brief getDataOperationType : 获取业务类型
     * @return : 业务类型
     */
    DataOperationType getDataOperationType();

    /**
    * @brief requestObjs : 请求和任务相关的设备或者零部件
    * @param objIDModifyTimeMap : 项ID和该项ID对应的modifyDate
    * @param taskId : 任务ID
    */
    bool requestObjs(const QMap<QString, QString> &objIDModifyTimeMap, const QString &taskId);

signals:
    /**
    * @brief onResultOfResponse : 本地缓存网络提交返回结果信号
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true:成功； false:失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果
    */
    void onResultOfResponse(QString objects, QString action, bool result, int error, DbusPackage map);

    /**
    * @brief onBeginSyncSignal : 增量同步开始信号
    * @param messageType : 消息类型
    * @param action : 序列号
    * @param objects : 业务编码
    * @param map : 返回的结果
    */
    void onBeginSyncSignal(BusinessMessageType messageType,  QString action, QString objects, QVariantMap map);

public slots:
    /**
    * @brief networkResponse : 接受manager的网络返回信号
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true : 成功； false：失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果
    */
    void networkResponse(QString objects, QString action, bool result, int error, DbusPackage map);

    /**
    * @brief networkResult : 接受manager的网络返回信号
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true : 成功； false：失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果
    */
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, DbusPackage &map);

protected:
    QMap <QString, DataPackage> m_nRequestPkgMap;///<请求包映射
    DataOperationType m_nDataOperationType; ///<业务类型
    DBHelper *m_nDbHelper; ///<数据库访问对象
    /**
    * @brief createUuid : 创建一个同步网络序列号(uuid)
    * @return : 网络序列号
    */
    QString createUuid();

    void sendNoNetworkResponse(const QString &objects, const QString &action);

private:
    QMap<QString, QString> m_nActionList;  ///<网络请求的序列号列表
    QStringList m_nPureOnLineActionList;///<纯在线请求的action

    /**
    * @brief isNeedSync : 判断该项是否需要同步
    * @param objID : 项ID
    * @param modifiedDate : 该项的修改时间
    * @return : true:需要同步; false:不需要同步
    */
    bool isNeedSync(const QString &objID, const QString &modifiedDate);

    /**
    * @brief writeDeviceToDB : 将数据写入设备表
    * @param dataPackage : 数据
    */
    void writeDeviceToDB(const DataPackage &dataPackage);
};

#endif // DATAHANDLERBASE_H
