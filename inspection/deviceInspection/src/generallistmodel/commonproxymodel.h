#ifndef COMMONPROXYMODEL_H
#define COMMONPROXYMODEL_H

#include <QSortFilterProxyModel>
class CommonProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CommonProxyModel(QObject *parent = 0);
    ~CommonProxyModel();
    Q_INVOKABLE QObject *model() const;
    Q_INVOKABLE int itemCount();
    Q_INVOKABLE void setModel(QObject *model);
    Q_INVOKABLE void setModelFilter(const QString &filterStr);
    Q_INVOKABLE QVariant getItem(const int &index);
    Q_INVOKABLE int getSourceIndex(const int &index);
    Q_INVOKABLE void setListData(const QList<QVariant > &data);
signals:
    void modelChanged();
};

#endif // COMMONPROXYMODEL_H
