/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2015 Vlad Vasilyeu
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

#ifndef Q_OS_ANDROID
    #include <libintl.h>
    #include <locale.h>
#endif
#include <fstream>
#include <iostream>
#if defined(MEEGO_EDITION_HARMATTAN)
#include <MLocale>
#endif

#ifndef Q_OS_ANDROID
    #define _(String) gettext(String)
#endif

#ifdef Q_OS_ANDROID
    #define _(String) String
#endif


class DataItem : public QObject, public Core::Data
{
    Q_OBJECT
    enum Roles {
        NameRole = Qt::UserRole+1,
        Temp_hiRole,
        Temp_loRole,
        Temp_Role,
        IconRole,
        UVindexRole,
        Wind_directionRole,
        Wind_speedRole,
        Wind_gustRole,
        HumidityRole,
        VisibleRole,
        DescriptionRole,
        CurrentRole,
        DateRole,
        ShortDateRole,
        FullDateRole,
        HourDateRole,
        StartRole,
        EndRole,
        PressureRole,
        FlikeRole,
        DewpointRole,
        PpcpRole,
        PrecipitationRole,
        PrecipitationRateRole,
        TemperatureLabelRole,
        HumidityLabelRole,
        WindLabelRole,
        WindSpeedLabelRole,
        PressureLabelRole,
        SunSetRole,
        SunRiseRole,
        DayLengthRole,
        LastUpdateRole,
        LastUpdateTimeRole,
        NowLabelRole,
        MapPatternRole,
        MapBackgroundPatternRole,
        CountOfMapsRole
    };
    QDateTime _lastupdate;

public:
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString temperature_high READ temperature_high)
    Q_PROPERTY(QString temperature_low READ temperature_low)
    Q_PROPERTY(QString wind_direction READ wind_direction)
    Q_PROPERTY(QString wind_speed READ wind_speed)
    Q_PROPERTY(QString humidity READ humidity)
    Q_PROPERTY(QString pressure READ pressure)
    Q_PROPERTY(QString uv_index READ uv_index)
    Q_PROPERTY(QString visible READ visible)
    Q_PROPERTY(QString icon READ icon)
    Q_PROPERTY(QString shortdate READ shortdate)
    Q_PROPERTY(QString sunrise READ sunrise)
    Q_PROPERTY(QString sunset READ sunset)
    Q_PROPERTY(QString daylength READ daylength)
    Q_PROPERTY(QString lastupdate READ lastupdate)
    Q_PROPERTY(QString lastupdatetime READ lastupdatetime)
    Q_PROPERTY(QString map_pattern READ map_pattern)
    Q_PROPERTY(QString count_of_maps READ count_of_maps)

    DataItem();
    DataItem(const Core::Data *data);
    virtual ~DataItem(){};
    QVariant data(int role);
    int getRole(QString roleName);
    QHash<int, QByteArray> roleNames() const;
    QString temperature_high();
    QString temperature_low();
    QString temperature();
    QString wind_direction();
    QString uv_index();
    QString visible();
    QString wind_speed();
    inline QString wind_gust();
    QString humidity();
    QString icon();
    bool current();
    QString description();
    inline QString date();
    QString shortdate();
    inline QString fulldate();
    inline QString hourdate();
    inline QString start();
    inline QString end();
    inline QString flike();
    inline QString dewpoint();
    QString pressure();
    inline QString ppcp();
    inline QString precipitation();
    inline QString precipitation_rate();
    QString map_pattern();
    QString mapbackground_pattern();
    QString count_of_maps();
    QString sunrise();
    QString sunset();
    QString daylength();
    QString lastupdate();
    QString lastupdatetime();
    void LastUpdate(time_t date_and_time);

    Q_INVOKABLE void update(QString filename);

    QString temperatureunit;
    QString windunit;
    QString pressureunit;
    QString visibleunit;
};

#endif // DATAITEM_H

