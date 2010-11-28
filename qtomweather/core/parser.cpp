#include "parser.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Parser::Parser(const std::string& filename, const std::string& schema_filename){
        if(filename.empty())
            throw("Invalid source file.");
        int r = access(filename.c_str(), R_OK);
        if(r)
            throw(std::string(strerror(errno)));
        if(schema_filename.empty())
            throw("Invalid source schema file.");
        r = access(schema_filename.c_str(), R_OK);
        if(r)
            throw(std::string(strerror(errno)));
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            parser = new xmlpp::DomParser;
            validator = new xmlpp::SchemaValidator;
            parser->set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.

            validator->parse_file(schema_filename);
            if(validator->validate(filename))
                throw("Document is not valid.");
            parser->parse_file(filename);
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
