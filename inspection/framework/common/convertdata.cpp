#include "convertdata.h"
#include <QDebug>
ConvertData* ConvertData::m_pInstance = NULL;

ConvertData *ConvertData::getInstance(){
    if(m_pInstance == NULL)
        m_pInstance = new ConvertData();
    return m_pInstance;
}

ConvertData::ConvertData(QObject *parent) :
    QObject(parent)
{
}

QVariantMap ConvertData::convert(DataPackage &package){
    QVariantMap map;
    convert(package, map);
    return map;
}

void ConvertData::convert(const DataPackage &package, QVariantMap &map){
    QVariantMap headmap;
    headmap.insert("key", package.head.key);
    headmap.insert("type", package.head.type);
    headmap.insert("objects", package.head.objects);
    headmap.insert("time", package.head.date);
    map.insert("head", headmap);
    QVariantMap paramap;
    QMap<QString, QString>::const_iterator it = package.para.constBegin();
    for(; it != package.para.constEnd(); it++){
        paramap.insert(it.key(), it.value());
    }
    map.insert("para", paramap);
    map.insert("pageTotalCount", package.pagerTotalCount);
    QList<QVariant> tablesmap;
    // don't use foreach, because  DataTable not define ==
    for(int i = 0; i < package.tables.size(); i++){
        DataTable table = package.tables.at(i);
        QVariantMap tabmap;
        tabmap.insert("name", table.name);
        tabmap.insert("key", table.key);
        QList<QVariant> valuemap;
        foreach (DataRow row, table.value) {
            QVariantMap rowmap;
            DataRow::const_iterator it = row.constBegin();
            for(; it != row.constEnd(); it++){
                rowmap.insert(it.key(), it.value());
            }
            valuemap.append(QVariant(rowmap));
        }
        tabmap.insert("value", QVariant(valuemap));
        tablesmap.append(QVariant(tabmap));
    }
    map.insert("tables", QVariant(tablesmap));

    QList <QVariant> attachmentMap;
    for(int i = 0; i<package.attachments.size(); i++)
    {
        Attachment attach = package.attachments.at(i);
        QVariantMap attachMap;
        //qDebug() << "name:" << attach.name;
        attachMap.insert("name", attach.name);
        attachMap.insert("mime", attach.mime);
        attachMap.insert("path", attach.path);
        attachMap.insert("serialKey",attach.serialKey);
        attachMap.insert("length", attach.length);
        attachmentMap.append(QVariant(attachMap));
    }
    map.insert("attachments",QVariant(attachmentMap));
}

QString ConvertData::convertForSql(const QStringList &list)
{
    QString result;
    for(int i = 0; i < list.size(); i++){
        result += "'";
        result += list.at(i);
        result += "'";
        if(i != list.size() -1){
            result += ",";
        }
    }
    return result;
}

QString ConvertData::convert(const QStringList &list)
{
    QString result;
    for(int i = 0; i < list.size(); i++){
        result += list.at(i);
        if(i != list.size() -1){
            result += ",";
        }
    }
    return result;
}
