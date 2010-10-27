#ifndef STATION_H
#define STATION_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "source.h"
#include "data.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Station{
        std::string *_name;
        std::string *_id;
        std::string *_fileName;
        int _timezone;
        Source *_source;
        Data *_data;
        bool dataValid();
        bool prepareFile();
        public:
            Station();
            Station(const Station& station);
            Station& operator=(const Station& station);
            virtual ~Station();
            void name(const std::string& name);
            std::string& name() const;
            void id(const std::string& id);
            std::string& id() const;
            void timezone(const int timezone = 0);
            int timezone() const;
            void source(const Source& source);
            Source& source() const;
            bool updateData(bool force = false);
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // STATION_H
