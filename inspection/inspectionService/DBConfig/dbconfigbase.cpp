#include "dbconfigbase.h"
#include <QXmlAttributes>
#include <QDomDocument>
DBConfigBase::DBConfigBase()
{
}

DBConfigBase::~DBConfigBase()
{
}
QList<TableConfig> DBConfigBase::getTables(){
    return m_nTableConfigList;
}
TableConfig DBConfigBase::getTable(const QString &name){
    TableConfig tableConfig;
    for(int i = 0; i < m_nTableConfigList.count(); i++){
        tableConfig = m_nTableConfigList.at(i);
        if(tableConfig.getName() == name){
            break;
        }
    }
    return tableConfig;
}

QList<TableConfig> DBConfigBase::parseXml2TableConfig(const QString &xmlFile){
    QList<TableConfig> tableList;
    QDomDocument doc("mydocument");
    QFile file(xmlFile);
    if (!file.open(QIODevice::ReadOnly))
        return tableList;
    if (!doc.setContent(&file)) {
        file.close();
        return tableList;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while (!n.isNull()){
        QDomElement e = n.toElement();
        if (!e.isNull()){
            //table
            if(e.tagName() == "Table"){
                TableConfig tableConfig;
                tableConfig.setName(e.attribute("name", ""));
                tableConfig.setDescription(e.attribute("description", ""));
                QDomNodeList fieldList = e.childNodes();
                for(int i = 0; i < fieldList.count(); i++){
                    QDomElement field = fieldList.at(i).toElement();
                    TableField tableField;
                    tableField.setName(field.attribute("name", ""));
                    tableField.setType(field.attribute("type", ""));
                    tableField.setLength(field.attribute("length", "0").toInt());
                    tableField.setIsKey(field.attribute("isKey", "false") == "true");
                    tableField.setEnableNull(field.attribute("enableNull", "false") == "true");
                    tableConfig.addField(tableField);
                }
                tableList.append(tableConfig);
            }
        }
        n = n.nextSibling();
    }
    return tableList;
}

DataOperationType DBConfigBase::getDataOperationType(){
    return m_nDataOperationType;
}
