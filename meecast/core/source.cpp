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


#include "source.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Source::Source(const std::string& filename, const std::string& schema_filename) : Parser(filename, schema_filename){
        _name = new std::string;
        _logo = new std::string;
        _library = new std::string;
        _binary = new std::string;
        _url_template = new std::string;
        _url_detail_template = new std::string;
        _url_for_view = new std::string;
        _cookie = new std::string;
        _hasForecast = false;
        _hasDetail = false;
        _hasSearch = false;

        _libraryHandler = 0;
        _sourceInit = 0;
        _sourceDestroy = 0;
        _sourceSearch = 0;
        _sourceGetForecast = 0;
        _sourceGetDetail = 0;

#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED

            if(1){
                #ifdef LIBXML
                //Walk the tree:
                const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
                processNode(pNode);
                #else //LIBXML

                QDomElement root = _doc.documentElement();
                QDomNode n = root.firstChild();
                while (!n.isNull()){
                    QDomElement el = n.toElement();
                    QString tag = el.tagName();

                    if (tag == "name"){
                        _name->assign(el.text().toStdString());
                    }else if (tag == "logo"){
                        _logo->assign(el.text().toStdString());
                    }else if (tag == "search"){
                        _hasSearch = (el.text() == "true") ? true : false;
                    }else if (tag == "library"){
                        _library->assign(el.text().toStdString());
                    }else if (tag == "binary"){
                        _binary->assign(el.text().toStdString());
                    }else if (tag == "forecast"){
                        if (el.hasAttribute("url"))
                            _url_template->assign(el.attribute("url").toStdString());
                    }else if (tag == "detail"){
                        if (el.hasAttribute("url")){
                            _hasDetail = (el.text() == "true") ? true : false;
                            _url_detail_template->assign(el.attribute("url").toStdString());
                        }
                    }else if (tag =="showurl"){
                        if (el.hasAttribute("url"))
                            _url_for_view->assign(el.attribute("url").toStdString());
                    }else if (tag =="cookie"){
                        _cookie->assign(el.text().toStdString());
                    }
                    n = n.nextSibling();
                }
                #endif //LIBXML
                // TODO check binaryName for empty
                /*
                std::string binaryWithPath = prefix + libPath;
                binaryWithPath += *_library;
                _libraryHandler = dlopen(binaryWithPath.c_str(), RTLD_NOW);
                if(!_libraryHandler)
                    throw(std::string("Failed, while loading source library."));
                void *_handler = 0;
                char *possibleError = 0;
                // init function from source
                dlerror();
                _handler = dlsym(_libraryHandler, "init");
                possibleError = dlerror();
                if(possibleError)
                    throw(std::string(possibleError));
                _sourceInit = (void(*)())_handler;
                // search function from source
                _handler = possibleError = 0;
                dlerror();
                _handler = dlsym(_libraryHandler, "search");
                possibleError = dlerror();
                if(!possibleError){
                    _sourceSearch = (StationsList& (*)(const std::string&))_handler;
                }
                // forecast function from source
                _handler = possibleError = 0;
                dlerror();
                _handler = dlsym(_libraryHandler, "forecast");
                possibleError = dlerror();
                if(!possibleError){
                    _sourceGetForecast = (bool (*)(const std::string&, const std::string&))_handler;
                    _hasForecast = true;
                }
                // detail function from source
                _handler = possibleError = 0;
                dlerror();
                _handler = dlsym(_libraryHandler, "detail");
                possibleError = dlerror();
                if(!possibleError){
                    _sourceGetDetail = (bool (*)(const std::string&, const std::string&))_handler;
                    _hasDetail = true;
                }
                */
            }

#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }catch(const std::exception& ex){
            throw(ex.what());
        }
        catch(std::string& er){
            throw(er);
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED

    }
////////////////////////////////////////////////////////////////////////////////
    Source::~Source(){
        delete _name;
        delete _logo;
        if (_library)
            delete _library;
        if (_binary)
            delete _binary;
        delete _url_template;
        delete _url_detail_template;
        delete _url_for_view;
        if(_libraryHandler)
            dlclose(_libraryHandler);
    }
////////////////////////////////////////////////////////////////////////////////
    Source& Source::operator=(const Source& source){
        if(this != &source){
            delete _name;
            _name = new std::string(*(source._name));
            delete _logo;
            _logo = new std::string(*(source._logo));
            delete _library;
            _library = new std::string(*(source._library));
            delete _binary;
            _binary = new std::string(*(source._binary));
            delete _url_template;
            _url_template = new std::string(*(source._url_template));
            delete _url_detail_template;
            _url_detail_template = new std::string(*(source._url_detail_template));
            delete _url_for_view;
            _url_for_view = new std::string(*(source._url_for_view));
            delete _cookie;
            _cookie = new std::string(*(source._cookie));

        }
        return *this;
    }
////////////////////////////////////////////////////////////////////////////////
#ifdef LIBXML
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
        // forecast tag
        if(nodeName == "forecast"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            std::string str = nodeText->get_content();
            (str.compare("true")) ? (_hasForecast = false) : (_hasForecast = true);
            return;
        }
        // detail tag
        if(nodeName == "detail"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            std::string str = nodeText->get_content();
            (str.compare("true")) ? (_hasDetail = false) : (_hasDetail = true);
            return;
        }
        // library tag
        if(nodeName == "library"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            _library->assign(nodeText->get_content());
            return;
        }
        // binary tag
        if(nodeName == "binary"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            _binary->assign(nodeText->get_content());
            return;
        }
        // url_template tag
        if(nodeName == "forecast"){
            const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
            const xmlpp::Attribute* attribute = nodeElement->get_attribute("url");
            if (attribute)
                _url_template->assign(attribute->get_value().c_str());
            return;
        }
	// url_template tag
        if(nodeName == "detail"){
            const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
            const xmlpp::Attribute* attribute = nodeElement->get_attribute("url");
            if (attribute)
                _url_detail_template->assign(attribute->get_value().c_str());
            return;
        }

        // url_showurl tag
        if(nodeName == "showurl"){
            const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
            const xmlpp::Attribute* attribute = nodeElement->get_attribute("url");
            if (attribute)
                _url_for_view->assign(attribute->get_value().c_str());
            return;
        }
        // cookie tag
        if(nodeName == "cookie"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            _cookie->assign(nodeText->get_content());

            return;
        }

    }
#endif
////////////////////////////////////////////////////////////////////////////////
    std::string& Source::name() const{
        return *_name;
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Source::logo() const{
        return *_logo;
    }
////////////////////////////////////////////////////////////////////////////////
    StationsList& Source::search(const std::string& station){
        return _sourceSearch(station);
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Source::url_template() const{
        return *_url_template;
    }
////////////////////////////////////////////////////////////////////////////////
    std::string& Source::url_detail_template() const{
        return *_url_detail_template;
    }

/////////////////////////////////////////////////////////////////////////////////
    std::string& Source::url_for_view() const{
        return *_url_for_view;
    }
/////////////////////////////////////////////////////////////////////////////////
    std::string& Source::cookie() const{
        return *_cookie;
    }
///////////////////////////////////////////////////////////////////////////////
    std::string& Source::binary() const{
        return *_binary;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
