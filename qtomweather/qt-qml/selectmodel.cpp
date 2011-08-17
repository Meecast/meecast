#include "selectmodel.h"

SelectData::SelectData(const QString &name, const QString &key)
    : _name(name), _key(key)
{
}

QString SelectData::name() const
{
    return _name;
}
QString SelectData::key() const
{
    return _key;
}

SelectModel::SelectModel(QObject *parent)
    :QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[KeyRole] = "key";
    setRoleNames(roles);
}

int SelectModel::rowCount(const QModelIndex &parent) const
{
    return _list.count();
}

void SelectModel::addData(const SelectData &data)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _list << data;
    endInsertRows();
}

QVariant SelectModel::data(const QModelIndex & index, int role) const {
     if (index.row() < 0 || index.row() > _list.count())
         return QVariant();

     const SelectData &data = _list[index.row()];
     if (role == NameRole)
         return data.name();
     else if (role == KeyRole)
         return data.key();
     return QVariant();
 }
