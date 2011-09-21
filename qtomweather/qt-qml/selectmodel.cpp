/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/

#include "selectmodel.h"

SelectData::SelectData(){};

SelectData::SelectData(const SelectData* d)
{
    _name = d->_name;
    _key = d->_key;
    _category = d->_category;
}

SelectData::SelectData(QString name, QString key, QString category)
    : _name(name), _key(key), _category(category)
{
}

QString SelectData::name()
{
    return _name;
}
QString SelectData::key()
{
    return _key;
}
QString SelectData::category()
{
    return _category;
}
void SelectData::setName(QString name)
{
    _name = name;
    emit nameChanged(name);
}
void SelectData::setKey(QString key)
{
    _key = key;
    emit keyChanged(key);
}
void SelectData::setCategory(QString category)
{
    _category = category;
    emit categoryChanged(category);
}

SelectModel::SelectModel(QObject *parent)
    :QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[KeyRole] = "key";
    roles[CategoryRole] = "category";
    setRoleNames(roles);
}

QVariant SelectModel::get(int index)
{
    QVariantMap map;
    map.insert(QLatin1String("category"), _list.at(index)->category());
    map.insert(QLatin1String("name"), _list.at(index)->name());
    map.insert(QLatin1String("key"), _list.at(index)->key());
    return map;
}

int SelectModel::rowCount(const QModelIndex &parent) const
{
    return _list.count();
}

void SelectModel::addData(SelectData* data)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _list.append(data);
    endInsertRows();
}

QVariant SelectModel::data(const QModelIndex & index, int role) const {
     if (index.row() < 0 || index.row() > _list.count())
         return QVariant();

     SelectData* data = _list[index.row()];
     if (role == NameRole)
         return data->name();
     else if (role == KeyRole)
         return data->key();
     else if (role == CategoryRole)
         return data->category();
     return QVariant();
 }

