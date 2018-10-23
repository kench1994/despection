#ifndef TABLECONFIG_H
#define TABLECONFIG_H
#include <QList>
#include "tablefield.h"
/**
 * @brief The TableConfig class : 数据库表配置类
 */
class TableConfig
{
public:
    TableConfig();
    ~TableConfig();
    /**
     * @brief setName : 设置表名
     * @param name : 表名
     */
    void setName(const QString &name);
    /**
     * @brief getName : 获取表名
     * @return : 表名
     */
    QString getName();
    /**
     * @brief setDescription : 设置表的描述
     * @param description : 表的描述
     */
    void setDescription(const QString &description);
    /**
     * @brief getDescription : 获取表的描述
     * @return  : 表的描述
     */
    QString getDescription();
    /**
     * @brief addField : 添加表中的属性字段类
     * @param field : 表中的属性字段类
     */
    void addField(const TableField &field);
    /**
     * @brief getFieldList : 获取表中的属性字段列表
     * @return : 表中的属性字段列表
     */
    QList<TableField> getFieldList();
private:
    QString m_nName;  ///< 表名
    QString m_nDescription;   ///< 表的描述
    QList<TableField> m_nFieldList; ///< 属性字段列表
};

#endif // TABLECONFIG_H
