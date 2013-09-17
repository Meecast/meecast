/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2013 Vlad Vasilyeu
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

#include <FApp.h>
#include <FIo.h>
int convert_station_openweathermaporg_data(const char *days_data_path, const char *result_file, const char *current_data_path, const char *hours_data_path );
int convert_station_gismeteo_data(const char *days_data_path, const char *result_file, const char *current_data_path);
int convert_station_forecacom_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data );
int convert_station_hkogovhk_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data );
int convert_station_bomgovau_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data );
int convert_station_yrno_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data );

using namespace Tizen::Base;
using namespace Tizen::Io;
using namespace Tizen::Ui;
using namespace Tizen::Net::Http;
using namespace Tizen::Ui::Controls;


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
                 double latitude, double longitude ): __pHttpSession(null){

        _sourceName = new std::string(source_name);
        _id = new std::string(id);
        _name = new std::string(name);
        _country = new std::string(country);
        _region = new std::string(region);
        _forecastURL = new std::string(forecastURL);
        _detailURL = new std::string(detailURL);
        _hoursURL = new std::string(hoursURL);
        _cookie = new std::string(cookie);
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
        _downloading = NONE;

    }
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const std::string& source_name, const std::string& id, const std::string& name,
                     const std::string& country, const std::string& region, const bool gps, double latitude, double longitude): __pHttpSession(null){
        AppLog("STation ");
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

        std::string path;

        path =  (const char*) (Tizen::Base::Utility::StringUtil::StringToUtf8N(App::GetInstance()->GetAppResourcePath())->GetPointer());

        AppLog("STation888p %s", path.c_str());
        AppLog("Region %s", _region->c_str());
        AppLog("Country %s", _country->c_str());
        AppLog("Source %s", source_name.c_str());
        AppLog("Station %s", _name->c_str());
        AppLog("Code %s", _id->c_str());
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

        AppLog("STation!!! ");
        char forecast_url[4096];
        snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), id.c_str());
        char forecast_detail_url[4096];
        snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, url_detail_template.c_str(), id.c_str());
        char forecast_hours_url[4096];
        snprintf(forecast_hours_url, sizeof(forecast_hours_url)-1, url_hours_template.c_str(), id.c_str());
        char view_url[4096];
        snprintf(view_url, sizeof(view_url)-1, url_for_view.c_str(), id.c_str());
        char map_url[4096];
        char basemap_url[4096];
        memset(map_url, 0, sizeof(map_url));
        memset(basemap_url, 0, sizeof(basemap_url));
        if (url_for_map.length() > 0) {
            if (sourcelist->at(source_id)->map_type() == 1) 
                snprintf(map_url, sizeof(map_url)-1, url_for_map.c_str(), _latitude, _longitude);
            if (sourcelist->at(source_id)->map_type() == 2) 
                snprintf(map_url, sizeof(map_url)-1, url_for_map.c_str(), _longitude - 1, _latitude - 1, _longitude + 1, _latitude + 1);
            fprintf(stderr,"map_url: %s\n", map_url);
        }
        if (base_map_url.length() > 0) {
            if (sourcelist->at(source_id)->map_type() == 1) 
                snprintf(basemap_url, sizeof(basemap_url)-1, base_map_url.c_str(), _latitude, _longitude);
            if (sourcelist->at(source_id)->map_type() == 2) 
                snprintf(basemap_url, sizeof(basemap_url)-1, base_map_url.c_str(), _longitude  - 1, _latitude - 1, _longitude  + 1, _latitude + 1);
            fprintf(stderr,"map_url: %s\n", basemap_url);
        }

        std::string filename("");
        filename += source_name;
        filename += "_";
        filename += id;
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
         _viewURL = new std::string(view_url);

         /* Hack for yr.no */
         if  (source_name=="yr.no"){
             std::replace(_forecastURL->begin(), _forecastURL->end(),'#', '/');
             std::replace(_viewURL->begin(), _viewURL->end(),'#', '/');
             std::replace(_detailURL->begin(), _detailURL->end(),'#', '/');
         }
        _fileName = new std::string(filename);
        _converter = new std::string(sourcelist->at(source_id)->binary());

        _downloading = NONE;
        delete sourcelist;

    }
