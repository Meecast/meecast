#include "dataparser.h"
#include <iostream>
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    DataParser::DataParser(const std::string& filename, const std::string& schema_filename) : Parser(filename, schema_filename) {
        _timezone = 0;
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            _list = new DataList;
            if(parser){
                //Walk the tree:
                const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
                processNode(pNode);
            }
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            throw(ex.what());
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }
////////////////////////////////////////////////////////////////////////////////
    DataParser::~DataParser(){
        delete _list;
    }
////////////////////////////////////////////////////////////////////////////////
    void DataParser::processNode(const xmlpp::Node* node){
        static Data* forecast_data;
        if(!node)
            return;
        std::string nodeName = node->get_name();

        // source tag
        if(nodeName == "station"){
            xmlpp::Node::NodeList list = node->get_children();
            for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
                processNode(*iter);
        }
        // timezone tag
        if(nodeName == "timezone"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            _timezone = atoi(nodeText->get_content().c_str());
            /* std::cout<<"timezone "<<_timezone<<std::endl; */
            return;
        }
        // period tag
        if(nodeName == "period"){
            forecast_data = new Data();
            const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
            const xmlpp::Attribute* attribute = nodeElement->get_attribute("start");
            forecast_data->StartTime(atoi(attribute->get_value().c_str()));
            attribute = nodeElement->get_attribute("end");
            forecast_data->EndTime(atoi(attribute->get_value().c_str()));
            xmlpp::Node::NodeList list = node->get_children();
            for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
                processNode(*iter);
            _list->push_back(forecast_data);
        }
        // temperature tag
        if(nodeName == "temperature"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"temperature "<< nodeText->get_content() <<std::endl; */
            forecast_data->temperature_hi().value(atof(nodeText->get_content().c_str()));
            return;
        }
        // temperature tag
        if(nodeName == "temperature_low"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"temperature "<< nodeText->get_content() <<std::endl; */
            forecast_data->temperature_low().value(atof(nodeText->get_content().c_str()));
            return;
        }
        // temperature tag
        if(nodeName == "temperature_hi"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"temperature "<< nodeText->get_content() <<std::endl; */
            forecast_data->temperature_hi().value(atof(nodeText->get_content().c_str()));
            return;
        }
        // icon tag
        if(nodeName == "icon"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            std::cout<<"icon "<< nodeText->get_content() <<std::endl;
            /* forecast_data->Icon(atoi(nodeText->get_content().c_str())); */
            return;
        }
        // Text tag
        if(nodeName == "description"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"Description "<< nodeText->get_content() <<std::endl; */
            forecast_data->Text(nodeText->get_content().c_str());
            return;
        }

    }
////////////////////////////////////////////////////////////////////////////////
    DataList& DataParser::data(){
        return *_list;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
