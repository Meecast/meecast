#include "config.h"
#include "station.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
////////////////////////////////////////////////////////////////////////////////
    Config::Config()
    {
        _pathPrefix = new std::string;
        _iconset = new std::string("Glance");
        _temperature_unit = new std::string("C");
        _font_color = new std::string("#00ff00");
        _stations = new StationsList;
        _current_station_id = INT_MAX;
        //std::cerr << Core::AbstractConfig::configPath << std::endl;
        std::string path(AbstractConfig::getConfigPath());
        path += "config.xml";
        _filename = new std::string(path);
    }
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

        el = doc.createElement("current_station_id");
        t = doc.createTextNode(QString::number(_current_station_id));
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
    Config::Config(const std::string& filename, const std::string& schema_filename)
                        : Parser(filename, schema_filename){
        _filename = new std::string;
        _filename->assign(filename);
        _pathPrefix = new std::string;
        _iconset = new std::string("Glance");
        _temperature_unit = new std::string("C");
        _font_color = new std::string("#00ff00");
        _stations = new StationsList;
        _current_station_id = INT_MAX;
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
            if (!el.isNull())
                _pathPrefix->assign(el.text().toStdString());
            el = root.firstChildElement("iconset");
            if (!el.isNull())
                _iconset->assign(el.text().toStdString());
            el = root.firstChildElement("current_station_id");
            if (!el.isNull())
                _current_station_id = el.text().toInt();
            el = root.firstChildElement("temperature_unit");
            if (!el.isNull())
                _temperature_unit->assign(el.text().toStdString());

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
        _temperature_unit->assign(text);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string&
    Config::TemperatureUnit(){
        return *_temperature_unit;
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
            _pathPrefix->assign(nodeText->get_content());
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
        // iconset tag
        if(nodeName == "temperature_unit"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            _temperature_unit->assign(nodeText->get_content());
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
