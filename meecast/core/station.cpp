/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2012 Vlad Vasilyeu
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
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
Station::Station(const std::string& source_name, const std::string& id, const std::string& name,
                     const std::string& country, const std::string& region, 
                     const std::string& forecastURL, const std::string& detailURL,
                     const std::string& viewURL, const std::string&  cookie, const bool gps){
        _sourceName = new std::string(source_name);
        _id = new std::string(id);
        _name = new std::string(name);
        _country = new std::string(country);
        _region = new std::string(region);
        _forecastURL = new std::string(forecastURL);
        _detailURL = new std::string(detailURL);
        _cookie = new std::string(cookie);
        _viewURL = new std::string(viewURL);
        _timezone = 0;
        _fileName = new std::string();
        _source = this->getSourceByName();
        _converter = new std::string();
        _gps = gps;
    }
////////////////////////////////////////////////////////////////////////////////
    Station::Station(const std::string& source_name, const std::string& id, const std::string& name,
                     const std::string& country, const std::string& region, const bool gps){
        _sourceName = new std::string(source_name);
        _id = new std::string(id);
        _name = new std::string(name);
        _country = new std::string(country);
        _region = new std::string(region);
        _timezone = 0;
        _source = this->getSourceByName();
        _gps = gps;
        std::string path(Core::AbstractConfig::prefix);
        path += Core::AbstractConfig::sourcesPath;
        Core::SourceList *sourcelist = new Core::SourceList(path);
        int source_id = sourcelist->source_id_by_name(source_name);
        std::string url_template = sourcelist->at(source_id)->url_template();
        std::string url_detail_template = sourcelist->at(source_id)->url_detail_template();
        std::string url_for_view = sourcelist->at(source_id)->url_for_view();
        std::string cookie = sourcelist->at(source_id)->cookie();

        char forecast_url[4096];
        snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), id.c_str());
        char forecast_detail_url[4096];
        snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, url_detail_template.c_str(), id.c_str());
        char view_url[4096];
        snprintf(view_url, sizeof(view_url)-1, url_for_view.c_str(), id.c_str());

        _forecastURL = new std::string(forecast_url);
        _detailURL = new std::string(forecast_detail_url);
        _cookie = new std::string(cookie);
        _viewURL = new std::string(view_url);

        std::string filename(Core::AbstractConfig::getConfigPath());
        filename += source_name;
        filename += "_";
        filename += id;
        if (source_name == "bom.gov.au")
            filename += "_" + name;
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
        delete _cookie;
        delete _viewURL;
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
        _cookie = new std::string(*(station._cookie));
        _viewURL = new std::string(*(station._viewURL));
        _fileName = new std::string(*(station._fileName));
        _converter = new std::string(*(station._converter));
        _gps = station._gps;
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
            delete _cookie;
            _cookie = new std::string(*(station._cookie));
            delete _viewURL;
            _viewURL = new std::string(*(station._viewURL));
            delete _fileName;
            _fileName = new std::string(*(station._fileName));
            delete _converter;
            _converter = new std::string(*(station._converter));
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
        _sourceName->assign(forecastURL);
    }
    ////////////////////////////////////////////////////////////////////////////////
    void Station::detailURL(const std::string& detailURL){
        _sourceName->assign(detailURL);
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
    std::string& Station::cookie() const{
        return *_cookie;
    }
    ////////////////////////////////////////////////////////////////////////////////
    std::string& Station::viewURL() const{
        return *_viewURL;
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
    ////////////////////////////////////////////////////////////////////////////////
    void Station::gps(const bool gps)
    {
        _gps = gps;
    }
    ////////////////////////////////////////////////////////////////////////////////
    bool Station::gps() const{
        return _gps;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Station::dataValid(){
        return true;
    }
////////////////////////////////////////////////////////////////////////////////
    bool Station::prepareFile(){
        return true;
    }

////////////////////////////////////////////////////////////////////////////////
    bool Station::updateData(bool force){
    bool result = false;
//        if(!force || dataValid())
//            return true;
        std::string command;
        //std::string command = "wget -O ";
        //command = command + this->fileName() + ".orig '" + this->forecastURL() + "'";
        //std::cerr<<" URL "<<command<<std::endl;
        //if (system(command.c_str()) == 0){
        if (Downloader::downloadData(this->fileName()+".orig", this->forecastURL(), this->cookie())) {
            result = true;
        }else{
            std::cerr<<"ERROR downloading  "<<this->forecastURL()<<std::endl;
            result = false;
        }
        if ((result) && (this->detailURL() != "") && (Downloader::downloadData(this->fileName()+".detail.orig", this->detailURL(), this->cookie()))){
            command = this->converter()+ " " +  this->fileName() + ".orig " + this->fileName()+" " + this->fileName()+".detail.orig";
            std::cerr<<" EXEC "<<command<<std::endl;
            if (system(command.c_str()) == 0)
                result = true;
            else
               result = false;
        }else{
            command = this->converter()+ " " +  this->fileName() + ".orig " + this->fileName();
            std::cerr<<" EXEC "<<command<<std::endl;
            if (system(command.c_str()) == 0)
                result = true;
            else
               result = false;
        }
        return result;
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
