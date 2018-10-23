#ifndef DATALOCALCACHEBASE_H
#define DATALOCALCACHEBASE_H
#include "../common/dataoperationtype.h"
#include "../common/dbdefine.h"
#include "../common/dbhelper.h"
#include "../../framework/network/networkmanager.h"
#include "../../framework/common/commondbustypes.h"
#include <QObject>
#include <QDebug>
/**
 * @brief The DataLocalCacheBase class : 本地缓存
 */
class DataLocalCacheBase : public QObject
{
    Q_OBJECT
public:
    DataLocalCacheBase();
    ~DataLocalCacheBase();
    /**
     * @brief addCache : 添加缓存
     * @param dataPackage : 缓存数据
     * @param action : 动作的action
     * @return : 是否成功
     */
     virtual bool addCache(const DataPackage &dataPackage, const QString &action) ;
    /**
     * @brief submitLocalCache : 向服务器提交本地缓存
     * @param binaryType : true: 提交二进制文件； false：提交xml文件
     */
    virtual void submitLocalCache(bool binaryType = false) ;
    /**
     * @brief clearCache : 清除本地缓存
     */
    virtual void clearCache(bool binaryType = false) ;
    /**
     * @brief hasLocalCache : 是否有未提交的缓存
     * @param binaryType : true: 二进制文件； false：xml文件
     * @return : 是否有未提交的缓存
     */
    virtual bool hasLocalCache(bool binaryType = false);

    /**
     * @brief writeBinaryDBTable : 写二进制数据库
     */
    virtual void writeBinaryDBTable(const QString &action, const Attachment &attachment, const QString &tableName, const QString &binaryFilePath, const QString &offlineModel);

    /**
     * @brief isOnlieInterface : 是否纯在线接口
     * @param object : 接口名称
     * @return true: 是, false: 否
     */
    virtual bool isOnlieInterface(const QString &objects);

    /**
     * @brief getDataOperationType : 获取业务类型
     * @return  : 业务类型
     */
    DataOperationType getDataOperationType() const;
    /**
      * @brief writeXMLFile : 将datapackage写入本地文件
      * @param dataPackage : 缓存数据
      * @param savePath : 缓存路径
      * @return  : 是否保存成功
      */
     QString writeXMLFile(const DataPackage &dataPackage, const QString &savePath);
     /**
      * @brief readXmlFile : 将xml文件转换成datapackage
      * @param filePath: xml文件
      * @param dataPackage : 缓存数据
      * @return : 是否读取成功
      */
     bool readXmlFile(const QString &filePath, DataPackage &dataPackage);

     /**
      * @brief saveBinaryLocalCache : 保存二进制文件到缓存中，并写入数据库
      * @param attachment : 二进制
      * @param savePath : 缓存路径
      * @return : 二进制文件保存路径
      */
     QString saveBinaryLocalCache(const Attachment &attachment, const QString &savePath);

public slots:
    /**
    * @brief networkResponse : 接受manager的网络返回信号
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true : 成功； false：失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果
    */
    virtual  void networkResponse(QString objects, QString action, bool result, int error, DbusPackage map);

    /**
    * @brief networkResult : 接受manager的网络返回信号
    * @param objects : 业务编码
    * @param action : 序列号
    * @param result : true : 成功； false：失败
    * @param error : 错误代码;０:无错误
    * @param map : 返回的结果
    */
    virtual void networkResult(const QString &objects, const QString &action, const bool &result, const int &error, const DbusPackage &map);

signals:
     /**
     * @brief onSubmitProgress : 本地缓存提交进度
     * @param type : 业务模块类型
     * @param progress : 提交进度
     * @param description : 详情描述
     * @param binaryType : 是否时提交的二进制
     */
    void onSubmitProgress(DataOperationType type, int progress, const QString &description, bool binaryType);
    /**
     * @brief onSubmitFinished : 本地缓存提交完成
     * @param type : 业务模块类型
     * @param result : true : 成功； false：失败
     * @param binaryType : 是否时提交的二进制
     */
    void onSubmitFinished( DataOperationType type, bool result, bool binaryType);
    /**
     * @brief onSubmitStop : 本地缓存提交停止信号
     * @param type : 业务模块类型
     * @param binaryType : 是否时提交的二进制
     */
    void onSubmitStop(DataOperationType type, bool binaryType);
    /**
     * @brief onSubmitStart : 本地缓存提交开始信号
     * @param type : 业务模块类型
     * @param binaryType : 是否时提交的二进制
     */
    void onSubmitStart(DataOperationType type, bool binaryType);
    /**
     * @brief onResultOfResponse : 本地缓存网络提交返回结果信号
     * @param objects : 业务编码
     * @param action : 序列号
     * @param result : true:成功； false:失败
     * @param error : 错误代码;０:无错误
     * @param map : 返回的结果
     */
    void onResultOfResponse(QString objects, QString action, bool result, int error, DbusPackage map);
protected:
    DataOperationType m_nDataOperationType; ///<业务类型
    NetworkManager *m_pNetworkManager; ///<网络操作类
    QList<QMap<QString, QString> > m_nXmlValueList; ///< 需要提交的xml列表
    QList<QMap<QString, QString> > m_nBinaryValueList; ///< 需要提交的二进制列表
    int m_nFailedCount;  ///<xml文件提交失败的数量
    DBHelper *m_nDbHelper; ///<数据库访问对象

    /**
    * @brief createUuid : 创建提交队列序列号(uuid)
    * @return : 序列号
    */
    QString createUuid();

    /**
    * @brief sendData : 发送数据包
    * @param dataPackage :  数据包
    * @param action : action
    */
    void sendData(const DataPackage &dataPackage, const QString &action, const bool &binaryType = false);

private:
    QMap<QString, QString> m_nActionSequences;
};

#endif // DATALOCALCACHEBASE_H
