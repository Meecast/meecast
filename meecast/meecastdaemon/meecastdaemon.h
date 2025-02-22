/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2025 Vlad Vasiliev
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
#ifndef MEEGOTOUCHPLUGIN_H 
#define MEEGOTOUCHPLUGIN_H

#include <QString>
#include <QObject>
#include <MGConfItem>
#include <QProcess>
#include <QDir>
#include <QDate>
#include <QTimer>
#include <QNetworkConfigurationManager>

#include "../dconf/dconfvalue.h"
#include "keepalive/backgroundactivity.h"
// Debug

#include <QFile>
#include <QTextStream>

#include <QCoreApplication>
#include <QGuiApplication>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
//#include <QNetworkSession>

#include <QFileSystemWatcher>

#include <QXmlStreamReader>


class MyMWidget : public QObject
{
   Q_OBJECT                                                                                                                                                                       
private:
    QProcess process;
    QString  _stationname;
    QString  _temperature;
    QString  _temperature_high;
    QString  _temperature_low;
    QString  _iconpath;
    QString  _lastupdate;
    QString  _description;
    bool    _current;
    bool    _lockscreen;
    bool    _standbyscreen;
    QTimer  *_lazyrenderingtimer; /* Timer lazy rendering */
    QTimer  *_lazyupdatedatatimer; /* Timer lazy data updating */
    BackgroundActivity *keepalive;
    MDConfItem *_wallpaperItem;
    MDConfItem *_standbyItem;
    MDConfItem *_original_wallpaperItem;
    QString _wallpaper_path;
    QImage *_image;
    QImage *_events_image;
    bool _down;
    bool _isOnline;
    uint _next_time_for_check;
    QFileSystemWatcher *_watcher;
    QFileSystemWatcher *_watcher_config;
    void parsePeriod(QXmlStreamReader& xml, int itemnumber);
    bool _force_draw;
    bool _need_update;


    QNetworkConfigurationManager *manager;


private slots:
    void networkStatusChanged(bool isOnline);
    void connectionActivated();
    void connectionDeactivated();
public:

    MyMWidget();
    ~MyMWidget();
   
    void refreshwallpaper(bool new_wallpaper = false);
    void updateIntervalChanged(int interval);
    void setOriginalWallpaper();

    Q_INVOKABLE void startpredeamon(); 


    void icon(const QString &iconpath){
	    _iconpath = iconpath;
    }

    QString icon(){
	    return _iconpath; 
    } 

    void station(const QString &stationname){
	    _stationname = stationname;
    }

    QString station(){
	    return _stationname;
    } 

    void temperature(const QString &temperature){
	    _temperature = temperature;
    }

    QString temperature(){
	    return _temperature;
    }

    void temperature_high(const QString &temperature){
	    _temperature_high = temperature;
    }

    QString temperature_high(){
	    return _temperature_high;
    }

    void temperature_low(const QString &temperature){
	    _temperature_low = temperature;
    }

    QString temperature_low(){
	    return _temperature_low;
    }

    void description(const QString &description){
	    _description = description;
    }

    QString description(){
	    return _description;
    } 
    void lastupdate(const QString &lastupdate){
	    _lastupdate = lastupdate;
    }

    QString lastupdate(){
	    return _lastupdate;
    } 

    void current(bool cur){
        _current = cur;
    }

    bool current(){
        return _current;
    }

    void lockscreen(bool cur){
        _lockscreen = cur;
    }
    bool lockscreen(){
        return _lockscreen;
    }
    void standbyscreen(bool cur){
        _standbyscreen = cur;
    }
    bool syandbyscreen(){
        return _standbyscreen;
    }

public Q_SLOTS:
    void SetCurrentData(const QString &station, const QString &temperature, const QString &temperature_high, const QString &temperature_low, const QString &icon, const QString &description, 
                        const uint until_valid_time, bool current, bool lockscreen_param, bool standbyscreen_param, const QString &last_update);

    void update_data();
    void intervalupdate();
    void refreshRequested();
    void updateWallpaperPath();
    void updateStandbyPath();
    void refreshview();
    void checkActivity();
    void wakeupStopped();
    bool isNetworkAvailable();
    void currentfileChanged(QString path);
    void configfileChanged(QString path);
signals:
    void iconChanged();
    void stationChanged();
    void temperatureChanged();
    void temperature_highChanged();
    void temperature_lowChanged();
    void currentChanged();
};


class MyMWidget;

#endif //MEEGOTOUCHPLUGIN_H
