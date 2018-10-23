/*!
* \file RuntimeParam.h
* \brief 运行时参数，包括登陆状态，登陆用户信息等
*
*运行时参数，包括登陆状态，登陆用户信息等
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/8
*/
#ifndef RUNTIMEPARAM_H
#define RUNTIMEPARAM_H
#include <QObject>
#include <QVariantMap>
#include <QQuickView>
#include <csystemdevicemanager.h>
/*!
  \class RuntimeParam
  \inModule 运行时参数，包括登陆状态，登陆用户信息等
  \brief 运行时参数，包括登陆状态，登陆用户信息等
  \ingroup common
  RuntimeParam运行时参数，包括登陆状态，登陆用户信息等
*/

class RuntimeParam:public QObject
{
    Q_OBJECT

public:
    /**
     * @brief   getInstance:获取单例对象
     * @return  RuntimeParam*, 非NULL 成功, NULL 失败
     */
    static RuntimeParam *getInstance();

    /**
     * @brief   getViewer:获取对象
     * @return  QQuickView * ,viewer
     */
    QQuickView *getViewer();

    /**
     * @brief  setViewer : 设置对象
     * @param  viewer : QQuickView *: 对象
     */
    void setViewer(QQuickView *viewer);

    /**
     * @brief  getPadId : 获取设备id
     * @return QString, 设备id
     */
    Q_INVOKABLE QString getPadId();

    /**
     * @brief  setUserValueMap : 设置user相关数据的map数组
     * @param  userMap : QVariantMap &: user相关数据的map数组
     */
    Q_INVOKABLE void setUserValueMap(const QVariantMap &userMap);

    /**
     * @brief  getUserMap : 获取user相关数据的map数组
     * @return QVariantMap, user相关数据的map数组
     */
    Q_INVOKABLE QVariantMap getUserMap();

    /**
     * @brief setLoginUserMsg : 设置当前登陆用户的信息
     * @param userMsg : 当前登陆用户的信息
     */
    void setLoginUserMsg(const QMap<QString, QString> &userMsg);

    /**
     * @brief getLoginUserMsg : 获取当前登陆用户的信息
     * @return : 当前登陆用户的信息
     */
    QMap<QString, QString> getLoginUserMsg() const;

    /**
     * @brief getLoginUserMsgByKey : 获取当前登陆用户的具体某个属性信息
     * @param key : 登陆用户的具体某个属性
     * @return : 属性信息
     */
    QString getLoginUserMsgByKey(const QString &key) const;

    /**
     * @brief  getUserId : 获取用户Id
     * @return QString, 用户Id
     */
    Q_INVOKABLE QString getUserId();

    /**
     * @brief  getUserId : 获取用户编号
     * @return QString, 用户编号
     */
    Q_INVOKABLE QString getUserCode();

    /**
     * @brief  getUserName : 获取用户名称
     * @return QString, 用户名称
     */
    Q_INVOKABLE QString getUserName();

    /**
     * @brief  getOrganiseUnitIDs : 获取所属组织机构ID
     * @return QString, 所属组织机构ID
     */
    Q_INVOKABLE QString getOrganiseUnitIDs();

    /**
     * @brief  getOrganiseUnitCodes : 获取所属组织机构编码
     * @return QString, 所属组织机构编码
     */
    Q_INVOKABLE QString getOrganiseUnitCodes();


    /**
     * @brief  getOrganiseUnitIDs : 获取所属组织机构名称
     * @return QString, 所属组织机构名称
     */
    Q_INVOKABLE QString getOrganiseUnitNames();

    /**
     * @brief  getOrganiseUnitIDs : 获取所属部门
     * @return QString, 所属部门
     */
    Q_INVOKABLE QString getDepartmentIDs();

    /**
     * @brief  getOrganiseUnitIDs : 获取所属部门编号
     * @return QString, 所属部门编号
     */
    Q_INVOKABLE QString getDepartmentCodes();

    /**
     * @brief  getOrganiseUnitIDs : 获取所属部门名称
     * @return QString, 所属部门名称
     */
    Q_INVOKABLE QString getDepartmentNames();

