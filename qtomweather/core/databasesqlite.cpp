#include "databasesqlite.h"

namespace Core {

DatabaseSqlite::DatabaseSqlite(const std::string& filename)
{
    db = NULL;
    databasename = new std::string;
    databasename->assign(filename);
}

bool
DatabaseSqlite::open_database()
{
    std::string lang = std::string();
    int rc;
    std::string key ("gismeteo.ru.db");
    std::cerr << (databasename->length() - databasename->rfind(key)) << " " << key.length() << std::endl;
    if (sqlite3_open(databasename->c_str(), &db)){
        std::cerr << "error open " << *databasename << std::endl;
        return false;
    }
    if ((databasename->length() - databasename->rfind(key)) == key.length()) {
        getenv("LC_ALL");
        if(!lang.empty()){
            lang = getenv("LC_MESSAGES");
            if(!lang.empty())
                lang = getenv("LANG");
        }

        if (!lang.empty() && !lang.compare("ru_RU"))
            rc = sqlite3_exec(db, "CREATE TEMP VIEW nstations AS SELECT russian_name as name, id, region_id, longititude, latitude, code, id_gismeteo_new, id_gismeteo_old  FROM stations",
                  NULL, NULL, NULL);
        else
            rc = sqlite3_exec(db, "CREATE TEMP VIEW nstations AS SELECT * FROM stations where russian_name != name",
                  NULL, NULL, NULL);
    }else {
        rc = sqlite3_exec(db, "CREATE TEMP VIEW nstations AS SELECT * FROM stations",
                  NULL, NULL, NULL);
    }
    if(rc != SQLITE_OK){
        std::cerr << "Problem in sqlite3_exec" << std::endl;
        return false;
    }
    return true;
}

} // namespace Core
