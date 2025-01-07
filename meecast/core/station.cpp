/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2024 Vlad Vasilyeu
 * Copyright (C) 2006-2011 Pavel Fialko
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


#include "station.h"
#include <iostream> 
#include <cstdlib>
#include <algorithm>
int convert_station_openweathermaporg_data(const char *days_data_path, const char *result_file, const char *current_data_path, const char *hours_data_path );
int convert_station_gismeteo_data(const char *days_data_path, const char *result_file, const char *current_data_path);
int convert_station_forecacom_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data );
int convert_station_hkogovhk_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data );
int convert_station_bomgovau_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data );
int convert_station_yrno_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data );
int convert_station_weather_com_data(const char *station_id_with_path, const char *result_file, const char *station_detail_id_with_path);
int convert_station_fmi_fi_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data);

////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
Station::Station(const std::string& source_name, const std::string& id, 
                 const std::string& name,
                 const std::string& country, const std::string& region, 
                 const std::string& forecastURL, const std::string& detailURL,
                 const std::string& hoursURL,
                 const std::string& viewURL, const std::string& mapURL, 
                 const std::string& basemapURL, 
                 const std::string&  cookie, const bool gps, 
                 double latitude, double longitude, const std::string& user_agent){
        _sourceName = new std::string(source_name);
        _id = new std::string(id);
        _name = new std::string(name);
        _country = new std::string(country);
        _region = new std::string(region);
        _forecastURL = new std::string(forecastURL);
        _detailURL = new std::string(detailURL);
        _hoursURL = new std::string(hoursURL);
        _cookie = new std::string(cookie);
        _user_agent = new std::string(user_agent);
        _viewURL = new std::string(viewURL);
        _mapURL = new std::string(mapURL);
        _basemapURL = new std::string(basemapURL);
        _timezone = 0;
        _fileName = new std::string();
        _source = this->getSourceByName();
        _converter = new std::string();
        _gps = gps;
        _latitude = latitude;
        _longitude = longitude;


           
       


    }
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const std::string& source_name, const std::string& id, const std::string& name,
                     const std::string& country, const std::string& region, const bool gps, double latitude, double longitude){
        _sourceName = new std::string(source_name);
        _id = new std::string(id);
        _name = new std::string(name);
        _country = new std::string(country);
        _region = new std::string(region);
        _timezone = 0;
        _source = this->getSourceByName();
        _gps = gps;
        _latitude = latitude;
        _longitude = longitude;
        std::string path(Core::AbstractConfig::prefix);
        path += Core::AbstractConfig::sourcesPath;
        Core::SourceList *sourcelist = new Core::SourceList(path);
        int source_id = sourcelist->source_id_by_name(source_name);
        std::string url_template = sourcelist->at(source_id)->url_template();
        std::string url_detail_template = sourcelist->at(source_id)->url_detail_template();
        std::string url_hours_template = sourcelist->at(source_id)->url_hours_template();
        std::string url_for_view = sourcelist->at(source_id)->url_for_view();
        std::string url_for_map = sourcelist->at(source_id)->url_for_map();
        std::string base_map_url = sourcelist->at(source_id)->url_for_basemap();
        std::string cookie = sourcelist->at(source_id)->cookie();
        std::string user_agent = sourcelist->at(source_id)->user_agent();

        char forecast_url[4096];
        /* From 2023 Yr.no using lat lon in URL */
        if (source_name=="yr.no"){
            std::string part_of_url;
            setlocale(LC_NUMERIC, "POSIX");
            part_of_url = "lat=" + std::to_string(latitude) + "&" + "lon=" + std::to_string(longitude);
            snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), part_of_url.c_str());
            setlocale(LC_NUMERIC, "");
        }else{
            snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), id.c_str());
        }
        char forecast_detail_url[4096];
        /* For detail url for fmi.fi */

        if (source_name=="fmi.fi"){
            if (_id->find("?station=") != std::string::npos){
                snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, url_detail_template.c_str(), id.substr(_id->find("?station=") + 9, _id->size() - (_id->find("?station=") + 9)).c_str());
            }else{
                snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, url_detail_template.c_str(), id.c_str());
            }
        }else{
            snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, url_detail_template.c_str(), id.c_str());
        }
        char forecast_hours_url[4096];
        snprintf(forecast_hours_url, sizeof(forecast_hours_url)-1, url_hours_template.c_str(), id.c_str());
        char view_url[4096];
        snprintf(view_url, sizeof(view_url)-1, url_for_view.c_str(), id.c_str());
        char map_url[4096];
        char basemap_url[4096];
        memset(map_url, 0, sizeof(map_url));
        memset(basemap_url, 0, sizeof(basemap_url));
        setlocale(LC_NUMERIC, "POSIX");
        if (url_for_map.length() > 0) {
            if (sourcelist->at(source_id)->map_type() == GPS1_TYPE) 
                snprintf(map_url, sizeof(map_url)-1, url_for_map.c_str(), _latitude, _longitude);
            if (sourcelist->at(source_id)->map_type() == GPS2_TYPE) 
                snprintf(map_url, sizeof(map_url)-1, url_for_map.c_str(), _longitude - 1, _latitude - 1, _longitude + 1, _latitude + 1);
            fprintf(stderr,"map_url: %s\n", map_url);
        }
        if (base_map_url.length() > 0) {
            if (sourcelist->at(source_id)->map_type() == GPS1_TYPE) 
                snprintf(basemap_url, sizeof(basemap_url)-1, base_map_url.c_str(), _latitude, _longitude);
            if (sourcelist->at(source_id)->map_type() == GPS2_TYPE) 
                snprintf(basemap_url, sizeof(basemap_url)-1, base_map_url.c_str(), _longitude  - 1, _latitude - 1, _longitude  + 1, _latitude + 1);
            fprintf(stderr,"map_url: %s\n", basemap_url);
        }
        setlocale(LC_NUMERIC, "");

        std::string filename(Core::AbstractConfig::getConfigPath());
        filename += source_name;
        filename += "_";
        filename += id;
        /* Normalization filename */
        std::replace(filename.begin(), filename.end(),'=', '_');
        std::replace(filename.begin(), filename.end(),'?', '_');
        _forecastURL = new std::string(forecast_url);
        _detailURL = new std::string(forecast_detail_url);
        _hoursURL = new std::string(forecast_hours_url);
        _mapURL = new std::string(map_url);
        _basemapURL = new std::string(basemap_url);
        if (source_name == "bom.gov.au"){
           if (_id->find("IDV") == 0)
               _detailURL = new std::string("http://www.bom.gov.au/vic/observations/vicall.shtml");
           if (_id->find("IDN") == 0)
               _detailURL = new std::string("http://www.bom.gov.au/nsw/observations/nswall.shtml");     
           if (_id->find("IDS") == 0)
               _detailURL = new std::string("http://www.bom.gov.au/sa/observations/saall.shtml");     
           if (_id->find("IDT") == 0)
               _detailURL = new std::string("http://www.bom.gov.au/tas/observations/tasall.shtml");     


           if (_name->find("Darwin") == 0)
               _detailURL = new std::string("http://www.bom.gov.au/nt/observations/ntall.shtml");
           if (_name->find("Brisbane") == 0)
               _detailURL = new std::string("http://www.bom.gov.au/qld/observations/qldall.shtml");     
           if (_name->find("Perth") == 0)
               _detailURL = new std::string("http://www.bom.gov.au/wa/observations/waall.shtml");     
           if (_name->find("Broome") == 0)
               _detailURL = new std::string("http://www.bom.gov.au/wa/observations/waall.shtml");     


           //http://www.bom.gov.au/vic/observations/vicall.shtml
           //http://www.bom.gov.au/nsw/observations/nswall.shtml
            filename += "_" + name;
         }
         _cookie = new std::string(cookie);
         _user_agent = new std::string(user_agent);
         _viewURL = new std::string(view_url);

         /* Hack for yr.no */
         if (source_name=="yr.no"){
             std::replace(_forecastURL->begin(), _forecastURL->end(),'#', '/');
             std::replace(_viewURL->begin(), _viewURL->end(),'#', '/');
             std::replace(_detailURL->begin(), _detailURL->end(),'#', '/');
         }
        _fileName = new std::string(filename);
        _converter = new std::string(sourcelist->at(source_id)->binary());

        delete sourcelist;


    }
