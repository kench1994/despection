/*!
* \file thememanager.h
* \brief 皮肤系统管理类头文件
*
*此类提供了皮肤模块的解析等从调度
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QList>
#include "theme.h"

/*!
  \class ThemeManager
  \inModule 皮肤模块
  \brief 皮肤系统管理类
  \ingroup theme
  ThemeManager类提供对于皮肤系统的从解析和切换等调度功能
*/
class ThemeManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 析构函数
     *
     */
    virtual ~ThemeManager();


    /**
     * @brief 获取皮肤列表
     *
     * @return const QList<Theme>
     */
    const QList<Theme>& getThemeList();

    /**
     * @brief 获取皮肤
     *
     * @param id : 皮肤ID
     * @param ret : 皮肤对象
     * @return bool : true 成功, false 失败
     */
    bool getTheme(const QString& id, Theme& ret);

    /**
     * @brief 获取当前皮肤
     *
     * @param ret : 皮肤对象
     * @return bool : true 成功, false 失败
     */
    bool getCurrentTheme(Theme& ret);


    /**
     * @brief 设置当前皮肤
     *
     * @param id : 皮肤ID
     * @return bool : true 成功, false 失败
     */
    bool setCurrentTheme(const QString& id);

    /**
     * @brief getInstance:获取皮肤管理对象
     * @param parent : QObject* 改对象的父对象
     * @return  ThemeManager*: 非空 获取成功, NULL 获取失败
     */
    static ThemeManager* getInstance(QObject *parent = 0);

    /**
     * @brief 获取当前皮肤ID
     *
     * @return QString : 当前皮肤ID
     */
    Q_INVOKABLE QString getCurrentThemeID();

    /**
     * @brief 获取当前皮肤风格
     *
     * @return QString : 当前皮肤风格
     */
    Q_INVOKABLE QString getCurrentThemeStyle();

private:
    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    explicit ThemeManager(QObject *parent = 0);

    /**
     * @brief 初始化函数
     *
     */
    void initialize(const QString &theme = "");

    /**
     * @brief 加载皮肤
     *
     * @param path : 皮肤路径
     */
    void loadTheme(const QString& path);

    /**
     * @brief 获取皮肤总路径
     *
     * @return QString : 皮肤总路径
     */
    QString getThemesDir();
    /**
     * @brief 获取/data/data/com.syberos.deviceInspection/systemConfig.ini文件中皮肤名称信息
     *
     * @return QString : 皮肤名称
     */
    QString getThemesConfig();

private:
    QList<Theme> m_nThemeList;    ///< 皮肤列表
    QString m_nCurrentThemeID;    ///< 当前的皮肤ID
    static ThemeManager* m_pInstance;    ///< 对应单例模型的对象

signals:
    /**
     * @brief currentThemeChanged: 当前的皮肤变更的信号
     * @param id    : QString  当前的皮肤ID
     * @param style    : QString  当前皮肤风格
     */
    void currentThemeChanged(const QString id, const QString style);

public slots:
};

#endif // THEMEMANAGER_H
