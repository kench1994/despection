/*!
* \file generallistmodel.h
* \brief 系统数据Model模块
*
*系统数据Model模块
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
#ifndef GENERALLISTMODEL_H
#define GENERALLISTMODEL_H

#include <QList>
#include <QString>
#include <QPair>
#include <QAbstractListModel>

/*!
  \class GeneralListModel
  \inModule 系统数据Model模块
  \brief 系统数据Model模块
  \ingroup generallistmodel
  系统数据Model模块
*/

class GeneralListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit GeneralListModel(QObject *parent = 0);

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void setRolesName(QList<QString> roleNames);
    Q_INVOKABLE void setListData(const QList<QVariant > &data);
    //Q_INVOKABLE void deleteItems(QList<int> indexList);
    Q_INVOKABLE void updateData();
    Q_INVOKABLE bool setData(const int &index, const QString &roleName, const QVariant &value);
    Q_INVOKABLE bool removeData(const int &index);
    Q_INVOKABLE bool removeDataByKeys(const QString &roleName, const QString &keys);
    Q_INVOKABLE int getRoleByRoleName(const QString &roleName);
    //Q_INVOKABLE QVariant get(const int &index) const;
    Q_INVOKABLE QVariant getItem(const int &index) const;
signals:

public slots:

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QHash<int,QByteArray> roles;
    QList<QVariantMap > items;

};

#endif // GENERALLISTMODEL_H