////////////////////////////////////////////////////////////////////////////////
    Station::~Station(){
        delete _sourceName;
        delete _id;
        delete _name;
        delete _country;
        delete _region;
        delete _forecastURL;
        delete _detailURL;
        delete _hoursURL;
        delete _cookie;
        delete _user_agent;
        delete _viewURL;
        delete _mapURL;
        delete _basemapURL;
        if(_fileName)
            delete _fileName;
        if(_converter)
            delete _converter;
        if (_source)
            delete _source;
    }
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const Station& station){
        _sourceName = new std::string(*(station._sourceName));
        _id = new std::string(*(station._id));
        _name = new std::string(*(station._name));
        _country = new std::string(*(station._country));
        _region = new std::string(*(station._region));
        _forecastURL = new std::string(*(station._forecastURL));
        _detailURL = new std::string(*(station._detailURL));
        _hoursURL = new std::string(*(station._hoursURL));
        _cookie = new std::string(*(station._cookie));
        _user_agent = new std::string(*(station._user_agent));
        _viewURL = new std::string(*(station._viewURL));
        _mapURL = new std::string(*(station._mapURL));
        _basemapURL = new std::string(*(station._basemapURL));
        _fileName = new std::string(*(station._fileName));
        _converter = new std::string(*(station._converter));
        _gps = station._gps;
        _latitude = station._latitude;
        _longitude = station._longitude;
    }
