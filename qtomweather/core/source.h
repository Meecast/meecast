#ifndef SOURCE_H
#define SOURCE_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <dlfcn.h>
class StationList;
#include "stationlist.h"
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Source : public Parser {
        #ifdef LIBXML
        void processNode(const xmlpp::Node* node);
        #endif
        std::string *_name;
        std::string *_logo;
        std::string *_library;
        void *_libraryHandler;
        bool _hasForecast;
        bool _hasDetail;
        // source init function
        void (*_sourceInit)();
        // source destroy function
        void (*_sourceDestroy)();
        // source search station function
        StationList& (*_sourceSearch)(const std::string& station);
        // source get forecast url function
        bool (*_sourceGetForecast)(const std::string& id, const std::string& filename);
        // source get detail url function
        bool (*_sourceGetDetail)(const std::string& id, const std::string& filename);
        public:
            Source(const std::string& filename,
                   const std::string& schema_filename = prefix + schemaPath + "source.xsd");
            Source& operator=(const Source& source);
            std::string& name() const;
            std::string& logo() const;
            virtual ~Source();
            StationList& search(const std::string& station);
    };
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // SOURCE_H
