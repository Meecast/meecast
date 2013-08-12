/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2013 Vlad Vasilyeu
 * Copyright (C) 2006-2011 Pavel Fialko
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
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
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/


#include "databasesqlite.h"
#include <FApp.h>
#include <FIo.h>

using namespace Tizen::Base; 
using namespace Tizen::Base::Collection; 
using namespace Tizen::Io; 
using namespace Tizen::App;


namespace Core {

DatabaseSqlite::DatabaseSqlite(Tizen::Base::String& filename)
{
    //db = NULL;

    AppLog(" Create database");
    databasename = new Tizen::Base::String;
    databasename->Append(filename);
}

DatabaseSqlite::~DatabaseSqlite()
{
    delete databasename;
//    if (!db) sqlite3_close(db);
}

void
DatabaseSqlite::set_databasename(Tizen::Base::String& filename)
{
    databasename->Append(filename);
}

Tizen::Base::String&
DatabaseSqlite::get_databasename()
{
    AppLog("get_databasename %S", databasename->GetPointer() );
    return *databasename;
}

bool
DatabaseSqlite::open_database()
{
    std::string lang = std::string();
    int rc;
    char * msg;
    std::string key ("gismeteo.ru.db");

    result r = E_SUCCESS;
    r = db.Construct(databasename->GetPointer(), "r");
    TryReturn(!IsFailed(r), false, "Database::Construct failed with: %s", GetErrorMessage(r));
    if (IsFailed(r))
        return false;
    else
        return true;
//    std::cerr << (databasename->length() - databasename->rfind(key)) << " " << key.length() << std::endl;
/*
    if (sqlite3_open(databasename->c_str(), &db)){
        std::cerr << "error open " << *databasename << std::endl;
        return false;
    }
    if (databasename->rfind(key) != std::string::npos && (databasename->length() - databasename->rfind(key)) == key.length()) {
    if (getenv("LANG") != NULL)
        lang.assign(getenv("LANG"));
        if (!lang.empty() && !lang.compare("ru"))
            rc = sqlite3_exec(db, "CREATE TEMP VIEW nstations AS SELECT russian_name as name, id, region_id, longititude, latitude, code, id_gismeteo_new, id_gismeteo_old  FROM stations",
                  NULL, NULL, &msg);
        else
            rc = sqlite3_exec(db, "CREATE TEMP VIEW nstations AS SELECT * FROM stations where russian_name != name",
                  NULL, NULL, &msg);
    }else {
        rc = sqlite3_exec(db, "CREATE TEMP VIEW nstations AS SELECT * FROM stations",
                  NULL, NULL, &msg);
    }
    if(rc != SQLITE_OK){
        std::cerr << "Problem in sqlite3_exec " << rc << " - " << msg << std::endl;
        return false;
    }
*/
    return true;
}

Tizen::Base::Collection::HashMap* 
DatabaseSqlite::create_countries_list()
{
    listdata *list = NULL;
    int rc;
    char *errMsg = NULL;
    int nrow, ncol;
    Tizen::Base::Collection::HashMap *map; 
    String sql;
    DbEnumerator* pEnum;
    result r = E_SUCCESS;

    map = new Tizen::Base::Collection::HashMap;

        AppLog("Country1");

    map->Construct();
    sql.Append(L"CREATE TEMP VIEW nstations AS SELECT * FROM stations");

    r = db.ExecuteSql(sql, true);
    if (IsFailed(r)){
        AppLog("Problem with creation of TEMP VIEW");
        return map;
    }
    sql.Clear();
    sql.Append(L"SELECT id, name FROM countries where (select count(name) from nstations where nstations.region_id = (select distinct regions.id from regions where regions.country_id=countries.id )) >0 ORDER BY name");
    pEnum = db.QueryN(sql);

    String strWord;
    int nIndx = 0;
    while (pEnum->MoveNext() == E_SUCCESS){
        if (pEnum->GetStringAt(1, strWord) != E_SUCCESS){
            break;
        }
        map->Add(*(new (std::nothrow) Integer(nIndx++)), *(new (std::nothrow) String(strWord)));
    }
    if (pEnum != null){
        delete pEnum;
    }

    return map;
/*
    if(!db)
        return NULL;    
    list = new listdata;
    rc = sqlite3_get_table(db,
                           "SELECT id, name FROM countries where (select count(name) from nstations where nstations.region_id = (select distinct regions.id from regions where regions.country_id=countries.id )) >0 ORDER BY name",
                           &result,
                           &nrow,
                           &ncol,
                           &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        std::cerr << errMsg << std::endl;
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    for (int i=0; i<ncol*nrow; i=i+2){
        list->push_back(std::make_pair(result[ncol+i], result[ncol+i+1]));
    }
    sqlite3_free_table(result);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    */
}

Tizen::Base::Collection::HashMap* 
DatabaseSqlite::create_region_list_by_name(Tizen::Base::String& country_name)
{
    listdata *list = NULL;
    int rc;
    char *errMsg = NULL;
    int nrow, ncol;
    //std::string sql;

    Tizen::Base::Collection::HashMap *map; 
    String sql;
    DbEnumerator* pEnum;
    result r = E_SUCCESS;



    AppLog("Country name %S", country_name.GetPointer() );

    map = new Tizen::Base::Collection::HashMap;

    map->Construct();


    sql.Append(L"select id, name from regions where country_id = (select id from countries where name='");
    sql.Append(country_name);
    sql.Append("') order by name");
    pEnum = db.QueryN(sql);

    String strWord;
    int nIndx = 0;
    while (pEnum->MoveNext() == E_SUCCESS){
        if (pEnum->GetStringAt(1, strWord) != E_SUCCESS){
            break;
        }
        map->Add(*(new (std::nothrow) Integer(nIndx++)), *(new (std::nothrow) String(strWord)));
    }
    if (pEnum != null){
        delete pEnum;
    }


    return map;

//    list = new listdata;
    /*
    if(!db){
        return NULL;
    }
*/
//    if (country_name =="") return list;

/*
    snprintf(sql,
                sizeof(sql) - 1,
                "select id, name from regions where country_id = (select id from countries where name='%s') order by name",
                country_name.c_str());
    std::cerr <<"Select region: "<< sql << std::endl; 
    rc = sqlite3_get_table(db,
                           sql,
                           &result,
                           &nrow,
                           &ncol,
                           &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        std::cerr << errMsg << std::endl;
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    for (int i=0; i<ncol*nrow; i=i+2)
        list->push_back(std::make_pair(result[ncol+i], result[ncol+i+1]));

    sqlite3_free_table(result);
*/
}

listdata*
DatabaseSqlite::create_region_list(int country_id)
{
    listdata *list = NULL;
    int rc;
    char *errMsg = NULL;
    char **result;
    int nrow, ncol;
    //std::string sql;
    char sql[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    list = new listdata;
    /*
    if(!db){
        return NULL;
    }
*/
    if (country_id == 0 || country_id == -1) return list;

    if(country_id == 0) /* for GPS */
        snprintf(sql, sizeof(sql) - 1,
                 "SELECT id, name FROM regions");
    else {

        snprintf(sql,
                 sizeof(sql) - 1,
                 "SELECT id, name FROM regions WHERE country_id = %d ORDER BY name",
                 country_id);
    }
/*    std::cerr <<"Select region: "<< sql << std::endl; */
/*
    rc = sqlite3_get_table(db,
                           sql,
                           &result,
                           &nrow,
                           &ncol,
                           &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        std::cerr << errMsg << std::endl;
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    for (int i=0; i<ncol*nrow; i=i+2)
        list->push_back(std::make_pair(result[ncol+i], result[ncol+i+1]));

    sqlite3_free_table(result);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
*/
    return list;
}

Tizen::Base::Collection::HashMap* 
DatabaseSqlite::create_stations_list_by_name(Tizen::Base::String& country_name, Tizen::Base::String&  region_name)
{
    int rc;
    char *errMsg = NULL;
    int nrow, ncol;
/*
    if(!db || country_name =="" || region_name == "")
        return list;    // database doesn't open 
*/

    Tizen::Base::Collection::HashMap *map; 
    String sql;
    DbEnumerator* pEnum;
    result r = E_SUCCESS;


    map = new Tizen::Base::Collection::HashMap;

    map->Construct();

    sql.Append(L"select code, name from nstations where region_id = (select id from regions where name = '");
    sql.Append(region_name);
    sql.Append("' and country_id = (select id from countries where name= '");
    sql.Append(country_name);
    sql.Append("')) order by name");

    pEnum = db.QueryN(sql);

    String strWord;
    int nIndx = 0;

    while (pEnum && pEnum->MoveNext() == E_SUCCESS){
        if (pEnum->GetStringAt(1, strWord) != E_SUCCESS){
            break;
        }
        map->Add(*(new (std::nothrow) Integer(nIndx++)), *(new (std::nothrow) String(strWord)));
    }
    if (pEnum != null){
        delete pEnum;
    }


    return map;
    /*
    snprintf(sql, sizeof(sql) - 1,
            "select code, name from stations where region_id = \
            (select id from regions where name = '%s' and country_id = \
            (select id from countries where name= '%s')) order by name",
            region_name.c_str(), country_name.c_str());
    std::cerr << sql << std::endl;
    rc = sqlite3_get_table(db,
                           sql,
                           &result,
                           &nrow,
                           &ncol,
                           &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        std::cerr << errMsg << std::endl;
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    for (int i=0; i<ncol*nrow; i=i+2){
        list->push_back(std::make_pair(result[ncol+i], result[ncol+i+1]));
        // std::cerr << result[ncol+i] << " - " << result[ncol+i+1] << std::endl;
    }
    sqlite3_free_table(result);
*/
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}

Tizen::Base::String& 
DatabaseSqlite::get_station_code_by_name(Tizen::Base::String& country_name, 
                                         Tizen::Base::String& region_name,
                                         Tizen::Base::String& station_name)
{
    Tizen::Base::String *stationcode; 
    String sql;
    DbEnumerator* pEnum;
    result r = E_SUCCESS;

    sql.Append(L"select code from nstations where name='");
    sql.Append(station_name);
    sql.Append("' and region_id = (select id from regions where name = '");
    sql.Append(region_name);
    sql.Append("' and country_id = (select id from countries where name= '");
    sql.Append(country_name);
    sql.Append("')) order by name");

    pEnum = db.QueryN(sql);

    String strWord;
    if (pEnum->MoveNext() == E_SUCCESS){
        pEnum->GetStringAt(0, strWord);
        stationcode = new String(strWord);
    }

    return *stationcode;

    /*
    snprintf(sql, sizeof(sql) - 1,
            "select code from stations where name='%s' and region_id = \
            (select id from regions where name = '%s' and country_id = \
            (select id from countries where name= '%s')) order by name",
            station_name.c_str(), region_name.c_str(), country_name.c_str());
    std::cerr << sql << std::endl;
    rc = sqlite3_get_table(db,
                           sql,
                           &result,
                           &nrow,
                           &ncol,
                           &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        std::cerr << errMsg << std::endl;
#endif
        sqlite3_free(errMsg);
        return *stationname;
    }
    for (int i=0; i<ncol*nrow; i=i+1){

        stationname = new std::string(result[ncol+i]);
//        list->push_back(std::make_pair(result[ncol+i], result[ncol+i+1]));
        // std::cerr << result[ncol+i] << " - " << result[ncol+i+1] << std::endl;
    }
    sqlite3_free_table(result);
*/
}


listdata*
DatabaseSqlite::create_stations_list(int region_id)
{
    listdata *list = NULL;
    int rc;
    char *errMsg = NULL;
    char **result;
    int nrow, ncol;
    //std::string sql;
    char sql[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    list = new listdata;
/*    
    if(!db || region_id == 0 || region_id == -1)
        return list;    // database doesn't open 
*/

    snprintf(sql, sizeof(sql) - 1,
        "SELECT code, name FROM nstations WHERE \
        region_id = %d ORDER BY name", region_id);
    //std::cerr << sql << std::endl;
 /*
    rc = sqlite3_get_table(db,
                           sql,
                           &result,
                           &nrow,
                           &ncol,
                           &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        std::cerr << errMsg << std::endl;
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    for (int i=0; i<ncol*nrow; i=i+2){
        list->push_back(std::make_pair(result[ncol+i], result[ncol+i+1]));
        // std::cerr << result[ncol+i] << " - " << result[ncol+i+1] << std::endl;
    }
    sqlite3_free_table(result);
*/
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return list;
}

#define PI   (3.14159265358979323846)
#define EARTH_RADIUS (6378.137)
#define deg2rad(deg) ((deg) * (PI / 180.0))
/*******************************************************************************/
double
DatabaseSqlite::calculate_distance(double lat1, double lon1,
                                   double lat2, double lon2) {
    double dlat, dlon, slat, slon, a;

    /* Convert to radians. */
    lat1 = deg2rad(lat1);
    lon1 = deg2rad(lon1);
    lat2 = deg2rad(lat2);
    lon2 = deg2rad(lon2);

    dlat = lat2 - lat1;
    dlon = lon2 - lon1;

    slat = sin(dlat / 2.0F);
    slon = sin(dlon / 2.0F);
    a = (slat * slat) + (cos(lat1) * cos(lat2) * slon * slon);
    return ((2.0F * atan2(sqrt(a), sqrt(1.0F - a))) * EARTH_RADIUS);
}

void
DatabaseSqlite::get_nearest_station(double lat, double lon,
                                    std::string& country, std::string& region,
                                    std::string& code, std::string& name,
                                    double& latitude, double& longitude)
{
    int rc;
    char *errMsg = NULL;
    char **result;
    int nrow, ncol;
    double  distance,
            min_distance = 40000.0;

    double result_lat, result_lon;
    String sql;
    DbEnumerator* pEnum;
    latitude = INT_MAX;
    longitude = INT_MAX;
    AppLog ("get_nearest_station");
    sql.Append(L"select regions.name, stations.code, stations.name, stations.latitude, stations.longititude, countries.name \
             from regions left join stations on regions.id=stations.region_id \
             left join countries on regions.country_id=countries.id \
             where regions.latitudemax>");
     sql.Append(lat);        
     sql.Append(" and regions.latitudemin<");
     sql.Append(lat);        
     sql.Append(" and regions.longititudemax>");
     sql.Append(lon);        
     sql.Append(" and regions.longititudemin<");
     sql.Append(lon);        

     AppLog ("Sql %S",sql.GetPointer());
     pEnum = db.QueryN(sql);
     while (pEnum && pEnum->MoveNext() == E_SUCCESS){
        pEnum->GetDoubleAt(3, result_lat);
        pEnum->GetDoubleAt(4, result_lon);
        AppLog("sssss2 %f %f", result_lat, result_lon);
        distance = DatabaseSqlite::calculate_distance(lat, lon, result_lat, result_lon);
        AppLog ("Distance %f", distance);
        if (distance < min_distance){
            min_distance = distance;
            String Country, Region, Code, Name;
            pEnum->GetStringAt(5, Country);
            country = (char*)Tizen::Base::Utility::StringUtil::StringToUtf8N(Country)->GetPointer();
            pEnum->GetStringAt(0, Region);
            region = (char*)Tizen::Base::Utility::StringUtil::StringToUtf8N(Region)->GetPointer();
            pEnum->GetStringAt(1, Code);
            code = (char*)Tizen::Base::Utility::StringUtil::StringToUtf8N(Code)->GetPointer();
            pEnum->GetStringAt(2, Name);
            name = (char*)Tizen::Base::Utility::StringUtil::StringToUtf8N(Name)->GetPointer();
            latitude = result_lat;
            longitude = result_lon;
        }
     }


/*
    rc = sqlite3_get_table(db,
                           sql,
                           &result,
                           &nrow,
                           &ncol,
                           &errMsg);

    //rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        std::cerr << errMsg << std::endl;
#endif
        sqlite3_free(errMsg);
        return;
    }

    std::cerr << (ncol*nrow) << " " << ncol << " " << nrow  << std::endl;
    for (int i=0; i<ncol*nrow; i=i+6){
        distance = DatabaseSqlite::calculate_distance(lat, lon, atof(result[ncol+i+3]), atof(result[ncol+i+4]));
        if (distance < min_distance){
            min_distance = distance;
            country = result[ncol+i+5];
            region = result[ncol+i+0];
            code = result[ncol+i+1];
            name = result[ncol+i+2];
            latitude = atof(result[ncol+i+3]);
            longitude = atof(result[ncol+i+4]);
        }
    }
    sqlite3_free_table(result);
*/

}

void DatabaseSqlite::get_station_coordinate(Tizen::Base::String& code, double &latitude, double &longitude)
{
    String sql;
    DbEnumerator* pEnum;
    result r = E_SUCCESS;

    sql.Append(L"select latitude, longititude from nstations where code='");
    sql.Append(code);
    sql.Append("'");

    pEnum = db.QueryN(sql);
    latitude = -1;
    longitude = -1;
 
    String strWord;
    if (pEnum->MoveNext() == E_SUCCESS){
        pEnum->GetDoubleAt(0, latitude);
        pEnum->GetDoubleAt(1, longitude);
    }



/*
    if (!db)
        return; // database doesn't open 
    snprintf(sql,
             sizeof(sql) - 1,
             "select latitude, longititude from stations where code='%s'",
             code.c_str());
    rc = sqlite3_get_table(db,
                           sql,
                           &result,
                           &nrow,
                           &ncol,
                           &errMsg);

    //rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        std::cerr << errMsg << std::endl;
#endif
        sqlite3_free(errMsg);
        latitude = -1;
        longitude = -1;
        return;
    }
    if (nrow == 0){
        latitude = -1;
        longitude = -1;
        return;
    }

    latitude = atof(result[ncol+0]);
    longitude = atof(result[ncol+1]);

//    std::cerr<<"Latitude "<< latitude << " longitude"<<longitude<<std::endl; 
    sqlite3_free_table(result);
*/
}

}// namespace Core
