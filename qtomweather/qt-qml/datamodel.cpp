#include "datamodel.h"

DataModel::DataModel(DataItem* prototype, QObject *parent) :
    QAbstractListModel(parent), _prototype(prototype)
{
    setRoleNames(_prototype->roleNames());
}

int
DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list.size();
}

QVariant
DataModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list.size())
        return QVariant();
    return _list.at(index.row())->data(role);
}

DataModel::~DataModel()
{
    delete _prototype;
}

void
DataModel::clear()
{
    qDeleteAll(_list);
    _list.clear();
}

void
DataModel::appendRow(DataItem *item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _list.append(item);
    endInsertRows();
    //return (_list.size() - 1);
}
void
DataModel::update()
{
    this->clear();
}
