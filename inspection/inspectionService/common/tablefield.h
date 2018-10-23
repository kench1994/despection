/*!
* \file tablefield.h
* \brief 数据库表字段解析类
*
*数据库表字段解析类
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2017/2/15
*/
#ifndef TABLEFIELD_H
#define TABLEFIELD_H
#include <QString>
/*!
  \class TableField
  \inModule 数据库表字段解析类
  \brief 将xml文件中对应的字段属性解析成该类
  \ingroup common
*/
class TableField
{
public:
    TableField();
    ~TableField();
    /**
     * @brief getName: 获取表中字段的名称
     * @return :字段的名称
     */
    QString getName() const;
    /**
     * @brief setName:设置表中字段的名称
     * @param name:字段的名称
     */
    void setName(const QString &name);
    /**
     * @brief getType:获取表中字段的类型
     * @return : 字段的类型
     */
    QString getType() const;
    /**
     * @brief setType:设置表中字段的类型
     * @param type:字段的类型
     */
    void setType(const QString &type);
    /**
     * @brief getLength:获取表中字段的长度
     * @return :字段的长度
     */
    int getLength()const;
    /**
     * @brief setLength:设置表中字段的长度
     * @param length:字段的长度
     */
    void setLength(const int &length);
    /**
     * @brief getIsKey:获取表中字段是否是主键
     * @return : 字段是否是主键
     */
    bool getIsKey() const;
    /**
     * @brief setIsKey:设置表中字段是否是主键
     * @param isKey:字段是否是主键
     */
    void setIsKey(const bool &isKey);
    /**
     * @brief getIsEnableNull:获取表中字段是否允许为空
     * @return : 字段是否允许为空
     */
    bool getIsEnableNull() const;
    /**
     * @brief setEnableNull : 设置表中字段是否允许为空
     * @param enableNull : 字段是否允许为空
     */
    void setEnableNull(const bool &enableNull);
private:

    QString m_nName; ///< 字段的名称
    QString m_nType;   ///< 字段的类型
    int m_nLength;    ///< 字段的长度
    bool m_nIsKey;  ///< 字段是否是主键
    bool m_nEnableNull; ///< 字段是否允许为空
};

#endif // TABLEFIELD_H
