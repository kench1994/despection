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
#include <QVariantMap>
/*!
  \class RuntimeParam
  \inModule 运行时参数，包括登陆状态，登陆用户信息等
  \brief 运行时参数，包括登陆状态，登陆用户信息等
  \ingroup common
  RuntimeParam运行时参数，包括登陆状态，登陆用户信息等
*/

class RuntimeParam
{
public:
    /**
     * @brief   getInstance:获取单例对象
     * @return  RuntimeParam*, 非NULL 成功, NULL 失败
     */
    static RuntimeParam *getInstance();


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
     * @brief  clearUserPermission : 清除用户权限
     */
    Q_INVOKABLE void clearUserPermission();

    /**
     * @brief needCreateDB : 是否需要创建数据库
     * @return : true： 创建；false：不创建
     */
    bool needCreateDB();

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
     * @param parent : 父对象
     */
    RuntimeParam();

private:
    static RuntimeParam *m_pSysParam;///<  本地单例对象
    QVariantMap m_nUserMap;///<  user相关数据的map数组,app使用
    QMap<QString, QString> m_nLoginUserMsg; ///<当前登陆用户的信息,service使用
    QMap<QString, bool> m_nUserPermissions;///<  权限map数组
    QMap<QString, QString> m_nModuleID;///<  模块map数组
    QMap<QString, QVariantMap> m_nModuleData;///<  模块数据
    QString m_nImei;
    bool m_nLogin;///<  登陆状态
    QString m_nOfflineModel;
};

#endif // RUNTIMEPARAM_H

