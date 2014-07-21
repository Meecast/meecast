/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-fmi-fi-source - MeeCast
 *
 * Copyright (C) 2014 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
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
#include <fstream>
#include <iostream> 
#include <vector>

static xmlHashTablePtr hash_for_icons;
#define buff_size 2048

/*******************************************************************************/

int
parse_and_write_days_xml_data(const char *days_data_path, const char *result_file){
    int count_day = -1;
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    Json::Value val;
    Json::Value nullval ;
    FILE   *file_out;
    char buffer  [4096],
         buffer2 [4096],
         *delimiter = NULL;
    time_t current_time = 0;
    time_t utc_time = 0;
    time_t local_time = 0;
    int current_temperature = INT_MAX;
    int current_humidity = INT_MAX;
    int current_wind_speed = INT_MAX;
    int current_wind_gust = INT_MAX;
    int current_pressure = INT_MAX;
    int current_visibility = INT_MAX;
    int current_dewpoint = INT_MAX;
    std::string current_description = "";
    int current_icon = 48;
    float current_ppcp_rate = INT_MAX; 
    int check_timezone = false;
    int timezone = 0;
    int localtimezone = 0;
    int first_day = false;
    int afternoon = false;
    int dark = false;
    std::string current_wind_direction = "";
    struct tm time_tm1 = {0,0,0,0,0,0,0,0,0,0,0};
    struct tm time_tm2 = {0,0,0,0,0,0,0,0,0,0,0};
    struct tm tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};

    std::ifstream jsonfile(days_data_path, std::ifstream::binary);
    bool parsingSuccessful = reader.parse(jsonfile, root, false);
    if (!parsingSuccessful)
        return -1;


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
    localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 
    setlocale(LC_NUMERIC, "POSIX");
    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", buffer);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");

    //std::cerr<<root["observations"].get(root["observations"].getMemberNames()[0], nullval)<<std::endl;
    val = root["observations"].get(root["observations"].getMemberNames()[0], nullval);

    double min_distance = 32000;
    uint max_count_of_parameters = 0;

    for (uint i = 0; i < val.size(); i++){
        /* Current weather */
        if (atof(val[i].get("distance","").asCString()) < min_distance || (val[i].size()>max_count_of_parameters && atof(val[i].get("distance","").asCString()) - min_distance < 10)){
            std::string cur_time;
            cur_time = val[i].get("time","").asCString();
            if (cur_time!=""){
                min_distance = atof(val[i].get("distance","").asCString());
                max_count_of_parameters = val[i].size();
                setlocale(LC_TIME, "POSIX");
                strptime((const char*)cur_time.c_str(), "%Y%m%d%H%M", &tmp_tm);
                setlocale(LC_TIME, "");
                current_time = mktime(&tmp_tm) + 3600*localtimezone; 
                if (val[i].get("Temperature","").asString() != ""){
                    if (val[i].get("Temperature","").asString() == "nan"){
                        current_temperature = INT_MAX;
                        max_count_of_parameters--;
                    }else
                        current_temperature = atoi(val[i].get("Temperature","").asCString());
                }    
                if (val[i].get("Humidity","").asString() != ""){
                    if (val[i].get("Humidity","").asString() == "nan"){
                        current_humidity = INT_MAX;
                        max_count_of_parameters--;
                    }else{
                        current_humidity = atoi(val[i].get("Humidity","").asCString());
                    }
                }    
                if (val[i].get("WindSpeedMS","").asString() != ""){
                    if (val[i].get("WindSpeedMS","").asString() == "nan"){
                        current_wind_speed = INT_MAX;
                        max_count_of_parameters--;
                    }else
                        current_wind_speed = atoi(val[i].get("WindSpeedMS","").asCString());
                }    
                if (val[i].get("WindCompass8","").asString() != ""){

                    if (val[i].get("WindCompass8","").asString() == "nan"){
                        current_wind_direction = "N/A";
                        max_count_of_parameters--;
                    }else{
                        current_wind_direction = val[i].get("WindCompass8","").asCString();
                    }
                }    
                if (val[i].get("WindGust","").asString() != ""){
                    if (val[i].get("WindGust","").asString() == "nan"){
                        current_wind_gust = INT_MAX;
                        max_count_of_parameters--;
                    }else
                        current_wind_gust = atoi(val[i].get("WindGust","").asCString());
                }    
                if (val[i].get("Pressure","").asString() != ""){
                    if (val[i].get("Pressure","").asString() == "nan"){
                        current_pressure = INT_MAX;
                        max_count_of_parameters--;
                    }else
                        current_pressure = atoi(val[i].get("Pressure","").asCString());
                }    
                if (val[i].get("Visibility","").asString() != ""){
                    if (val[i].get("Visibility","").asString() == "nan"){
                        current_visibility = INT_MAX;
                    }else
                        current_visibility = atoi(val[i].get("Visibility","").asCString());
                }    
                if (val[i].get("DewPoint","").asString() != ""){
                    if (val[i].get("DewPoint","").asString() == "nan"){
                        current_dewpoint = INT_MAX;
                    }else
                        current_dewpoint = atoi(val[i].get("DewPoint","").asCString());
                }    
                if (val[i].get("RI_10MIN","").asString() != ""){
                    if (val[i].get("RI_10MIN","").asString() == "nan"){
                        current_ppcp_rate = INT_MAX;
                    }else
                        current_ppcp_rate = atof(val[i].get("RI_10MIN","").asCString());
                }    
                if (val[i].get("WW_AWS","").asString() != "" && val[i].get("WW_AWS","").asString() != "nan"){
                    int code = atoi(val[i].get("WW_AWS","").asCString());
                    if (code==0 || (code>=20&&code<=29)){
                        current_icon = 32;
                        current_description = "Clear";
                    }
                    if (code==4 || code==5){
                        current_icon = 22;
                        current_description = "Haze, Smoke or Dust";
                    }
                    if (code==10){
                        current_icon = 20;
                        current_description = "Mist";
                    }
                    if (code>=30 && code<=34){
                        current_icon = 20;
                        current_description = "Fog";
                    }
                    if (code==40){
                        current_icon = 12;
                        current_description = "Precipitation";
                    }
                    if (code>=50 && code<=53){
                        current_icon = 9;
                        current_description = "Drizzle";
                    }
                    if (code==60){
                        current_icon = 12;
                        current_description = "Rain";
                    }
                    if (code==41){
                        current_icon = 39;
                        current_description = "Light or Moderate Precipitation";
                    }
                    if (code==42){
                        current_icon = 12;
                        current_description = "Heavy Precipitation";
                    }
                    if (code>=54 && code<=56){
                        current_icon = 8;
                        current_description = "Freezing Drizzle";
                    }
                    if (code==61){
                        current_icon = 39;
                        current_description = "Light Rain";
                    }
                    if (code==62){
                        current_icon = 12;
                        current_description = "Moderate Rain";
                    }
                    if (code==63){
                        current_icon = 12;
                        current_description = "Heavy Rain";
                    }
                    if (code==64){
                        current_icon = 10;
                        current_description = "Light Freezing Rain";
                    }
                    if (code==65){
                        current_icon = 10;
                        current_description = "Moderate Freezing Rain";
                    }
                    if (code==66){
                        current_icon = 10;
                        current_description = "Heavy Freezing Rain";
                    }
                    if (code==67){
                        current_icon = 6;
                        current_description = "Light Sleet";
                    }
                    if (code==68){
                        current_icon = 6;
                        current_description = "Moderate Sleet";
                    }
                    if (code==70){
                        current_icon = 14;
                        current_description = "Snow";
                    }
                    if (code==71){
                        current_icon = 14;
                        current_description = "Light Snow";
                    }
                    if (code==72){
                        current_icon = 14;
                        current_description = "Light Snow";
                    }
                    if (code==73){
                        current_icon = 16;
                        current_description = "Heavy Snow";
                    }
                    if (code==74 || code==75 || code==76){
                        current_icon = 7;
                        current_description = "Ice Pellets";
                    }
                    if (code==80){
                        current_icon = 39;
                        current_description = "Showers or Intermittent Precipitation";
                    }
                    if (code==81){
                        current_icon = 39;
                        current_description = "Light Rain Showers";
                    }
                    if (code==82){
                        current_icon = 39;
                        current_description = "Moderate Rain Showers";
                    }
                    if (code==83){
                        current_icon = 39;
                        current_description = "Heavy Rain Showers";
                    }
                    if (code==84){
                        current_icon = 39;
                        current_description = "Violent Rain Showers";
                    }
                    if (code==85){
                        current_icon = 41;
                        current_description = "Light Snow Showers";
                    }
                    if (code==86){
                        current_icon = 41;
                        current_description = "Moderate Snow Showers";
                    }
                    if (code==87){
                        current_icon = 41;
                        current_description = "Heavy Snow Showers";
                    }
                }    
            }
        }
    }

    /* Forecasts */
    val = root["forecasts"][0].get("forecast", nullval);
    for (uint i = 0; i < val.size(); i++){
        std::string utc_time_string;
        std::string _time_string;
        std::string local_time_string;
        int icon = 48;
        time_t offset_time = 0;
        std::string description = "";
        dark = false;

        utc_time_string = val[i].get("utctime","").asCString();
        if (utc_time_string != ""){
            setlocale(LC_TIME, "POSIX");
            strptime((const char*)utc_time_string.c_str(), "%Y%m%dT%H%M%S", &tmp_tm);
            setlocale(LC_TIME, "");
            utc_time = mktime(&tmp_tm); 
            /* get timezone */
            if (!check_timezone){
                local_time_string = val[i].get("localtime","").asCString();
                setlocale(LC_TIME, "POSIX");
                strptime((const char*)local_time_string.c_str(), "%Y%m%dT%H%M%S", &tmp_tm);
                setlocale(LC_TIME, "");
                local_time = mktime(&tmp_tm); 
                timezone = (int)((local_time-utc_time)/3600);
                fprintf(file_out,"  <timezone>%i</timezone>\n", timezone);
                check_timezone = true;
                
                first_day = true;
                /* set forecast for whole day */
                if (tmp_tm.tm_hour >=15){
                    offset_time = (tmp_tm.tm_hour - localtimezone - 1)*3600;
                    utc_time = utc_time - offset_time; 
                    afternoon = true;
                }else{
                    offset_time = 2*3600;
                    utc_time = utc_time - offset_time; 
                }    
            }    
            
            if (val[i].get("Temperature","").asString() == "nan" && val[i].get("WeatherSymbol3","").asString() == "nan" ){
                continue;
            }
            if (first_day){
                if (afternoon){
                    fprintf(file_out,"    <period start=\"%li\"", utc_time + 3600*localtimezone);
                    fprintf(file_out," end=\"%li\">\n", utc_time + offset_time + 3*3600 + 3600*localtimezone); 
                }else{    
                    fprintf(file_out,"    <period start=\"%li\"", utc_time + 3600*localtimezone) ;
                    fprintf(file_out," end=\"%li\">\n", utc_time + 3600*localtimezone + 3*3600 + offset_time);
                }
            }else{
                fprintf(file_out,"    <period start=\"%li\" hour=\"true\"", utc_time + 3600*localtimezone);
                fprintf(file_out," end=\"%li\">\n", utc_time + 3600*localtimezone + 3*3600); 
            }    

            if (val[i].get("Temperature","").asString() != "" || val[i].get("Temperature","").asString() != "nan"){
                fprintf(file_out,"     <temperature>%i</temperature>\n", atoi(val[i].get("Temperature","").asCString()));
            }    
            if (val[i].get("WeatherSymbol3","").asString() != "" || val[i].get("WeatherSymbol3","").asString() != "nan"){
                int result = 0;
                dark = atoi(val[i].get("dark","").asCString());
                result = 100*(dark);
                result = result + atoi(val[i].get("WeatherSymbol3","").asCString());
                switch (result){
                    case 1:
                        icon = 32;
                        description = "Clear";
                        break;
                    case 101:
                        icon = 31;
                        description = "Clear";
                        break;
                    case 2:
                        icon = 30;
                        description = "Partly Cloudy";
                        break;
                    case 102:
                        icon = 29;
                        description = "Partly Cloudy";
                        break;
                    case 3:
                        icon = 26;
                        description = "Cloudy";
                        break;
                    case 103:
                        icon = 26;
                        description = "Cloudy";
                        break;
                    case 21:
                        icon = 39;
                        description = "Light Rain Showers";
                        break;
                    case 121:
                        icon = 45;
                        description = "Light Rain Showers";
                        break;
                    case 22:
                        icon = 39;
                        description = "Rain Showers";
                        break;
                    case 122:
                        icon = 45;
                        description = "Rain Showers";
                        break;
                    case 23:
                        icon = 11;
                        description = "Heavy Rain Showers";
                        break;
                    case 123:
                        icon = 11;
                        description = "Heavy Rain Showers";
                        break;
                    case 31:
                        icon = 11;
                        description = "Light Rain";
                        break;
                    case 131:
                        icon = 11;
                        description = "Light Rain";
                        break;
                    case 32:
                        icon = 12;
                        description = "Rain";
                        break;
                    case 132:
                        icon = 12;
                        description = "Rain";
                        break;
                    case 33:
                        icon = 12;
                        description = "Heavy Rain";
                        break;
                    case 133:
                        icon = 12;
                        description = "Heavy Rain";
                        break;
                    case 41:
                        icon = 41;
                        description = "Light Snow Showers";
                        break;
                    case 141:
                        icon = 41;
                        description = "Light Snow Showers";
                        break;
                    case 42:
                        icon = 41;
                        description = "Snow Showers";
                        break;
                    case 142:
                        icon = 41;
                        description = "Snow Showers";
                        break;
                    case 43:
                        icon = 41;
                        description = "Heavy Snow Showers";
                        break;
                    case 143:
                        icon = 41;
                        description = "Heavy Snow Showers";
                        break;
                    case 51:
                        icon = 14;
                        description = "Light Snowfall";
                        break;
                    case 151:
                        icon = 14;
                        description = "Light Snowfall";
                        break;
                    case 52:
                        icon = 14;
                        description = "Snowfall";
                        break;
                    case 152:
                        icon = 14;
                        description = "Snowfall";
                        break;
                    case 53:
                        icon = 42;
                        description = "Heavy Snowfall";
                        break;
                    case 153:
                        icon = 42;
                        description = "Heavy Snowfall";
                        break;
                    case 61:
                        icon = 38;
                        description = "Thundershowers";
                        break;
                    case 161:
                        icon = 47;
                        description = "Thundershowers";
                        break;
                    case 62:
                        icon = 38;
                        description = "Strong Thundershowers";
                        break;
                    case 162:
                        icon = 47;
                        description = "Strong Thundershowers";
                        break;
                    case 63:
                        icon = 17;
                        description = "Thunder";
                        break;
                    case 163:
                        icon = 17;
                        description = "Thunder";
                        break;
                    case 64:
                        icon = 4;
                        description = "Heavy Thunder";
                        break;
                    case 164:
                        icon = 4;
                        description = "Heavy Thunder";
                        break;
                    case 71:
                        icon = 6;
                        description = "Light Sleet Showers";
                        break;
                    case 171:
                        icon = 6;
                        description = "Light Sleet Showers";
                        break;
                    case 72:
                        icon = 6;
                        description = "Sleet Showers";
                        break;
                    case 172:
                        icon = 6;
                        description = "Sleet Showers";
                        break;
                    case 73:
                        icon = 6;
                        description = "Heavy Sleet Showers";
                        break;
                    case 173:
                        icon = 6;
                        description = "Heavy Sleet Showers";
                        break;
                    case 81:
                        icon = 5;
                        description = "Light Sleet";
                        break;
                    case 181:
                        icon = 5;
                        description = "Light Sleet";
                        break;
                    case 82:
                        icon = 5;
                        description = "Sleet";
                        break;
                    case 182:
                        icon = 5;
                        description = "Sleet";
                        break;
                    case 83:
                        icon = 5;
                        description = "Heavy Sleet";
                        break;
                    case 183:
                        icon = 5;
                        description = "Heavy Sleet";
                        break;


                }
                fprintf(file_out,"     <icon>%i</icon>\n", icon);
                fprintf(file_out, "     <description>%s</description>\n", description.c_str());
            }    
            if (val[i].get("PoP","").asString() != ""){
                fprintf(file_out,"     <ppcp>%i</ppcp>\n", atoi(val[i].get("PoP","").asCString()));
            }    
            if (val[i].get("WindSpeedMS","").asString() != ""){
                fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", atoi(val[i].get("WindSpeedMS","").asCString()));
            }    
            if (val[i].get("WindCompass8","").asString() != ""){
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", val[i].get("WindCompass8","").asCString());
            }    
            if (val[i].get("Precipitation1h","").asString() != ""){
                fprintf(file_out,"     <precipitation>%.1f</precipitation>\n", atof(val[i].get("Precipitation1h","").asCString()));
            }    
            if (val[i].get("FeelsLike","").asString() != ""){
                fprintf(file_out,"     <flike>%i</flike>\n", atoi(val[i].get("FeelsLike","").asCString()));
            }    

            fprintf(file_out,"    </period>\n");
            if (first_day){
                utc_time = current_time - localtimezone*3600;
                fprintf(file_out,"    <period start=\"%li\"", utc_time - 2*3600);
                fprintf(file_out," end=\"%li\" current=\"true\">\n", utc_time + 6*3600); 

                fprintf(file_out,"     <temperature>%i</temperature>\n", current_temperature); 
                if (current_icon != 48){
                    if (current_icon == 32 && dark)
                        current_icon = 31;
                    if (current_icon == 39 && dark)
                        current_icon = 45;
                    if (current_icon == 41 && dark)
                        current_icon = 46;
                    fprintf(file_out,"     <icon>%i</icon>\n", current_icon);
                }else{

                    fprintf(file_out,"     <icon>%i</icon>\n", icon);
                }
                if (current_description != "") 
                    fprintf(file_out,"     <description>%s</description>\n", current_description.c_str());
                else
                    fprintf(file_out,"     <description>%s</description>\n", description.c_str());
                fprintf(file_out,"     <pressure>%i</pressure>\n", current_pressure);
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n", current_wind_direction.c_str());
                fprintf(file_out,"     <humidity>%i</humidity>\n", current_humidity);
                fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", current_wind_speed);
                fprintf(file_out,"     <wind_gust>%i</wind_gust>\n", current_wind_gust);
                fprintf(file_out,"     <dewpoint>%i</dewpoint>\n", current_dewpoint);
                fprintf(file_out,"     <precipitation>%.1f</precipitation>\n", current_ppcp_rate);
                fprintf(file_out,"     <visible>%i</visible>\n", current_visibility);
                fprintf(file_out,"    </period>\n");
            }    
            first_day = false;
            afternoon = false;
        }
    }
    fclose(file_out);
    setlocale(LC_NUMERIC, "");
    count_day=1;
    return count_day;
}


/*******************************************************************************/
int
convert_station_fmi_fi_data(const char *days_data_path, const char *result_file){
 
    int     days_number = -1;
    char    *delimiter = NULL;
    FILE    *file_out;
    
    if(!days_data_path)
        return -1;

    //hash_for_icons = hash_icons_fmifi_table_create();
 //   snprintf(buffer, sizeof(buffer)-1,"%s.timezone", result_file);
    /* check file accessability */
    if(!access(days_data_path, R_OK)){
            days_number = parse_and_write_days_xml_data(days_data_path, result_file);
    }else{
        return -1;/* file isn't accessability */
    }

    
    if (days_number > 0){
        file_out = fopen(result_file, "a");
        if (file_out){
            fprintf(file_out,"</station>");
            fclose(file_out);
        }
    }

    return days_number;
}

/*******************************************************************************/
int
main(int argc, char *argv[]){
    int result; 
    if (argc < 2) {
        fprintf(stderr, "fmifi <input_days_file> <output_file> <input_hours_file> <input_current_file>\n");
        return -1;
    }
    result = convert_station_fmi_fi_data(argv[1], argv[2]);
    //fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
