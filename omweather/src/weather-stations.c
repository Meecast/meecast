/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
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
#include "weather-stations.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
struct request_data{
    GtkListStore        *list;
    gint                count;
};
/*******************************************************************************/
sqlite3*
open_database(const char *path, const char *filename){
    sqlite3     *db = NULL;
    gchar       name[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!path || !filename)
        return NULL;
    *name = 0;
    snprintf(name, sizeof(name) - 1, "%s%s", path, filename);
    if(sqlite3_open(name, &db)){
        fprintf(stderr,"Error in connection to database %s\n",sqlite3_errmsg(db));
        return NULL;   /* error */
    }
    return db;
}
/*******************************************************************************/
void
close_database(sqlite3 *database){
    if(database)
        sqlite3_close(database);
}
/*******************************************************************************/
GtkListStore*
create_countries_list(sqlite3 *database){
    GtkListStore        *list = NULL;
    gint                rc;
    gchar               *errMsg = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!database)
        return NULL;    /* database doesn't open */
    list = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
    rc = sqlite3_exec(database, "SELECT name, id FROM countries ORDER BY name",
                        countries_callback, (void*)list, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        fprintf(stderr, "\n>>>>%s\n", errMsg);
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return list;
}
/*******************************************************************************/
GtkListStore*
create_regions_list(sqlite3 *database, int country_id, int *region_count){
    GtkListStore        *list = NULL;
    gint                rc;
    gchar               *errMsg = NULL;
    gchar               sql[256];
    struct request_data data = { 0, 0 };
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (country_id == -1){
        region_count = 0;
        return NULL;
    }

    if(!database)
        return NULL;    /* database doesn't open */
    list = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_INT, G_TYPE_DOUBLE, G_TYPE_DOUBLE, 
                                 G_TYPE_DOUBLE, G_TYPE_DOUBLE);
    data.list = list;
    *sql = 0;
    if(country_id == 0) /* for GPS */
        snprintf(sql, sizeof(sql) - 1,
                 "SELECT id, name, longititudemax, latitudemax, longititudemin, latitudemin \
                  FROM regions");
    else
        snprintf(sql, sizeof(sql) - 1,
                 "SELECT id, name, longititudemax, latitudemax, longititudemin, latitudemin \
                  FROM regions WHERE country_id = %d ORDER BY name",
                  country_id);
    rc = sqlite3_exec(database, sql, regions_callback, (void*)&data, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        fprintf(stderr, "\n>>>>%s\n", errMsg);
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    *region_count = data.count;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return list;
}
/*******************************************************************************/
GtkListStore*
create_stations_list(sqlite3 *database, int region_id){
    GtkListStore        *list = NULL;
    gint                rc;
    gchar               *errMsg = NULL,
                        sql[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!database || region_id == 0 || region_id == -1)
        return NULL;/* database doesn't open */
    list = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_DOUBLE,
                                G_TYPE_DOUBLE);
    *sql = 0;
    snprintf(sql, sizeof(sql) - 1,
        "SELECT name, code, longititude, latitude FROM stations WHERE \
        region_id = %d ORDER BY name", region_id);
    rc = sqlite3_exec(database, sql, stations_callback, (void*)list, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        fprintf(stderr, "\n>>>>%s\n", errMsg);
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return list;
}
/*******************************************************************************/
gint
get_all_information_callback(void *user_data, int argc, char **argv, char **azColName){
    int                 i;
    GtkTreeIter         iter;
    GtkListStore        *list = GTK_LIST_STORE(user_data);
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* add new item for each first element */
    gtk_list_store_append(list, &iter);
    for(i = 0; i < argc; i++){
        if(!strcmp(azColName[i], "cname"))
            gtk_list_store_set(list, &iter, 0, argv[i], -1);
        if(!strcmp(azColName[i], "name"))
            gtk_list_store_set(list, &iter, 1, argv[i], -1);
        if(!strcmp(azColName[i], "cid"))
            gtk_list_store_set(list, &iter, 2, atoi(argv[i]), -1);
        if(!strcmp(azColName[i], "region_id"))
            gtk_list_store_set(list, &iter, 3, atoi(argv[i]), -1);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return 0;
}
/*******************************************************************************/
GtkListStore*
get_all_information_about_station(gchar *source, gchar *station_code){
    sqlite3    *database = NULL;
    gchar buffer[2048];
    gchar       *errMsg = NULL;
    gchar       sql[1024];
    gint        rc;
    GtkListStore    *list = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    snprintf(buffer, sizeof(buffer) - 1, "%s.db",source);
    database = open_database(DATABASEPATH, buffer);
    if (!database)
        return NULL;
    list = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
    snprintf(sql, sizeof(sql) - 1, "Select countries.name as cname, regions.name, countries.id as cid, \
                                    stations.region_id from stations,regions,countries \
                                    where stations.code='%s' and stations.region_id=regions.id and \
                                    regions.country_id=countries.id and countries.id=regions.country_id",
                                    station_code);
    rc = sqlite3_exec(database, sql, get_all_information_callback, (void*)list, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
      fprintf(stderr, "\n>>>>%s\n", errMsg);
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    close_database(database);
    return list;
}
/*******************************************************************************/
gint
get_country_code(gchar *source, gchar *country_name){
    sqlite3    *database = NULL;
    gchar buffer[2048];
    gchar       *errMsg = NULL;
    gchar       sql[1024];
    gint        rc;
    GtkListStore    *list = NULL;
    GtkTreeIter     iter;
    gboolean valid;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    snprintf(buffer, sizeof(buffer) - 1, "%s.db",source);
    database = open_database(DATABASEPATH, buffer);
    if (!database)
        return -1;
    list = gtk_list_store_new(1, G_TYPE_INT);
    snprintf(sql, sizeof(sql) - 1, "Select id from countries \
                                    where name='%s'", country_name);
    rc = sqlite3_exec(database, sql, get_country_code_callback, (void*)list, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
      fprintf(stderr, "\n>>>>%s\n", errMsg);
#endif
        sqlite3_free(errMsg);
        return -1;
    }
    close_database(database);
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                      (list), &iter);
    if(valid)
      gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, 0, &rc, -1);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return rc;
}
/*******************************************************************************/
gint
get_state_code(gchar *source, gchar *region_name){
    sqlite3    *database = NULL;
    gchar buffer[2048];
    gchar       *errMsg = NULL;
    gchar       sql[1024];
    gint        rc;
    GtkListStore    *list = NULL;
    GtkTreeIter     iter;
    gboolean valid;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    snprintf(buffer, sizeof(buffer) - 1, "%s.db",source);
    database = open_database(DATABASEPATH, buffer);
    if (!database)
        return -1;
    list = gtk_list_store_new(1, G_TYPE_INT);
    snprintf(sql, sizeof(sql) - 1, "Select id from regions \
                                    where name='%s'", region_name);
    rc = sqlite3_exec(database, sql, get_state_code_callback, (void*)list, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
      fprintf(stderr, "\n>>>>%s\n", errMsg);
#endif
        sqlite3_free(errMsg);
        return -1;
    }
    close_database(database);
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                      (list), &iter);
    if(valid)
        gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, 0, &rc, -1);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return rc;
}
/*******************************************************************************/
gchar*
get_station_code(gchar *source, gint region_id, gchar *station_name){
    sqlite3             *database = NULL;
    gchar               buffer[2048],
                        *errMsg = NULL,
                        sql[1024];
    gint                rc;
    GtkListStore        *list = NULL;
    GtkTreeIter         iter;
    gboolean            valid;
    gchar               *code;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    snprintf(buffer, sizeof(buffer) - 1, "%s.db",source);
    database = open_database(DATABASEPATH, buffer);
    if(!database)
        return NULL;
    list = gtk_list_store_new(1, G_TYPE_STRING);
    /* Correct SQL */
    snprintf(sql, sizeof(sql) - 1, "Select code from stations \
                                    where name='%s' and region_id='%i'", station_name, region_id);
    rc = sqlite3_exec(database, sql, get_station_code_callback, (void*)list, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
      fprintf(stderr, "\n>>>>%s\n", errMsg);
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    close_database(database);
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list), &iter);
    if(valid)
        gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, 0, &code, -1);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return code;
}
/*******************************************************************************/
GtkListStore* 
search_station_in_database(sqlite3 *database, char *code_name){
    GtkListStore	*list = NULL;
    gint		rc;
    gchar		*errMsg = NULL;
    gchar		sql[1024];

    if(!database || !code_name)
        return NULL;
    *sql = 0;

    list = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
              G_TYPE_STRING);
    snprintf(sql, sizeof(sql) - 1,
        "SELECT stations.name, sources.code, regions.name AS region_name, \
        countries.name AS country_name FROM stations JOIN sources,regions, \
        countries ON stations.id = sources.station_id AND stations.region_id \
        = regions.id AND regions.country_id = countries.id WHERE stations.name \
        LIKE('%s%%') OR sources.code LIKE('%s%%')",
        code_name, code_name);
    rc = sqlite3_exec(database, sql, search_callback, (void*)list, &errMsg);
    if(rc != SQLITE_OK){
#ifndef RELEASE
        fprintf(stderr, "\n>>>>%s\n", errMsg);
#endif
        sqlite3_free(errMsg);
        return NULL;
    }
    return list;
}
/*******************************************************************************/
int
countries_callback(void *user_data, int argc, char **argv, char **azColName){
    int                 i;
    GtkTreeIter         iter;
    GtkListStore        *list = GTK_LIST_STORE(user_data);
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* add new item for each first element */
    gtk_list_store_append(list, &iter);
    for(i = 0; i < argc; i++){
        if(!strcmp(azColName[i], "id"))
          gtk_list_store_set(list, &iter, 1, atoi(argv[i]), -1);
        if(!strcmp(azColName[i], "name"))
          gtk_list_store_set(list, &iter, 0, argv[i], -1);
    }
    return 0;
}
/*******************************************************************************/
int
get_country_code_callback(void *user_data, int argc, char **argv, char **azColName){
    int                 i;
    GtkTreeIter         iter;
    GtkListStore        *list = GTK_LIST_STORE(user_data);
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* add new item for each first element */
    gtk_list_store_append(list, &iter);
    for(i = 0; i < argc; i++){
        if(!strcmp(azColName[i], "id"))
            gtk_list_store_set(list, &iter, 0, atoi(argv[i]), -1);
        fprintf(stderr,"azColName[i] %s %i",azColName[i],atoi(argv[i]));
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return 0;
}
/*******************************************************************************/
int
get_station_code_callback(void *user_data, int argc, char **argv, char **azColName){
    int                 i;
    GtkTreeIter         iter;
    GtkListStore        *list = GTK_LIST_STORE(user_data);
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* add new item for each first element */
    gtk_list_store_append(list, &iter);
    for(i = 0; i < argc; i++){
        if(!strcmp(azColName[i], "code"))
            gtk_list_store_set(list, &iter, 0, argv[i], -1);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return 0;
}
/*******************************************************************************/
int
get_state_code_callback(void *user_data, int argc, char **argv, char **azColName){
    int                 i;
    GtkTreeIter         iter;
    GtkListStore        *list = GTK_LIST_STORE(user_data);
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* add new item for each first element */
    gtk_list_store_append(list, &iter);
    for(i = 0; i < argc; i++){
        if(!strcmp(azColName[i], "id"))
            gtk_list_store_set(list, &iter, 0, atoi(argv[i]), -1);
        fprintf(stderr,"azColName[i] %s %i",azColName[i],atoi(argv[i]));
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return 0;
}
/*******************************************************************************/
int
regions_callback(void *user_data, int argc, char **argv, char **azColName){
    int                 i;
    GtkTreeIter         iter;
    struct request_data *data = (struct request_data*)user_data;
    GtkListStore        *list = GTK_LIST_STORE(data->list);
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    setlocale(LC_NUMERIC, "POSIX");
    data->count += (int)argc / 6;
/* add new item for each first element */
    gtk_list_store_append(list, &iter);
    for(i = 0; i < argc; i++){
        if(!strcmp(azColName[i], "id"))
            gtk_list_store_set(list, &iter, 1, atoi(argv[i]), -1);
        if(!strcmp(azColName[i], "name"))
            gtk_list_store_set(list, &iter, 0, argv[i], -1);
        if(!strcmp(azColName[i], "longititudemax")){
            if (argv[i])
                gtk_list_store_set(list, &iter, 2, atof(argv[i]), -1);
            else
                gtk_list_store_set(list, &iter, 2, 0, -1);
        }
        if(!strcmp(azColName[i], "latitudemax")){
            if (argv[i])
                gtk_list_store_set(list, &iter, 3, atof(argv[i]), -1);
            else
                gtk_list_store_set(list, &iter, 3, 0, -1);
        }
        if(!strcmp(azColName[i], "longititudemin")){
            if (argv[i])
                gtk_list_store_set(list, &iter, 4, atof(argv[i]), -1);
            else
                gtk_list_store_set(list, &iter, 4, 0, -1);
        }
        if(!strcmp(azColName[i], "latitudemin")){
            if (argv[i])
                gtk_list_store_set(list, &iter, 5, atof(argv[i]), -1);
            else
                gtk_list_store_set(list, &iter, 5, 0, -1);
        }
    }
    setlocale(LC_NUMERIC, "");
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return 0;
}
/*******************************************************************************/
int
stations_callback(void *user_data, int argc, char **argv, char **azColName){
    int                 i;
    GtkTreeIter         iter;
    GtkListStore        *list = GTK_LIST_STORE(user_data);
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* add new item for each first element */
    gtk_list_store_append(list, &iter);
    setlocale(LC_NUMERIC, "POSIX");
    for(i = 0; i < argc; i++){
        if(!strcmp(azColName[i], "name"))
            gtk_list_store_set(list, &iter, 0, argv[i], -1);
        if(!strcmp(azColName[i], "code"))
            gtk_list_store_set(list, &iter, 1, argv[i], -1);
        if(!strcmp(azColName[i], "longititude")){
            if (argv[i])
                gtk_list_store_set(list, &iter, 2, atof(argv[i]), -1);
            else 
                gtk_list_store_set(list, &iter, 2, 0, -1);
        }
        if(!strcmp(azColName[i], "latitude")){
            if (argv[i])
                gtk_list_store_set(list, &iter, 3, atof(argv[i]), -1);
            else
                gtk_list_store_set(list, &iter, 3, 0, -1);
        }
    }
    setlocale(LC_NUMERIC, "");
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return 0;
}
/*******************************************************************************/
int
search_callback(void *user_data, int argc, char **argv, char **azColName){
    int                 i;
    GtkTreeIter         iter;
    GtkListStore        *list = GTK_LIST_STORE(user_data);
/* add new item for each first element */
    gtk_list_store_append(list, &iter);
    for(i = 0; i < argc; i++){
        if(!strcmp(azColName[i], "code"))
            gtk_list_store_set(list, &iter, 0, argv[i], -1);
        if(!strcmp(azColName[i], "name"))
            gtk_list_store_set(list, &iter, 1, argv[i], -1);
        if(!strcmp(azColName[i], "region_name"))
            gtk_list_store_set(list, &iter, 2, argv[i], -1);
        if(!strcmp(azColName[i], "country_name"))
            gtk_list_store_set(list, &iter, 3, argv[i], -1);
    }
    return 0;
}
/*******************************************************************************/
