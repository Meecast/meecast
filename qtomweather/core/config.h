#ifndef CONFIG_H
#define CONFIG_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "parser.h"
#include "station.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Config : public Parser{
            void processNode(const xmlpp::Node* node);
            std::string *_pathPrefix;
            std::string *_iconset;
            std::string *_temperature_unit;
            std::string *_font_color;
            std::vector<Station*> _stations;
        public:
            const static std::string schemaPath;
            const static std::string iconsPath;
            const static std::string libPath;
            std::string& prefix() const;
            Config(const std::string& filename, const std::string& schema_filename = "/usr/" + schemaPath + "config.xsd");
            Config(const Config& config);
            Config& operator=(const Config& config);
            virtual ~Config();
            void iconSet(const std::string& text);
            std::string& iconSet(void);
            void TemperatureUnit(const std::string& text);
            std::string& TemperatureUnit(void);
            void FontColor(const std::string& text);
            std::string& FontColor(void);
            std::vector<Station*>& StationsList();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // CONFIG_H
