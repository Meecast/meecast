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

ConfigQml::ConfigQml() : QObject(), Core::Config()
{
}

ConfigQml::ConfigQml(const std::string& filename, const std::string& schema_filename) : QObject(),Core::Config(filename, schema_filename)
{
}

QStringList
ConfigQml::icons()
{
    QStringList icons;
    Dirent *dp = 0;
    DIR *dir_fd = opendir((Core::AbstractConfig::prefix+Core::AbstractConfig::iconsPath).c_str());
    //std::cerr << (Core::AbstractConfig::prefix+Core::AbstractConfig::iconsPath) << std::endl;
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
ConfigQml::UpdatePeriodValues()
{
    QStringList l;
    l << "2 hour" << "1 hour" << "30 min" << "10 min" << "never";
    return l;
}
QStringList
ConfigQml::UpdatePeriodKeys()
{
    QStringList l;
    l << QString::number(2*60*60) << QString::number(60*60) << QString::number(30*60);
    l << QString::number(10*60) << QString::number(INT_MAX);
    return l;
}
void
ConfigQml::UpdatePeriod(const int period){
    ConfigQml::Config::UpdatePeriod(period);
}
int
ConfigQml::UpdatePeriod(){
    return ConfigQml::Config::UpdatePeriod();
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
