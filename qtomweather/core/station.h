#ifndef STATION_H
#define STATION_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include "data.h"
#include "source.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Station{
        std::string *_name;
        std::string *_id;
        std::string *_country;
        std::string *_region;
        std::string *_fileName;
        int _timezone;
        Data *_data; /* Is it really needed ? Vlad */
        Source *_source;
        bool dataValid();
        bool prepareFile();
        public:
        Station(const std::string& source_name, const std::string& id, const std::string& name = "",
                    const std::string& country = "", const std::string& region = "");
            Station(const Station& station);
            Station& operator=(const Station& station);
            virtual ~Station();
            void name(const std::string& name);
            std::string& name() const;
            void id(const std::string& id);
            std::string& id() const;
            void timezone(const int timezone = 0);
            void source (Source *source);
            int timezone() const;
            bool updateData(bool force = false);
            std::string& forecastURL() const;
    };
////////////////////////////////////////////////////////////////////////////////
typedef std::vector<Station*> StatList;
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // STATION_H
