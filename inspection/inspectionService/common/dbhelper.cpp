#include "dbhelper.h"
#include <QSqlRecord>
#include <QDebug>
#include "../../framework/common/utils.h"
DBHelper * DBHelper::m_pDBHelper = NULL;
DBHelper::DBHelper()
{
    m_nDB = QSqlDatabase::addDatabase("QSQLITE");
    m_nDB.setDatabaseName(DATABASE_PATH);
    if(!m_nDB.open()){
        qDebug() << "failed to open database" << DATABASE_PATH;
    }
}

DBHelper::~DBHelper()
{
    m_nDB.close();
}

DBHelper *DBHelper::getInstance(){
    if(m_pDBHelper == NULL)
        m_pDBHelper = new DBHelper();
    return m_pDBHelper;
}

bool DBHelper::execSQL(const QString &sql){
    QSqlQuery query;

    bool rv = query.exec(sql);
    if(!rv)
        qDebug() << "DBHelper::execSQL fail sqlStr is  " << sql;
    return rv;
}
bool DBHelper::select(const QString &sqlStr,  QList<QMap<QString, QString> > &valueList){
    QSqlQuery query;
    query.prepare(sqlStr);

    if(!query.exec()){
        qDebug() << "DBHelper::select1  false   sqlStr is  " << sqlStr;
        return false;
    }
    while(query.next()){
        QMap<QString, QString> value;
        QSqlRecord rec = query.record();
        for(int i = 0; i < rec.count(); i ++){
            value.insert(rec.fieldName(i), rec.value(i).toString());
        }
        valueList.append(value);
    }
    return true;
}

QList<QMap<QString, QString> > DBHelper::select(const QString &tableName, const QStringList &column, const QString &selectionArg){
    int i = 0;
    QSqlQuery query;
    QList<QMap<QString, QString> > valueList;
    QString sqlStr = "select ";
    if(column.size() == 0)
        sqlStr += " * ";
    else {
        for(i = 0; i < column.size(); i++){
            sqlStr += column.at(i);
            if(i != column.size() - 1)
                sqlStr += ",";
        }
    }
    sqlStr += " from " + tableName;
    if(!selectionArg.isEmpty())
        sqlStr = sqlStr + selectionArg;
    query.prepare(sqlStr);


    if(!query.exec()){
        qDebug() << "DBHelper::select fail   sqlStr is  " << sqlStr;
        return valueList;
    }
    while(query.next()){
        QMap<QString, QString> value;
        QSqlRecord rec = query.record();
        for(int i = 0; i < rec.count(); i ++){
            value.insert(rec.fieldName(i), rec.value(i).toString());
        }
        valueList.append(value);
    }
    return valueList;
}

bool DBHelper::insert(const QString &sqlStr, const QStringList &argList){
    QSqlQuery query;
    query.prepare(sqlStr);
    for(int i = 0; i < argList.count(); i++){
        query.addBindValue(argList.at(i));
    }
    bool rv =  query.exec();
    if(!rv)
        qDebug() << "DBHelper::insert1 fail   sqlStr is  " << sqlStr;
    return rv;
}

bool DBHelper::insert(const QString &tableName, const QMap<QString, QString> &fields)
{
    QSqlQuery query;
    QString sqlStr = "insert into " + tableName + " (";
    QString values = " values(";
    QMap<QString, QString>::const_iterator iter = fields.constBegin();
    while (iter != fields.constEnd()) {
        sqlStr = sqlStr + iter.key();
        values = values + ":" + iter.key();
        if(iter != fields.constEnd() - 1){
            sqlStr += ",";
            values += ",";
        }
        ++iter;
    }
    sqlStr = sqlStr + ")";
    values = values + ")";
    sqlStr = sqlStr + values;
    query.prepare(sqlStr);

    iter = fields.constBegin();
    while (iter != fields.constEnd()) {
        query.bindValue(":" + iter.key(), iter.value());
        //qDebug() << "bindValue is " << ":" + iter.key() << "     " << iter.value();
        ++iter;
    }

    bool execRv =  query.exec();
    if(!execRv)
        qDebug() << "DBHelper::insert fail sqlStr is  " << sqlStr;
    return execRv;
}

bool DBHelper::update(const QString &tableName, const QMap<QString, QString> &keyValue, const QString &updateArg)
{
    //UPDATE COMPANY SET ADDRESS = 'Texas', SALARY = 20000.00;
    QSqlQuery query;
    QString sqlStr = "update " + tableName + " set ";
    QMap<QString, QString>::const_iterator iter = keyValue.constBegin();
    while (iter != keyValue.constEnd()) {
        sqlStr = sqlStr + iter.key() + " = '" + iter.value() + "'";
        if(iter != keyValue.constEnd() - 1){
            sqlStr += ",";
        }
        ++iter;
    }
    if(!updateArg.isEmpty())
        sqlStr += updateArg;
    query.prepare(sqlStr);
    bool rv = query.exec();
    if(!rv)
        qDebug() << "DBHelper::update fail sqlStr is " << sqlStr;
    return rv;
}

bool DBHelper::deleteRow(const QString &tableName, const QString &deleteArg)
{
    QString sqlStr = "delete from " + tableName;
    if(!deleteArg.isEmpty())
        sqlStr = sqlStr + deleteArg;
    QSqlQuery query;
    query.prepare(sqlStr);
    bool rv =  query.exec();
    if(!rv)
        qDebug() << "DBHelper::deleteRow fail    sqlStr " << sqlStr ;
    return rv;
}

bool DBHelper::beginTransaction()
{
    return m_nDB.transaction();
}

bool DBHelper::endTransaction(bool success)
{
    if(success)
        return m_nDB.commit();
    else
        return m_nDB.rollback();
}

bool DBHelper::checkTableExist(const QString &tableName){
    QSqlQuery query;
    query.exec("SELECT count(*) FROM sqlite_master WHERE type='table' and name = '" + tableName+ " ';");
    int i = 0;
    while(query.next()){
        qDebug() << tableName << " count is "  <<  query.value(0);
        i = query.value(0).toInt();
    }
    if(i > 0)
        return true;
    else
        return false;
}

bool DBHelper::checkHasTableExist(){
    QSqlQuery query;
    query.exec("SELECT count(*) FROM sqlite_master WHERE type='table';");
    int i = 0;
    while(query.next()){
        qDebug() << "the table in db ,count is   " << query.value(0);
        i = query.value(0).toInt();
    }
    if(i > 0)
        return true;
    else
        return false;
}
