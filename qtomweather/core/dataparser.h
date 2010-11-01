#ifndef DATAPARSER_H
#define DATAPARSER_H
////////////////////////////////////////////////////////////////////////////////
#include "parser.h"
#include "data.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class DataParser : public Parser {
        void processNode(const xmlpp::Node* node);
        public:
            DataParser();
            DataParser(const std::string& filename);
            virtual ~DataParser();
            bool parse(const std::string& filename);
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATAPARSER_H
