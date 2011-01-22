#include "config.h"
#include "station.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
////////////////////////////////////////////////////////////////////////////////
    Config::Config(const std::string& filename, const std::string& schema_filename)
                        : Parser(filename, schema_filename){
        _pathPrefix = new std::string;
        _iconset = new std::string("Glance");
        _temperature_unit = new std::string("C");
        _font_color = new std::string("#00ff00");
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
        #ifdef LIBXML
            if(parser){
                //Walk the tree:
                const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
                processNode(pNode);
            }
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
////////////////////////////////////////////////////////////////////////////////
    std::string&
    Config::iconSet(){
        return *_iconset;
    }
////////////////////////////////////////////////////////////////////////////////
    std::string&
    Config::prefix_path(){
            return prefix;
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
        // station
        if(nodeName == "station"){
            _stations->push_back(new Station("weather.com", "BOXX0014", "Vitebsk"));
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
    StationList&
    Config::stationList(){
        return *_stations;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
