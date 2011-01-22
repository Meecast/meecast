#ifndef STATION_H
#define STATION_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
class Source;
#include "source.h"
#include "data.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Station{
        std::string *_sourceName;
        std::string *_name;
        std::string *_country;
        std::string *_region;
        std::string *_fileName;
        int _timezone;
        Source *_source;
        Data *_data; /* Is it really needed ? Vlad */
        bool dataValid();
        bool prepareFile();
        public:
        Station(const std::string& source_name, const std::string& name,
                    const std::string& country = 0, const std::string& region = 0);
            Station(const Station& station);
            Station& operator=(const Station& station);
            virtual ~Station();
            void name(const std::string& name);
            std::string& name() const;
            void id(const std::string& id);
            std::string& id() const;
            void timezone(const int timezone = 0);
            int timezone() const;
            bool updateData(bool force = false);
            void updateSource(const Source* source);
    };
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // STATION_H
