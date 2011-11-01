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
    db = new Core::DatabaseSqlite("");

    thread = new UpdateThread();
    connect(thread, SIGNAL(finished()), this, SLOT(downloadFinishedSlot()));

    wind_list << "m/s" << "km/h" << "mi/h";
}

ConfigQml::ConfigQml():QObject(),Core::Config(){
db = new Core::DatabaseSqlite("");

    thread = new UpdateThread();
    connect(thread, SIGNAL(finished()), this, SLOT(downloadFinishedSlot()));

    wind_list << "m/s" << "km/h" << "mi/h";
}

QString
ConfigQml::iconset(){
    QString c;
    c = ConfigQml::Config::iconSet().c_str();
    return c;
}
QStringList
ConfigQml::icon_list()
{
    QStringList icon_list;
    Dirent *dp = 0;
    DIR *dir_fd = opendir((Core::AbstractConfig::prefix+Core::AbstractConfig::iconsPath).c_str());
    if(dir_fd){
        while((dp = readdir(dir_fd))){
            std::string name = dp->d_name;
            if(name == "." || name == "..")
                continue;
            if(dp->d_type == DT_DIR && name[0] != '.'){
                try{
                    icon_list << QString::fromStdString(name);
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
    return icon_list;
}
void
ConfigQml::set_iconset(QString c)
{
    ConfigQml::Config::iconSet(c.toStdString());
    saveConfig();
    refreshconfig();
}

QString
ConfigQml::iconspath(){
    return ConfigQml::Config::iconspath().c_str(); 
}

QString
ConfigQml::imagespath(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("/images");
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

QStringList ConfigQml::temperature_list()
{
    QStringList l;
    l << "C" << "F";
    return l;
}
void ConfigQml::temperature_unit(QString c)
{
    ConfigQml::Config::TemperatureUnit(c.toStdString());
    saveConfig();
    refreshconfig();
}
QString
ConfigQml::windspeedunit(){
    QString c;
    //c = QString(QString::fromUtf8(_(ConfigQml::Config::WindSpeedUnit().c_str())));
    c = ConfigQml::Config::WindSpeedUnit().c_str();
    return c;
}

QStringList 
ConfigQml::windspeed_list()
{
    QStringList l;
    for (int i=0; i < wind_list.size(); i++){
        l.append(QString(QString::fromUtf8(_(wind_list.at(i).toStdString().c_str()))));
    }
    return l;
}

void 
ConfigQml::windspeed_unit(int index)
{
    ConfigQml::Config::WindSpeedUnit(wind_list.at(index).toStdString());
    saveConfig();
    refreshconfig();
}

bool
ConfigQml::fullscreen()
{
    return ConfigQml::Config::Fullscreen();
}
void
ConfigQml::setfullscreen(bool c)
{
    ConfigQml::Config::Fullscreen(c);
    saveConfig();
    refreshconfig();
}

bool
ConfigQml::gps()
{
    return ConfigQml::Config::Gps();
}
void
ConfigQml::setgps(bool c)
{
    if (c)
        enableGps();
    ConfigQml::Config::Gps(c);
    saveConfig();
    refreshconfig();
}

QString
ConfigQml::version(){
    QString c;
    c = QString(OMWEATHER_VERSION);
    return c;
}


QColor
ConfigQml::fontcolor(){
    QColor c;
    c.setNamedColor(ConfigQml::Config::FontColor().c_str());
    return c;
}

QStringList
ConfigQml::stations()
{
    QStringList l;
    for (unsigned int i=0; i<stationsList().size(); i++){
        l << QString::fromUtf8(stationsList().at(i)->name().c_str());
    }
    return l;
}
void
ConfigQml::removeStation(int index)
{
    stationsList().erase(stationsList().begin() + index);
    //stationlist->erase(stationlist->begin()+index);
    //ConfigQml::Config::stationsList(*stationlist);
    if (this->stationsList().size() > 0){
        this->current_station_id(0);
    }
    saveConfig();
    refreshconfig();
}
QStringList
ConfigQml::Sources()
{
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sourcesPath;
    Core::SourceList *sourcelist = new Core::SourceList(path);
    QStringList l;
    for (unsigned int i=0; i<sourcelist->size(); i++){
        l << QString::fromStdString(sourcelist->at(i)->name());
    }
    return l;
}
QStringList
ConfigQml::Countries(QString source)
{
    QStringList l;
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    if (source == "") return l;
    QString filename(source);
    filename.append(".db");
    filename.prepend(path.c_str());
    if (!this->db) {
        this->db->set_databasename(filename.toStdString());
    }else {
        this->db->set_databasename(filename.toStdString());

    }
    if (!this->db->open_database()){
        l << "error open db";
        return l;
    }
    Core::listdata * list = db->create_countries_list();
    Core::listdata::iterator cur;

    if (!list)
        return l;
    for (cur=list->begin(); cur<list->end(); cur++){
        l << QString::fromStdString((*cur).second);
        /*if (isKeys) {
            l << QString::fromStdString((*cur).first);
        }else {
            l << QString::fromStdString((*cur).second);
        }*/
    }

    return l;
}

int
ConfigQml::getCountryId(int index)
{
    Core::listdata * list = db->create_countries_list();

    Core::listdata::iterator cur;

    if (!list)
        return 0;
    cur = list->begin()+index;
    return atoi((*cur).first.c_str());
}
int
ConfigQml::getRegionId(int country, int index)
{
    Core::listdata * list = db->create_region_list(getCountryId(country));

    Core::listdata::iterator cur;

    if (!list)
        return 0;
    cur = list->begin()+index;
    return atoi((*cur).first.c_str());
}
QString
ConfigQml::getCityId(int region_id, int index)
{
    Core::listdata * list = db->create_stations_list(region_id);
    Core::listdata::iterator cur;

    if (!list)
        return 0;
    cur = list->begin()+index;
    return QString::fromStdString((*cur).first);
}

QStringList
ConfigQml::Regions(int index)
{
    QStringList l;

    Core::listdata * list = db->create_region_list(getCountryId(index));

    if (list->size() == 0) return l;
    Core::listdata::iterator cur;
    for (cur=list->begin(); cur<list->end(); cur++)
        l << QString::fromStdString((*cur).second);

    return l;
}
QStringList
ConfigQml::Cities(int country_index, int index)
{
    QStringList l;
    Core::listdata * list = db->create_stations_list(getRegionId(country_index, index));

    if (list->size() == 0) return l;
    Core::listdata::iterator cur;
    for (cur=list->begin(); cur<list->end(); cur++)
        l << QString::fromStdString((*cur).second);

    return l;
}
void
ConfigQml::saveStation1(QString city_id, QString city_name, QString region, QString country,
                        QString source, int source_id, bool gps)
{
    Core::Station *station;
    std::string code = city_id.toStdString();

    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sourcesPath;
    Core::SourceList *sourcelist = new Core::SourceList(path);

    std::string url_template = sourcelist->at(source_id)->url_template();
    std::string url_for_view = sourcelist->at(source_id)->url_for_view();

    char forecast_url[4096];
    snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), code.c_str());
    char view_url[4096];
    snprintf(view_url, sizeof(view_url)-1, url_for_view.c_str(), code.c_str());

    station = new Core::Station(
                source.toStdString(),
                code,
                city_name.toUtf8().data(),
                country.toStdString(),
                region.toStdString(),
                forecast_url,
                view_url,
                gps);
    std::string filename(Core::AbstractConfig::getConfigPath());
    filename += source.toStdString();
    filename += "_";
    filename += code;
    station->fileName(filename);
    station->converter(sourcelist->at(source_id)->binary());

    stationsList().push_back(station);
    //ConfigQml::Config::stationsList(*stationlist);
    saveConfig();
    refreshconfig();

}
void
ConfigQml::saveStation(int city_id, QString city,
                       int region_id, QString region,
                       int country_id, QString country,
                       int source_id, QString source)
{
    Core::Station *station;
    region_id = getRegionId(country_id, region_id);
    country_id = getCountryId(country_id);
    //city_id = getCityId(region_id, city_id);
    //std::string code = city_id.toStdString();
    std::string code = getCityId(region_id, city_id).toStdString();

    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sourcesPath;
    Core::SourceList *sourcelist = new Core::SourceList(path);

    std::string url_template = sourcelist->at(source_id)->url_template();
    std::string url_for_view = sourcelist->at(source_id)->url_for_view();

    char forecast_url[4096];
    snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), code.c_str());
    char view_url[4096];
    snprintf(view_url, sizeof(forecast_url)-1, url_for_view.c_str(), code.c_str());

    station = new Core::Station(
                source.toStdString(),
                code,
                city.toUtf8().data(),
                country.toStdString(),
                region.toStdString(),
                forecast_url,
                view_url,
                false);
    std::string filename(Core::AbstractConfig::getConfigPath());
    filename += source.toStdString();
    filename += "_";
    filename += code;
    station->fileName(filename);
    station->converter(sourcelist->at(source_id)->binary());

    stationsList().push_back(station);
    //ConfigQml::Config::stationsList(*stationlist);
    saveConfig();
    refreshconfig();

}

QString
ConfigQml::stationname()
{
    QString qstr = "";
    return qstr.fromUtf8(ConfigQml::Config::stationname().c_str()); 
}

QString
ConfigQml::nextstationname()
{
    QString qstr = "";
    return qstr.fromUtf8(ConfigQml::Config::nextstationname().c_str()); 
}

QString
ConfigQml::prevstationname()
{
    QString qstr = "";
    return qstr.fromUtf8(ConfigQml::Config::prevstationname().c_str()); 
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

QString
ConfigQml::source()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->sourceName().c_str();
    else
        return QString();
}

QString
ConfigQml::viewURL()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->viewURL().c_str();
    else
        return QString();
}

