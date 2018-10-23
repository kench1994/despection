/*!
* \file transmitmanager.h
* \brief 数据获取与提交模块
*
*数据获取与提交模块
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/8
*/
#ifndef TRANSMITMANAGER_H
#define TRANSMITMANAGER_H

#include <QObject>
#include "../../../framework/common/utils.h"
#include "upgrade/upgradeconfig.h"
#include "../../../framework/common/functiontype.h"
#include "clientinterface.h"
#include <QQuickView>
#include <QQuickItem>
#include <QTimer>
#include <ctoastmanager.h>

/*!
  \class TransmitManager
  \inModule 数据获取与提交模块
  \brief 获取服务端数据并将客户端数据上传给服务端
  \ingroup common
  ConvertData类为为应用提供获取服务端数据并将客户端数据上传给服务端的接口
*/
typedef struct _INSPECTION_ACTION{
  DataPackage package;
  QString actionName;
  QString errorString;
  QString functionType;
  bool runInBackground;
  QStringList downLoadFileList;
}InspectionActions;

class SystemParam;
class TransmitManager : public QObject
{

    Q_OBJECT
public:

    /**
     * @brief   getInstance:获取单例对象
     * @return  TransmitManager*, 非NULL 成功, NULL 失败
     */
    static TransmitManager* getInstance();

    ~TransmitManager();

    Q_INVOKABLE void setStatus(const QString &statusType, const QVariantMap &map);
    /**
     * @brief requestData : 数据请求函数
     * @param head : 包头
     * @param para : 请求参数
     * @param action : 请求标识
     * @param errorText : 错误描述
     * @return : -1： 参数错误； 0：参数正确
     */
    Q_INVOKABLE int requestData(const QVariantMap &head, const QVariantMap &para, const QString &action, const QString &errorText,
                                bool runInBackground = false, bool needShowLoading = true);
    /**
     * @brief submitData : 数据提交函数
     * @param head : 包头
     * @param table : 表头字段
     * @param dataRow : 提交参数
     * @param attachementes : 附件列表
     * @param binaries : 二进制附件列表
     * @param action : 请求标识
     * @param errorText : 错误描述
     * @return : -1： 参数错误； 0：参数正确成功
     */
    Q_INVOKABLE int submitData(const QVariantMap &head, const QVariantMap &table,
                    QVariant dataRowList, QVariant attachementes,
                    const QString &serialKey,  const QString &action, const QString &errorText,
                               bool runInBackground = false, bool needShowLoading = true);

    /**
     * @brief submitDataWidthPart : 提交带有零部件的数据,只有保养提交和运维流程中维修提交中使用
     * @param head : 包头
     * @param table : 表头字段
     * @param dataRowList : 提交参数列表
     * @param attachementes : 附件列表
     * @param partTable :
     * @param partDataRowList
     * @param serialKey : 序列号
     * @param action : 请求标识
     * @param errorText : 错误描述
     * @param runInBackground : 后台运行
     * @param needShowLoading : 显示正在提交数据界面
     * @return :  -1： 参数错误； 0：参数正确成功
     */
    Q_INVOKABLE int submitDataWithPart(const QVariantMap &head, const QVariantMap &table,
                    QVariant dataRowList, QVariant attachementes,const QVariantMap &partTable,
                            QVariant partDataRowList,
                    const QString &serialKey,  const QString &action, const QString &errorText,
                               bool runInBackground = false, bool needShowLoading = true);

    Q_INVOKABLE void requestFaultRepairImageList(const QStringList &fileUrlList,
                                                 const QString &action, const QString &errorText = "");
    /**
     * @brief  sendOriginalForm : 发送巡检数据
     * @param  dataPkg : DataPackage & 发送的数据
     * @param  action : QString & 业务逻辑序列号
     * @param  errorText : QString & 错误信息
     */
    Q_INVOKABLE void sendOriginalForm(const DataPackage &dataPkg, const QString &action, const QString &errorText);

    /**
     * @brief syncPreTask :  同步未来任务函数
     * @param head : 包头
     * @param para : 请求参数
     * @param action : 请求标识
     * @param errorText : 错误描述
     * @return : -1： 参数错误； 0：参数正确
     */
    Q_INVOKABLE int syncPreTask(const QVariantMap &head, const QVariantMap &para, const QString &action, const QString &errorText,
                                bool runInBackground = false, bool needShowLoading = true);

    /**
     * @brief requestTaskData : 数据请求函数
     * @param head : 包头
     * @param para : 请求参数
     * @param action : 请求标识
     * @param errorText : 错误描述
     * @return : -1： 参数错误； 0：参数正确
     */
    Q_INVOKABLE int requestTaskData(const QVariantMap &head, const QVariantMap &para, const QString &action, const QString &errorText,
                                bool runInBackground = false, bool needShowLoading = true);


signals:

    /**
     * @brief resultOfResponse : 数据返回信号
     * @param result  : bool  返回结果
     * @param error  : int  错误信息
     * @param action  : QString  action
     * @param map  : QVariantMap 数据map数组
     */
    void resultOfResponse(bool result, int error, QString action, QVariantMap map);
    void downLoadFileResponse(const QVariantMap &map, const QString &action);
    void messageUpdated(const QString &operType);
public slots:
    void serviceResponse(QString objects, QString action, bool result, int error, DbusPackage map);
    void messageUpdate(QString messageType);
private:

    /**
     * @brief 构造函数
     * @param parent : 父对象
     */
    TransmitManager(QObject *parent = 0);
    /**
     * @brief showLoading : 显示刷新提示
     * @param runInBackground : bool 是否弹出提示
     * @param needShowLoading : bool 是否显示提示
     */
    void showLoading(bool runInBackground = false, bool needShowLoading = true);

    /**
     * @brief processTimeout : 隐藏刷新提示
     */
    void hideLoading();

    /**
     * @brief sendData : 发送数据
     * @param package : DataPackage & 数据包
     * @param action : QString & action
     * @param errorText : QString & 错误信息
     * @param runInBackground : bool 是否弹出提示
     * @param needShowLoading : bool 是否显示提示
     */
    void sendData(const DataPackage &package, const QString& action, QString errorText = "",
                  bool runInBackground = false, bool needShowLoading = true, QString functionType = FUNCTION_TYPE_REQUEST);

    void downLoadFile(const QStringList &fileUrlList, const QString& action, const QString errorText = "",
                      bool runInBackground = false, bool needShowLoading = true);
    /**
     * @brief sendResult : 发送结果
     */
    void sendResult(const bool &result, const int &error, const QString &action, const QVariantMap &map);
    void doTask(InspectionActions actiontask);

    static TransmitManager *m_pTransMng;///<  本地单例对象
    CToastManager* m_pToast;///<  toast管理对象
    QQuickView *m_pViewer;///< Viewer
    QQuickItem* m_pLoading;///< QQuickItem
    SystemParam *m_pSysParam;///< 系统管理对象
    QString m_networkError;///< 网络错误
    QList <InspectionActions> m_ActionList;
    ClientInterface *m_pClientInterface;
    bool isRunnig;
};
#endif // TRANSMITMANAGER_H
