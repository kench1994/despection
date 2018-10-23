#ifndef LOCALCACHEMANAGER_H
#define LOCALCACHEMANAGER_H
#include <QObject>
#include "common/dataoperationtype.h"
#include "common/bussinessmap.h"
#include "../../framework/common/utils.h"
#include "../../framework/common/commondbustypes.h"
#include "localcachefactory.h"
#include "../dataprovider.h"
class LocalCacheManager :public QObject{
    Q_OBJECT
public:
    static LocalCacheManager* getInstance();
    ~LocalCacheManager();
   /**
     * 将待提交的数据添加到本地缓存
     * @param dataPackage ： 业务数据
     * @param action ： 标识
     * @return 添加缓存是否成功
     */
    bool addCache(const DataPackage &dataPackage, const QString  &action);

    /**
     * 提交本地所有业务缓存
     */
    void submitLocalCacheAll();

    /**
     * 提交本地指定类型业务的本地缓存
     * @param type :  业务类型
     * @param type :  true: 提交二进制文件； false：提交xml文件
     */
    void submitLocalCache(DataOperationType operationType, bool binaryType = false);

    /**
     * 清除本地所有业务缓存
     */
    void clearLocalCacheAll();

    /**
     * 清除本地指定类型业务的本地缓存
     * @param type :  业务类型
     */
    void clearLocalCache(DataOperationType type);

    /**
     * @brief hasLocalCache : 本地所有业务是否有未提交的缓存
     * @return : 是否有未提交的缓存
     */
    bool hasLocalCache();

    /**
     * @brief hasLocalCache : 本地是否有某业务的未提交的缓存
     * @param type : 业务类型
     * @return : 是否有未提交的缓存
     */
    bool hasLocalCache(DataOperationType type);
    /**
     * @brief hasBinaryLocalCache : 本地是否有某业务的二进制未提交的文件缓存
     * @param type : 业务类型
     * @return : 是否有未提交的缓存
     */
    bool hasBinaryLocalCache(DataOperationType type);
    /**
     * @brief hasBinaryLocalCache : 本地所有业务是否有二进制未提交的文件缓存
     * @return : 是否有未提交的缓存
     */
    bool hasBinaryLocalCache();
    /**
     * @brief connectNetworkManagerSingnals : 链接函数
     */
    void connectNetworkManagerSingnals();
    /**
     * @brief disconnectNetworkManagerSingnals : 取消链接函数
     */
    void disconnectNetworkManagerSingnals();

signals:
    void onResultOfResponse(QString objects, QString action, bool result, int error, DbusPackage map);
public slots:
    void sendNetworkResultSlot(QString objects, QString action, bool result, int error, DbusPackage map);

    void submitProgress(DataOperationType type, int progress, const QString &description, bool binaryType);
    void submitFinished( DataOperationType type, bool result, bool binaryType);
    void submitStop(DataOperationType type, bool binaryType);
    void submitStart(DataOperationType type, bool binaryType);

    void networkConnectChanged(const QString &networkType);
private:
    LocalCacheManager();
    bool isOnlineSubmit(const DataPackage &dataPackage);
    void sendNoNetworkResponse(const QString &objects, const QString &action);
private:
    static LocalCacheManager *m_PLocalCacheManager;
    LocalCacheFactory m_nLocalCacheFactory;  ///< 本地缓存业务工厂
    DataLocalCacheBase *m_pSubmittingLocalCache;   ///<正在提交xml的缓存模块
    DataLocalCacheBase *m_pSubmittingBinaryLocalCache; ///<正在提交二进制的缓存模块
    bool m_nIsSubmitting; ///< 是否正在向服务器提交数据
    bool m_nIsSubmittingBinary; ///< 是否正在向服务器提交数据
    NetworkManager *m_pNetworkManager;  ///<网络管理模块
    DataOperationType m_nSubmittingBusiness;
    QList<DataOperationType> m_nXmlFailList;
    QList<DataOperationType> m_nBinaryFailList;
};

#endif // LOCALCACHEMANAGER_H
