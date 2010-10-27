#ifndef PARSER_H
#define PARSER_H
////////////////////////////////////////////////////////////////////////////////
#include <libxml++/libxml++.h>
#include <string>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Parser{
        protected:
            xmlpp::DomParser *parser;
            Parser();
        public:
            virtual ~Parser();
            virtual void parse(const std::string& filename) = 0;
    };

} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // PARSER_H
