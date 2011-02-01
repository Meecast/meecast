#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>

#include "databaseabstract.h"

namespace Core{

//typedef std::vector< std::pair<std::string, std::string> > listdata;

class DatabaseSqlite : public DatabaseAbstract
{
private:
    sqlite3 *db;
    std::string *databasename;

public:
    DatabaseSqlite(const std::string& filename);
    ~DatabaseSqlite();
    void set_databasename(const std::string& filename);
    bool open_database();
    listdata* create_countries_list();
    listdata* create_region_list(int country_id);
    listdata* create_stations_list(int region_id);
};
} // namespace Core

#endif // DATABASESQLITE_H
