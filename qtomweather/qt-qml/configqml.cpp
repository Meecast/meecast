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


#include "configqml.h"

//ConfigQml::ConfigQml():QObject(),Core::Config("config.xml", "../core/data/config.xsd"){}
ConfigQml::ConfigQml(const std::string& filename, const std::string& schema_filename):QObject(),Core::Config(filename, schema_filename)
{
}

ConfigQml::ConfigQml():QObject(),Core::Config(){

}

QString
ConfigQml::iconset(){
    QString c;
    c = ConfigQml::Config::iconSet().c_str();
    return c;
}

QString
ConfigQml::iconspath(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("/icons");
    return c;
}

QString
ConfigQml::iconsbutton(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("/buttons_icons");
    return c;
}

QString
ConfigQml::temperatureunit(){
    QString c;
    c = ConfigQml::Config::TemperatureUnit().c_str();
    return c;
}

QString
ConfigQml::windspeedunit(){
    QString c;
    c = QString(QString::fromUtf8(_(ConfigQml::Config::WindSpeedUnit().c_str())));
    return c;
}

QColor
ConfigQml::fontcolor(){
    QColor c;
    c.setNamedColor(ConfigQml::Config::FontColor().c_str());
    return c;
}
QString
ConfigQml::stationname()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->name().c_str();
    else
        return "Unknown";
}
QString
ConfigQml::filename()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->fileName().c_str();
    else
        return QString();
}
void ConfigQml::changestation()
{
    if ((uint)(this->current_station_id() + 1) < this->stationsList().size()){
       this->current_station_id(this->current_station_id() + 1);

   }else {
       this->current_station_id(0);
   }

}
void
ConfigQml::refreshconfig(){
    emit ConfigQml::iconsetChanged();
    emit ConfigQml::iconspathChanged();
    emit ConfigQml::temperatureunitChanged();
    emit ConfigQml::windspeedunitChanged();
    emit ConfigQml::fontcolorChanged();
    emit ConfigQml::stationnameChanged();
}

void
ConfigQml::updatestations()
{
    uint i;
    int success = 0;
    Core::Station* station;

    for (i=0; i < this->stationsList().size();i++){
        station = this->stationsList().at(i);
        if (station->updateData(true)){
            success ++;

        }
    }
}
void
ConfigQml::runsetting()
{
    char *args[] = {"/usr/bin/omweather-settings", (char *) 0 };

    pid_t pID = fork();
    if (pID == 0)
        execv("/usr/bin/omweather-settings", args );
}

void
ConfigQml::closeapplication()
{
   exit (0);
}


void
ConfigQml::reload_config()
{
   this->ReLoadConfig();
   this->refreshconfig();
}
