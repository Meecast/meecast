#ifndef STATION_H
#define STATION_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
class Source;
#include "source.h"
#include "data.h"
#include "sourcelist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Station{
        std::string *_sourceName;
        std::string *_id;
        std::string *_name;
        std::string *_country;
        std::string *_region;
        std::string *_fileName;
        std::string *_forecastURL;
        int _timezone;
        Source *_source;
        Data *_data; /* Is it really needed ? Vlad */
        bool dataValid();
        bool prepareFile();
        public:
        Station(const std::string& source_name, const std::string& id, const std::string& name,
                    const std::string& country = 0, const std::string& region = 0, 
                    const std::string& forecastURL = 0);
            Station(const Station& station);
            Station& operator=(const Station& station);
            virtual ~Station();
            void name(const std::string& name);
            std::string& name() const;
            void id(const std::string& id);
            std::string& id() const;
            void sourceName(const std::string& sourceName);
            std::string& sourceName() const;
            void country(const std::string& country);
            std::string& country() const;
            void region(const std::string& region);
            std::string& region() const;
            void forecastURL(const std::string& region);
            std::string& forecastURL() const;
            void timezone(const int timezone = 0);
            int timezone() const;
            bool updateData(bool force);
            void updateSource(const Source* source);
            void fileName(const std::string& fileName);
            std::string& fileName() const;
    };
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // STATION_H
