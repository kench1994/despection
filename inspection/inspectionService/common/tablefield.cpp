#include "tablefield.h"
TableField::TableField()
{
    m_nIsKey = false;
    m_nEnableNull = false;
}

TableField::~TableField()
{

}
QString TableField::getName() const{
    return m_nName;
}
void TableField::setName(const QString &name){
    m_nName = name;
}
QString TableField::getType() const{
    return m_nType;
}
void TableField::setType(const QString &type){
    m_nType = type;
}
int TableField::getLength()const{
    return m_nLength;
}
void TableField::setLength(const int &length){
    m_nLength = length;
}
bool TableField::getIsKey() const{
    return m_nIsKey;
}
void TableField::setIsKey(const bool &isKey){
    m_nIsKey = isKey;
}
bool TableField::getIsEnableNull() const{
    return m_nEnableNull;
}
void TableField::setEnableNull(const bool &enableNull){
    m_nEnableNull = enableNull;
}
