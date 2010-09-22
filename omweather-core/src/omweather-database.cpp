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
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include "omweather-database.hpp"
#include <fstream>
#include <iostream>
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
    std::ifstream file(full_name.c_str());
    if(!file.is_open())
        return false;
    if(sqlite3_open_v2(full_name.c_str(), &db, SQLITE_OPEN_READONLY, NULL))
        return false;
    return true;
}
/*******************************************************************************/
void Database::close(){
    if(db)
        sqlite3_close(db);
}
/*******************************************************************************/
int callback(void *user_data, int argc, char **argv, char **azColName){
    Result *r = (Result*)user_data;
    int i;
    Item itm;
    for(i = 0; i < argc; i++)
        itm.insert(std::pair<std::string,std::string>(azColName[i],argv[i]));
    r->push_back(itm);
    return 0;
}
/*******************************************************************************/
bool Database::countries(CountriesList& list){
    char *errMsg = NULL;
    bool id_ready = false, name_ready = false;
    Result *r = new Result;
    r->clear();
    list.clear();
    int rc = sqlite3_exec(db, "SELECT name, id FROM countries ORDER BY name",
                            callback, (void*)r, &errMsg);
    if(rc != SQLITE_OK){
        sqlite3_free(errMsg);
        return false;
    }

    std::map<std::string,std::string>::iterator it;
    for(unsigned i = 0; i < r->size(); i++){
        std::string name;
        int id = -1;
        Item itm = r->at(i);
        it = itm.find("name");
        if(it != itm.end()){
            name = it->second;
            name_ready = true;
        }
        it = itm.find("id");
        if(it != itm.end()){
            id = atoi(it->second.c_str());
            id_ready = true;
        }
        if(name_ready && id_ready){
            list.push_back(Country(id, name));
            name_ready = false;
            id_ready = false;
        }
    }
    delete r;
    return true;
}
/*******************************************************************************/
bool Database::regions(int country_id, RegionsList& list){
    char *errMsg = NULL;
    bool id_ready = false, name_ready = false;
    Result *r = new Result;
    r->clear();
    list.clear();
    std::string sql = "SELECT id, name, longititudemax, latitudemax, longititudemin, latitudemin FROM regions WHERE country_id = ";
    sql += country_id + " ORDER BY name";
    int rc = sqlite3_exec(db, sql.c_str(), callback, (void*)r, &errMsg);
    if(rc != SQLITE_OK){
        sqlite3_free(errMsg);
        return false;
    }

    std::map<std::string,std::string>::iterator it;
    for(unsigned i = 0; i < r->size(); i++){
        std::string name;
        int id = -1;
        float longitude_max = 0.0f, latitude_max = 0.0f,
                longitude_min = 0.0f, latitude_min = 0.0f;
        Item itm = r->at(i);
        it = itm.find("name");
        if(it != itm.end()){
            name = it->second;
            name_ready = true;
        }
        it = itm.find("id");
        if(it != itm.end()){
            id = atoi(it->second.c_str());
            id_ready = true;
        }
        it = itm.find("longititudemax");
        if(it != itm.end())
            longitude_max = atof(it->second.c_str());
        it = itm.find("latitudemax");
        if(it != itm.end())
            latitude_max = atof(it->second.c_str());
        it = itm.find("longititudemin");
        if(it != itm.end())
            longitude_min = atof(it->second.c_str());
        it = itm.find("latitudemin");
        if(it != itm.end())
            latitude_min = atof(it->second.c_str());
        if(name_ready && id_ready){
            list.push_back(
                Region(id, name, longitude_max, latitude_max, longitude_min, latitude_min)
            );
            name_ready = false;
            id_ready = false;
        }
    }
    delete r;
    return true;
}
/*******************************************************************************/
