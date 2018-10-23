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
     * @brief  setLoginUserInfo : 设置user相关数据的map数组,同时设置登陆状态为true
     * @param  userMap : QVariantMap &: user相关数据的map数组
     */
    Q_INVOKABLE void setLoginUserInfo(const QVariantMap &userMap);

    /**
     * @brief  clearLoginUserInfo : 清除登陆用户信息,同时设置登陆状态为false
     */
    Q_INVOKABLE void clearLoginUserInfo();

    /**
     * @brief  getUserMap : 获取user相关数据的map数组
     * @return QVariantMap, user相关数据的map数组
     */
    Q_INVOKABLE QVariantMap getUserMap();

    /**
     * @brief  getUserId : 获取用户Id
     * @return QString, 用户Id
     */
    Q_INVOKABLE QString getUserId();

    /**
     * @brief  getOrganiseUnitIDs : 获取所属组织机构ID
     * @return QString, 所属组织机构ID
     */
    Q_INVOKABLE QString getOrganiseUnitIDs();

    /**
     * @brief  getUserPropertyByKey : 获取用户属性
     * @param QString, 属性名
     */
    Q_INVOKABLE QString getUserPropertyByKey(const QString &key);

    /**
     * @brief  getLoginStatus : 获取登陆状态
     * @return QString, 登陆状态
     */
    Q_INVOKABLE bool getLoginStatus() const;

    /**
     * @brief  checkUserPermission : 查看用户权限
     * @param  module : QString & 模块
     * @return bool, true 有权限 false无 权限
     */
    Q_INVOKABLE bool checkUserPermission(const QString& module);

    /**
     * @brief  getUserPermissionData : 获取模块数据
     * @param  module : QString & 模块
     * @param  key : QString & 模块中的属性
     */
    Q_INVOKABLE QString getUserPermissionData(const QString& module, const QString &key);

private:

    /**
     * @brief 构造函数
     * @param parent : 父对象
     */
    RuntimeParam(QObject *parent = 0);

private:
    static RuntimeParam *m_pSysParam;///<  本地单例对象
    QVariantMap m_nUserMap;///<  user相关数据的map数组,app使用
    QString m_nImei;
    bool m_nLogin;///<  登陆状态
    QQuickView *m_pViewer;///< Viewer
};

#endif // RUNTIMEPARAM_H

