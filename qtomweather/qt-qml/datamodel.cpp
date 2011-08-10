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
int
DataModel::count()
{
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
    //qDeleteAll(this->_list);
    this->_list.clear();
    //emit dataChanged(this->createIndex(0, 0), this->createIndex(count, 0));
    this->reset();
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
DataModel::update(QString filename)
{
    this->clear();
    DataItem *forecast_data = NULL;

    int i = 0;
    Core::DataParser* dp = NULL;
    Core::Data *temp_data = NULL;
    if (!filename.isEmpty()){
        try{
            dp = new Core::DataParser(filename.toStdString(),
                                      Core::AbstractConfig::prefix+Core::AbstractConfig::schemaPath+"data.xsd");
        }
        catch(const std::string &str){
            std::cerr<<"Error in DataParser class: "<< str << std::endl;
            //return NULL;
        }
        catch(const char *str){
            std::cerr<<"Error in DataParser class: "<< str << std::endl;
            //return NULL;
        }
    }

    while  (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL) + i))) {
        i = i + 3600*24;
        forecast_data = new DataItem(temp_data);
        forecast_data->Text(forecast_data->Text().c_str());
        this->appendRow(forecast_data);

    }
    this->reset();
}
