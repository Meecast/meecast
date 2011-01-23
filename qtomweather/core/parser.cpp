#include "parser.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Parser::Parser(){}
    Parser::Parser(const std::string& filename, const std::string& schema_filename)
                        : AbstractConfig(){
    #ifdef LIBXML
        if(filename.empty())
            throw("Invalid source file.");
        int r = access(filename.c_str(), R_OK);
        if(r)
            throw("File: " + filename + " - " + std::string(strerror(errno)));
        if(schema_filename.empty())
            throw("Invalid source schema file.");
        r = access(schema_filename.c_str(), R_OK);
        if(r)
            throw("File: " + filename + " - " + std::string(strerror(errno)));
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
    #else //LIBXML
        QXmlSchema schema;
        if (!schema.load(QUrl(":" + QString::fromStdString(schema_filename)))){
            throw("Invalid schema file");
            return;
        }
        if (!schema.isValid()){
            throw("Schema is invalid");
            return;
        }
        QFile file(QString::fromStdString(filename));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            std::cerr<<"error file open"<<std::endl;
            throw("Invalid source file");
            return;
        }
        QXmlSchemaValidator validator(schema);
        if (!validator.validate(&file, QUrl::fromLocalFile(file.fileName()))){
            //qDebug() << "File " << filename << " is invalid";
            file.close();
            throw("Xml file is invalid");
            return;
        }
        file.close();
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!_doc.setContent((&file))){
            file.close();
            throw("Error set content");
            return;
        }
        file.close();
    #endif //LIBXML
    }
////////////////////////////////////////////////////////////////////////////////
    Parser::~Parser(){
    #ifdef LIBXML
        delete parser;
        delete validator;
    #endif
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