void
ConfigQml::changestation()
{
    if ((uint)(this->current_station_id() + 1) < this->stationsList().size()){
       this->current_station_id(this->current_station_id() + 1);

   }else {
       this->current_station_id(0);
   }
   this->saveConfig();
}

void
ConfigQml::nextstation()
{
    if ((uint)(this->current_station_id() + 1) < this->stationsList().size()){
       this->current_station_id(this->current_station_id() + 1);

   }else {
       this->current_station_id(0);
   }
   this->saveConfig();
}

void
ConfigQml::prevstation()
{
    if ((uint)(this->current_station_id() - 1) >= 0){
       this->current_station_id(this->current_station_id() - 1);

   }else {
       this->current_station_id(stationsList().size());
   }
   this->saveConfig();
}
void
ConfigQml::refreshconfig(){
    emit ConfigQml::iconsetChanged();
    emit ConfigQml::iconspathChanged();
    emit ConfigQml::imagespathChanged();
    emit ConfigQml::temperatureunitChanged();
    emit ConfigQml::windspeedunitChanged();
    emit ConfigQml::fontcolorChanged();
    emit ConfigQml::stationnameChanged();
    emit ConfigQml::configChanged();
}

void
ConfigQml::updatestations()
{
    thread->start();

}

void
ConfigQml::showweb()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        QDesktopServices::openUrl(QUrl(this->stationsList().at(this->current_station_id())->viewURL().c_str()));     
}

