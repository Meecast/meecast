#ifndef DATAPARSER_H
#define DATAPARSER_H
////////////////////////////////////////////////////////////////////////////////
#include "parser.h"
#include "data.h"
#include "datalist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class DataParser : public Parser {
        DataList *_list;
        int _timezone;
        #ifdef LIBXML
        void processNode(const xmlpp::Node* node);
        #endif
        public:
            DataParser(const std::string& filename, const std::string& schema_filename = "/usr/share/omweather/schemas/data.xsd");
            virtual ~DataParser();
            DataList& data();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATAPARSER_H
