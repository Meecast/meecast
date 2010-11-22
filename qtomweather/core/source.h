#ifndef SOURCE_H
#define SOURCE_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Source : public Parser{
        bool parse(const std::string& filename);
        void processNode(const xmlpp::Node* node);
        std::string *_filename;
        std::string *_name;
        std::string *_logo;
        std::string *_forecastUrl;
        std::string *_detailUrl;
        std::string *_searchUrl;
        std::string *_databaseName;
        std::string *_binaryName;
        public:
            Source(const std::string& filename);
            Source(const Source& source);
            Source& operator=(const Source& source);
            virtual ~Source();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // SOURCE_H
