/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2025 Vlad Vasilyeu
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


#ifndef CONFIGQML_H
#define CONFIGQML_H

#include "core.h"
#include <QObject>
#include <QColor>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include "updatethread.h"
#include "gpsposition.h"
#ifdef MEEGO_EDITION_HARMATTAN
#include <MGConfItem>
#endif
#include <QSettings>
#include <QtGui/QGuiApplication>
#include <QTranslator>
#include <QDir>

#include <libintl.h>
#include <locale.h>
#include  "networkingcontrol.h"


//#define _(String) gettext(String)

#define _(String)  QObject::trUtf8(String).toStdString().c_str()
class ConfigQml : public QObject, public Core::Config

{
    Q_OBJECT
    Q_PROPERTY(QString iconset READ iconset NOTIFY iconsetChanged)
    Q_PROPERTY(QString iconspath READ iconspath NOTIFY iconspathChanged)
    Q_PROPERTY(QString imagespath READ imagespath NOTIFY imagespathChanged)
    Q_PROPERTY(QString iconsbutton READ iconsbutton NOTIFY iconsbuttonChanged)
    Q_PROPERTY(QString temperatureunit READ temperatureunit NOTIFY temperatureunitChanged)
    Q_PROPERTY(QString windspeedunit READ windspeedunit NOTIFY windspeedunitChanged)
    Q_PROPERTY(QString pressureunit READ pressureunit NOTIFY pressureunitChanged)
    Q_PROPERTY(QString visibleunit READ visibleunit NOTIFY visibleunitChanged)
    Q_PROPERTY(bool fullscreen READ fullscreen NOTIFY fullscreenChanged)
    Q_PROPERTY(bool updating READ updating NOTIFY updatingChanged)
    Q_PROPERTY(bool lockscreen READ lockscreen NOTIFY lockscreenChanged)
    Q_PROPERTY(bool standbyscreen READ standbyscreen NOTIFY standbyscreenChanged)
    Q_PROPERTY(bool eventwidget READ eventwidget NOTIFY eventwidgetChanged)
    Q_PROPERTY(bool splash READ splash NOTIFY splashChanged)
    Q_PROPERTY(bool gps READ gps NOTIFY gpsChanged)
    Q_PROPERTY(bool logocoverpage READ logocoverpage NOTIFY logocoverpageChanged)
    Q_PROPERTY(bool windcoverpage READ windcoverpage NOTIFY windcoverpageChanged)
    Q_PROPERTY(bool transparency READ transparency NOTIFY transparencyChanged)
    Q_PROPERTY(bool lastupdatecoverpage READ lastupdatecoverpage NOTIFY lastupdatecoverpageChanged)
    Q_PROPERTY(QColor fontcolor READ fontcolor NOTIFY fontcolorChanged)
    Q_PROPERTY(QColor standby_color_font_stationname READ standby_color_font_stationname NOTIFY standby_color_font_stationnameChanged)
    Q_PROPERTY(QColor standby_color_font_temperature READ standby_color_font_temperature NOTIFY standby_color_font_temperatureChanged)
    Q_PROPERTY(QColor standby_color_font_current_temperature READ standby_color_font_current_temperature NOTIFY standby_color_font_current_temperatureChanged)
    Q_PROPERTY(QString stationname READ stationname NOTIFY stationnameChanged)
    Q_PROPERTY(QString prevstationname READ prevstationname NOTIFY prevstationnameChanged)
    Q_PROPERTY(QString nextstationname READ nextstationname NOTIFY nextstationnameChanged)
    Q_PROPERTY(QString data_filename READ data_filename NOTIFY data_filenameChanged)
    Q_PROPERTY(QString source READ source NOTIFY sourceChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(int updateinterval READ updateinterval NOTIFY updateintervalChanged)
    Q_PROPERTY(int lock_screen_x_position READ lock_screen_x_position NOTIFY lock_screen_x_positionChanged)
    Q_PROPERTY(int lock_screen_y_position READ lock_screen_y_position NOTIFY lock_screen_y_positionChanged)
private:
    Core::DatabaseSqlite *db;
    UpdateThread *thread;
    GpsPosition *_gps;
    int getCountryId(int index);
    int getRegionId(int country, int index);
    int getGpsStation();
    QString getCityId(int region_id, int index);
    void init();
    QStringList wind_list;
    QStringList press_list;
    QStringList vis_list;
    QSettings *standby_settings;
    QColor _standby_color_font_stationname;
    QColor _standby_color_font_temperature;
    QColor _standby_color_font_current_temperature;
    QSettings *lockscreen_settings;
    int _lockscreen_x_position;
    int _lockscreen_y_position;
    uint _time_for_updating;
    bool _updating;
    QGuiApplication *_app;
    QTranslator _translator;
protected:
    static ConfigQml* _self;
    static int _refcount_;
    virtual ~ConfigQml();
    ConfigQml();
    ConfigQml(const std::string& filename, const std::string& schema_filename = "/usr/" + schemaPath + "config.xsd", QGuiApplication *app = NULL);

public:
    static ConfigQml* Instance();
    static ConfigQml* Instance(const std::string& filename, const std::string& schema_filename = "/usr/" + schemaPath + "config.xsd", QGuiApplication *app = NULL);
    static bool DeleteInstance();
    QString iconset();
    QString iconspath();
    QString imagespath();
    QString iconsbutton();
    QString temperatureunit();
    QString windspeedunit();
    QString pressureunit();
    QString visibleunit();
    bool fullscreen();
    bool lockscreen();
    bool standbyscreen();
    bool eventwidget();
    bool gps();
    bool splash();
    bool logocoverpage();
    bool windcoverpage();
    bool transparency();
    bool lastupdatecoverpage();
    bool updating();
    void updating(bool c);
    QColor fontcolor();
    QColor standby_color_font_stationname();
    QColor standby_color_font_temperature();
    QColor standby_color_font_current_temperature();
    int lock_screen_x_position();
    int lock_screen_y_position();
    void time_for_updating(uint _time);
    QString stationname();
    QString prevstationname();
    QString nextstationname();
    QString data_filename();
    QString source();
    QString version();
    Q_INVOKABLE void saveConfig();
    int updateinterval();
    QString viewURL();
    Q_INVOKABLE QStringList stations();
    Q_INVOKABLE void removeStation(int index);
    Q_INVOKABLE QStringList Sources();
    Q_INVOKABLE QStringList Countries(QString source);
    Q_INVOKABLE QStringList Regions(int index);
    Q_INVOKABLE QStringList Cities(int country_index, int index);
    Q_INVOKABLE void saveStation(int city_id, QString city,
                                 int region_id, QString region,
                                 int country_id, QString country,
                                 int source_id, QString source);
    Q_INVOKABLE void saveStation1(QString city_id, QString city_name, QString region,
                                  QString country, QString source, int source_id, bool gps=false, double latitude = 0.0, double longitude = 0.0);
    Q_INVOKABLE void changestation();
    Q_INVOKABLE void changestationback();
    Q_INVOKABLE void nextstation();
    Q_INVOKABLE void prevstation();
    Q_INVOKABLE void updatestations();
    Q_INVOKABLE void updategpsstation();
    Q_INVOKABLE void showweb();
    Q_INVOKABLE void showwebdonation();
    Q_INVOKABLE void showwebsupport();
    Q_INVOKABLE void closeapplication();
    Q_INVOKABLE QStringList temperature_list();
    Q_INVOKABLE void temperature_unit(QString c);
    Q_INVOKABLE QStringList windspeed_list();
    Q_INVOKABLE void windspeed_unit(int index);
    Q_INVOKABLE void update_interval(int interval);
    Q_INVOKABLE QStringList pressure_list();
    Q_INVOKABLE void pressure_unit(int index);
    Q_INVOKABLE QStringList visible_list();
    Q_INVOKABLE void visible_unit(int index);
    Q_INVOKABLE void setfullscreen(bool c);
    Q_INVOKABLE bool setlockscreen(bool c);
    Q_INVOKABLE void setstandbyscreen(bool c);
    Q_INVOKABLE void seteventwidget(bool c);
    Q_INVOKABLE void setgps(bool c);
    Q_INVOKABLE void setsplash(bool c);
    Q_INVOKABLE void setlogocoverpage(bool c);
    Q_INVOKABLE void settransparency(bool c);
    Q_INVOKABLE void setwindcoverpage(bool c);
    Q_INVOKABLE void setlastupdatecoverpage(bool c);
    Q_INVOKABLE QStringList icon_list();
    Q_INVOKABLE QStringList languages_list();
    Q_INVOKABLE int index_of_current_language();
    Q_INVOKABLE void set_iconset(QString c);
    Q_INVOKABLE void set_language(QString c);
    Q_INVOKABLE void set_standby_color_font_stationname(QColor c);
    Q_INVOKABLE void set_standby_color_font_temperature(QColor c);
    Q_INVOKABLE void set_standby_color_font_current_temperature(QColor c);
    Q_INVOKABLE void set_lock_screen_x_position(int x);
    Q_INVOKABLE void set_lock_screen_y_position(int y);
    Q_INVOKABLE QString tr(QString str);
    Q_INVOKABLE void enableGps();
    Q_INVOKABLE QString stationname_index(int i);
    Q_INVOKABLE void station_by_index(int i);
    Q_INVOKABLE QString source_index(int i);
    Q_INVOKABLE QString _source();
    Q_INVOKABLE int _current_station_id();
    Q_INVOKABLE void _current_station_id(int i);
    Q_INVOKABLE void refreshconfig();
    Q_INVOKABLE void refreshconfig2();
    Q_INVOKABLE void refreshconfig3();
    Q_INVOKABLE void check_and_update_station();
    Q_INVOKABLE void reload_config();
    Q_INVOKABLE void setLanguage();
//    void refreshconfig();
    NetworkingControl *networkingcontrol; 
    bool isOnline();
    void connectSession(bool background);
    bool need_updating;
private slots:    
    void onNetworkSessionOpened();
    void onNetworkSessionError();

signals:
    void iconsetChanged();
    void iconspathChanged();
    void imagespathChanged();
    void iconsbuttonChanged();
    void temperatureunitChanged();
    void windspeedunitChanged();
    void pressureunitChanged();
    void visibleunitChanged();
    void fullscreenChanged();
    void updatingChanged();
    void lockscreenChanged();
    void standbyscreenChanged();
    void eventwidgetChanged();
    void gpsChanged();
    void fontcolorChanged();
    void standby_color_font_stationnameChanged();
    void standby_color_font_temperatureChanged();
    void standby_color_font_current_temperatureChanged();
    void stationnameChanged();
    void prevstationnameChanged();
    void nextstationnameChanged();
    void data_filenameChanged();
    void sourceChanged();
    void versionChanged();
    void updateintervalChanged();
    void configChanged();
    void configReloadCoverPage();
    void configChangeStationOnMainPage();
    void splashChanged();
    void logocoverpageChanged();
    void windcoverpageChanged();
    void transparencyChanged();
    void lastupdatecoverpageChanged();
    void lock_screen_x_positionChanged();
    void lock_screen_y_positionChanged();
    void errorInConfig(QString text);
    void languageChanged();
public Q_SLOTS:
    void addGpsStation(double latitude, double longitude);
private slots:
    void downloadFinishedSlot();

};

#endif // CONFIGQML_H
