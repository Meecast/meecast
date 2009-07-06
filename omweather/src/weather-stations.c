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
    GtkListStore	*list;
    gint		count;
};
/*******************************************************************************/
GtkListStore *create_items_list(const char *path, const char *filename,
                                long start, long end, long *items_number) {
    FILE *fh;
    GtkListStore *list = NULL;
    GtkTreeIter iter;
    gchar buffer[512], buff[512];
    Region_item r_item;
    Country_item c_item;
    Station station;
    long max_bytes = 0, readed_bytes = 0, count = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* prepare full file name with path */
    buff[0] = 0;
    snprintf(buff, sizeof(buff) - 1, "%s%s", path, filename);

    max_bytes = end - start;
    fh = fopen(buff, "rt");
    if (!fh) {
        fprintf(stderr, "\nCan't read file %s: %s", buff, strerror(errno));
        items_number && (*items_number = count);
        list = NULL;
    } else {
        if (!strcmp(filename, LOCATIONSFILE))
            list = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING,
					G_TYPE_STRING,
                                      G_TYPE_DOUBLE, G_TYPE_DOUBLE);
        else
            list =
                gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT,
                                   G_TYPE_INT);
        if (start > -1)
            if (fseek(fh, start, SEEK_SET)) {
                fprintf(stderr,
                        "\nCan't seek to the position %ld on %s file: %s\n",
                        start, buff, strerror(errno));
                items_number && (*items_number = count);
                return NULL;
            }
        while (!feof(fh)) {
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer) - 1, fh);
            readed_bytes += strlen(buffer);

            if (!strcmp(filename, LOCATIONSFILE)) {
                if (!parse_station_string(buffer, &station)) {
                    gtk_list_store_append(list, &iter);
                    gtk_list_store_set(list, &iter,
                                       NAME_COLUMN, station.name,
                                       ID0_COLUMN, station.id0,
                                       ID1_COLUMN, station.id1,
                                       LATITUDE_COLUMN, station.latitude,
                                       LONGTITUDE_COLUMN, station.longtitude,
                                       -1);
                    count++;
                }
            } else {
                if (!strcmp(filename, REGIONSFILE)) {
                    if (!parse_region_string(buffer, &r_item)) {
                        gtk_list_store_append(list, &iter);
                        gtk_list_store_set(list, &iter,
                                           0, r_item.name,
                                           1, r_item.start, 2, r_item.end,
                                           -1);
                        count++;
                    }
                } else {
                    if (!parse_country_string(buffer, &c_item)) {
                        gtk_list_store_append(list, &iter);
                        gtk_list_store_set(list, &iter,
                                           0, c_item.name,
                                           1, c_item.start, 2, c_item.end,
                                           -1);
                        count++;
                    }
                }
            }

            if (start > -1 && end > -1 && readed_bytes >= max_bytes)
                break;
        }
        fclose(fh);
    }
    items_number && (*items_number = count);
    return list;
}

/*******************************************************************************/
int parse_country_string(const char *string, Country_item * result) {
    char *delimiter = NULL, *tmp, buffer[32];
    int res = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    tmp = (char *)string;
    delimiter = strchr(tmp, ';');
    if (!delimiter)
        res = 1;
    else {
        memset(result->name, 0, sizeof(result->name));
        memcpy(result->name, tmp,
               ((sizeof(result->name) - 1 >
                 (int)(delimiter - tmp)) ? ((int)(delimiter - tmp))
                : (sizeof(result->name) - 1)));
        tmp = delimiter + 1;
        delimiter = strchr(tmp, ';');
        if (!delimiter) {
            result->start = -1;
            res = 1;
        } else {
            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, tmp,
                   ((sizeof(buffer) - 1 >
                     (int)(delimiter - tmp)) ? ((int)(delimiter - tmp))
                    : (sizeof(buffer) - 1)));
            result->start = atol(buffer);
            tmp = delimiter + 1;
            delimiter = strchr(tmp, ';');
            if (!delimiter) {
                result->end = -1;
                res = 1;
            } else {
                memset(buffer, 0, sizeof(buffer));
                memcpy(buffer, tmp,
                       ((sizeof(buffer) - 1 >
                         (int)(delimiter - tmp)) ? ((int)(delimiter - tmp))
                        : (sizeof(buffer) - 1)));
                result->end = atol(buffer);
            }
        }
    }
    return res;
}

