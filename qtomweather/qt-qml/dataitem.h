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


#ifndef DATAITEM_H
#define DATAITEM_H
#include "core.h"
#include <QObject>
#include <QVariant>
#include <QtCore>

#include <libintl.h>
#include <locale.h>

#define _(String) gettext(String)

class DataItem : public QObject, public Core::Data
{
    Q_OBJECT
    enum Roles {
        NameRole = Qt::UserRole+1,
        Temp_hiRole,
        Temp_loRole,
        IconRole,
        Wind_directionRole,
        Wind_speedRole,
        Wind_gustRole,
        HumidityRole,
        DescriptionRole,
        CurrentRole,
        DateRole,
        ShortDateRole,
        FullDateRole,
        StartRole,
        EndRole,
        PressureRole,
        FlikeRole,
        PpcpRole,
        TemperatureLabelRole,
        HumidityLabelRole,
        WindLabelRole,
        WindSpeedLabelRole,
        PressureLabelRole
    };
public:
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString temperature_high READ temperature_high)
    Q_PROPERTY(QString temperature_low READ temperature_low)
    Q_PROPERTY(QString wind_direction READ wind_direction)
    Q_PROPERTY(QString wind_speed READ wind_speed)
    Q_PROPERTY(QString humidity READ humidity)
    Q_PROPERTY(QString pressure READ pressure)
    Q_PROPERTY(QString icon READ icon)
    Q_PROPERTY(QString shortdate READ shortdate)

    DataItem();
    DataItem(const Core::Data *data);
    virtual ~DataItem(){};
    QVariant data(int role);
    int getRole(QString roleName);
    QHash<int, QByteArray> roleNames() const;
    QString temperature_high();
    QString temperature_low();
    QString wind_direction();
    QString wind_speed();
    inline QString wind_gust();
    QString humidity();
    QString icon();
    inline bool current();
    QString description();
    inline QString date();
    QString shortdate();
    inline QString fulldate();
    inline QString start();
    inline QString end();
    inline QString flike();
    QString pressure();
    inline QString ppcp();

    Q_INVOKABLE void update(QString filename);

};

#endif // DATAITEM_H

