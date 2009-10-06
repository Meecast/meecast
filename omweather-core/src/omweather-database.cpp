/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 *  for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *            az@pocketpcrussia.com 
 *  for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include "omweather-database.hpp"
#include <fstream>
/*******************************************************************************/
#define DATABASEPATH        "/usr/share/omweather/db/"
/*******************************************************************************/
Database::Database(){
    db = NULL;
    result = NULL;
}
/*******************************************************************************/
Database::~Database(){
    close();
    if(result)
        delete result;
}
/*******************************************************************************/
bool Database::open(const std::string name){
    if(name.empty())
        return false;
    std::string full_name = DATABASEPATH + name;
    std::fstream file(full_name.c_str());
    if(!file.is_open())
        return false;
    if(sqlite3_open_v2(full_name.c_str(), &db, SQLITE_OPEN_READONLY, NULL))
        return false;
}
/*******************************************************************************/
void Database::close(){
    if(db)
        sqlite3_close(db);
}
/*******************************************************************************/
int Database::callback(void *user_data, int argc, char **argv, char **azColName){
    Result *r = (Result*)user_data;
    int i;
    for(i = 0; i < argc; i++){
        std::string name = azColName[i];
        std::string value = argv[i];
        r->insert(std::pair<std::string,std::string>(name, value));
    }
    return i;
}
/*******************************************************************************/
CountriesList& Database::countries(){
    char *errMsg = NULL;
    Result *r = new Result;
    r->clear();
    int rc = sqlite3_exec(db, "SELECT name, id FROM countries where \
                                (select count(name) from nstations where \
                                nstations.region_id = \
                                (select distinct regions.id from regions where \
                                regions.country_id=countries.id )) > 0 \
                                ORDER BY name", callback, (void*)list, &errMsg);
    if(rc != SQLITE_OK){
        sqlite3_free(errMsg);

    }

}
/*******************************************************************************/
