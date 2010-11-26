#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Parser::Parser(){
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            parser = new xmlpp::DomParser;
            validator = new xmlpp::SchemaValidator;
            parser->set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            throw(ex.what());
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }
////////////////////////////////////////////////////////////////////////////////
    Parser::~Parser(){
        delete parser;
        delete validator;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

