#ifndef STATION_H
#define STATION_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include "data.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Station{
        std::string *_name;
        std::string *_id;
        std::string *_country;
        std::string *_region;
        std::string *_fileName;
        int _timezone;
        Data *_data;
        bool dataValid();
        bool prepareFile();
        public:
            Station(const std::string& id, const std::string& name = 0,
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
    };
////////////////////////////////////////////////////////////////////////////////
typedef std::vector<Station*> StatList;
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // STATION_H
