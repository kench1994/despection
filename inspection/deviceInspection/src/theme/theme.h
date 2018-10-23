/*!
* \file theme.h
* \brief 皮肤类头文件
*
*此类提供了单个皮肤的封装
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef THEME_H
#define THEME_H

#include "themenode.h"
#include <QList>
#include <QString>
#include <QDomElement>
#include <QObject>

/*!
  \class Theme
  \inModule 皮肤模块
  \brief 单个皮肤对象的封装
  \ingroup theme
  Theme类为封装了单个皮肤的解码以及相关接口
*/
class Theme
{
public:
    /**
     * @brief 构造函数
     *
     */
    Theme();

    /**
     * @brief 析构函数
     *
     */
    virtual ~Theme();

    /**
     * @brief 获取皮肤ID
     *
     * @return QString 皮肤ID
     */
    QString getID();


    /**
     * @brief 获取皮肤英文名称
     *
     * @return QString 皮肤英文名称
     */
    QString getNameEN();

    /**
     * @brief 获取皮肤中文名称
     *
     * @return QString 皮肤中文名称
     */
    QString getNameCN();

    /**
     * @brief 获取皮肤风格
     *
     * @return QString 皮肤风格
     */
    QString getStyle();

    /**
     * @brief 获取皮肤效果图
     *
     * @return QString 皮肤效果图
     */
    QString getImage();

    /**
     * @brief 获取皮肤字节点
     *
     * @return QString 皮肤字节点
     */
    QList<ThemeNode> getNodes();

    /**
     * @brief 设置皮肤ID
     *
     * @param value : 皮肤ID
     */
    void setID(const QString& value);

    /**
     * @brief 设置皮肤英文名称
     *
     * @param value : 皮肤英文名称
     */
    void setNameEN(const QString& value);

    /**
     * @brief 设置皮肤中文名称
     *
     * @param value : 皮肤中文名称
     */
    void setNameCN(const QString& value);

    /**
     * @brief 设置皮肤风格
     *
     * @param value : 皮肤风格
     */
    void setStyle(const QString& value);

    /**
     * @brief 设置皮肤效果图
     *
     * @param value : 皮肤效果图
     */
    void setImage(const QString& value);


    /**
     * @brief 解析皮肤
     *
     * @return bool : true 成功, false 失败
     */
    bool parse();


    /**
     * @brief 解析节点
     *
     * @param node : 节点
     * @param out : 返回值
     * @return bool : true 成功, false 失败
     */
    bool parseNode(QDomElement node, ThemeNode& out);


    /**
     * @brief 获取皮肤资源路径
     *
     * @return QString : 皮肤资源路径
     */
    QString getThemeResPath();


    /**
     * @brief 获取皮肤路径
     *
     * @return QString : 皮肤路径
     */
    QString getThemePath();


    /**
     * @brief 设置皮肤路径
     *
     * @param value : 皮肤路径
     */
    void setThemePath(const QString& value);


    /**
     * @brief 对节点进行排序
     * 以节点的Index值做升序排序组织Node节点
     */
    void orderNode();

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

private:

    /**
     * @brief 新增静态节点
     *
     * @param node : 静态节点
     */
    void addStaticNode(ThemeNode node);

    /**
     * @brief 新增图标节点
     *
     * @param node : 图标节点
     */
    void addIconNode(ThemeNode node);

    /**
     * @brief 初始化函数
     *
     */
    void initialize();

private:
    QString m_nThemePath;   ///< 皮肤路径
    QString m_nID;    ///< 皮肤ID
    QString m_nNameCN;    ///< 中文名称
    QString m_nNameEN;    ///< 英文名称
    QString m_nImage;    ///< 皮肤效果图
    QString m_nStyle;    ///< 皮肤风格
    QList<ThemeNode> m_nIconNodeList;    ///< 图标节点列表
    QList<ThemeNode> m_nStaticNodeList;    ///< 静态节点列表
};

#endif // THEME_H