/*******************************************************************************/
int parse_region_string(const char *string, Region_item * result) {
    char *delimiter = NULL, *tmp;
    int res = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    tmp = (char *)string;
    delimiter = strchr(tmp, ';');
    setlocale(LC_NUMERIC, "POSIX");
    if (!delimiter)
        res = 1;
    else {
        memset(result->name, 0, sizeof(result->name));
        memcpy(result->name, tmp,
               ((sizeof(result->name) - 1 >
                 (int)(delimiter - tmp)) ? ((int)(delimiter - tmp))
                : (sizeof(result->name) - 1)));
        tmp = delimiter + 1;
        delimiter = strchr(tmp, ';');
        if (!delimiter) {
            result->start = -1;
            res = 1;
        } else {
            *delimiter = 0;
            result->start = atol(tmp);
            tmp = delimiter + 1;
            delimiter = strchr(tmp, ';');
            if (!delimiter) {
                result->end = -1;
                res = 1;
            } else {
                *delimiter = 0;
                result->end = atol(tmp);
                tmp = delimiter + 1;
                delimiter = strchr(tmp, ';');
                if (!delimiter) {
                    result->minlat = 0;
                    res = 1;
                } else {
                    *delimiter = 0;
                    result->minlat = atof(tmp);
                    tmp = delimiter + 1;
                    delimiter = strchr(tmp, ';');
                    if (!delimiter) {
                        result->minlon = 0;
                        res = 1;
                    } else {
                        *delimiter = 0;
                        result->minlon = atof(tmp);
                        tmp = delimiter + 1;
                        delimiter = strchr(tmp, ';');
                        if (!delimiter) {
                            result->maxlat = 0;
                            res = 1;
                        } else {
                            *delimiter = 0;
                            result->maxlat = atof(tmp);
                            tmp = delimiter + 1;
                            delimiter = strchr(tmp, ';');
                            if (!delimiter) {
                                result->maxlon = 0;
                                res = 1;
                            } else {
                                *delimiter = 0;
                                result->maxlon = atof(tmp);
                            }
                        }
                    }
                }
            }
        }
    }
    setlocale(LC_NUMERIC, "");
    return res;
}

