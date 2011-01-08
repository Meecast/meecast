#include "source.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Source::Source(const std::string& filename, const std::string& schema_filename) : Parser(filename, schema_filename){
        _name = new std::string;
        _logo = new std::string;
        _forecastUrl = new std::string;
        _detailUrl = new std::string;
        _searchUrl = new std::string;
        _databaseName = new std::string;
        _binaryName = new std::string;
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            if(parser){
                //Walk the tree:
                const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
                processNode(pNode);
            }
        }
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        catch(const std::exception& ex){
            throw(ex.what());
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }
////////////////////////////////////////////////////////////////////////////////
    Source::~Source(){
        delete _name;
        delete _logo;
        delete _forecastUrl;
        delete _detailUrl;
        delete _searchUrl;
        delete _databaseName;
        delete _binaryName;
    }
////////////////////////////////////////////////////////////////////////////////
    Source& Source::operator=(const Source& source){
        if(this != &source){
            delete _name;
            _name = new std::string;
            _name->assign(*(source._name));
            delete _logo;
            _logo = new std::string;
            _logo->assign(*(source._logo));
            delete _forecastUrl;
            _forecastUrl = new std::string;
            _forecastUrl->assign(*(source._forecastUrl));
            delete _detailUrl;
            _detailUrl = new std::string;
            _detailUrl->assign(*(source._detailUrl));
            delete _searchUrl;
            _searchUrl = new std::string;
            _searchUrl->assign(*(source._searchUrl));
            delete _databaseName;
            _databaseName = new std::string;
            _databaseName->assign(*(source._databaseName));
            delete _binaryName;
            _binaryName = new std::string;
            _binaryName->assign(*(source._binaryName));
        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
    void Source::processNode(const xmlpp::Node* node){
        if(!node)
            return;
        std::string nodeName = node->get_name();
        // source tag
        if(nodeName == "source"){
            xmlpp::Node::NodeList list = node->get_children();
            for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
                processNode(*iter);
        }
        // name tag
        if(nodeName == "name"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            _name->assign(nodeText->get_content());
            return;
        }
        // logo tag
        if(nodeName == "logo"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            _logo->assign(nodeText->get_content());
            return;
        }
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
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Source::name() const{
        return *_name;
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Source::logo() const{
        return *_logo;
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Source::forecastURL() const{
            return *_forecastUrl;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
