#include "data.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Data::Data() : Parser(){
        parser->set_validate(true);
    }
////////////////////////////////////////////////////////////////////////////////
    void Data::parse(const std::string& filename){
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            parser->parse_file(filename);
            if(parser){
                //Walk the tree:
                const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
//                print_node(pNode);
            }
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            std::cout << "Exception caught: " << ex.what() << std::endl;
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }
////////////////////////////////////////////////////////////////////////////////
    void Data::wallkTree(const xmlpp::Node* node){
        if(!node)
            return;
        const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node)

        if(nodeElement){
            const Glib::ustring nodename = node->get_name();
            if(nodename == "station"){
                const xmlpp::Attribute* attribute = nodeElement->get_attribute("name");
                if(attribute)
            }
            //Print attributes:
            const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
            for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter){
                const xmlpp::Attribute* attribute = *iter;
                const Glib::ustring namespace_prefix = attribute->get_namespace_prefix();
                if(namespace_prefix.empty())
                    std::cout << "  Attribute " << attribute->get_name() << " = " << attribute->get_value() << std::endl;
                else
                    std::cout << "  Attribute " << namespace_prefix  << ":" << attribute->get_name() << " = " << attribute->get_value() << std::endl;
            }

            const xmlpp::Attribute* attribute = nodeElement->get_attribute("title");
            if(attribute)
            {
              std::cout << "title found: =" << attribute->get_value() << std::endl;
            }
          }

          if(!nodeContent)
          {
            //Recurse through child nodes:
            xmlpp::Node::NodeList list = node->get_children();
            for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
            {
              print_node(*iter, indentation + 2); //recursive
            }
          }

    }
////////////////////////////////////////////////////////////////////////////////
    void Data::clear(){

    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