/*******************************************************************************/
int parse_station_string(const char *string, Station * result) {
    char *delimiter = NULL, *tmp;
    int res = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    tmp = (char *)string;
    delimiter = strchr(tmp, ';');
    setlocale(LC_NUMERIC, "POSIX");
    if (!delimiter)
        res = 1;
    else {
        memset(result->name, 0, sizeof(result->name));
        memcpy(result->name, tmp,
               ((sizeof(result->name) - 1 >
                 (int)(delimiter - tmp)) ? ((int)(delimiter - tmp))
                : (sizeof(result->name) - 1)));
        tmp = delimiter + 1;
        delimiter = strchr(tmp, ';');
        if (!delimiter)
            res = 1;
        else {
            memset(result->id0, 0, sizeof(result->id0));
            memcpy(result->id0, tmp,
                   ((sizeof(result->id0) - 1 >
                     (int)(delimiter - tmp)) ? ((int)(delimiter - tmp))
                    : (sizeof(result->id0) - 1)));

            tmp = delimiter + 1;
            delimiter = strchr(tmp, ';');
            if (!delimiter)
                res = 1;
            else{
		memset(result->id1, 0, sizeof(result->id1));
		memcpy(result->id1, tmp,
			((sizeof(result->id1) - 1 >
			(int)(delimiter - tmp)) ? ((int)(delimiter - tmp))
						: (sizeof(result->id1) - 1)));
		tmp = delimiter + 1;
		delimiter = strchr(tmp, ';');
		if (!delimiter)
		    res = 1;
		else {
		    *delimiter = 0;
		    result->latitude = atof(tmp);
		    tmp = delimiter + 1;
		    delimiter = strchr(tmp, ';');
		    if (!delimiter)
			res = 1;
            	    else {
                	*delimiter = 0;
                	result->longtitude = atof(tmp);
            	    }
        	}
    	    }
        }
    }
    setlocale(LC_NUMERIC, "");
    return res;
}
/*******************************************************************************/
sqlite3* open_database(const char *path, const char *filename){
    sqlite3	*db = NULL;
    gchar	name[256];
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
void close_database(sqlite3 *database){
    if(database)
        sqlite3_close(database);
}
/*******************************************************************************/
GtkListStore* create_countries_list(sqlite3 *database){
    GtkListStore	*list = NULL;
    gint		rc;
    gchar		*errMsg = NULL;

    if(!database)
	return NULL;	/* database doesn't open */

    list = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
    rc = sqlite3_exec(database, "SELECT * FROM countries ORDER BY name",
			countries_callback, (void*)list, &errMsg);
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
GtkListStore* create_regions_list(sqlite3 *database, int country_id, int *region_count){
    GtkListStore	*list = NULL;
    gint		rc;
    gchar		*errMsg = NULL;
    gchar		sql[256];
    struct request_data	data = { 0, 0 };

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if(!database)
        return NULL;    /* database doesn't open */

    list = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_INT, G_TYPE_DOUBLE, G_TYPE_DOUBLE, 
                                 G_TYPE_DOUBLE, G_TYPE_DOUBLE);
    data.list = list;
    *sql = 0;
    if (country_id == 0) /* for GPS */
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
GtkListStore* create_stations_list(sqlite3 *database, int region_id){
    GtkListStore	*list = NULL;
    gint		rc;
    gchar		*errMsg = NULL;
    gchar		sql[256];

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if(!database || !region_id)
	return NULL;	/* database doesn't open */

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
GtkListStore* search_station_in_database(sqlite3 *database, char *code_name){
    GtkListStore	*list = NULL;
    gint		rc;
    gchar		*errMsg = NULL;
    gchar		sql[256];

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
int countries_callback(void *user_data, int argc, char **argv, char **azColName){
    int			i;
    GtkTreeIter		iter;
    GtkListStore	*list = GTK_LIST_STORE(user_data);

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
int regions_callback(void *user_data, int argc, char **argv, char **azColName){
    int			i;
    GtkTreeIter		iter;
    struct request_data	*data = (struct request_data*)user_data;
    GtkListStore	*list = GTK_LIST_STORE(data->list);

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
        if(!strcmp(azColName[i], "longititudemax"))
            gtk_list_store_set(list, &iter, 2, atof(argv[i]), -1);
        if(!strcmp(azColName[i], "latitudemax"))
            gtk_list_store_set(list, &iter, 3, atof(argv[i]), -1);
        if(!strcmp(azColName[i], "longititudemin"))
            gtk_list_store_set(list, &iter, 4, atof(argv[i]), -1);
        if(!strcmp(azColName[i], "latitudemin"))
            gtk_list_store_set(list, &iter, 5, atof(argv[i]), -1);
    }
	setlocale(LC_NUMERIC, "");
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return 0;
}
/*******************************************************************************/
int stations_callback(void *user_data, int argc, char **argv, char **azColName){
    int			i;
    GtkTreeIter		iter;
    GtkListStore	*list = GTK_LIST_STORE(user_data);

/* add new item for each first element */
    gtk_list_store_append(list, &iter);
	setlocale(LC_NUMERIC, "POSIX");
    for(i = 0; i < argc; i++){
	if(!strcmp(azColName[i], "name"))
	    gtk_list_store_set(list, &iter, 0, argv[i], -1);
	if(!strcmp(azColName[i], "code"))
	    gtk_list_store_set(list, &iter, 1, argv[i], -1);
	if(!strcmp(azColName[i], "longititude"))
	    gtk_list_store_set(list, &iter, 2, atof(argv[i]), -1);
	if(!strcmp(azColName[i], "latitude"))
	    gtk_list_store_set(list, &iter, 3, atof(argv[i]), -1);
    }
	setlocale(LC_NUMERIC, "");
    return 0;
}
/*******************************************************************************/
int search_callback(void *user_data, int argc, char **argv, char **azColName){
    int			i;
    GtkTreeIter		iter;
    GtkListStore	*list = GTK_LIST_STORE(user_data);

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
