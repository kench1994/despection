#include "commonproxymodel.h"
#include "generallistmodel.h"
#include <QDebug>
CommonProxyModel::CommonProxyModel(QObject *parent):QSortFilterProxyModel(parent)
{
}

CommonProxyModel::~CommonProxyModel()
{
}

QObject *CommonProxyModel::model() const
{
    return sourceModel();
}

int CommonProxyModel::itemCount()
{
    if(!sourceModel())
        return 0;
    return this->rowCount();
}

void CommonProxyModel::setModel(QObject *model)
{
    if (model == sourceModel())
        return;
    GeneralListModel *generalListModel = qobject_cast<GeneralListModel*>(model);
    if (model && !generalListModel) {
        return;
    }
    setSourceModel(generalListModel);
    emit modelChanged();
}

void CommonProxyModel::setModelFilter(const QString &filterStr)
{
    setFilterRegExp(QRegExp(filterStr));
}

QVariant CommonProxyModel::getItem(const int &index)
{
    if(!sourceModel())
        return QVariant();
    GeneralListModel *generalListModel = qobject_cast<GeneralListModel *>(sourceModel());
    return generalListModel->getItem(mapToSource(this->index(index,0)).row());
}

int CommonProxyModel::getSourceIndex(const int &index)
{
    if(!sourceModel())
        return -1;
    int sourceIndex = mapToSource(this->index(index,0)).row();
    qDebug() << Q_FUNC_INFO << "  index is " << index << "  source index is  " << sourceIndex;
    return sourceIndex;
}

void CommonProxyModel::setListData(const QList<QVariant> &data)
{
    qDebug() << Q_FUNC_INFO <<"  data.size " << data.size();
    QAbstractItemModel *itemModel = sourceModel();
    if(itemModel){
        GeneralListModel *generalListModel = qobject_cast<GeneralListModel*>(itemModel);
        if(generalListModel)
            generalListModel->setListData(data);
    }
}

