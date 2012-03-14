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


#include "config.h"
#include "station.h"
#include <iostream>
#include <fstream>
////////////////////////////////////////////////////////////////////////////////
namespace Core{
////////////////////////////////////////////////////////////////////////////////
Config::Config()
{
    _pathPrefix = new std::string(AbstractConfig::prefix + AbstractConfig::sharePath);
    _iconset = new std::string("Meecast");
    _temperature_unit = new std::string("C");
    _wind_speed_unit = new std::string("m/s");
    _pressure_unit = new std::string("mbar");
    _update_connect = false;
    _fullscreen = false;
    _lockscreen = false;
    _standbyscreen = false;
    _gps = false;
    _font_color = new std::string("#00ff00");
    _stations = new StationsList;
    _current_station_id = INT_MAX;
    _update_period = INT_MAX;
    //std::cerr << Core::AbstractConfig::configPath << std::endl;
    std::string path(AbstractConfig::getConfigPath());
    path += "config.xml";
    _filename = new std::string(path);
}
////////////////////////////////////////////////////////////////////////////////
void
Config::saveConfig()
{
    #ifdef QT 
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

        el = doc.createElement("detail_url");
        /* Temporary hack for weather.com and gismeteo.ru. This must be delete after version 0.5.0 */
        if (QString::fromStdString((*i)->detailURL()) == "" && QString::fromStdString((*i)->sourceName()) == "weather.com"){
            char forecast_detail_url[4096];
            snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, "http://xml.weather.com/weather/local/%s?cm_ven=1CW&site=wx.com-bar&cm_ite=wx-cc&par=1CWFFv1.1.9&cm_pla=wx.com-bar&cm_cat=FFv1.1.9&unit=m&hbhf=12", (*i)->id().c_str());
            t = doc.createTextNode(QString::fromStdString(forecast_detail_url));
            el.appendChild(t);
            st.appendChild(el);
        }else{
            if (QString::fromStdString((*i)->sourceName()) == "gismeteo.ru"){
                char forecast_detail_url[4096];
                snprintf(forecast_detail_url, sizeof(forecast_detail_url)-1, "http://www.gismeteo.by/city/hourly/%s/", (*i)->id().c_str());
                t = doc.createTextNode(QString::fromStdString(forecast_detail_url));
                el.appendChild(t);
                st.appendChild(el);
            }
            else{
                t = doc.createTextNode(QString::fromStdString((*i)->detailURL()));
                el.appendChild(t);
                st.appendChild(el);
            }
        }
//        t = doc.createTextNode(QString::fromStdString((*i)->detailURL()));
//        el.appendChild(t);
//        st.appendChild(el);

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

    QFile file(QString::fromStdString(*_filename));
    if (!file.open(QIODevice::WriteOnly)){
        std::cerr<<"error file open "<<_filename<<std::endl;
        throw("Invalid destination file");
        return;
    }

