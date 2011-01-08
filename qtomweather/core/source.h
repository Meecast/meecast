#ifndef SOURCE_H
#define SOURCE_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Source : public Parser{
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
            Source(const std::string& filename, const std::string& schema_filename = "/usr/share/omweather/schemas/source.xsd");
            Source& operator=(const Source& source);
            std::string& name() const;
            std::string& logo() const;
            std::string& forecastURL() const;
            virtual ~Source();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // SOURCE_H
