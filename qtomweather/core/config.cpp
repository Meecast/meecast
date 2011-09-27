/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
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
////////////////////////////////////////////////////////////////////////////////
namespace Core{
////////////////////////////////////////////////////////////////////////////////
    Config::Config()
    {
        _pathPrefix = new std::string(AbstractConfig::prefix + AbstractConfig::sharePath);
        _iconset = new std::string("Meecast");
        _temperature_unit = new std::string("C");
        _wind_speed_unit = new std::string("m/s");
        _update_connect = false;
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
        #ifndef LIBXML
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

        std::vector<Station*>::iterator i = _stations->begin();
        while (i != _stations->end()){
            QDomElement st = doc.createElement("station");
            el = doc.createElement("source_name");
            t = doc.createTextNode(QString::fromStdString((*i)->sourceName()));
            el.appendChild(t);
            st.appendChild(el);

            el = doc.createElement("station_name");
            t = doc.createTextNode(QString::fromStdString((*i)->name()));
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

            el = doc.createElement("converter");
            t = doc.createTextNode(QString::fromStdString((*i)->converter()));
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
        //file.write(doc.toString());
        //std::cerr << doc.toString().toStdString() << std::endl;
        file.close();
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
        _update_connect = false;
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
            el = root.firstChildElement("update_connect");
            if (!el.isNull())
                _update_connect = (el.text() == "true") ? true : false;
            el = root.firstChildElement("update_period");
            if (!el.isNull())
                _update_period = el.text().toInt();

            nodelist = root.elementsByTagName("station");
            for (int i=0; i<nodelist.count(); i++){
                QString source_name, station_name, station_id, country, region, forecastURL, fileName, converter;
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
                    else if (tag == "converter")
                        converter = el.text();
                    n = n.nextSibling();
                }
/* Hack for yr.no */
                if  (source_name=="yr.no")
                    forecastURL.replace("#","/");
                Station *st = new Station(source_name.toStdString(),
                                          station_id.toStdString(),
                                          station_name.toStdString(),
                                          country.toStdString(),
                                          region.toStdString(),
                                          forecastURL.toStdString());
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
    Config::UpdateConnect(const bool uc){
        _update_connect = uc;
    }
    ////////////////////////////////////////////////////////////////////////////////
    bool
    Config::UpdateConnect(void){
        return _update_connect;
    }

    void
    Config::UpdatePeriod(const int period){
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
            &&  this->stationsList().at(this->current_station_id()))
            return this->stationsList().at(this->current_station_id())->name();
        else{
            std::string *_name;
            _name = new std::string("Unknown"); 
            return *_name;
        }
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