void
ConfigQml::downloadFinishedSlot()
{
    emit configChanged();
}
void
ConfigQml::runsetting()
{
    char *args[] = {"/usr/bin/omweather-settouch", (char *) 0 };

    pid_t pID = fork();
    if (pID == 0)
        execv("/usr/bin/omweather-settouch", args );
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

QString
ConfigQml::tr(QString str)
{
    return QString(QString::fromUtf8(_(str.toUtf8())));
}
void
ConfigQml::enableGps()
{
    GpsPosition *gps = new GpsPosition();
    qDebug() << "create gps, add slot";
    connect(gps, SIGNAL(findCoord(double, double)), this, SLOT(addGpsStation(double, double)));
    //addGpsStation(55.1882, 30.2177);
}
void
ConfigQml::addGpsStation(double latitude, double longitude)
{
    Core::DatabaseSqlite *db_w = new Core::DatabaseSqlite("");
    std::string country, region, code, name;
    int source_id = 0;
    qDebug() << "gggggggggg lat=" << latitude << " lon" << longitude;
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    QString filename = "weather.com";
    filename.append(".db");
    filename.prepend(path.c_str());
    db_w->set_databasename(filename.toStdString());
    if (!db_w->open_database()){
        qDebug() << "error open database";
        return;
    }
    db_w->get_nearest_station(latitude, longitude, country, region, code, name);
    qDebug() << "find station " << name.c_str();

    path = Core::AbstractConfig::prefix;
    path += Core::AbstractConfig::sourcesPath;
    Core::SourceList *sourcelist = new Core::SourceList(path);

    for (int i=0; i < sourcelist->size(); i++){
        if (sourcelist->at(i)->name().compare("weather.com") == 0)
            source_id = i;
    }

    saveStation1(QString::fromStdString(code), QString::fromStdString(name)+" (GPS)", QString::fromStdString(region),
                 QString::fromStdString(country), "weather.com", source_id, true);
}
