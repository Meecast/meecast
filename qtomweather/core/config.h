#ifndef CONFIG_H
#define CONFIG_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <QTextStream>
#include "parser.h"
#include "stationlist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Config : public Parser{
            #ifdef LIBXML
            void processNode(const xmlpp::Node* node);
            #endif
            std::string *_pathPrefix;
            std::string *_iconset;
            std::string *_temperature_unit;
            std::string *_font_color;
            std::string *_current_station_id;
            StationsList *_stations;
        public:
            Config(const std::string& filename, const std::string& schema_filename = prefix + schemaPath + "config.xsd");
            Config();
            Config(const Config& config);
            Config& operator=(const Config& config);
            virtual ~Config();
            std::string& prefix_path(void);
            void iconSet(const std::string& text);
            std::string& iconSet(void);
            void current_station_id(const std::string& text);
            std::string& current_station_id(void);
            void TemperatureUnit(const std::string& text);
            std::string& TemperatureUnit(void);
            void FontColor(const std::string& text);
            std::string& FontColor(void);
            StationsList& stationsList();
            //void stationsList(std::vector<Core::Station*> list);
            void stationsList(StationsList list);
            void saveConfig(const std::string& filename);
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // CONFIG_H