    QTextStream ts(&file);
    ts << doc.toString();
    file.write(doc.toString());
    //std::cerr << doc.toString().toStdString() << std::endl;
    file.close();
    #else
    using namespace std;
    ofstream file_out;
    file_out.open(_filename->c_str());
    file_out<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>"<<endl;
    file_out<<"<config xmlns=\"http://omweather.garage.maemo.org/schemas\">"<<endl;
    file_out<<" <base>"<< *_pathPrefix<<"</base>"<<endl;
    file_out<<" <iconset>"<< *_iconset<<"</iconset>"<<endl;
    file_out<<" <temperature_unit>"<< *_temperature_unit<<"</temperature_unit>"<<endl;
    file_out<<" <wind_speed_unit>"<< *_wind_speed_unit<<"</wind_speed_unit>"<<endl;
    file_out<<" <pressure_unit>"<< *_pressure_unit<<"</pressure_unit>"<<endl;
    file_out<<" <current_station_id>"<< _current_station_id<<"</current_station_id>"<<endl;
    file_out<<" <update_period>"<< _update_period<<"</update_period>"<<endl;
    std::vector<Station*>::iterator i = _stations->begin();
    while (i != _stations->end()){
        file_out<<"  <station>"<<endl;
        file_out<<"  <source_name>"<< (*i)->sourceName()<<"</source_name>"<<endl;
        file_out<<"  <station_name>"<< (*i)->name()<<"</station_name>"<<endl;
        file_out<<"  <station_id>"<< (*i)->id()<<"</station_id>"<<endl;
        file_out<<"  <country>"<< (*i)->country()<<"</country>"<<endl;
        file_out<<"  <region>"<< (*i)->region()<<"</region>"<<endl;
        file_out<<"  <file_name>"<< (*i)->fileName()<<"</file_name>"<<endl;
        file_out<<"  <forecast_url>"<< (*i)->forecastURL()<<"</forecast_url>"<<endl;
        file_out<<"  <detail_url>"<< (*i)->detailURL()<<"</detail_url>"<<endl;
        file_out<<"  <view_url>"<< (*i)->viewURL()<<"</view_url>"<<endl;
        file_out<<"  <converter>"<< (*i)->converter()<<"</converter>"<<endl;
        if ((*i)->gps() == false)
            file_out<<"  <gps>false</gps>"<<endl;
        else
            file_out<<"  <gps>true</gps>"<<endl;
        file_out<<"  </station>"<<endl;
        ++i;
    }
    file_out<<"</config>"<<endl;
    file_out.close();
    #endif
}
////////////////////////////////////////////////////////////////////////////////
Config::Config(const std::string& filename, const std::string& schema_filename)
                    : Parser(filename, schema_filename){
    _filename = new std::string;
    _filename->assign(filename);
    _pathPrefix = new std::string(AbstractConfig::prefix + AbstractConfig::sharePath);
    _iconset = new std::string("Meecast");
    _temperature_unit = new std::string("C");
    _wind_speed_unit = new std::string("m/s");
    _pressure_unit = new std::string("mbar");
    _update_connect = false;
    _fullscreen = false;
    _lockscreen = false;
    _standbyscreen = false;
    _gps = false;
    _update_period = INT_MAX;
    _font_color = new std::string("#00ff00");
    _stations = new StationsList;
    _current_station_id = INT_MAX;
    this->LoadConfig();
}
////////////////////////////////////////////////////////////////////////////////
void
Config::ReLoadConfig(){
    _stations->clear();
    this->LoadConfig();
}
////////////////////////////////////////////////////////////////////////////////
void
Config::LoadConfig(){
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
    #ifdef QT
        QDomElement root = _doc.documentElement();

        QDomNodeList nodelist;
        QDomElement el = root.firstChildElement("base");
       // if (!el.isNull())
       //     _pathPrefix->assign(el.text().toStdString());
        el = root.firstChildElement("iconset");
        if (!el.isNull())
            _iconset->assign(el.text().toStdString());
        el = root.firstChildElement("current_station_id");
        if (!el.isNull())
            _current_station_id = el.text().toInt();
        el = root.firstChildElement("temperature_unit");
        if (!el.isNull())
            this->TemperatureUnit(el.text().toStdString());
        el = root.firstChildElement("wind_speed_unit");
        if (!el.isNull())
            _wind_speed_unit->assign(el.text().toStdString());
        el = root.firstChildElement("pressure_unit");
        if (!el.isNull())
            _pressure_unit->assign(el.text().toStdString());
        el = root.firstChildElement("update_connect");
        if (!el.isNull())
            _update_connect = (el.text() == "true") ? true : false;
        el = root.firstChildElement("fullscreen");
        if (!el.isNull())
            _fullscreen = (el.text() == "true") ? true : false;
        el = root.firstChildElement("lockscreen");
        if (!el.isNull())
            _lockscreen = (el.text() == "true") ? true : false;
        el = root.firstChildElement("standbyscreen");
        if (!el.isNull())
            _standbyscreen = (el.text() == "true") ? true : false;
        el = root.firstChildElement("gps");
        if (!el.isNull())
            _gps = (el.text() == "true") ? true : false;
        el = root.firstChildElement("update_period");
        if (!el.isNull())
            _update_period = el.text().toInt();

        nodelist = root.elementsByTagName("station");
        for (int i=0; i<nodelist.count(); i++){
            QString source_name, station_name, station_id, country, region, forecastURL, fileName, converter, viewURL, detailURL, cookie;
            bool gps = false;
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
                else if (tag == "detail_url")
                    detailURL = el.text();
                else if (tag == "view_url")
                    viewURL = el.text();
                else if (tag == "converter")
                    converter = el.text();
                else if (tag == "gps")
                    gps = (el.text() == "true") ? true : false;
                n = n.nextSibling();
            }
/* Hack for yr.no */
            if  (source_name=="yr.no")
                forecastURL.replace("#","/");
            if  (source_name=="yr.no")
                viewURL.replace("#","/");

            Station *st = new Station(source_name.toStdString(),
                                      station_id.toStdString(),
                                      station_name.toUtf8().data(),
                                      country.toStdString(),
                                      region.toStdString(),
                                      forecastURL.toStdString(),
				                      detailURL.toStdString(),
                                      viewURL.toStdString(),
                                      cookie.toStdString(),
                                      gps);
            st->fileName(fileName.toStdString());
            st->converter(converter.toStdString());
            _stations->push_back(st);
            //_stations->push_back(new Station("weather.com", "BOXX0014", "Vitebsk", "Belarus", "Belarus"));

        }
        if (nodelist.count()>0 && _current_station_id == INT_MAX)
            _current_station_id = 0;
    #else
       if (!_doc)
           return;
       xmlNodePtr root = xmlDocGetRootElement(_doc);
       if (!root)
           return;
       std::string   source_name, station_name, station_id, country, region, forecastURL, fileName, converter, viewURL, detailURL, cookie;
       for(xmlNodePtr p = root->children; p; p = p->next) {
            if (p->type != XML_ELEMENT_NODE)
                continue;
            if (!xmlStrcmp(p->name, (const xmlChar*)"iconset")){
                _iconset->assign(std::string((char *)xmlNodeGetContent(p)));
                std::cerr << "Iconset !!!!" << xmlNodeGetContent(p)<< std::endl;
            }
            if (!xmlStrcmp(p->name, (const xmlChar*)"current_station_id")){
                _current_station_id = atoi((char *)xmlNodeGetContent(p)); 
                std::cerr << "Station id " << _current_station_id << std::endl;
            }
            if (!xmlStrcmp(p->name, (const xmlChar*)"station")){
                bool gps = false;
                for(xmlNodePtr p1 = p->children; p1; p1 = p1->next) {
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"source_name"))
                        source_name = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"station_name"))
                        station_name = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"station_id"))
                        station_id = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"country"))
                        country = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"region"))
                        region = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"file_name"))
                        fileName = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"forecast_url"))
                        forecastURL = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"cookie"))
                        cookie = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"detail_url"))
                        detailURL = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"view_url"))
                        viewURL = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"converter"))
                        converter = std::string((char *)xmlNodeGetContent(p1)); 
                    if (!xmlStrcmp(p1->name, (const xmlChar*)"gps")){
                        if(!xmlStrcmp(xmlNodeGetContent(p1),(const xmlChar*)"true"))
                            gps = true;
                        else
                            gps = false;
                     }

                  //  if  (source_name=="yr.no")
                  //      forecastURL.replace("#","/");
                  //  if  (source_name=="yr.no")
                  //      viewURL.replace("#","/");

                }
                Station *st = new Station(source_name,
                                      station_id,
                                      station_name,
                                      country,
                                      region,
                                      forecastURL,
				                      detailURL,
                                      viewURL,
                                      cookie,
                                      gps);
                     st->fileName(fileName);
                     st->converter(converter);
                     _stations->push_back(st);
            }
       }
    #endif //LIBXML
    #endif
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
    }
    catch(const std::exception& ex){
        throw(ex.what());
    }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED

}
////////////////////////////////////////////////////////////////////////////////
Config::~Config(){
    delete _pathPrefix;
    delete _iconset;
    delete _temperature_unit;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::iconSet(const std::string& text){
    /* ToDo Check access to path */
    _iconset->assign(text);
}
std::string&
Config::iconSet(){
    return *_iconset;
}
////////////////////////////////////////////////////////////////////////////////
void
Config::current_station_id(int id_station){
    _current_station_id = id_station;
}
int   
Config::current_station_id(){
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
    return *_pressure_unit;
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
Config::stationname()
{
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
Config::prevstationname()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
        && (this->current_station_id() - 1) >= 0
        &&  this->stationsList().at(this->current_station_id()-1)){
         return this->stationsList().at(this->current_station_id()-1)->name();
    }else{
        std::string *_name;
        _name = new std::string(""); 
        return *_name;
    }
}

////////////////////////////////////////////////////////////////////////////////    
std::string&
Config::nextstationname()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
        && (this->current_station_id() + 1) < this->stationsList().size()
        &&  this->stationsList().at(this->current_station_id()+1)){
         return this->stationsList().at(this->current_station_id()+1)->name();
    }else {
        std::string *_name;
        _name = new std::string(""); 
        return *_name;
    }
}
////////////////////////////////////////////////////////////////////////////////
std::string&
Config::iconspath(){
    std::string *_path;
    _path = new std::string("");
    _path->assign( _pathPrefix->c_str());
    _path->append("/icons");
    return *_path;
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
