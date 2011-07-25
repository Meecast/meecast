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

ConfigQml::ConfigQml() : QObject(), Core::Config(Core::AbstractConfig::getConfigPath()+"config.xml")
{    
}

QStringList
ConfigQml::icons()
{
    QStringList icons;
    Dirent *dp = 0;
    DIR *dir_fd = opendir((Core::AbstractConfig::prefix+Core::AbstractConfig::iconsPath).c_str());
    int i = 0;
    if(dir_fd){
        while((dp = readdir(dir_fd))){
            std::string name = dp->d_name;
            if(name == "." || name == "..")
                continue;
            if(dp->d_type == DT_DIR && name[0] != '.'){
                try{
                    icons << QString::fromStdString(name);
                }
                catch(std::string& err){
                    std::cerr << "error " << err << std::endl;
                    continue;
                }
                catch(const char *err){
                    std::cerr << "error " << err << std::endl;
                    continue;
                }
            }
        }
        closedir(dir_fd);
    }
    return icons;
}

QString
ConfigQml::iconSet(){
    QString c;
    c = ConfigQml::Config::iconSet().c_str();
    return c;
}
void
ConfigQml::iconSet(QString c){
    ConfigQml::Config::iconSet(c.toStdString());
}
QStringList
ConfigQml::UpdatePeriods()
{
    QStringList l;
    l << "2 hour" << "1 hour" << "30 min" << "10 min" << "never";
    return l;
}

void
ConfigQml::UpdatePeriod(QString str){
    int period;
    if (str == "2 hour")
        period = 2*60*60;
    else if (str == "1 hour")
        period = 60*60;
    else if (str == "30 min")
        period = 30*60;
    else if (str == "10 min")
        period = 10*60;
    else if (str == "never")
        period = INT_MAX;
    ConfigQml::Config::UpdatePeriod(period);
}
QString
ConfigQml::UpdatePeriod(){
    int val = ConfigQml::Config::UpdatePeriod();
    QString res;
    if (val == 2*60*60)
        res = "2 hour";
    else if (val == 60*60)
        res = "1 hour";
    else if (val == 30*60)
        res = "30 min";
    else if (val == 10*60)
        res = "10 min";
    else if (val == INT_MAX)
        res = "never";
    return res;

}
void
ConfigQml::UpdateConnect(bool uc)
{
    ConfigQml::Config::UpdateConnect(uc);
}
bool
ConfigQml::UpdateConnect()
{
    return ConfigQml::Config::UpdateConnect();
}
QStringList
ConfigQml::TemperatureUnits()
{
    QStringList l;
    l << "C" << "F";
    return l;
}
QString
ConfigQml::TemperatureUnit()
{
    return ConfigQml::Config::TemperatureUnit().c_str();
}
void
ConfigQml::TemperatureUnit(QString c)
{
    ConfigQml::Config::TemperatureUnit(c.toStdString());
}
QStringList
ConfigQml::WindSpeedUnits()
{
    QStringList l;
    l << "m/s" << "km/h" << "mi/h";
    return l;
}
QString
ConfigQml::WindSpeedUnit()
{
    return ConfigQml::Config::WindSpeedUnit().c_str();
}
void
ConfigQml::WindSpeedUnit(QString c)
{
    ConfigQml::Config::WindSpeedUnit(c.toStdString());
}

QStringList
ConfigQml::Stations()
{
    //Core::StationsList *stationlist = new Core::StationList;
    Core::StationsList stationlist = ConfigQml::Config::stationsList();
    Core::StationsList::iterator cur;
    QStringList l;
    for (cur=stationlist.begin(); cur<stationlist.end(); cur++){
        //l << (*cur)->name() + " (" + (*cur)->id() + ")";
        l << (*cur)->name().c_str();
    }
    return l;
}

void
ConfigQml::saveConfig()
{
    ConfigQml::Config::saveConfig();
}
