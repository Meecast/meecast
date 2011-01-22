#ifndef PARSER_H
#define PARSER_H
////////////////////////////////////////////////////////////////////////////////
#include <libxml++/libxml++.h>
#include <libxml++/validators/schemavalidator.h>
#include <string>
#include <iostream>
#include "abstractconfig.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Parser : public AbstractConfig{
        protected:
            xmlpp::DomParser *parser;
            xmlpp::SchemaValidator *validator;
            Parser(const std::string& filename, const std::string& schema_filename);
        public:
            virtual ~Parser();
            virtual void processNode(const xmlpp::Node* node) = 0;
    };

} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // PARSER_H