    /**
     * @brief  getUserProperty : 获取用户属性
     * @param QString, 属性名
     * @return QString, 属性值
     */
    Q_INVOKABLE QString getUserProperty(const QString &key);

    /**
     * @brief  getLoginStatus : 获取登陆状态
     * @return QString, 登陆状态
     */
    Q_INVOKABLE bool getLoginStatus() const;

    /**
     * @brief  setLoginStatus : 设置登陆状态
     * @param  login : bool 登陆状态
     */
    Q_INVOKABLE void setLoginStatus(bool login);

    /**
     * @brief  setUserPermission : 设置用户权限
     * @param  module : QString & 模块
     * @param  value : bool 用户权限
     */
    Q_INVOKABLE void setUserPermission(const QString& module, bool value);

    /**
     * @brief  checkUserPermission : 查看用户权限
     * @param  module : QString & 模块
     * @return bool, true 有权限 false无 权限
     */
    Q_INVOKABLE bool checkUserPermission(const QString& module);

    /**
     * @brief  setModuleID : 设置模块id
     * @param  module : QString & 模块
     * @param  id : QString & id
     */
    Q_INVOKABLE void setModuleID(const QString& module, const QString& id);

    /**
     * @brief  getLoginStatus : 获取模块id
     * @param  module : QString & 模块
     * @return QString, 模块id
     */
    Q_INVOKABLE QString getModuleID(const QString& module);

    /**
     * @brief  setUserPermissionData : 设置模块数据
     * @param  module : QString & 模块
     * @param  dataMap : QVariantMap & dataMap
     */
    Q_INVOKABLE void setUserPermissionData(const QString& module, const QVariantMap &dataMap);

    /**
     * @brief  getUserPermissionData : 获取模块数据
     * @param  module : QString & 模块
     * @param  key : QString & 模块中的属性
     */
    Q_INVOKABLE QString getUserPermissionData(const QString& module, const QString &key);

    /**
     * @brief  clearUserPermission : 清除用户权限
     */
    Q_INVOKABLE void clearUserPermission();

    /**
     * @brief needCreateDB : 是否需要创建数据库
     * @return : true： 创建；false：不创建
     */
    bool needCreateDB();

    /**
      * @brief setHistoryUser:新增用户历史记录
      * @param user    : const QString& 用户名
      * @return  bool, true 成功, false 失败
      */
     Q_INVOKABLE bool setHistoryUser(const QStringList& userList);

    /**
      * @brief getHistoryUser:获取登陆用户的历史列表
      * @param list    : QStringList& 返回的结果
      * @return  bool, true 成功, false 失败
      */
     Q_INVOKABLE QStringList getHistoryUser();

    /**
     * @brief removeUserHistory:删除用户历史记录
     */
    Q_INVOKABLE void removeUserHistory();

    /**
     * @brief getAppId:获取 app id
     * @return  QString app id
     */
    Q_INVOKABLE QString getAppId();
    /**
     * @brief setOfflineModel : 设置离线模式
     * @param offlineModel : 离线模式:"０":纯在线;"1" : 离线;"2":纯离线
     */
    Q_INVOKABLE void setOfflineModel(const QString &offlineModel);
    /**
     * @brief getOfflineModel : 获取离线模式
     * @return : 离线模式
     */
    Q_INVOKABLE QString getOfflineModel();

private:

    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    RuntimeParam(QObject *parent = 0);

private:
    static RuntimeParam *m_pSysParam;///<  本地单例对象
    QVariantMap m_nUserMap;///<  user相关数据的map数组,app使用
    QMap<QString, QString> m_nLoginUserMsg; ///<当前登陆用户的信息,service使用
    QMap<QString, bool> m_nUserPermissions;///<  权限map数组
    QMap<QString, QString> m_nModuleID;///<  模块map数组
    QMap<QString, QVariantMap> m_nModuleData;///<  模块数据
    QString m_nImei;
    QQuickView *m_pViewer;///< Viewer
    bool m_nLogin;///<  登陆状态
    QString m_nOfflineModel;
};

#endif // RUNTIMEPARAM_H

