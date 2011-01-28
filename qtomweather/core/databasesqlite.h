#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <errno.h>
#include <stdlib.h>

namespace Core{

class DatabaseSqlite
{
private:
    sqlite3 *db;
    std::string *databasename;
public:
    DatabaseSqlite(const std::string& databasename);
    bool open_database();
};
} // namespace Core

#endif // DATABASESQLITE_H
