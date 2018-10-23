#ifndef DBCONFIGBASE_H
#define DBCONFIGBASE_H
#include "../common/tableconfig.h"
#include "../common/dataoperationtype.h"
/**
 * @brief The DBConfigBase class : 业务模块数据库表操作类
 */
class DBConfigBase
{
public:
    DBConfigBase();
    ~DBConfigBase();
    /**
     * @brief getTables : 获取业务所有的表结构体
     * @return : 表结构体列表
     */
    virtual QList<TableConfig> getTables();
    /**
     * @brief getTable : 根据表名称获取表结构体
     * @param name : 表名称
     * @return : 表结构体
     */
    virtual TableConfig getTable(const QString &name);
    /**
     * @brief getDataOperationType : 获取业务类型
     * @return : 业务类型
     */
    DataOperationType getDataOperationType();
    /**
     * @brief parseXml2TableConfig : 从xml文件中解析出表结构体
     * @param xmlFile : xml文件
     * @return  : 表结构体列表
     */
    QList<TableConfig> parseXml2TableConfig(const QString &xmlFile);
protected:
    DataOperationType m_nDataOperationType;  ///<业务模块类型
    QList<TableConfig> m_nTableConfigList; ///< 业务模块包含的表列表
};

#endif // DBCONFIGBASE_H
