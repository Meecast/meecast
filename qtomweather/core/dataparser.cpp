#include "dataparser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    DataParser::DataParser() : Parser() {
        validator->parse_file("/usr/share/omweather/schemas/data.xsd");
    }
////////////////////////////////////////////////////////////////////////////////
    DataParser::~DataParser(){
    }
////////////////////////////////////////////////////////////////////////////////
    void DataParser::parse(const std::string& filename){
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            if(validator->validate(filename))
                std::cout << "Document is not valid." << std::endl;
            parser->parse_file(filename);
            if(parser){
                //Walk the tree:
                const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
                processNode(pNode);
            }
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            std::cout << "Exception caught: " << ex.what() << std::endl;
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }
////////////////////////////////////////////////////////////////////////////////
    void DataParser::processNode(const xmlpp::Node* node){
        if(!node)
            return;
        std::cout << "Node name: " << node->get_name() << std::endl;
        if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node)){

        }
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
