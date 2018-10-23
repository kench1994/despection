#ifndef DATAHANDLERMANAGER_H
#define DATAHANDLERMANAGER_H
#include <QObject>
#include "common/dataoperationtype.h"
#include "common/bussinessmap.h"
#include "../../framework/common/utils.h"
#include "../../framework/network/networkmanager.h"
#include "../../framework/common/commondbustypes.h"
#include "datahandlerfactory.h"
#include "../dataprovider.h"
#include <QVariantMap>
/**
 * @brief The DataHandlerManager class : 数据请求操作管理类
 */
class DataHandlerManager : public QObject
{
    Q_OBJECT
public:
    static DataHandlerManager *getInstance();
    ~DataHandlerManager();
    /**
     * @brief request : 数据请求函数
     * @param dataPackage : 数据请求的datapackage
     * @param action : 数据请求的序列号
     */
    void request(const DataPackage &dataPackage, const QString &action);

    /**
     * @brief requestTask : 数据请求函数
     * @param dataPackage : 数据请求的datapackage
     * @param action : 数据请求的序列号
     */
    void requestTask(const DataPackage &dataPackage, const QString &action);

    /**
     * @brief syncFinished : 同步完成回调函数，由messageLoopFunction调用
     * @param action : 网络请求的序列号
     * @param objects : objects
     * @param map : 数据
     */
    void syncFinished(const QString &action, const QString &objects, const QVariantMap &map);
signals:
    /**
     * @brief onResultOfResponse :
     * @param objects : objects
     * @param action : 网络请求的序列号
     * @param result : 网络请求结果；true:成功；false：失败
     * @param error : 网络请求的错误代码；0：无错误；其他：错误代码
     * @param map : 网络请求返回数据
     */
    void onResultOfResponse(QString objects, QString action, bool result, int error, DbusPackage map);
    void onBeginSyncSignal(BusinessMessageType messageType, QString action, QString objects, QVariantMap map);
public slots:

    /**
     * @brief sendNetworkResultSlot : 接受网络返回信号的槽函数
     * @param objects : objects
     * @param action : 网络请求的序列号
     * @param result : 网络请求结果；true:成功；false：失败
     * @param error : 网络请求的错误代码；0：无错误；其他：错误代码
     * @param map : 网络请求返回数据
     */
    void sendNetworkResultSlot(QString objects, QString action, bool result, int error, DbusPackage map);
private:
    DataHandlerManager();
private:
    static DataHandlerManager *m_pDataHandlerManager;
    DataHandlerFactory m_nDataHandlerFactory;  ///< 本地业务工厂
};

#endif // DATAHANDLERMANAGER_H
