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

#include "citymodel.h"

CityModel::CityModel(QObject *parent): SelectModel(parent)
{
}

void
CityModel::populate(QString source, QString region)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _list.clear();
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    if (source == "") return ;
    QString filename(source);
    filename.append(".db");
    filename.prepend(path.c_str());
    Core::DatabaseSqlite *db;
    db = new Core::DatabaseSqlite("");
    qDebug() << "filename = " << filename;
    if (db) {
        db->set_databasename(filename.toStdString());
    }else {
        qDebug() << "error db";

    }
    if (!db->open_database()){
        qDebug() << "error open database";
        return;
    }

    Core::listdata * list = db->create_stations_list(region.toInt());

    if (list->size() == 0) return;
    Core::listdata::iterator cur;
    for (cur=list->begin(); cur<list->end(); cur++){
        QString str = QString::fromStdString((*cur).second);
        _list.append(new SelectData(str, QString::fromStdString((*cur).first), str.left(1)));
    }
    endInsertRows();
    reset();
}
