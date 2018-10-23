/*!
* \file upgradeconfig.h
* \brief  读取升级配置文件
*
*此类是读取升级配置文件操作的封装
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/10
*/
#ifndef UPGRADECONFIG_H
#define UPGRADECONFIG_H

#include <QObject>
#include <QVariant>
class QSettings;

/*!
  \class UpgradeConfig
  \inModule 升级模块
  \brief   读取升级配置文件
  \ingroup upgrade
  UpgradeConfig类是对读取升级配置文件的操作的封装
*/

class UpgradeConfig: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief   getAPPName:获取软件名称
     * @return  “”--失败; 否则，成功;
     */
    Q_INVOKABLE QString getAPPName();
    /**
     * @brief   getVersion:获取软件版本
     * @return  “”--失败; 否则，成功;
     */
    Q_INVOKABLE QString getVersion();
    /**
     * @brief   getChangeLogFullPath:获取更新说明文件路径
     * @return  “”--失败; 否则，成功;
     */
    Q_INVOKABLE QString getChangeLogFullPath();
    /**
     * @brief   getMd5Code:获取压缩包MD5值
     * @return  “”--失败; 否则，成功;
     */
    Q_INVOKABLE QString getMd5Code();
    /**
     * @brief   getPackageType:获取压缩包类型
     * @return  “”--失败; 否则，成功;
     */
    Q_INVOKABLE QString getPackageType();
    /**
     * @brief   getPriorVersion:获取兼容的最低版本
     * @return  “”--失败; 否则，成功;
     */
    Q_INVOKABLE QString getPriorVersion();
    /**
     * @brief   getPriorVersion:是否清理App数据
     * @return  “”--失败; 否则，成功;
     */
    Q_INVOKABLE QString isClearAppData();
    /**
     * @brief   getInstance:获取单例对象
     * @param configFullPath: QString 配置文件的完整路径
     * @return  UpgradeConfig*, 非NULL 成功, NULL 失败
     */
    static UpgradeConfig *getInstance(const QString & configFullPath);

private:
    /**
     * @brief 构造函数
     * @param configFullPath: QString 配置文件的完整路径
     * @param parent : 父对象
     */
    UpgradeConfig(const QString &configFullPath);
private:
    QSettings * m_pSetting; ///< 存取升级配置的对象
    static UpgradeConfig *m_pUpgradeConf; ///<  本地单例对象
};

#endif // UPGRADECONFIG_H
