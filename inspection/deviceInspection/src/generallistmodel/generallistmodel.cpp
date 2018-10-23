#include "generallistmodel.h"
#include <QDebug>
GeneralListModel::GeneralListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int GeneralListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return items.size();
}

QVariant GeneralListModel::data(const QModelIndex &index, int role) const
{
    //qDebug() << Q_FUNC_INFO << " index is " <<index << " role is " << role;
    if (!index.isValid() || index.row() < 0 || index.row() >= items.size())
        return QVariant();

    QVariantMap obj = items.at(index.row());
    QString roleName = roles.value(role);
    return obj.value(roleName);
}

void GeneralListModel::setRolesName(QList<QString> roleNames)
{
    qDebug() << Q_FUNC_INFO << " roleNames is " << roleNames;
    roles.clear();
    for(int i = 0; i< roleNames.size(); i++){
        roles[Qt::UserRole + i + 1] = roleNames.at(i).toUtf8();
    }
}

void GeneralListModel::setListData(const QList<QVariant > &data)
{
    qDebug() << Q_FUNC_INFO << " data is " << data.size();
    beginResetModel();
    items.clear();
    foreach (QVariant item, data) {
        QVariantMap map = item.toMap();
        items.append(map);
    }
    endResetModel();
}

bool GeneralListModel::setData(const int &index, const QString &roleName, const QVariant &value){
    if(index < rowCount()){
        beginResetModel();
        QVariantMap obj = items.at(index);
        obj[roleName] = value.toString();
        items.replace(index, obj);
        qDebug() << Q_FUNC_INFO << "roleName is " << roleName << "  index is " << index  << "  value.toString() " << value.toString();
        endResetModel();
        return true;
    }
    return false;
}

bool GeneralListModel::removeData(const int &index)
{
    if(index < rowCount()){
        beginResetModel();
        items.removeAt(index);
        endResetModel();
        return true;
    }
    return false;
}

bool GeneralListModel::removeDataByKeys(const QString &roleName, const QString &keys)
{
    QStringList keyList = keys.split(",");
    beginResetModel();
    for(int i = 0; i < keyList.count(); i++){
        for(int j = 0; j < items.count(); j++){
            QVariantMap obj = items.at(j);
            if(obj[roleName].toString() == keyList.at(i)){
                items.removeAt(j);
                break;
            }
        }
    }
    endResetModel();
    emit dataChanged(index(0, 0), index(0, 0));
    return true;
}

//QVariant GeneralListModel::get(const int &index) const{
//    qDebug() << Q_FUNC_INFO << " index is " << index;
//    if(index > rowCount()){
//        return QVariant();
//    }
//    QVariantMap obj = items.at(index);
//    return QVariant(obj);
//}

QVariant GeneralListModel::getItem(const int &index) const
{
    qDebug() << Q_FUNC_INFO << " index is " << index;
    if(index > rowCount()){
        return QVariant();
    }
    QVariantMap obj = items.at(index);
    return QVariant(obj);
}

int GeneralListModel::getRoleByRoleName(const QString &roleName)
{
    QHash<int, QByteArray>::const_iterator iter = roles.constBegin();
    while (iter != roles.constEnd()) {
        if(roleName == iter.value())
            return iter.key();
        ++iter;
    }
    return -1;
}

void GeneralListModel::updateData(){
    beginResetModel();
    endResetModel();
    emit dataChanged(index(0, 0), index(0, 0));
}

QHash<int, QByteArray> GeneralListModel::roleNames() const
{
    qDebug() << Q_FUNC_INFO;
    return roles;
}
