#include "tableconfig.h"

TableConfig::TableConfig()
{
}

TableConfig::~TableConfig()
{
}

void TableConfig::setName(const QString &name) {
    m_nName = name;
}
QString TableConfig::getName() {
    return m_nName;
}
void TableConfig::setDescription(const QString &description) {
    m_nDescription = description;
}

QString TableConfig::getDescription() {
    return m_nDescription;
}
void TableConfig::addField(const TableField &field){
    m_nFieldList.append(field);
}
QList<TableField> TableConfig::getFieldList() {
    return m_nFieldList;
}
