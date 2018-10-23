/*!
* \file themenode.h
* \brief  皮肤模块节点类头文件
*
*此类提供对于皮肤子节点的封装
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef THEMENODE_H
#define THEMENODE_H

#include <QList>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QObject>

/*!
  \class ThemeNode
  \inModule 皮肤模块
  \brief 皮肤模块节点类
  \ingroup theme
  ThemeNode类提供对于皮肤子节点的封装
*/
class ThemeNode
{
public:

    /**
     * @brief 构造函数
     *
     */
    ThemeNode();

    /**
     * @brief 析构函数
     *
     */
    virtual ~ThemeNode();

    /**
     * @brief 获取节点ID
     *
     * @return QString 节点ID
     */
    QString getID();

    /**
     * @brief 获取节点类型
     *
     * @return QString 节点类型
     */
    QString getType();

    /**
     * @brief 获取节点参数
     *
     * @return QMap<QString, QVariant> : 节点参数
     */
    QMap<QString, QVariant> getPara();

    /**
     * @brief 获取静态节点总数
     *
     * @return int : 静态节点总数
     */
    int getStaticNodeCount();

    /**
     * @brief 获取图标节点总数
     *
     * @return int : 图标节点总数
     */
    int getIconNodeCount();

    /**
     * @brief 获取静态节点列表
     *
     * @return QList<ThemeNode> : 静态节点列表
     */
    QList<ThemeNode> getStaticNodes();

    /**
     * @brief 获取图标节点列表
     *
     * @return QList<ThemeNode> : 图标节点列表
     */
    QList<ThemeNode> getIconNodes();

    /**
     * @brief 设置节点ID
     *
     * @param value : 节点ID
     */
    void setID(const QString& value);

    /**
     * @brief 设置节点类型
     *
     * @param value : 节点类型
     */
    void setType(const QString& value);

    /**
     * @brief 添加子静态节点
     *
     * @param node : 子静态节点
     */
    void addStaticNode(ThemeNode node);

    /**
     * @brief 添加子图标节点
     *
     * @param node : 子图标节点
     */
    void addIconNode(ThemeNode node);

    /**
     * @brief 设置节点参数
     *
     * @param value : QMap<QString, QVariant> 节点参数
     */
    void setPara(const QMap<QString, QVariant> value);

    /**
     * @brief 设置节点索引
     *
     * @param value : 索引
     */
    void setIndex(const int& value);

    /**
     * @brief 获取节点索引
     *
     * @return int : 索引
     */
    int getIndex() const;

    /**
     * @brief 对节点进行排序
     * 以节点的Index值做升序排序组织Node节点
     */
    void orderNode();

private:
    QString m_nID;    ///< 节点ID
    QString m_nType;    ///< 节点类型
    QMap<QString, QVariant> m_nPara;    ///< 节点参数
    QList<ThemeNode> m_nIconNodeList;    ///< 静态节点列表
    QList<ThemeNode> m_nStaticNodeList;    ///< 图标节点列表
    int m_nIndex;    ///< 节点索引
};

#endif // THEMENODE_H
