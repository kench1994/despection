#ifndef DBCONFIGMANAGER_H
#define DBCONFIGMANAGER_H
#include "../common/tableconfig.h"
#include "../common/dataoperationtype.h"
class DBConfigManager
{
public:
    DBConfigManager();
    ~DBConfigManager();
    void initDBTables();
    void clearDBTables();
    QList<TableConfig> getTables(const DataOperationType &type);
    TableConfig getTable(const DataOperationType &type, const QString &name);
    void createTable(const QString  &tableName, QList<TableField> tableFieldList);
    void getTableFieldNameList(const DataOperationType &type, const QString &tableName, QStringList &fieldNameList);
};

#endif // DBCONFIGMANAGER_H
