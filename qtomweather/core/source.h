#ifndef SOURCE_H
#define SOURCE_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Source : public Parser{
        void parse(const std::string& filename);
        std::string *_name;
        std::string *_logo;
        std::string *_forecastUrl;
        std::string *_detailUrl;
        std::string *_searchUrl;
        std::string *_databaseName;
        std::string *_binaryName;
        public:
            Source();
            Source(const Source& source);
            Source& operator=(const Source& source);
            virtual ~Source();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // SOURCE_H
