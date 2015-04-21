/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2015 Vlad Vasilyeu
 *     for the code
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


#ifndef MEECASTDATA_H
#define MEECASTDATA_H

#include <QObject>
#include <QQmlParserStatus>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QXmlStreamReader>
#include <QDir>
#include <QFileSystemWatcher>
#include <iostream>
#include <contentaction.h>

class Meecastdata : public QObject, public QQmlParserStatus{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:


    Meecastdata(QObject *parent = 0);
    ~Meecastdata();
    void classBegin();
    void componentComplete();

    Q_PROPERTY(QString nameString READ nameString NOTIFY nameStringChanged)
    Q_PROPERTY(QVariantMap forecastdata READ forecastdata NOTIFY forecastdataChanged)
    QString nameString() const;
    QVariantMap forecastdata() const;

public Q_SLOTS:
    void currentfileChanged(QString path);
    void startMeeCast();
    void SetCurrentData(const QString &station, const QString &temperature, const QString &temperature_high, const QString &temperature_low, const QString &icon, const QString &description, 
                        const uint until_valid_time, bool current, bool lockscreen_param, bool standbyscreen_param, const QString &last_update);
signals:
    void refreshWidget();
    void nameStringChanged();
    void forecastdataChanged();

private:
    void getWeatherdata();
    QTimer  *_filemonitoring; /* For check exsisting of file */
    QTimer  *_lazyrenderingtimer; /* Timer lazy rendering */
    QVariantMap _weatherdata;
    QFileSystemWatcher *_watcher;
    void parsePeriod(QXmlStreamReader& xml, int itemnumber);

private slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &propertiesChanged, const QStringList &propertiesInvalidated);
    void updatefilemonitoring();
    void refreshview();
};

#endif // MEECASTDATA_H

