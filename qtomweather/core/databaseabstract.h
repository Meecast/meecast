#ifndef DATABASEABSTRACT_H
#define DATABASEABSTRACT_H

#include <vector>
#include <string>

namespace Core{

typedef std::vector< std::pair<std::string, std::string> > listdata;

class DatabaseAbstract
{

public:
    virtual void set_databasename(const std::string& filename) = 0;
    virtual bool open_database() = 0;
    virtual listdata* create_countries_list() = 0;
    virtual listdata* create_region_list(int country_id) = 0;
    virtual listdata* create_stations_list(int region_id) = 0;
};
} // namespace Core
#endif // DATABASEABSTRACT_H
