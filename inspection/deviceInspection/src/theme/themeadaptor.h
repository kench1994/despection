/*!
* \file themeadaptor.h
* \brief 皮肤代理类的头文件
*
*此类提供了皮肤模块对外的接口以及封装
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef THEMEADAPTOR_H
#define THEMEADAPTOR_H

#include <QObject>
#include "themenode.h"
#include "theme.h"

/*!
  \class ThemeAdaptor
  \inModule 皮肤模块
  \brief 皮肤模块对QML的代理类
  \ingroup theme
  ThemeAdaptor类提供了对QML的接口
*/
class ThemeAdaptor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    explicit ThemeAdaptor(QObject *parent = 0);

    /**
     * @brief 析构函数
     *
     */
    ~ThemeAdaptor();

    /**
     * @brief 重新载入当前的皮肤
     *
     */
    Q_INVOKABLE virtual void reflesh(const QString &theme);

    /**
     * @brief 获取皮肤路径
     *
     * @return QString : 皮肤路径
     */
    Q_INVOKABLE QString getThemePath();

    /**
     * @brief 获取皮肤资源路径
     *
     * @return QString : 皮肤资源路径
     */
    Q_INVOKABLE QString getThemeResPath();

    /**
     * @brief 获取皮肤ID
     *
     * @return QString 皮肤ID
     */
    Q_INVOKABLE QString getID();

    /**
     * @brief 获取皮肤中文名称
     *
     * @return QString 皮肤中文名称
     */
    Q_INVOKABLE QString getNameCN();

    /**
     * @brief 获取皮肤英文名称
     *
     * @return QString 皮肤英文名称
     */
    Q_INVOKABLE QString getNameEN();

    /**
     * @brief 获取皮肤效果图
     *
     * @return QString 皮肤效果图
     */
    Q_INVOKABLE QString getImagePath();

    /**
     * @brief 获取皮肤风格
     *
     * @return QString 皮肤风格
     */
    Q_INVOKABLE QString getStyle();

    /**
     * @brief 获取图标节点总数
     *
     * @return int : 图标节点总数
     */
    Q_INVOKABLE int getIconCount();

    /**
     * @brief 获取静态节点总数
     *
     * @return int : 静态节点总数
     */
    Q_INVOKABLE int getStaticCount();


    /**
     * @brief 根据ID获取节点参数
     *
     * @param id : 节点ID
     * @param parentId : 父节点ID,若为第一级节点则是空
     * @return QMap<QString, QVariant> : 节点参数
     */
    Q_INVOKABLE QMap<QString, QVariant> getNodeParas(const QString& id, const QString& parentId);

    /**
     * @brief 根据Index获取节点参数
     *
     * @param index : 索引
     * @param parentId : 父节点ID,若为第一级节点则是空
     * @return QMap<QString, QVariant> : 节点参数
     */
    Q_INVOKABLE QMap<QString, QVariant> getNodeParas(const int& index, const QString& parentId);

    /**
     * @brief 获取静态节点参数
     *
     * @param id : 节点ID
     * @return QMap<QString, QVariant> : 节点参数
     */
    Q_INVOKABLE QMap<QString, QVariant> getStaticParas(const QString& id);

protected:
    Theme m_nTheme; ///< 皮肤对象
    QList<ThemeNode> m_nStaticNodes; ///< 静态节点列表
    QList<ThemeNode> m_nIconNodes;   ///< 图标节点列表

signals:

public slots:
};

#endif // THEMEADAPTOR_H
