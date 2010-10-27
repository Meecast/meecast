#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Parser::Parser(){
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            parser = new xmlpp::DomParser;
            parser->set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            std::cout << "Exception caught: " << ex.what() << std::endl;
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }
////////////////////////////////////////////////////////////////////////////////
    Parser::~Parser(){
        delete parser;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

