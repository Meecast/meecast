/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2023 Vlad Vasilyeu
 * Copyright (C) 2006-2011 Pavel Fialko
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
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


#include "databasesqlite.h"

#include <QString>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QDebug>
namespace Core {

DatabaseSqlite::DatabaseSqlite(const std::string& filename)
{
    db = NULL;
    databasename = new std::string;
    this->set_databasename(filename);
}

DatabaseSqlite::~DatabaseSqlite()
{
    delete databasename;
    if (!db) sqlite3_close(db);
}

QString appdir(){
    QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    if (dirs.length() >= 2)
        return dirs[1];
    if (dirs.length() == 1)
        return dirs[0];
    return "";
}

void
DatabaseSqlite::set_databasename(const std::string& filename)
{
#ifdef ANDROID
    QString dbSourcePath(filename.c_str());
    QFileInfo dbSourceInfo(dbSourcePath);
    QString _filename = dbSourcePath.split("/").last();
    QString appdir = "";
    QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    if (dirs.length() >= 2)
        appdir =dirs[1];
    if (dirs.length() == 1)
        appdir = dirs[0];
    // determine destination path
    QDir writableLocation(appdir + "/databases/");
    if (!writableLocation.exists()) {
        qDebug()<<"Directory is not exists";
        writableLocation.mkpath(".");
    }
    QString dbDestPath = writableLocation.filePath(_filename);
    QFileInfo dbDestInfo(dbDestPath);

    // determine if the source db has changed
    bool dbSourceUpdated = false;
    if (dbSourceInfo.lastModified() > dbDestInfo.lastModified()){
        dbSourceUpdated = dbSourceInfo.lastModified() > dbDestInfo.lastModified();
    }
    /*
    if (dbDestInfo.exists()){
        qDebug()<<"dbDestInfo"<<dbDestInfo.lastModified();
        qDebug()<<"dbDestInfo"<<dbDestInfo.size();
    }
    if (dbSourceInfo.exists()){
        qDebug()<<"dbSourceInfo"<<dbSourceInfo.lastModified();
        qDebug()<<"dbSourceInfo"<<dbSourceInfo.size();
    }else{
        qDebug()<<"dbSourceInfo.exists() not exists";
    }
    */

    // copy or replace db if needed
    if ((!dbDestInfo.exists()) || dbSourceUpdated) {
        QFile::remove(dbDestPath);
        if (!QFile::copy(dbSourcePath, dbDestPath)) {
            qCritical() << "ERROR: source db " << dbSourcePath << " not copied to "<< dbDestPath;
        } else {
            qDebug() << "db successfully copied or replaced to " << dbDestPath;
        }

    } else {
        qDebug() << "dest db " << dbDestPath << " already exists";
    }
    databasename->assign(dbDestPath.toStdString());
#else 
    databasename->assign(filename);
#endif
}

std::string&
DatabaseSqlite::get_databasename()
{
    return *databasename;
}

bool
DatabaseSqlite::open_database()
{
    std::string lang = std::string();
    int rc;
    char * msg;
    std::string key ("gismeteo.ru.db");
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
    return true;
}

listdata*
DatabaseSqlite::create_countries_list()
{
    listdata *list = NULL;
    int rc;
    char *errMsg = NULL;
    char **result;
    int nrow, ncol;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!db)
        return NULL;    /* database doesn't open */
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
    return list;
}

listdata*
DatabaseSqlite::create_region_list_by_name(const std::string& country_name)
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
    if(!db){
        return NULL;
    }

    if (country_name =="") return list;

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

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return list;
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
    if(!db){
        return NULL;
    }

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
    return list;
}

listdata*
DatabaseSqlite::create_stations_list_by_name(const std::string& country_name, const std::string& region_name)
{
    listdata *list = NULL;
    int rc;
    char *errMsg = NULL;
    char **result;
    int nrow, ncol;
    //std::string sql;
    char sql[1024];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    list = new listdata;

    if(!db || country_name =="" || region_name == "")
        return list;    /* database doesn't open */


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
        /* std::cerr << result[ncol+i] << " - " << result[ncol+i+1] << std::endl;*/
    }
    sqlite3_free_table(result);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return list;
}

std::string&
DatabaseSqlite::get_station_code_by_name(const std::string& country_name, 
                                         const std::string& region_name,
                                         const std::string& station_name)
{
    listdata *list = NULL;
    int rc;
    char *errMsg = NULL;
    char **result;
    int nrow, ncol;
    //std::string sql;
    char sql[1024];
    std::string *stationname;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    list = new listdata;

    if(!db || country_name =="" || region_name == "" || station_name == "")
        return *stationname; /* database doesn't open */

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
        /* std::cerr << result[ncol+i] << " - " << result[ncol+i+1] << std::endl;*/
    }
    sqlite3_free_table(result);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return *stationname;
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
    if(!db || region_id == 0 || region_id == -1)
        return list;    /* database doesn't open */


    snprintf(sql, sizeof(sql) - 1,
        "SELECT code, name FROM nstations WHERE \
        region_id = %d ORDER BY name", region_id);
    //std::cerr << sql << std::endl;
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
        /* std::cerr << result[ncol+i] << " - " << result[ncol+i+1] << std::endl;*/
    }
    sqlite3_free_table(result);

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

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
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
    char sql[512];
    int rc;
    char *errMsg = NULL;
    char **result;
    int nrow, ncol;
    double  distance,
            min_distance = 40000.0;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!db)
        return; /* database doesn't open */
    setlocale(LC_NUMERIC, "POSIX");
    snprintf(sql,
             sizeof(sql) - 1,
             "select regions.name, stations.code, stations.name, stations.latitude, stations.longititude, countries.name \
             from regions left join stations on regions.id=stations.region_id \
             left join countries on regions.country_id=countries.id \
             where regions.latitudemax>%f and regions.latitudemin<%f and regions.longititudemax>%f and regions.longititudemin<%f",
             lat, lat, lon, lon);
    std::cerr << "sql = " << sql << std::endl;
    setlocale(LC_NUMERIC, "");
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

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}

void DatabaseSqlite::get_station_coordinate(std::string code, double &latitude, double &longitude)
{
    char sql[256];
    int rc;
    char *errMsg = NULL;
    char **result;
    int nrow, ncol;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!db)
        return; /* database doesn't open */

    snprintf(sql,
             sizeof(sql) - 1,
             "select latitude, longititude from stations where code='%s'",
             code.c_str());
    /* std::cerr << "sql = " << sql << std::endl; */

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



    setlocale(LC_NUMERIC, "POSIX");
    latitude = atof(result[ncol+0]);
    longitude = atof(result[ncol+1]);
    setlocale(LC_NUMERIC, "");

    /* std::cerr<<"Latitude "<< latitude << " longitude "<<longitude<<std::endl;  */
    sqlite3_free_table(result);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}

}// namespace Core