////////////////////////////////////////////////////////////////////////////////
    Station& Station::operator=(const Station& station){
        if(this != &station){
            delete _sourceName;
            _sourceName = new std::string(*(station._sourceName));
            delete _id;
            _id = new std::string(*(station._id));
            delete _name;
            _name = new std::string(*(station._name));
            delete _country;
            _country = new std::string(*(station._country));
            delete _region;
            _region = new std::string(*(station._region));
            delete _forecastURL;
            _forecastURL = new std::string(*(station._forecastURL));
            delete _detailURL;
            _detailURL = new std::string(*(station._detailURL));
            delete _hoursURL;
            _hoursURL = new std::string(*(station._hoursURL));
            delete _cookie;
            _cookie = new std::string(*(station._cookie));
            delete _user_agent;
            _user_agent = new std::string(*(station._user_agent));
            delete _viewURL;
            _viewURL = new std::string(*(station._viewURL));
            delete _fileName;
            _fileName = new std::string(*(station._fileName));
            delete _converter;
            _converter = new std::string(*(station._converter));
            delete _mapURL;
            _mapURL = new std::string(*(station._mapURL));
            delete _basemapURL;
            _basemapURL = new std::string(*(station._basemapURL));

        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
    void Station::name(const std::string& name){
        _name->assign(name);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Station::name() const{
        return *_name;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::id(const std::string& id){
        _id->assign(id);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::id() const{
        return *_id;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::timezone(const int timezone){
        _timezone = timezone;
    }
    ////////////////////////////////////////////////////////////////////////////////
    int Station::station_timezone() const{
        return _timezone;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::sourceName(const std::string& sourceName){
        _sourceName->assign(sourceName);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::sourceName() const{
        return *_sourceName;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::forecastURL(const std::string& forecastURL){
        _forecastURL->assign(forecastURL);
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::mapURL(const std::string& mapURL){
        _mapURL->assign(mapURL);
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::basemapURL(const std::string& basemapURL){
        _basemapURL->assign(basemapURL);
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::detailURL(const std::string& detailURL){
        _sourceName->assign(detailURL);
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::hoursURL(const std::string& hoursURL){
        _sourceName->assign(hoursURL);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::forecastURL() const{
        return *_forecastURL;
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::detailURL() const{
        return *_detailURL;
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::hoursURL() const{
        return *_hoursURL;
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::cookie() const{
        return *_cookie;
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::user_agent() const{
        return *_user_agent;
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::viewURL() const{
        return *_viewURL;
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::mapURL() const{
        return *_mapURL;
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::basemapURL() const{
        return *_basemapURL;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::country(const std::string& country){
        _country->assign(country);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::country() const{
        return *_country;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::region(const std::string& region){
        _region->assign(region);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::region() const{
        return *_region;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::fileName(const std::string& fileName){
        _fileName->assign(fileName);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::fileName() const{
        return *_fileName;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::converter(const std::string &converter)
    {
        _converter->assign(converter);
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::converter() const{
        return *_converter;
    }
//////////////////////////////////////////////////////////////////////////////
    void Station::gps(const bool gps)
    {
        _gps = gps;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Station::gps() const{
        return _gps;
    }
//////////////////////////////////////////////////////////////////////////////
    void Station::latitude(const double latitude)
    {
        _latitude = latitude;
    }
////////////////////////////////////////////////////////////////////////////////
    double Station::latitude() const{
        return _latitude;
    }
//////////////////////////////////////////////////////////////////////////////
    void Station::longitude(const double longitude)
    {
        _longitude = longitude;
    }
////////////////////////////////////////////////////////////////////////////////
    double Station::longitude() const{
        return _longitude;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Station::dataValid(){
        return true;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Station::prepareFile(){
        return true;
    }

///////////////////////////////////////////////////////////////////////////////
    bool Station::convertData(bool force){

     /*
        std::string command;
        bool result = false;
         if (this->detailURL() != "") {
            if (this->hoursURL() != ""){
                command = this->converter()+ " " +  this->fileName() + ".orig " + this->fileName()+" " + this->fileName()+".detail.orig" + " " + this->fileName()+".hours.orig";
                std::cerr<<" EXEC "<<command<<std::endl;
                if (system(command.c_str()) == 0)
                    result = true;
                else
                   result = false;
            }else{ 
                command = this->converter()+ " " +  this->fileName() + ".orig " + this->fileName()+" " + this->fileName()+".detail.orig";
                std::cerr<<" EXEC "<<command<<std::endl;
                if (system(command.c_str()) == 0)
                    result = true;
                else
                   result = false;
            }
         }else{
            command = this->converter()+ " " +  this->fileName() + ".orig " + this->fileName();
            std::cerr<<" EXEC "<<command<<std::endl;
            if (system(command.c_str()) == 0)
                result = true;
            else
               result = false;
        }
        return result;
       */
   }
///////////////////////////////////////////////////////////////////////////////
    bool Station::updateData(bool force){
        bool result = false;
        std::string command;
        std::string *forecastURL;
        std::string *detailURL;
        std::string *hoursURL;
        /* To do */
        std::cerr<<"Station::updateData(bool force)"<<std::endl;
        /* Check connection and if force true update connection */
        force = false;
        forecastURL = new std::string((this->forecastURL())); 
        detailURL = new std::string((this->detailURL()));
        hoursURL = new std::string((this->hoursURL()));
        /* TimeZone */
        if (this->sourceName() == "openweathermap.org"){
            std::string TZUrl;

            char lon[32];
            char lat[32];
            snprintf(lat, 32, "%g", this->latitude());
            snprintf(lon, 32, "%g", this->longitude());
            TZUrl = "http://api.geonames.org/timezone?lat=" + std::string(lat)  + "&lng=" + std::string(lon) + "&username=omweather";
            /* std::cerr<<"URL "<<TZUrl<<std::endl; */
            if (Downloader::downloadData(this->fileName()+".timezone", TZUrl, this->cookie(), this->user_agent())) {
                result = true;
            }else{
                std::cerr<<"ERROR downloading of TimeZone  "<< TZUrl <<std::endl;
                result = false;
            }
            std::string tt ="%@OOHC<a`2333`8/e/b4752768aca384dce7878";
            for (int i = 0; i < tt.size(); i++)
                    tt[i] = tt[i]+1;

            forecastURL->append(tt);
            detailURL->append(tt);
            hoursURL->append(tt);
        }
        /* Weather Forecast */
        if (Downloader::downloadData(this->fileName()+".orig", *forecastURL, this->cookie(), this->user_agent())) {
            result = true;
        }else{
            std::cerr<<"ERROR downloading  "<<this->forecastURL()<<std::endl;
            result = false;
        }
        if ((result) && (this->detailURL() != "") && (Downloader::downloadData(this->fileName()+".detail.orig", *detailURL, this->cookie(), this->user_agent()))){
            if ((this->hoursURL() != "") && (Downloader::downloadData(this->fileName()+".hours.orig", *hoursURL, this->cookie(), this->user_agent()))){
                /*
                command = this->converter()+ " " + this->fileName() + ".orig " + this->fileName()+" " + this->fileName()+".detail.orig" + " " + this->fileName()+ ".hours.orig";
                std::cerr<<" EXEC "<<command<<std::endl;
                if (system(command.c_str()) == 0)
                    result = true;
                else
                   result = false;
                */
            }else{
                /*
                command = this->converter()+ " " +  this->fileName() + ".orig " + this->fileName()+" " + this->fileName()+".detail.orig";
                std::cerr<<" EXEC "<<command<<std::endl;
                if (system(command.c_str()) == 0)
                    result = true;
                else
                   result = false;
                */
            }
        }else{
            /*
            command = this->converter()+ " " +  this->fileName() + ".orig " + this->fileName();
            std::cerr<<" EXEC "<<command<<std::endl;
            if (system(command.c_str()) == 0)
                result = true;
            else
               result = false;
            */
        }
#if 0        
        /* BAseMap */
        if (this->basemapURL() != ""){
            std::string mapfilename(Core::AbstractConfig::getCachePath());
            char map_url[4096];
            char map_url_additional[4096];
            struct stat attrib;
            mapfilename += this->sourceName().c_str();
            mapfilename += "_";
            mapfilename += _id->c_str();
            mapfilename += "_basemap_";
            mapfilename += "%i.png";
            snprintf(map_url, sizeof(map_url)-1, mapfilename.c_str(), 0);
            std::cerr<<map_url<<std::endl;
            /* Check modification time of the last file and download map if it more than 3 hours */
            if ((stat(map_url, &attrib) == 0) &&
                (time(NULL) - attrib.st_mtime > 3600*2.5)&&
                (attrib.st_size > 0)){
                for (int i=4; i>=0; i--){
                    snprintf(map_url, sizeof(map_url)-1, mapfilename.c_str(), i);
                    std::cerr<<map_url<<std::endl;
                    if (stat(map_url, &attrib) == 0){
                        snprintf(map_url_additional, sizeof(map_url_additional)-1,
                                 mapfilename.c_str(), i+1);
                        rename(map_url, map_url_additional);
                    }
                }
            }

            /* snprintf(map_url, sizeof(map_url)-1, mapfilename.c_str(), 0); */
            std::cerr<<map_url<<std::endl;
            if ((stat(map_url, &attrib) != 0)||
                (attrib.st_size == 0) ||
                (stat(map_url, &attrib) == 0) &&
                (time(NULL) - attrib.st_mtime > 3600*2.5)){
                std::cerr<<map_url<<" "<<attrib.st_mtime<< " "<<time(NULL)<< std::endl;
                Downloader::downloadData(map_url, this->basemapURL(), "");
            }
        }

        /* Map */
        if (this->mapURL() != ""){
            std::string mapfilename(Core::AbstractConfig::getCachePath());
            char map_url[4096];
            char map_url_additional[4096];
            struct stat attrib;
            mapfilename += this->sourceName().c_str();
            mapfilename += "_";
            mapfilename += _id->c_str();
            mapfilename += "_map_";
            mapfilename += "%i.png";
            snprintf(map_url, sizeof(map_url)-1, mapfilename.c_str(), 0);
            std::cerr<<map_url<<std::endl;
            /* Check modification time of the last file and download map if it more than 3 hours */
            if ((stat(map_url, &attrib) == 0) &&
                (time(NULL) - attrib.st_mtime > 3600*2.5)&&
                (attrib.st_size > 0)){
                for (int i=4; i>=0; i--){
                    snprintf(map_url, sizeof(map_url)-1, mapfilename.c_str(), i);
                    std::cerr<<map_url<<std::endl;
                    if (stat(map_url, &attrib) == 0){
                        snprintf(map_url_additional, sizeof(map_url_additional)-1,
                                 mapfilename.c_str(), i+1);
                        rename(map_url, map_url_additional);
                    }
                }
            }

            /* snprintf(map_url, sizeof(map_url)-1, mapfilename.c_str(), 0); */
            std::cerr<<map_url<<std::endl;
            if ((stat(map_url, &attrib) != 0)||
                (attrib.st_size == 0) ||
                (stat(map_url, &attrib) == 0) &&
                (time(NULL) - attrib.st_mtime > 3600*2.5)){
                std::cerr<<map_url<<" "<<attrib.st_mtime<< " "<<time(NULL)<< std::endl;
                Downloader::downloadData(map_url, this->mapURL(), "");
            }
        }

#endif    
        run_converter();
        delete forecastURL;
        delete detailURL;
        delete hoursURL;
        return result;
    }

////////////////////////////////////////////////////////////////////////////////
void
Station::run_converter(){

    /* AppLog("RUN CONVERTER "); */
    /* TO DO fixed for all sources */
    std::string forecast_file_string = "";
    std::string result_file_string  = "";
    std::string detail_file_string = "";
    std::string hours_file_string = "";
   
    forecast_file_string.append(this->fileName().c_str());
    forecast_file_string.append(".orig");
    result_file_string.append(this->fileName().c_str());
    detail_file_string.append(this->fileName().c_str());
    detail_file_string.append(".detail.orig");
    hours_file_string.append(this->fileName().c_str());
    hours_file_string.append(".hours.orig");

    if (*_sourceName == "openweathermap.org"){
        convert_station_openweathermaporg_data(
            (const char *)forecast_file_string.c_str(), 
            (const char *)result_file_string.c_str(), 
            (const char *)detail_file_string.c_str(), 
            (const char *)hours_file_string.c_str());
       /* AppLog("openweathermap.org"); */
    }
    if (*_sourceName =="gismeteo.ru"){
        convert_station_gismeteo_data(
            (const char *)forecast_file_string.c_str(), 
            (const char *)result_file_string.c_str(), 
            (const char *)detail_file_string.c_str());
    }
    if (*_sourceName =="foreca.com"){
        convert_station_forecacom_data(
            (const char *)forecast_file_string.c_str(), 
            (const char *)result_file_string.c_str(), 
            (const char *)detail_file_string.c_str());
    }
    if (*_sourceName =="hko.gov.hk"){
        convert_station_hkogovhk_data(
            (const char *)forecast_file_string.c_str(), 
            (const char *)result_file_string.c_str(), 
            (const char *)detail_file_string.c_str());
    }
    if (*_sourceName =="bom.gov.au"){
        convert_station_bomgovau_data(
            (const char *)forecast_file_string.c_str(), 
            (const char *)result_file_string.c_str(), 
            (const char *)detail_file_string.c_str());
    }
    if (*_sourceName =="yr.no"){
        convert_station_yrno_data(
            (const char *)forecast_file_string.c_str(), 
            (const char *)result_file_string.c_str(), 
            (const char *)detail_file_string.c_str());
    }
    if (*_sourceName =="weather.com"){
        convert_station_weather_com_data(
            (const char *)forecast_file_string.c_str(), 
            (const char *)result_file_string.c_str(), 
            (const char *)detail_file_string.c_str());
    }
    if (*_sourceName =="fmi.fi"){
        convert_station_fmi_fi_data(
            (const char *)forecast_file_string.c_str(), 
            (const char *)result_file_string.c_str(),
            (const char *)detail_file_string.c_str());
    }

}

   
////////////////////////////////////////////////////////////////////////////////
    void Station::updateSource(const Source* source){
        _source = const_cast<Source*>(source);
    }
////////////////////////////////////////////////////////////////////////////////
    Source* Station::getSourceByName()
    {
        std::string path(Core::AbstractConfig::prefix);
        path += Core::AbstractConfig::sourcesPath;
        SourceList *sourcelist = new Core::SourceList(path);
        for (int i=0; i<(int)sourcelist->size(); i++){
            if (_sourceName->compare(sourcelist->at(i)->name()) == 0){
                Source* source = new Core::Source(*sourcelist->at(i));
                delete sourcelist;
                return source; 
            }
        }
        return 0;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
