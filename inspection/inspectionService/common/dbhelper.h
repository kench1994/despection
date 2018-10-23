#ifndef DBHELPER_H
#define DBHELPER_H
#include <QList>
#include <QStringList>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
/**
 * @brief The DBHelper class : 数据库操作类
 */
class DBHelper
{
public:
    ~DBHelper();
    /**
     * @brief getInstance : 获取单例指针
     * @return : 单例指针
     */
    static DBHelper *getInstance();

    /**
     * @brief execSQL: 执行sql语句
     * @param sql: sql语句
     * @return : 是否成功
     */
    bool execSQL(const QString &sql);

    /**
     * @brief select : 数据库查询
     * @param sqlStr : 查询语句
     * @param valueList : 查询结果
     * @return : true:成功; false:失败
     */
    bool select(const QString &sqlStr,  QList<QMap<QString, QString> > &valueList);
    /**
     * @brief select:数据库查询
     * @param tableName: 表名
     * @param column: 列, 返回该表全部列
     * @param selectionArg: 查询条件
     * @return : 查询结果
     */
    QList<QMap<QString, QString> > select(const QString &tableName, const QStringList &column, const QString &selectionArg);
    /**
     * @brief insert : 数据库插入
     * @param sqlStr : 插入语句
     * @param argList : 插入值
     * @return : 是否成功
     */
    bool insert(const QString &sqlStr, const QStringList &argList);
    /**
     * @brief insert : 数据库插入
     * @param tableName : 表名
     * @param values : 字段值
     * @return : 是否成功
     */
    bool insert(const QString &tableName, const QMap<QString, QString> &fields);

    /**
     * @brief update : 更新数据库
     * @param tableName : 表名
     * @param keyValue : 字段值
     * @return : 是否成功
     */
    bool update(const QString &tableName, const QMap<QString, QString> &keyValue, const QString &updateArg);

    /**
     * @brief deleteFromTable : 删除行
     * @param tableName : 表名
     * @param selectionArg : 条件
     * @return : 是否成功
     */
    bool deleteRow(const QString &tableName, const QString &deleteArg);

    /**
     * @brief beginTransaction : 开始事务
     * @return : 是否成功
     */
    bool beginTransaction();
    /**
     * @brief endTransaction : 结束事务
     * @return : 是否成功
     */
    bool endTransaction(bool success = true);

    /**
     * @brief checkTableExist : 检查表是否存在
     * @param tableName : 表名
     * @return  : 是否存在
     */
    bool checkTableExist(const QString &tableName);
    /**
     * @brief checkHasTableExist : 检查数据库中是否有表存在
     * @return : 是否存在
     */
    bool checkHasTableExist();
private:
    DBHelper();
private:
    static DBHelper *m_pDBHelper; ///< 单例指针
    QSqlDatabase m_nDB; ///< 数据库对象
};

#endif // DBHELPER_H
