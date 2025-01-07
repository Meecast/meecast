/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-yr-no-stations-db - MeeCast
 *
 * Copyright (C) 2006-2023 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "meego-main.h"
#include "json/json.h"
#include <unistd.h>
#include <fstream>
#include <iostream> 
#include <string.h>
#include <time.h>
#include <locale.h>
#include <math.h> 
#define UNUSED(x) (void)(x)

static xmlHashTablePtr hash_for_icons;
static xmlHashTablePtr hash_for_translate;
#ifdef GLIB
static GHashTable *data = NULL;
#endif 
/*******************************************************************************/
#ifdef QT
static QHash<QString, QString> *hash_for_icons;
static QHash<QString, QString> *hash_for_translate;
QHash<QString, QString> *hash_icons_yrno_table_create(void);
#endif
/*******************************************************************************/
#ifdef GLIB
int
source_init(void){
    data = g_hash_table_new(g_str_hash, g_str_equal);
    if(!data)
        return FALSE;
    return TRUE;
}
#endif
/*******************************************************************************/
#ifdef GLIB
void
source_destroy(void){
    GHashTable  *hashtable1 = NULL;
    void *hashtable = NULL;
    void *tmp = NULL;
    /* free station location data */
    hashtable = g_hash_table_lookup(data, "location");
    if(hashtable){
        g_hash_table_foreach((GHashTable *)hashtable, free_fields, NULL);
        g_hash_table_remove_all((GHashTable *)hashtable);
        g_hash_table_unref((GHashTable *)hashtable);
    }
    /* free station current data */
    hashtable = g_hash_table_lookup(data, "current");
    if(hashtable){
        g_hash_table_foreach((GHashTable *)hashtable, free_fields, NULL);
        g_hash_table_remove_all((GHashTable *)hashtable);
        g_hash_table_unref((GHashTable *)hashtable);
    }
    /* free station days data */
    tmp = g_hash_table_lookup(data, "forecast");
    while((GSList *)tmp){
        hashtable1 = (GHashTable *)(((GSList *)tmp)->data);
        g_hash_table_foreach((GHashTable *)hashtable1, free_fields, NULL);
        g_hash_table_remove_all((GHashTable *)hashtable1);
        g_hash_table_unref((GHashTable *)hashtable1);
        tmp = g_slist_next(tmp);
    }
    tmp = g_hash_table_lookup(data, "forecast");
    if(tmp)
        g_slist_free((GSList *)tmp);
    if(data){
        g_hash_table_remove_all(data);
        g_hash_table_destroy(data);
    }
}
/*******************************************************************************/
void
free_fields(gpointer key, gpointer val, gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(val){
        g_free(val);
        val = NULL;
    }
}
#endif
/*******************************************************************************/

