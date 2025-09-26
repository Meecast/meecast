/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2025 Vlad Vasilyeu
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


#include "config.h"
#include "station.h"
#include "databasesqlite.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{  
    Config* Config::_self;
    int Config::_refcount;

////////////////////////////////////////////////////////////////////////////////
Config::Config()
{
    /* std::cerr<<"CONFIG CREATE111111!!!!!!!!!!!!!!"<<std::endl; */
    _pathPrefix = new std::string(AbstractConfig::prefix + AbstractConfig::sharePath);
    _iconspath  = new std::string("");
    _iconspath->assign(_pathPrefix->c_str());
    _iconspath->append("iconsets");
    _iconset = new std::string("Meecast");
    _temperature_unit = new std::string("C");
    _wind_speed_unit = new std::string("m/s");
    _pressure_unit = new std::string("mbar");
    _visible_unit = new std::string("m");
    _update_connect = false;
    _fullscreen = false;
    _lockscreen = false;
    _standbyscreen = false;
    _gps = false;
    _splash = true;
    _logo_on_cover = false;
    _wind_on_cover = false;
    _transparency = false;
    _lastupdate_on_cover = true;
    _font_color = new std::string("#00ff00");
    _language = new std::string("System");
    _mod = new std::string("Digia");
    _screen_orientation = new std::string("automatic");
    _nullname = new std::string("");
    _stations = new StationsList;
    _current_station_id = INT_MAX;
    _update_period = INT_MAX;
    _Xleft_corner_of_lockscreen_widget = 0;
    _Yleft_corner_of_lockscreen_widget = 0;
    //std::cerr << Core::AbstractConfig::configPath << std::endl;
    std::string path(AbstractConfig::getConfigPath());
    path += "config.xml";
    _filename = new std::string(path);
    _languages_list = new languages;
    this->InitLanguagesList();
}
////////////////////////////////////////////////////////////////////////////////
void
Config::saveConfig()
{
    std::cerr<<"Config::saveConfig "<<_filename->c_str() <<std::endl;
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

    QDomElement root = doc.createElement("config");
    root.setAttribute("xmlns", "http://omweather.garage.maemo.org/schemas");
    doc.appendChild(root);

    QDomElement el = doc.createElement("base");
    QDomText t = doc.createTextNode(QString::fromStdString(*_pathPrefix));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("iconset");
    t = doc.createTextNode(QString::fromStdString(*_iconset));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("screen_orienation");
    t = doc.createTextNode(QString::fromStdString(*_screen_orientation));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("temperature_unit");
    t = doc.createTextNode(QString::fromStdString(*_temperature_unit));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("wind_speed_unit");
    t = doc.createTextNode(QString::fromStdString(*_wind_speed_unit));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("pressure_unit");
    t = doc.createTextNode(QString::fromStdString(*_pressure_unit));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("visible_unit");
    t = doc.createTextNode(QString::fromStdString(*_visible_unit));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("language");
    t = doc.createTextNode(QString::fromStdString(*_language));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("current_station_id");
    t = doc.createTextNode(QString::number(_current_station_id));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("update_period");
    t = doc.createTextNode(QString::number(_update_period));
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("update_connect");
    if (_update_connect)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("fullscreen");
    if (_fullscreen)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("logo_on_cover");
    if (_logo_on_cover)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("wind_on_cover");
    if (_wind_on_cover)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("transparency");
    if (_transparency)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("lastupdate_on_cover");
    if (_lastupdate_on_cover)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("lockscreen");
    if (_lockscreen)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("standbyscreen");
    if (_standbyscreen)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("gps");
    if (_gps)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    el = doc.createElement("splash");
    if (_splash)
        t = doc.createTextNode("true");
    else
        t = doc.createTextNode("false");
    el.appendChild(t);
    root.appendChild(el);

    std::vector<Station*>::iterator i = _stations->begin();
    while (i != _stations->end()){
        QDomElement st = doc.createElement("station");
        el = doc.createElement("source_name");
        t = doc.createTextNode(QString::fromStdString((*i)->sourceName()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("station_name");
        t = doc.createTextNode(QString::fromUtf8((*i)->name().c_str()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("station_id");
        t = doc.createTextNode(QString::fromStdString((*i)->id()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("country");
        t = doc.createTextNode(QString::fromStdString((*i)->country()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("region");
        t = doc.createTextNode(QString::fromStdString((*i)->region()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("file_name");
        t = doc.createTextNode(QString::fromStdString((*i)->fileName()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("forecast_url");

        t = doc.createTextNode(QString::fromStdString((*i)->forecastURL()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("cookie");
        t = doc.createTextNode(QString::fromStdString((*i)->cookie()));
        el.appendChild(t);
        st.appendChild(el);
        
        el = doc.createElement("user_agent");
        t = doc.createTextNode(QString::fromStdString((*i)->user_agent()));
        el.appendChild(t);
        st.appendChild(el);
 
        el = doc.createElement("latitude");
        t = doc.createTextNode(QString::number((*i)->latitude()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("longitude");
        t = doc.createTextNode(QString::number((*i)->longitude()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("map_url");
        if (QString::fromStdString((*i)->mapURL()) != ""){
            t = doc.createTextNode(QString::fromStdString((*i)->mapURL()));
            el.appendChild(t);
            st.appendChild(el);
        }
        
        el = doc.createElement("basemap_url");
        if (QString::fromStdString((*i)->basemapURL()) != ""){
            t = doc.createTextNode(QString::fromStdString((*i)->basemapURL()));
            el.appendChild(t);
            st.appendChild(el);
        }

        el = doc.createElement("detail_url");
        t = doc.createTextNode(QString::fromStdString((*i)->detailURL()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("hours_url");
        t = doc.createTextNode(QString::fromStdString((*i)->hoursURL()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("view_url");
        t = doc.createTextNode(QString::fromStdString((*i)->viewURL()));
        el.appendChild(t);
        st.appendChild(el);

        el = doc.createElement("converter");
        t = doc.createTextNode(QString::fromStdString((*i)->converter()));
        el.appendChild(t);
        st.appendChild(el);

        
        el = doc.createElement("gps");
        if ((*i)->gps() == false)
            t = doc.createTextNode("false");
        else
            t = doc.createTextNode("true");
        el.appendChild(t);
        st.appendChild(el);
        
        root.appendChild(st);
        ++i;
    }

    try {
        QFile file(QString::fromStdString(*_filename));
        if (!file.open(QIODevice::WriteOnly)){
            std::cerr<<"error file open "<<_filename<<std::endl;
            throw std::runtime_error("Could not open file");
        }

        QTextStream ts(&file);
        ts << doc.toString();
        //std::cerr << doc.toString().toStdString() << std::endl;
        file.close();
    } catch (std::exception &ex) {
        std::cerr << "Ouch! That hurts in file operations, because: "
            << ex.what() << "!\n";
        throw std::runtime_error(ex.what());
    }
}
////////////////////////////////////////////////////////////////////////////////
Config* 
Config::Instance(){
    if (!_self)
        _self = new Config();
    _refcount++;
    std::cerr<<"Refcount for Config: "<<_refcount<<std::endl;
    return _self;
}
////////////////////////////////////////////////////////////////////////////////
Config* 
Config::Instance(const std::string& filename, const std::string& schema_filename){
    if (!_self)
        _self = new Config(filename, schema_filename);
    _refcount++;
    std::cerr<<"Refcount for Config: "<<_refcount<<std::endl;
    return _self;
}
////////////////////////////////////////////////////////////////////////////////
bool 
Config::DeleteInstance(){
    if (_refcount >1){
        _refcount --;
        return false;
    }
    if (_refcount == 1  && _self){
        delete _self;
        _self = 0;
        return true;
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////
Config::Config(const std::string& filename, const std::string& schema_filename)
                    : Parser(filename, schema_filename){
    /*  std::cerr<<"CONFIG CREATE222222!!!!!!!!!!!!!!"<<std::endl; */
    /* std::cerr<<"new Config"<<std::endl; */
    _filename = new std::string;
    _filename->assign(filename);
    _pathPrefix = new std::string(AbstractConfig::prefix + AbstractConfig::sharePath);
    _iconspath  = new std::string("");
    _iconspath->assign(_pathPrefix->c_str());
    _iconspath->append("iconsets");
    _iconset = new std::string("Meecast");
    _temperature_unit = new std::string("C");
    _wind_speed_unit = new std::string("m/s");
    _pressure_unit = new std::string("mbar");
    _visible_unit = new std::string("m");
    _language = new std::string("System");
    _mod = new std::string("Digia");
    _screen_orientation = new std::string("automatic");
    _nullname = new std::string("");
    _update_connect = false;
    _fullscreen = false;
    _logo_on_cover = false;
    _wind_on_cover = false;
    _transparency = false;
    _lastupdate_on_cover = true;
    _lockscreen = false;
    _standbyscreen = false;
    _Xleft_corner_of_lockscreen_widget = 0;
    _Yleft_corner_of_lockscreen_widget = 0;
    _gps = false;
    _splash = true;
    _update_period = INT_MAX;
    _font_color = new std::string("#00ff00");
   /* std::cerr<<"new StationList"<<std::endl; */
    _stations = new StationsList;
    _current_station_id = INT_MAX;
    _languages_list = new languages;
   /* std::cerr<<"Load Config"<<std::endl; */
    this->InitLanguagesList();
    this->LoadConfig();
}
////////////////////////////////////////////////////////////////////////////////
void
Config::ReLoadConfig(){

    std::cerr<<"ReLoadConfig"<<std::endl;
    for(unsigned i = 0; i < _stations->size(); i++){
        Core::Station *_station =_stations->at(i);
        delete (_station);
    }
    _stations->clear();
    this->Reloadfile();
    this->LoadConfig();
}
////////////////////////////////////////////////////////////////////////////////
void
Config::LoadConfig(){
    std::cerr<<"LoadConfig"<<std::endl;
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
    try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    #ifdef LIBXML
        if(parser){
            //Walk the tree:
            const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
            processNode(pNode);
        }
    #else //LIBXML
        QDomElement root = _doc.documentElement();

        QDomNodeList nodelist;
        QDomElement el = root.firstChildElement("base");
       // if (!el.isNull())
       //     _pathPrefix->assign(el.text().toStdString());
        el = root.firstChildElement("iconset");
        if (!el.isNull())
            _iconset->assign(el.text().toStdString());
        el = root.firstChildElement("screen_orienation");
        if (!el.isNull())
            _screen_orientation->assign(el.text().toStdString());
        el = root.firstChildElement("current_station_id");
        if (!el.isNull()){
            _current_station_id = el.text().toInt();
        }
        el = root.firstChildElement("temperature_unit");
        if (!el.isNull())
            this->TemperatureUnit(el.text().toStdString());
        el = root.firstChildElement("wind_speed_unit");
        if (!el.isNull())
            _wind_speed_unit->assign(el.text().toStdString());
        el = root.firstChildElement("pressure_unit");
        if (!el.isNull())
            _pressure_unit->assign(el.text().toStdString());
        el = root.firstChildElement("visible_unit");
        if (!el.isNull())
            _visible_unit->assign(el.text().toStdString());
        el = root.firstChildElement("language");
        if (!el.isNull())
            _language->assign(el.text().toStdString());
        el = root.firstChildElement("update_connect");
        if (!el.isNull())
            _update_connect = (el.text() == "true") ? true : false;
        el = root.firstChildElement("fullscreen");
        if (!el.isNull())
            _fullscreen = (el.text() == "true") ? true : false;
        el = root.firstChildElement("logo_on_cover");
        if (!el.isNull())
            _logo_on_cover = (el.text() == "true") ? true : false;
        el = root.firstChildElement("wind_on_cover");
        if (!el.isNull())
            _wind_on_cover = (el.text() == "true") ? true : false;
        el = root.firstChildElement("transparency");
        if (!el.isNull())
            _transparency = (el.text() == "true") ? true : false;
        el = root.firstChildElement("lastupdate_on_cover");
        if (!el.isNull())
            _lastupdate_on_cover = (el.text() == "true") ? true : false;
        el = root.firstChildElement("lockscreen");
        if (!el.isNull())
            _lockscreen = (el.text() == "true") ? true : false;
        el = root.firstChildElement("standbyscreen");
        if (!el.isNull())
            _standbyscreen = (el.text() == "true") ? true : false;
        el = root.firstChildElement("gps");
        if (!el.isNull())
            _gps = (el.text() == "true") ? true : false;
        el = root.firstChildElement("splash");
        if (!el.isNull())
            _splash = (el.text() == "true") ? true : false;
        el = root.firstChildElement("update_period");
        if (!el.isNull())
            _update_period = el.text().toInt();

        nodelist = root.elementsByTagName("station");
        for (int i=0; i<nodelist.count(); i++){
            QString source_name, station_name, station_id, country, region, forecastURL, fileName, converter, viewURL, detailURL, hoursURL, basemapURL,  mapURL, cookie, latitude, longitude, user_agent;
            bool gps = false;
            bool splash = true;
            QDomElement e = nodelist.at(i).toElement();
            QDomNode n = e.firstChild();
            while (!n.isNull()){
                el = n.toElement();
                QString tag = el.tagName();

                if (tag == "source_name")
                    source_name = el.text();
                else if (tag == "station_name")
                    station_name = el.text();
                else if (tag == "station_id")
                    station_id = el.text();
                else if (tag == "country")
                    country = el.text();
                else if (tag == "region")
                    region = el.text();
                else if (tag == "file_name")
                    fileName = el.text();
                else if (tag == "forecast_url")
                    forecastURL = el.text();
                else if (tag == "cookie")
                    cookie = el.text();
                else if (tag == "user_agent")
                    user_agent = el.text();
                else if (tag == "detail_url")
                    detailURL = el.text();
                else if (tag == "hours_url")
                    hoursURL = el.text();
                else if (tag == "view_url")
                    viewURL = el.text();
                else if (tag == "map_url")
                    mapURL = el.text();
                else if (tag == "basemap_url")
                    basemapURL = el.text();
                else if (tag == "longitude")
                    longitude = el.text();
                else if (tag == "latitude")
                    latitude = el.text();
                else if (tag == "converter")
                    converter = el.text();
                else if (tag == "gps")
                    gps = (el.text() == "true") ? true : false;
                else if (tag == "splash")
                    splash = (el.text() == "true") ? true : false;

                n = n.nextSibling();
            }
            /* Hack for yr.no */
            if  (source_name=="yr.no")
                forecastURL.replace("#","/");
            if  (source_name=="yr.no")
                viewURL.replace("#","/");
            if  (source_name=="gismeteo.ru" && (detailURL.indexOf("hourly",0) != -1)){
                detailURL.replace("hourly/","");
                std::cerr<<"New Gismeteo DetailURl new "<< detailURL.toStdString()<<std::endl;
                cookie="old=1";
            }
            /* Hack for gismeteo.ru */
            if  (source_name=="gismeteo.ru" && (forecastURL.indexOf("gismeteo.by",0) != -1)){
                std::cerr<<"Replaced Gismeteo.bt URL "<< forecastURL.toStdString()<<std::endl;
                forecastURL.replace("gismeteo.by", "gismeteo.ua");
                std::cerr<<"New Gismeteo ForecastURl new "<< forecastURL.toStdString()<<std::endl;
                if (detailURL.indexOf("gismeteo.by",0) != -1){
                    detailURL.replace("gismeteo.by", "gismeteo.ua");
                    std::cerr<<"New Gismeteo DetailURl new "<< detailURL.toStdString()<<std::endl;
                }
            }

            std::cerr<<"SOurce name "<<source_name.toStdString()<<std::endl;
            Station *st = new Station(source_name.toStdString(),
                                      station_id.toStdString(),
                                      station_name.toUtf8().data(),
                                      country.toStdString(),
                                      region.toStdString(),
                                      forecastURL.toStdString(),
				                      detailURL.toStdString(),
				                      hoursURL.toStdString(),
                                      viewURL.toStdString(),
                                      mapURL.toStdString(),
                                      basemapURL.toStdString(),
                                      cookie.toStdString(),
                                      gps, 
                                      latitude.toDouble(),
                                      longitude.toDouble(),
                                      user_agent.toStdString()
                                      );
            st->fileName(fileName.toStdString());
            st->converter(converter.toStdString());
            _stations->push_back(st);
            //_stations->push_back(new Station("weather.com", "BOXX0014", "Vitebsk", "Belarus", "Belarus"));

        }
        if (nodelist.count()>0 && _current_station_id == INT_MAX)
            _current_station_id = 0;

    #endif //LIBXML
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
    }
    catch(const std::exception& ex){
        throw(ex.what());
    }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED

}
////////////////////////////////////////////////////////////////////////////////
void 
Config::InitLanguagesList(){

    _languages_list->push_back(std::make_pair("System", ""));
    _languages_list->push_back(std::make_pair("Albanian", "sq_SQ"));
    _languages_list->push_back(std::make_pair("Arabic", "ar_AR"));
    _languages_list->push_back(std::make_pair("Bulgarian", "bg_BG"));
    _languages_list->push_back(std::make_pair("Catalan", "ca_CA"));
    _languages_list->push_back(std::make_pair("Chinese", "zh_ZH"));
    _languages_list->push_back(std::make_pair("Chinese(Hong Kong)", "zh_HK"));
    _languages_list->push_back(std::make_pair("Chinese(Taiwan)", "zh_TW"));
    _languages_list->push_back(std::make_pair("Czech", "cs_CS"));
    _languages_list->push_back(std::make_pair("Danish", "da_DK"));
    _languages_list->push_back(std::make_pair("Dutch", "nl_NL"));
    _languages_list->push_back(std::make_pair("German", "de_DE"));
    _languages_list->push_back(std::make_pair("English", "en_GB"));
    _languages_list->push_back(std::make_pair("Finnish", "fi_FI"));
    _languages_list->push_back(std::make_pair("French", "fr_FR"));
    _languages_list->push_back(std::make_pair("Galician", "gl_GL"));
    _languages_list->push_back(std::make_pair("Hungarian", "hu_HU"));
    _languages_list->push_back(std::make_pair("Italian", "it_IT"));
    _languages_list->push_back(std::make_pair("Norwegian", "no_NO"));
    _languages_list->push_back(std::make_pair("Polish", "pl_PL"));
    _languages_list->push_back(std::make_pair("Portuguese", "pt_PT"));
    _languages_list->push_back(std::make_pair("Russian", "ru_RU"));
    _languages_list->push_back(std::make_pair("Serbian", "sr_SR"));
    _languages_list->push_back(std::make_pair("Slovenian", "sl_SL"));
    _languages_list->push_back(std::make_pair("Slovak", "sk_SK"));
    _languages_list->push_back(std::make_pair("Spanish", "es_ES"));
    _languages_list->push_back(std::make_pair("Spanish(Mexico)", "es_MX"));
    _languages_list->push_back(std::make_pair("Swedish", "sv_SV"));
    _languages_list->push_back(std::make_pair("Turkish", "tr_TR"));
    _languages_list->push_back(std::make_pair("Ukraine", "uk_UA"));
    _languages_list->push_back(std::make_pair("Vietnamese", "vi_VI"));
/*
    languages::iterator cur;
    for (cur=_languages_list->begin(); cur<_languages_list->end(); cur++){
        std::cerr<<"Locale "<< (*cur).second<<std::endl;
    }
*/
}

languages&
Config::languagesList(){
    return *_languages_list;
}

////////////////////////////////////////////////////////////////////////////////
Config::~Config(){
    std::cerr<<"Config::~Config()"<<std::endl;
    delete _pathPrefix;
    delete _iconset;
    delete _temperature_unit;
    _languages_list->clear();
    delete _languages_list;
    for(unsigned i = 0; i < _stations->size(); i++){
        Core::Station *_station =_stations->at(i);
        delete (_station);
    }
    _stations->clear();
    delete _stations;
    delete _nullname;
    delete _wind_speed_unit;
    delete _pressure_unit;
    delete _visible_unit;
    delete _language;
    delete _mod;
    delete _filename;
    delete _iconspath;
    delete _screen_orientation;
    delete _font_color;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::iconSet(const std::string& text){
    /* ToDo Check access to path */
    /* std::cerr<<"Config::iconSet(const std::string& text) "<<text.c_str()<<std::endl; */
    _iconset->assign(text);
}
std::string&
Config::iconSet(){
    /* std::cerr<<"Config::iconSet() "<<_iconset->c_str()<<std::endl; */
    return *_iconset;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::screenOrientation(const std::string& text){
    _screen_orientation->assign(text);
}

std::string&
Config::screenOrientation(){
    return *_screen_orientation;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::current_station_id(int id_station){
    _current_station_id = id_station;
    if (id_station >= (int)this->stationsList().size())
        _current_station_id = 0;
    if (id_station < 0)
        _current_station_id = 0;
    if (this->stationsList().size() == 0)
        _current_station_id = INT_MAX;
}
////////////////////////////////////////////////////////////////////////////////
int   
Config::current_station_id(){
    if (_current_station_id >= (int)this->stationsList().size())
        _current_station_id = 0;
    if (this->stationsList().size() == 0)
        _current_station_id = INT_MAX;
    return _current_station_id;
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::prefix_path(){
        //return prefix;
    return *_pathPrefix;
    }
////////////////////////////////////////////////////////////////////////////////
void
Config::TemperatureUnit(const std::string& text){
    /* ToDo Check access to path */
    /* Check parametr */
     if (text!= std::string("C") && text!= std::string("F") ){
         _temperature_unit->assign("C");
     }else
         _temperature_unit->assign(text);
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::TemperatureUnit(){
    return *_temperature_unit;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::WindSpeedUnit(const std::string& text){
    /* ToDo Check access to path */
    _wind_speed_unit->assign(text);
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::WindSpeedUnit(){
    return *_wind_speed_unit;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::PressureUnit(const std::string& text){
    /* ToDo Check access to path */
    _pressure_unit->assign(text);
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::PressureUnit(){

    if (*_pressure_unit == "Pa")
        _pressure_unit->assign("kPa");
    return *_pressure_unit;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::VisibleUnit(const std::string& text){
    _visible_unit->assign(text);
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::VisibleUnit(){
    return *_visible_unit;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Language(const std::string& lang){
    _language->assign(lang);
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::Language(){
    return *_language;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Mod(const std::string& mod){
    _mod->assign(mod);
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::Mod(){
    return *_mod;
}
////////////////////////////////////////////////////////////////////////////////

void

Config::UpdateConnect(const bool uc){
    _update_connect = uc;
}
bool
Config::UpdateConnect(void){
    return _update_connect;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Fullscreen(const bool uc){
    _fullscreen = uc;
}
bool
Config::Fullscreen(void){
    return _fullscreen;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::LogoOnCover(const bool uc){
    _logo_on_cover = uc;
}
bool
Config::LogoOnCover(void){
    return _logo_on_cover;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::WindOnCover(const bool uc){
    _wind_on_cover = uc;
}
bool
Config::WindOnCover(void){
    return _wind_on_cover;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Transparency(const bool uc){
    _transparency = uc;
}
bool
Config::Transparency(void){
    return _transparency;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::LastUpdateOnCover(const bool uc){
    _lastupdate_on_cover = uc;
}
bool
Config::LastUpdateOnCover(void){
    return _lastupdate_on_cover;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Lockscreen(const bool uc){
    _lockscreen = uc;
}
bool
Config::Lockscreen(void){
    return _lockscreen;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Standbyscreen(const bool uc){
    _standbyscreen = uc;
}
bool
Config::Standbyscreen(void){
    return _standbyscreen;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Gps(const bool uc){
    _gps = uc;
}
bool
Config::Gps(void){
    return _gps;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Splash(const bool uc){
    _splash = uc;
}
bool
Config::Splash(void){
    return _splash;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Xleft_corner_of_lockscreen_widget(const int x){
    _Xleft_corner_of_lockscreen_widget = x;
}
int
Config::Xleft_corner_of_lockscreen_widget(void){
    return _Xleft_corner_of_lockscreen_widget;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::Yleft_corner_of_lockscreen_widget(const int y){
    _Yleft_corner_of_lockscreen_widget = y;
}
int
Config::Yleft_corner_of_lockscreen_widget(void){
    return _Yleft_corner_of_lockscreen_widget;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::UpdatePeriod(const int period){
    if (period > 3600 * 24)
        _update_period = INT_MAX;
    else 
        _update_period = period;
}
int
Config::UpdatePeriod(void){
    return _update_period;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::FontColor(const std::string& text){
    /* ToDo Check access to path */
    _font_color->assign(text);
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::FontColor(){
    return *_font_color;
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::stationname(){
    if (this->current_station_id() == INT_MAX && this->stationsList().size() > 0){
        this->current_station_id(0);
    }
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
        &&  this->stationsList().at(this->current_station_id())){
        return this->stationsList().at(this->current_station_id())->name();
    }
    else{
        std::string *_name;
        _name = new std::string("Unknown"); 
        return *_name;
    }
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::stationname(int i){
    if (this->stationsList().size() > 0
        &&  this->stationsList().at(i)){
        return this->stationsList().at(i)->name();
    }
    else{
        std::string *_name;
        _name = new std::string("Unknown"); 
        return *_name;
    }
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::prevstationname(){
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
        && (this->current_station_id() - 1) >= 0
        &&  this->stationsList().at(this->current_station_id()-1)){
         return this->stationsList().at(this->current_station_id()-1)->name();
    }else{
        return *_nullname;
    }
}

////////////////////////////////////////////////////////////////////////////////    
std::string&
Config::nextstationname(){
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
        && ((uint)this->current_station_id() + 1) < (uint)this->stationsList().size()
        &&  this->stationsList().at(this->current_station_id()+1)){
         return this->stationsList().at(this->current_station_id()+1)->name();
    }else {
        return *_nullname;
    }
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::iconspath(){
    /* std::cerr<<"Iconset path "<< _path->c_str()<<std::endl; */
    return *_iconspath;
}

////////////////////////////////////////////////////////////////////////////////
#ifdef LIBXML
void Config::processNode(const xmlpp::Node* node){
    Station* station;
    std::string station_name;
    std::string id;
    if(!node)
        return;
    std::string nodeName = node->get_name();
    // config tag
    if(nodeName == "config"){
        xmlpp::Node::NodeList list = node->get_children();
        for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
            processNode(*iter);
    }

    // base tag
    if(nodeName == "base"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
       // _pathPrefix->assign(nodeText->get_content());
        return;
    }
    // iconset tag
    if(nodeName == "iconset"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        _iconset->assign(nodeText->get_content());
        return;
    }
    // temperature unit
    if(nodeName == "temperature_unit"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        /* Temporary fix for bug */
        _temperature_unit->assign(nodeText->get_content());
        /* Temporary fix for bug */
        if (_temperature_unit == "m/s"){
            _temperature_unit->assign("C");
        }
        return;
    }
    // current station id
    if(nodeName == "current_station_id"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        _current_station_id->assign(nodeText->get_content());
        return;
    }

    // update connect
    if(nodeName == "update_connect"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        std::string str = nodeText->get_content();
        (str.compare("true")) ? (_update_connect = false) : (_update_connect = true);
        return;
    }
    // fullscreen
    if(nodeName == "fullscreen"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        std::string str = nodeText->get_content();
        (str.compare("true")) ? (_fullscreen = false) : (_fullscreen = true);
        return;
    }
    // lockscreen
    if(nodeName == "lockscreen"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        std::string str = nodeText->get_content();
        (str.compare("true")) ? (_lockscreen = false) : (_lockscreen = true);
        return;
    }
    // lockscreen
    if(nodeName == "standbyscreen"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        std::string str = nodeText->get_content();
        (str.compare("true")) ? (_standbyscreen = false) : (_standbyscreen = true);
        return;
    }
    // gps
    if(nodeName == "gps"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        std::string str = nodeText->get_content();
        (str.compare("true")) ? (_gps = false) : (_gps = true);
        return;
    }
    // splash
    if(nodeName == "splash"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        std::string str = nodeText->get_content();
        (str.compare("true")) ? (_splash = false) : (_splash = true);
        return;
    }
    // update period
    if(nodeName == "update_period"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        _update_period->assign(nodeText->get_content());
        return;
    }
    // station
    if(nodeName == "station"){
        _stations->push_back(new Station("weather.com", "BOXX0014", "Vitebsk", "Belarus", "Belarus"));
        return;
    }
/*
    // base tag
    if(nodeName == "base"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        _databaseName->assign(nodeText->get_content());
        return;
    }
    // forecast tag
    if(nodeName == "forecast"){
        const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
        const xmlpp::Attribute* attribute = nodeElement->get_attribute("url");
        if(attribute)
            _forecastUrl->assign(attribute->get_value());
        return;
    }
    // detail tag
    if(nodeName == "detail"){
        const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
        const xmlpp::Attribute* attribute = nodeElement->get_attribute("url");
        if(attribute)
            _detailUrl->assign(attribute->get_value());
        return;
    }
    // search tag
    if(nodeName == "search"){
        const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
        const xmlpp::Attribute* attribute = nodeElement->get_attribute("url");
        if(attribute)
            _searchUrl->assign(attribute->get_value());
        return;
    }
    // library tag
    if(nodeName == "library"){
        xmlpp::Node::NodeList list = node->get_children();
        xmlpp::Node::NodeList::iterator iter = list.begin();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
        _binaryName->assign(nodeText->get_content());
        return;
    }
*/
}
#endif
////////////////////////////////////////////////////////////////////////////////
StationsList&
Config::stationsList(){
    return *_stations;
}
void
Config::stationsList(StationsList list)
{
    _stations = new StationsList;
    StationsList::iterator cur;
    for (cur=list.begin(); cur<list.end(); cur++){
        _stations->push_back(*cur);
    }

}
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
