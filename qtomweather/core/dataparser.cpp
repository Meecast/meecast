#include "dataparser.h"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    DataParser::DataParser(const std::string& filename, const std::string& schema_filename) : Parser(filename, schema_filename) {
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            _data = new Data;
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
        delete _data;
        delete _list;
    }
////////////////////////////////////////////////////////////////////////////////
    void DataParser::processNode(const xmlpp::Node* node){
        if(!node)
            return;
    }
////////////////////////////////////////////////////////////////////////////////
    DataList& DataParser::data(){
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