int
parse_and_write_days_json_yrno_data(const char *days_data_path, const char *result_file){

    FILE   *file_out;
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    Json::Value val;
    Json::Value data;
    Json::Value details;
    Json::Value next_1_hours;
    Json::Value next_6_hours;
    Json::Value next_12_hours;
    Json::Value nullval;
    float precipitation = INT_MAX; 
    int pressure = INT_MAX;
    int temperature = INT_MAX;
    int dew_point = INT_MAX;
    int humidity = INT_MAX;
    int uv_index = INT_MAX;
    int wind_speed = INT_MAX;
    float _wind_direction = INT_MAX;
    int wind_index = INT_MAX;
    std::string symbol_icon_code = "";

    std::string wind_directions[17] = {"N","NNE","NE","ENE","E","ESE","SE","SSE","S","SSW","SW","WSW","W","WNW","NW","NNW","N"};
    char buffer  [4096],
         buffer2 [4096],
         *delimiter = NULL;

    int first_day = true;
    time_t current_time = 0;
    time_t begin_utc_time;
    int localtimezone = 0;
    struct tm time_tm1 = {0,0,0,0,0,0,0,0,0,0,0};
    struct tm time_tm2 = {0,0,0,0,0,0,0,0,0,0,0};
    struct tm tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};

    std::ifstream jsonfile(days_data_path, std::ifstream::binary);
    bool parsingSuccessful = reader.parse(jsonfile, root, false);
    if (!parsingSuccessful){

        fprintf(stderr,"Problem ");
        return -1;
    }

    hash_for_icons = hash_icons_yrno_table_create();
    hash_for_translate = hash_description_yrno_table_create();


    file_out = fopen(result_file, "w");
    if (!file_out)
        return -1;
    /* prepare station id */
    *buffer = 0;
    *buffer2 = 0;
    snprintf(buffer2, sizeof(buffer2) - 1, "%s", days_data_path);
    delimiter = strrchr(buffer2, '/');
    if(delimiter){
        delimiter++; /* delete '/' */
        snprintf(buffer, sizeof(buffer) - 1, "%s", delimiter);
    }
    /* Set localtimezone */
    current_time = time(NULL);
    gmtime_r(&current_time, &time_tm1);
    localtime_r(&current_time, &time_tm2);
    tmp_tm.tm_isdst = time_tm2.tm_isdst;
    localtimezone = time_tm2.tm_gmtoff/3600; 
    setlocale(LC_NUMERIC, "POSIX");
    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", buffer);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");
    fprintf(file_out,"  <timezone>%i</timezone>\n", localtimezone);

    if (root.type() != Json::objectValue){
        return -5;
    }

    val = root["properties"].get("timeseries", nullval);

    //fprintf(stderr,"size %i\n", val.size());
    for (uint i = 0; i < val.size(); i++){

        data = val[i].get("data", nullval);
        if (data == nullval){
            continue;
        }
        if (data.get("instant", nullval) !=nullval){
            details = data.get("instant", nullval).get("details", nullval);
            if (details != nullval){
                if (details.get("air_pressure_at_sea_level", nullval) != nullval){
                    pressure = details.get("air_pressure_at_sea_level", INT_MAX).asInt();
                }
                if (details.get("air_temperature", nullval) != nullval){
                    temperature = details.get("air_temperature", INT_MAX).asFloat();
                }
                if (details.get("dew_point_temperature", nullval) != nullval){
                    dew_point = details.get("dew_point_temperature", INT_MAX).asFloat();
                }
                if (details.get("relative_humidity", nullval) != nullval){
                    humidity = details.get("relative_humidity", INT_MAX).asFloat();
                }
                if (details.get("ultraviolet_index_clear_sky", nullval) != nullval){
                    uv_index = details.get("ultraviolet_index_clear_sky", INT_MAX).asInt();
                }
                if (details.get("wind_from_direction", nullval) != nullval){
                    _wind_direction = details.get("wind_from_direction", INT_MAX).asFloat();
                    wind_index = (int)round(_wind_direction/22.5) + 1;
                }
                if (details.get("wind_speed", nullval) != nullval){
                    wind_speed = (int)round(details.get("wind_speed", INT_MAX).asFloat());
                }
            }
        }
        next_12_hours = data.get("next_12_hours", nullval);
        next_6_hours = data.get("next_6_hours", nullval);
        next_1_hours = data.get("next_1_hours", nullval);

        if (val[i].get("time","").asString() != ""){
            //fprintf(stderr,"Time %s\n",val[i].get("time","").asString().c_str()); 
            tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
            setlocale(LC_TIME, "POSIX");
            strptime((const char*)val[i].get("time","").asString().c_str(), "%Y-%m-%dT%H:%M:00Z", &tmp_tm);
            begin_utc_time = mktime(&tmp_tm) + localtimezone*3600; 

        }else{
            continue;
        }
        if (next_1_hours != nullval){
            fprintf(file_out,"    <period start=\"%li\" hour=\"true\" end=\"%li\">\n", begin_utc_time, begin_utc_time + 1*3600);
            if (pressure != INT_MAX){
                fprintf(file_out,"     <pressure>%i</pressure>\n", pressure);
            }
            if (temperature != INT_MAX){
                fprintf(file_out,"     <temperature>%.0f</temperature>\n", round(temperature));
            }
            if (dew_point != INT_MAX){
                fprintf(file_out,"     <dewpoint>%.0f</dewpoint>\n", round(dew_point));
            }
            if (humidity != INT_MAX){
                fprintf(file_out,"     <humidity>%.0f</humidity>\n", round(humidity));
            }
            if (uv_index != INT_MAX){
                fprintf(file_out,"     <uv_index>%i</uv_index>\n", uv_index);
            }
            if (_wind_direction != INT_MAX){
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", wind_directions[wind_index].c_str());
            }
            if (wind_speed != INT_MAX){
                fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", wind_speed);
            }
            if (next_1_hours.get("summary", nullval) != nullval){
                Json::Value summary = next_1_hours.get("summary", nullval);
                if (summary.get("symbol_code", nullval) != nullval){
                   symbol_icon_code = summary.get("symbol_code", "").asString();
                    if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)symbol_icon_code.c_str())){
                        fprintf(file_out,"     <icon>%s</icon>\n",  
                            (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)symbol_icon_code.c_str()));
                    }else{
                        fprintf(stderr,"\n%s\n", symbol_icon_code.c_str());
                        fprintf(file_out,"     <icon>49</icon>\n");  
                    }
                    if ((char*)xmlHashLookup(hash_for_translate, (const xmlChar*)symbol_icon_code.c_str())){
                        fprintf(file_out,"     <description>%s</description>\n",
                                (char*)xmlHashLookup(hash_for_translate, (const xmlChar*)symbol_icon_code.c_str()));
                    }

                }
            }
            if (next_1_hours.get("details", nullval) != nullval){
                Json::Value details = next_1_hours.get("details", nullval);
                if (details.get("precipitation_amount", nullval) != nullval){
                   precipitation = details.get("precipitation_amount", nullval).asFloat();
                   fprintf(file_out,"     <precipitation>%.1f</precipitation>\n", precipitation);
                }
            }


            fprintf(file_out,"    </period>\n");
        }

        if (first_day){
            first_day = false;
            fprintf(file_out,"    <period start=\"%li\" current=\"true\" end=\"%li\">\n", begin_utc_time, begin_utc_time + 3*3600);
            if (pressure != INT_MAX){
                fprintf(file_out,"     <pressure>%i</pressure>\n", pressure);
            }
            if (temperature != INT_MAX){
                fprintf(file_out,"     <temperature>%.0f</temperature>\n", round(temperature));
            }
            if (dew_point != INT_MAX){
                fprintf(file_out,"     <dewpoint>%.0f</dewpoint>\n", round(dew_point));
            }
            if (humidity != INT_MAX){
                fprintf(file_out,"     <humidity>%.0f</humidity>\n", round(humidity));
            }
            if (uv_index != INT_MAX){
                fprintf(file_out,"     <uv_index>%i</uv_index>\n", uv_index);
            }
            if (_wind_direction != INT_MAX){
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", wind_directions[wind_index].c_str());
            }
            if (wind_speed != INT_MAX){
                fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", wind_speed);
            }
            if (symbol_icon_code != ""){
                if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)symbol_icon_code.c_str())){
                    fprintf(file_out,"     <icon>%s</icon>\n",  
                            (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)symbol_icon_code.c_str()));

                }else{
                    fprintf(stderr,"\n%s\n", symbol_icon_code.c_str());
                    fprintf(file_out,"     <icon>49</icon>\n");  
                }
                if ((char*)xmlHashLookup(hash_for_translate, (const xmlChar*)symbol_icon_code.c_str())){
                    fprintf(file_out,"     <description>%s</description>\n",
                            (char*)xmlHashLookup(hash_for_translate, (const xmlChar*)symbol_icon_code.c_str()));
                }

            }
            if (precipitation != INT_MAX){
               fprintf(file_out,"     <precipitation>%.1f</precipitation>\n", precipitation);
            }


            fprintf(file_out,"    </period>\n");
        }
        /*
        if (next_12_hours != nullval){
            fprintf(file_out,"    <period start=\"%li\" end=\"%li\">\n", begin_utc_time, begin_utc_time + 12*3600);
            if (pressure != INT_MAX){
                fprintf(file_out,"     <pressure>%i</pressure>\n", pressure);
            }
            if (temperature != INT_MAX){
                fprintf(file_out,"     <temperature>%i</temperature>\n", temperature);
            }
            if (dew_point != INT_MAX){
                fprintf(file_out,"     <dewpoint>%i</dewpoint>\n", dew_point);
            }
            if (humidity != INT_MAX){
                fprintf(file_out,"     <humidity>%i</humidity>\n", humidity);
            }
            if (uv_index != INT_MAX){
                fprintf(file_out,"     <uv_index>%i</uv_index>\n", uv_index);
            }
            if (_wind_direction != INT_MAX){
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", wind_directions[wind_index].c_str());
            }
            if (wind_speed != INT_MAX){
                fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", wind_speed);
            }
            if (next_12_hours.get("summary", nullval) != nullval){
                Json::Value summary = next_12_hours.get("summary", nullval);
                if (summary.get("symbol_code", nullval) != nullval){
                   std::string symbol_code = summary.get("symbol_code", "").asString();
                    if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)symbol_code.c_str())){
                        fprintf(file_out,"     <icon>%s</icon>\n",  
                            (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)symbol_code.c_str()));
                    }else{
                        fprintf(stderr,"\n%s\n", symbol_icon_code.c_str());
                        fprintf(file_out,"     <icon>49</icon>\n");  
                    }
                    if ((char*)xmlHashLookup(hash_for_translate, (const xmlChar*)symbol_icon_code.c_str())){
                        fprintf(file_out,"     <description>%s</description>\n",
                                (char*)xmlHashLookup(hash_for_translate, (const xmlChar*)symbol_icon_code.c_str()));
                    }
                }
            }


            fprintf(file_out,"    </period>\n");
        }
        */
        if (next_6_hours != nullval){
            fprintf(file_out,"    <period start=\"%li\" end=\"%li\">\n", begin_utc_time, begin_utc_time + 6*3600);
            if (pressure != INT_MAX){
                fprintf(file_out,"     <pressure>%i</pressure>\n", pressure);
            }
            if (temperature != INT_MAX){
                fprintf(file_out,"     <temperature>%.0f</temperature>\n", round(temperature));
            }
            if (dew_point != INT_MAX){
                fprintf(file_out,"     <dewpoint>%.0f</dewpoint>\n", round(dew_point));
            }
            if (humidity != INT_MAX){
                fprintf(file_out,"     <humidity>%.0f</humidity>\n", round(humidity));
            }
            if (uv_index != INT_MAX){
                fprintf(file_out,"     <uv_index>%i</uv_index>\n", uv_index);
            }
            if (_wind_direction != INT_MAX){
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", wind_directions[wind_index].c_str());
            }
            if (wind_speed != INT_MAX){
                fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", wind_speed);
            }
            if (next_6_hours.get("summary", nullval) != nullval){
                Json::Value summary = next_6_hours.get("summary", nullval);
                if (summary.get("symbol_code", nullval) != nullval){
                   std::string symbol_code = summary.get("symbol_code", "").asString();
                    if ((char*)xmlHashLookup(hash_for_icons, (const xmlChar*)symbol_code.c_str())){
                        fprintf(file_out,"     <icon>%s</icon>\n",  
                            (char*)xmlHashLookup(hash_for_icons, (const xmlChar*)symbol_code.c_str()));
                    }else{
                        fprintf(stderr,"\n%s\n", symbol_code.c_str());
                        fprintf(file_out,"     <icon>49</icon>\n");  
                    }
                    if ((char*)xmlHashLookup(hash_for_translate, (const xmlChar*)symbol_code.c_str())){
                        fprintf(file_out,"     <description>%s</description>\n",
                                (char*)xmlHashLookup(hash_for_translate, (const xmlChar*)symbol_code.c_str()));
                    }

                }
            }
            if (next_6_hours.get("details", nullval) != nullval){
                Json::Value details = next_6_hours.get("details", nullval);
                if (details.get("precipitation_amount", nullval) != nullval){
                   precipitation = details.get("precipitation_amount", nullval).asFloat();
                   fprintf(file_out,"     <precipitation>%.1f</precipitation>\n", precipitation);
                }
                if (details.get("air_temperature_max", nullval) != nullval){
                   float temp_hi = details.get("air_temperature_max", nullval).asFloat();
                   fprintf(file_out,"     <temperature_hi>%.0f</temperature_hi>\n", round(temp_hi));
                }
                if (details.get("air_temperature_min", nullval) != nullval){
                   float temp_low = details.get("air_temperature_min", nullval).asFloat();
                   fprintf(file_out,"     <temperature_low>%.0f</temperature_low>\n", round(temp_low));
                }
            }



            fprintf(file_out,"    </period>\n");
        }


        data = nullval;
        details = nullval;
        next_1_hours = nullval;
        next_6_hours = nullval;
        next_12_hours = nullval;
        pressure = INT_MAX;
        temperature = INT_MAX;
        dew_point = INT_MAX;
        uv_index = INT_MAX;
        wind_speed = INT_MAX;
        wind_index = INT_MAX;
        _wind_direction = INT_MAX;
        precipitation = INT_MAX;
        symbol_icon_code = "";
    }

    xmlHashFree(hash_for_icons, NULL);
    xmlHashFree(hash_for_translate, NULL);

    fclose(file_out);
    return val.size();

}
/*******************************************************************************/
int
convert_station_yrno_data(const char *station_id_with_path, const char *result_file, const char *detail_path_data){
 
    int     days_number = -1;
    char    buffer[1024];
    FILE    *file_out;
    
    UNUSED(detail_path_data);

    if(!station_id_with_path)
        return -1;
/* check for new file, if it exist, than rename it */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s.new", station_id_with_path);
    if(!access(buffer, R_OK))
        rename(buffer, station_id_with_path);
    /* check file accessability */
    if(!access(station_id_with_path, R_OK)){
       days_number =  parse_and_write_days_json_yrno_data(station_id_with_path, result_file);
       // fprintf(stderr,"days_number %i\n", days_number);
       if (days_number > 0){
            file_out = fopen(result_file, "a");
            if (file_out){
                fprintf(file_out,"</station>");
                fclose(file_out);
            }
        }
    }
    else
        return -1;/* file isn't accessability */
    return days_number;
}
/*******************************************************************************/
int
main_yr_no(int argc, char *argv[]){
    int result; 
    if (argc < 3) {
        fprintf(stderr, "yrno <input_file> <output_file> <input_detail_data>\n");
        return -1;
    }
    result = convert_station_yrno_data(argv[1], argv[2], argv[3]);
    fprintf(stderr, "\nresult = %d\n", result);
    /* fprintf(stderr, "\nresult = %d\n", result); */
    return result;
}