////////////////////////////////////////////////////////////////////////////////
    Station::~Station(){
        AppLog("Delete Station!!!!");
        delete _sourceName;
        delete _id;
        delete _name;
        delete _country;
        delete _region;
        delete _forecastURL;
        delete _detailURL;
        delete _hoursURL;
        delete _cookie;
        delete _viewURL;
        delete _mapURL;
        delete _basemapURL;
        if(_data)
            delete _data;
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
        _viewURL = new std::string(*(station._viewURL));
        _mapURL = new std::string(*(station._mapURL));
        _basemapURL = new std::string(*(station._basemapURL));
        _fileName = new std::string(*(station._fileName));
        _converter = new std::string(*(station._converter));
        _gps = station._gps;
        _latitude = station._latitude;
        _downloading = station._downloading;
        _longitude = station._longitude;
        __pHttpSession = station.__pHttpSession;
       AppLog("New Station!!!!");
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
    void Station::converter(const std::string& converter)
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
     char buffer_file[2048];
     char buffer_detail_file[2048];
     snprintf(buffer_detail_file, sizeof(buffer_detail_file) -1, "%s.detail.orig", this->fileName().c_str());
     snprintf(buffer_file, sizeof(buffer_file) -1, "%s.orig", this->fileName().c_str());

     std::cerr<<"Convert Data in core"<<std::endl;
     std::cerr<<"ddd "<<this->fileName().c_str() <<std::endl;
     std::cerr<<"oooo "<<this->converter().c_str()<<std::endl;
   /*  run_convert_js((char*)this->converter().c_str(), buffer_file, (char*)this->fileName().c_str(), buffer_detail_file); */
     return true;

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
        std::string command;

        char buffer_file[2048];
        RequestId reqId = 0;
        force = false;
        result res = E_SUCCESS;
	    result r = E_SUCCESS;
        AppLog ("_downloading  %i", _downloading);


        if  (this->sourceName() == "openweathermap.org" &&  _downloading == HOURS_FORECAST_DONE){
            snprintf(buffer_file, sizeof(buffer_file) -1, "%s.timezone", this->fileName().c_str());
            if (Tizen::Io::File::IsFileExist(buffer_file))
                Tizen::Io::File::Remove(buffer_file);

            _downloading = TIMEZONE;
            AppLog("First step ");
            r = RequestHttpGet();
            if (r == E_INVALID_SESSION){
                AppLog("Problem with downloading1");
                delete __pHttpSession;
                __pHttpSession = null;
                _downloading = NONE;
                return false;
            }
            else if (IsFailed(r)){
                AppLog("Problem with downloading2");
                _downloading = NONE;
                return false;
            }
        }else{
            if  (this->hoursURL() != "" &&  _downloading == DETAIL_FORECAST_DONE){
                snprintf(buffer_file, sizeof(buffer_file) -1, "%s.hours.orig", this->fileName().c_str());
                if (Tizen::Io::File::IsFileExist(buffer_file))
                    Tizen::Io::File::Remove(buffer_file);

                _downloading = HOURS_FORECAST;
                AppLog("First step ");
                r = RequestHttpGet();
                if (r == E_INVALID_SESSION){
                    AppLog("Problem with downloading1");
                    delete __pHttpSession;
                    __pHttpSession = null;
                    _downloading = NONE;
                    return false;
                }
                else if (IsFailed(r)){
                    AppLog("Problem with downloading2");
                    _downloading = NONE;
                    return false;
                }
            }else{
                if  (this->detailURL() != "" &&  _downloading == FORECAST_DONE){
                    snprintf(buffer_file, sizeof(buffer_file) -1, "%s.detail.orig", this->fileName().c_str());
                    if (Tizen::Io::File::IsFileExist(buffer_file))
                        Tizen::Io::File::Remove(buffer_file);

                    _downloading = DETAIL_FORECAST;
                    AppLog("First step ");
                    r = RequestHttpGet();
                    if (r == E_INVALID_SESSION){
                        AppLog("Problem with downloading1");
                        delete __pHttpSession;
                        __pHttpSession = null;
                        _downloading = NONE;
                        return false;
                    }
                    else if (IsFailed(r)){
                        AppLog("Problem with downloading2");
                        _downloading = NONE;
                        return false;
                    }
                }else {
                    if  (_downloading == FORECAST_DONE){
                        _downloading = NONE;
                        return true;
                    }
                }
            }
        }
        r = E_SUCCESS;
        if (_downloading == NONE){
            AppLog ("Begin download FORECAST");
            _downloading = FORECAST;
            snprintf(buffer_file, sizeof(buffer_file) -1, "%s.orig", this->fileName().c_str());
            if (Tizen::Io::File::IsFileExist(buffer_file))
                Tizen::Io::File::Remove(buffer_file);

            AppLog("Second step ");
            r = RequestHttpGet();
            if (r == E_INVALID_SESSION)
            {
                AppLog("Problem with downloading3");
                delete __pHttpSession;
                __pHttpSession = null;
                _downloading = NONE;
            }
            else if (IsFailed(r))
            {
                AppLog("Problem with downloading4");
                _downloading = NONE;
            }
        }
        return true;




#if 0
        if (Downloader::downloadData(this->fileName()+".orig", this->forecastURL(), this->cookie(), command)) {
        /* TimeZone */
        if (this->sourceName() == "openweathermap.org"){
            std::string TZUrl;

            char lon[32];
            char lat[32];
            snprintf(lat, 32, "%g", this->latitude());
            snprintf(lon, 32, "%g", this->longitude());
            TZUrl = "http://api.geonames.org/timezone?lat=" + std::string(lat)  + "&lng=" + std::string(lon) + "&username=omweather";
            /* std::cerr<<"URL "<<TZUrl<<std::endl; */
            if (Downloader::downloadData(this->fileName()+".timezone", TZUrl, this->cookie())) {
                result = true;
            }else{
                std::cerr<<"ERROR downloading of TimeZone  "<< TZUrl <<std::endl;
                result = false;
        }
 
        }
        /* Weather Forecast */
        if (Downloader::downloadData(this->fileName()+".orig", this->forecastURL(), this->cookie())) {
            result = true;
        }else{
            std::cerr<<"ERROR downloading  "<<this->forecastURL()<<std::endl;
            result = false;
        }
       if ((result) && (this->detailURL() != "") && (Downloader::downloadData(this->fileName()+".detail.orig", this->detailURL(), this->cookie()))){
            if ((this->hoursURL()!="") && (Downloader::downloadData(this->fileName()+".hours.orig", this->hoursURL(), this->cookie()))){
                command = this->converter()+ " " + this->fileName() + ".orig " + this->fileName()+" " + this->fileName()+".detail.orig" + " " + this->fileName()+ ".hours.orig";
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

        return result;
#endif
    }

////////////////////////////////////////////////////////////////////////////////
void
Station::run_converter(){

    AppLog("RUN CONVERTER ");
    /* TO DO fixed for all sources */
    Tizen::Base::String dirPath;
    dirPath = App::GetInstance()->GetAppDataPath();
    Tizen::Base::String forecast_file;
    forecast_file.Append(dirPath);
    forecast_file.Append(this->fileName().c_str());
    forecast_file.Append(".orig");
    Tizen::Base::String result_file;
    result_file.Append(dirPath);
    result_file.Append(this->fileName().c_str());
    Tizen::Base::String detail_file;
    detail_file.Append(dirPath);
    detail_file.Append(this->fileName().c_str());
    detail_file.Append(".detail.orig");
    Tizen::Base::String hours_file;
    hours_file.Append(dirPath);
    hours_file.Append(this->fileName().c_str());
    hours_file.Append(".hours.orig");
    AppLog("SOurce %s",_sourceName->c_str());
    if (*_sourceName == "openweathermap.org"){
    AppLog("openweathermap.org");
        convert_station_openweathermaporg_data(
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(forecast_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(result_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(detail_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(hours_file)->GetPointer());
        AppLog("openweathermap.org");
    }
    if (*_sourceName =="gismeteo.ru"){
        convert_station_gismeteo_data(
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(forecast_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(result_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(detail_file)->GetPointer());
        AppLog("gismeteo.ru");
    }
    if (*_sourceName =="foreca.com"){
        convert_station_forecacom_data(
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(forecast_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(result_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(detail_file)->GetPointer());
        AppLog("foreca.com");
    }
    if (*_sourceName =="hko.gov.hk"){
        convert_station_hkogovhk_data(
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(forecast_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(result_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(detail_file)->GetPointer());
        AppLog("hko.gov.hk");
    }
    if (*_sourceName =="bom.gov.au"){
        convert_station_bomgovau_data(
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(forecast_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(result_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(detail_file)->GetPointer());
        AppLog("bom.gov.au");
    }
    if (*_sourceName =="yr.no"){
        convert_station_yrno_data(
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(forecast_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(result_file)->GetPointer(), 
            (const char *)Tizen::Base::Utility::StringUtil::StringToUtf8N(detail_file)->GetPointer());
        AppLog("yr.no");
    }


}


////////////////////////////////////////////////////////////////////////////////
    void Station::updateSource(const Source* source){
        _source = const_cast<Source*>(source);
    }
////////////////////////////////////////////////////////////////////////////////
    Source* Station::getSourceByName()
    {
        std::string path =  (const char*) (Tizen::Base::Utility::StringUtil::StringToUtf8N(App::GetInstance()->GetAppResourcePath())->GetPointer());
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
bool 
Station::isupdating(){
    if (_downloading == NONE)
        return false;
    else
        return true;
}
////////////////////////////////////////////////////////////////////////////////


result
Station::RequestHttpGet(void)
{
	result r = E_SUCCESS;
	HttpTransaction* pHttpTransaction = null;
	HttpRequest* pHttpRequest = null;
    String str;

    AppLog("Station::RequestHttpGet");
	if (__pHttpSession == null)
	{
		__pHttpSession = new (std::nothrow) HttpSession();
        if (_downloading == FORECAST)
            Tizen::Base::Utility::StringUtil::Utf8ToString(this->forecastURL().c_str(), str);
        if (_downloading == DETAIL_FORECAST)
            Tizen::Base::Utility::StringUtil::Utf8ToString(this->detailURL().c_str(), str);
        if (_downloading == HOURS_FORECAST)
            Tizen::Base::Utility::StringUtil::Utf8ToString(this->hoursURL().c_str(), str);
        if (_downloading == TIMEZONE){
            if (this->sourceName() == "openweathermap.org"){
                std::string TZUrl;
                char lon[32];
                char lat[32];
                setlocale(LC_NUMERIC, "C");
                snprintf(lat, 32, "%g", this->latitude());
                snprintf(lon, 32, "%g", this->longitude());
                setlocale(LC_NUMERIC, "");
                TZUrl = "http://api.geonames.org/timezone?lat=" + std::string(lat)  + "&lng=" + std::string(lon) + "&username=omweather";
                Tizen::Base::Utility::StringUtil::Utf8ToString(TZUrl.c_str(), str);
             }
        }
        AppLog("URL %S",str.GetPointer());
		r = __pHttpSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, null, str, null);
		if (IsFailed(r))
		{
			delete __pHttpSession;
			__pHttpSession = null;
			AppLogException("Failed to create the HttpSession.");
			goto CATCH;
		}

		r = __pHttpSession->SetAutoRedirectionEnabled(true);
		TryCatch(r == E_SUCCESS, , "Failed to set the redirection automatically.");
	}

	pHttpTransaction = __pHttpSession->OpenTransactionN();
	r = GetLastResult();
	TryCatch(pHttpTransaction != null, , "Failed to open the HttpTransaction.");

	r = pHttpTransaction->AddHttpTransactionListener(*this);
	TryCatch(r == E_SUCCESS, , "Failed to add the HttpTransactionListener.");

	pHttpRequest = const_cast< HttpRequest* >(pHttpTransaction->GetRequest());

    if (_downloading == FORECAST)
        Tizen::Base::Utility::StringUtil::Utf8ToString(this->forecastURL().c_str(), str);
    if (_downloading == DETAIL_FORECAST)
        Tizen::Base::Utility::StringUtil::Utf8ToString(this->detailURL().c_str(), str);
    if (_downloading == HOURS_FORECAST)
        Tizen::Base::Utility::StringUtil::Utf8ToString(this->hoursURL().c_str(), str);
    if (_downloading == TIMEZONE){
        if (this->sourceName() == "openweathermap.org"){
            std::string TZUrl;
            char lon[32];
            char lat[32];
            setlocale(LC_NUMERIC, "C");
            snprintf(lat, 32, "%g", this->latitude());
            snprintf(lon, 32, "%g", this->longitude());
            setlocale(LC_NUMERIC, "");
            TZUrl = "http://api.geonames.org/timezone?lat=" + std::string(lat)  + "&lng=" + std::string(lon) + "&username=omweather";
            Tizen::Base::Utility::StringUtil::Utf8ToString(TZUrl.c_str(), str);
         }
    }

	r = pHttpRequest->SetUri(str);
	TryCatch(r == E_SUCCESS, , "Failed to set the uri.");

	r = pHttpRequest->SetMethod(NET_HTTP_METHOD_GET);
	TryCatch(r == E_SUCCESS, , "Failed to set the method.");
    if (this->cookie() != ""){
        Tizen::Base::Utility::StringUtil::Utf8ToString(this->cookie().c_str(), str);
        r = pHttpRequest->SetCookie(L"Cookie: " + str);
	    TryCatch(r == E_SUCCESS, , "Failed to set the cookie.");
    }
	r = pHttpTransaction->Submit();
	TryCatch(r == E_SUCCESS, , "Failed to submit the HttpTransaction.");

	return r;

CATCH:

	delete pHttpTransaction;
	pHttpTransaction = null;
    _downloading = NONE;
	AppLog("RequestHttpGet() failed. (%s)", GetErrorMessage(r));
	return r;
}


void
Station::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	AppLog("OnTransactionReadyToRead");
    char buffer_file[2048];
    File file;

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		if (pHttpHeader != null)
		{
			String* tempHeaderString = pHttpHeader->GetRawHeaderN();
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();
            
            if (_downloading == FORECAST){
                AppLog("FORECAST");
                snprintf(buffer_file, sizeof(buffer_file) -1, "%s.orig", this->fileName().c_str());
            }
            if (_downloading == DETAIL_FORECAST){
                AppLog("DETAILFORECAST");
                snprintf(buffer_file, sizeof(buffer_file) -1, "%s.detail.orig", this->fileName().c_str());
            }
            if (_downloading == HOURS_FORECAST){
                AppLog("HOURSFORECAST");
                snprintf(buffer_file, sizeof(buffer_file) -1, "%s.hours.orig", this->fileName().c_str());
            }
            if (_downloading == TIMEZONE){
                AppLog("TIMEZONE");
                snprintf(buffer_file, sizeof(buffer_file) -1, "%s.timezone", this->fileName().c_str());
            }


            AppLog ("File name %S", (App::GetInstance()->GetAppDataPath() + buffer_file).GetPointer());
            result r = E_SUCCESS;
            // Decodes a UTF-8 string into a Unicode string
            String str;
            Tizen::Base::Utility::StringUtil::Utf8ToString(buffer_file, str);
            // Creates file
            r = file.Construct(str, "a+");
            r = file.Write(*pBuffer);
            file.Flush();
            delete tempHeaderString;
            delete pBuffer;
		}
	}
}

void
Station::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("OnTransactionAborted(%s)", GetErrorMessage(r));
    _downloading = NONE;
	delete &httpTransaction;
}

void
Station::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("OnTransactionReadyToWrite");
}

void
Station::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool authRequired)
{
	AppLog("OnTransactionHeaderCompleted");
}

void
Station::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLog("OnTransactionCompleted");
	delete &httpTransaction;
    if ( _downloading == TIMEZONE){
        AppLog("_downloading = TIMEZONE");
        _downloading = NONE;
        run_converter();
        delete __pHttpSession;
        __pHttpSession = null;
    }
    if (this->hoursURL() != "" && _downloading == HOURS_FORECAST){
        if (this->sourceName() == "openweathermap.org"){
            AppLog("_downloading = NONE");
            _downloading = HOURS_FORECAST_DONE;
            delete __pHttpSession;
            __pHttpSession = null;
            updateData(true);
        }else{
            AppLog("_downloading = NONE");
            _downloading = NONE;
            run_converter();
            delete __pHttpSession;
            __pHttpSession = null;
        }
    }
    if (this->hoursURL() != "" && _downloading == DETAIL_FORECAST){
        AppLog("_downloading = DETAIL_FORECAST_DONE");
        _downloading = DETAIL_FORECAST_DONE;
      //  _downloading = NONE;
        delete __pHttpSession;
        __pHttpSession = null;
        updateData(true);
    }
    if (this->hoursURL() == "" && _downloading == DETAIL_FORECAST){

        AppLog("_downloading = NONE2");
        _downloading = NONE;
        run_converter();
        delete __pHttpSession;
        __pHttpSession = null;
    }
    if (_downloading == FORECAST){
        _downloading = FORECAST_DONE;
        delete __pHttpSession;
        __pHttpSession = null;
        updateData(true);
    }

}

void
Station::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert)
{
	AppLog("OnTransactionCertVerificationRequiredN");

	httpTransaction.Resume();

	delete pCert;
}
} // namespace Core
