#include "dataparser.h"
#include <iostream>
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    DataParser::DataParser(const std::string& filename, const std::string& schema_filename) : Parser(filename, schema_filename) {
        _timezone = 0;
        _list = new DataList;
#ifdef LIBXML
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
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
#else
        QDomElement root = _doc.documentElement();
        Data* forecast_data;

        QDomNodeList nodelist = root.elementsByTagName("timezone");
        if (nodelist.count() == 1) {
            _timezone = nodelist.at(0).toElement().text().toInt();
        }
        nodelist = root.elementsByTagName("period");
        for (int i=0; i<nodelist.count(); i++){
            forecast_data = new Data();
            QDomElement e = nodelist.at(i).toElement();
            forecast_data->StartTime(e.attribute("start").toInt());
            forecast_data->EndTime(e.attribute("end").toInt());
            if (e.hasAttribute("current") && e.attribute("current") == "true")
                forecast_data->Current(1);
            QDomNode n = e.firstChild();
            while (!n.isNull()){
                QDomElement el = n.toElement();
                QString tag = el.tagName();

                if (tag == "temperature" || tag == "temperature_hi"){
                    forecast_data->temperature_hi().value(el.text().toFloat());
                }else if (tag == "temperature_low"){
                    forecast_data->temperature_low().value(el.text().toFloat());
                }else if (tag == "icon"){
                    forecast_data->Icon(el.text().toInt());
                }else if (tag == "description"){
                    forecast_data->Text(el.text().toStdString());
                }else if (tag == "humidity"){
                    forecast_data->Humidity(el.text().toInt());
                }else if (tag == "wind_speed"){
                    if (el.text() == "calm")
                        forecast_data->WindSpeed(0);
                    else
                        forecast_data->WindSpeed(el.text().toFloat());
                }else if (tag == "wind_direction"){
                    forecast_data->WindDirection(el.text().toStdString());
                }else if (tag == "flike"){
                    forecast_data->Flike().value(el.text().toFloat());
                }else if (tag == "pressure"){
                    forecast_data->Pressure(el.text().toInt());
                }else if (tag == "ppcp")
                    forecast_data->Ppcp(el.text().toFloat());

                n = n.nextSibling();
            }
            _list->push_back(forecast_data);
        }
#endif
    }
////////////////////////////////////////////////////////////////////////////////
    DataParser::~DataParser(){
        delete _list;
    }
////////////////////////////////////////////////////////////////////////////////
#ifdef LIBXML
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
            attribute = nodeElement->get_attribute("current");

            if (attribute && attribute->get_value() == "true"){
                forecast_data->Current(1);
            }
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
            /* std::cout<<"icon "<< nodeText->get_content() <<std::endl; */
            forecast_data->Icon(atoi(nodeText->get_content().c_str()));
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
        // Humidity tag
        if(nodeName == "humidity"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"Humidity "<< nodeText->get_content() <<std::endl; */
            forecast_data->Humidity(atoi(nodeText->get_content().c_str()));
            return;
        }
        // Wind speed tag
        if(nodeName == "wind_speed"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"Wind speed "<< nodeText->get_content() <<std::endl; */
            forecast_data->WindSpeed    (atoi(nodeText->get_content().c_str()));
            return;
        }
        // Wind direction tag
        if(nodeName == "wind_direction"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"Wind direction "<< nodeText->get_content() <<std::endl; */
            forecast_data->WindDirection(nodeText->get_content().c_str());
            return;
        }
        // Flike tag
        if(nodeName == "flike"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"flike "<< nodeText->get_content() <<std::endl;*/
            forecast_data->Flike().value(atof(nodeText->get_content().c_str()));
            return;
        }
        // Pressure tag
        if(nodeName == "pressure"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"pressure "<< nodeText->get_content() <<std::endl;*/
            forecast_data->Pressure(atoi(nodeText->get_content().c_str()));
            return;
        }
        // Ppcp tag
        if(nodeName == "ppcp"){
            xmlpp::Node::NodeList list = node->get_children();
            xmlpp::Node::NodeList::iterator iter = list.begin();
            const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
            /* std::cout<<"ppcp "<< nodeText->get_content() <<std::endl;*/
            forecast_data->Ppcp(atof(nodeText->get_content().c_str()));
            return;
        }

    }
#endif
////////////////////////////////////////////////////////////////////////////////
    DataList& DataParser::data(){
        return *_list;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
