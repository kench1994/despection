#include "dbconfigmanager.h"
#include "dbconfigbase.h"
#include "dbconfigfactory.h"
#include "../common/dbhelper.h"
#include <QMap>
#include <QList>
#include <QDebug>
DBConfigManager::DBConfigManager()
{
}

DBConfigManager::~DBConfigManager()
{
}

QList<TableConfig> DBConfigManager::getTables(const DataOperationType &type){
    DBconfigFactory dbConfigFactory;
    DBConfigBase *dbBase = dbConfigFactory.getDBConfig(type);
    return dbBase->getTables();
}

TableConfig DBConfigManager::getTable(const DataOperationType &type, const QString &name){
    DBconfigFactory dbConfigFactory;
    DBConfigBase *dbBase = dbConfigFactory.getDBConfig(type);
    return dbBase->getTable(name);
}

void DBConfigManager::initDBTables(){
    DBconfigFactory dbConfigFactory;
    QList<DBConfigBase *> dbConfigBaseList = dbConfigFactory.getAllDBConfig();
    qDebug() << "baselist count is " << dbConfigBaseList.size();
    for(int i = 0 ; i < dbConfigBaseList.size(); i++){
        DBConfigBase *tmpBase = dbConfigBaseList.at(i);
        qDebug() << "tmpBase type is " << tmpBase->getDataOperationType();
        QList<TableConfig> tableList = tmpBase->getTables();
        for(int j = 0; j < tableList.size(); j++){
            TableConfig tmpTable = tableList.at(j);
            qDebug() << "tmpTable name is " << tmpTable.getName();
            QList<TableField> fieldList = tmpTable.getFieldList();
            QString tableName = tmpTable.getName();
            qDebug() << "tableName is " << tableName;
            createTable(tableName, fieldList);
        }
    }
}

void DBConfigManager::clearDBTables()
{
    qDebug() << Q_FUNC_INFO;
    QStringList column;
    column.append("name");
    DBHelper *dbHelper = DBHelper::getInstance();
    dbHelper->beginTransaction();
    QList< QMap<QString, QString> > dataRows = dbHelper->select("sqlite_master", column, " where type='table'");
    for(int i = 0 ; i < dataRows.size(); i++){
        const QMap<QString, QString> &dataRow = dataRows[i];
        dbHelper->deleteRow(dataRow["name"], "");
    }
    dbHelper->endTransaction();
}

void DBConfigManager::createTable(const QString  &tableName, QList<TableField> tableFieldList){
    QString sqlStr = "create table " + tableName + "(";
            for(int i = 0; i < tableFieldList.size(); i++){
                TableField tmpField = tableFieldList.at(i);
                sqlStr += tmpField.getName() + " " + tmpField.getType();
                if(tmpField.getIsKey()) {
                    sqlStr += " primary key";
                }
                if(!tmpField.getIsEnableNull()) {
                    sqlStr += " not null";
                }
                if(i != tableFieldList.size() -1){
                    sqlStr += ",";
                }
            }
            sqlStr += ")";
            qDebug() << "----------------createTable---sqlStr:" << sqlStr;
            DBHelper *dbHelper = DBHelper::getInstance();
            dbHelper->execSQL(sqlStr);
}

void DBConfigManager::getTableFieldNameList(const DataOperationType &type, const QString &tableName, QStringList &fieldNameList)
{
    TableConfig tableConfig = getTable(type, tableName);
    QList<TableField> fieldList = tableConfig.getFieldList();
    for(int i = 0; i < fieldList.count(); i++){
        fieldNameList.append(fieldList.at(i).getName());
    }
}
