#ifndef DATAPARSER_H
#define DATAPARSER_H
////////////////////////////////////////////////////////////////////////////////
#include "parser.h"
#include "data.h"
#include "datalist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class DataParser : public Parser {
        Data *_data;
        DataList *_list;
        void processNode(const xmlpp::Node* node);
        public:
            DataParser();
            virtual ~DataParser();
            bool parse(const std::string& filename);
            DataList& data();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATAPARSER_H
