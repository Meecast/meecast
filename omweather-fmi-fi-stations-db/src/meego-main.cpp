/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-fmi-fi-source - MeeCast
 *
 * Copyright (C) 2014-2024 Vlad Vasilyeu
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
#include <map>
#include "json/json.h"
#include "date/tz.h"
#include <math.h> 
#include <chrono>
#include <fstream>
#include <iostream> 
#include <vector>

#include <algorithm>
#include <regex>
#define buff_size 2048

/*******************************************************************************/

int
parse_and_write_days_xml_data(const char *days_data_path, const char *result_file, const char *current_data_path){
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
    int current_temperature = INT_MAX;
    int current_humidity = INT_MAX;
    int current_wind_speed = INT_MAX;
    int current_wind_gust = INT_MAX;
    int current_pressure = INT_MAX;
    int current_visibility = INT_MAX;
    int current_dewpoint = INT_MAX;
    std::string current_description = "";
    std::string sunrise_time = "";
    std::string sunset_time = "";
    int current_icon = 48;
    float current_precipitation = INT_MAX; 
    int check_timezone = false;
    int timezone = 0;
    int localtimezone = 0;
    int first_day = false;
    int afternoon = false;
    std::string current_wind_direction = "";
    struct tm time_tm1 = {0,0,0,0,0,0,0,0,0,0,0};
    struct tm time_tm2 = {0,0,0,0,0,0,0,0,0,0,0};
    struct tm tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
    std::string wind_directions[17] = {"N","NNE","NE","ENE","E","ESE","SE","SSE","S","SSW","SW","WSW","W","WNW","NW","NNW","N"};
    int wind_index = INT_MAX;


    std::ifstream htmlfile(days_data_path, std::ifstream::binary);
    htmlfile.seekg(0, std::ios::end);
    size_t size_0 = htmlfile.tellg();
    std::string buffer_0(size_0, ' ');
    htmlfile.seekg(0);
    htmlfile.read(&buffer_0[0], size_0);
    //size_t index_0 = buffer_0.find("__NUXT__=(function(", 0);
    size_t index_0 = buffer_0.find("__NUXT__=(function(", 0);
    if (index_0  == std::string::npos){
        std::cerr<<"Error in index_0"<<std::endl;
        return -1;
    }
    size_t index_1 = buffer_0.find("));</script>", index_0);
    if (index_1  == std::string::npos){
        std::cerr<<"Error in index_1"<<std::endl;
        return -1;
    }

    std::string buffer_keys_and_values = buffer_0.substr(index_0 + 19, index_1 - index_0 - 17);
    std::map<std::string, std::string> dictionary;
    size_t i = 0;
    std::string param = "";
    size_t index_for_begin_values = buffer_keys_and_values.find("}}(", 0) + 3;
    if (index_for_begin_values  == std::string::npos){
        std::cerr<<"Error in index_for_begin_values"<<std::endl;
        return -1;
    }
    //std::cout<<index_for_begin_values<<std::endl;
    size_t index_next_values = index_for_begin_values;
    while (i<(buffer_keys_and_values.length() -1)){
        //std::cerr<<buffer_keys_and_values[i]<<std::endl;
        if (buffer_keys_and_values[i] == ','){
            std::string value = ""; 
            bool stop_comma_flag = false;
            while (index_next_values<(buffer_keys_and_values.length() -1)){
                if (buffer_keys_and_values[index_next_values] == '"'){
                    if (stop_comma_flag == true){
                        stop_comma_flag = false;
                    } else{
                        stop_comma_flag = true;
                    }
                }
                if (buffer_keys_and_values[index_next_values] == ',' and not stop_comma_flag){
                    dictionary[param] = value;
                    index_next_values ++;
                    break;
                }
                value = value + buffer_keys_and_values[index_next_values];
                index_next_values ++;
            }
            //std::cerr<<param<<std::endl;
            param = "";
            i++;
            continue;
        }
        if (buffer_keys_and_values[i] == ')'){
            break;
        }

        param = param + buffer_keys_and_values[i];
        i++;
    }

    //std::cout << "Elements of Dictionary:" << std::endl;
    //for (const auto& pair : dictionary) {
    //    std::cout << pair.first << ": " << pair.second << std::endl;
    //}
    //std::cout<<buffer_keys_and_values<<std::endl;

    size_t index_for_begin_timezone = buffer_keys_and_values.find("foundLocation:", 0);
    if (index_for_begin_timezone  == std::string::npos){
        std::cerr<<"Error in index_for_begin_timezone"<<std::endl;
        return -1;
    }

    size_t index_for_timezone = buffer_keys_and_values.find("timezone:", 0);
    if (index_for_timezone  == std::string::npos){
        std::cerr<<"Error in timezone"<<std::endl;
        return -1;
    }


    size_t index_for_end_timezone = buffer_keys_and_values.find("},", index_for_begin_timezone);
    if (index_for_end_timezone  == std::string::npos){
        std::cerr<<"Error in index_for_end_timezone"<<std::endl;
        return -1;
    }
    
    std::string buffer_timezone = buffer_keys_and_values.substr(index_for_begin_timezone, index_for_end_timezone - index_for_begin_timezone + 1);
    buffer_timezone.erase(std::remove(buffer_timezone.begin(), buffer_timezone.end(), '"'), buffer_timezone.end());
    buffer_timezone.insert(0, "{");
    /* Replace substring , to "," */
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex(","), "\",\"");
    //std::cout << buffer_timezone << std::endl;

    /* Replace substring : to ":" */
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex(":"), "\":\"");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring { to {" */
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex("\\{"), "{\"");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring } to "} */
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex("\\}"), "\"}");

    /* Replace substring ":"{" to ":{" */
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex("\":\"\\{\""), "\":{\"");
    buffer_timezone += "}";
    /*
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex("foundLocation"), "\"foundLocation\"");
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex("place"), "\"place\"");
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex("area"), "\"area\"");
    buffer_timezone = std::regex_replace(buffer_timezone, std::regex("timezone"), "\"timezone\"");
    */

    //std::cout<<buffer_timezone<<std::endl;
    Json::Value _timezone_json;   // will contains the root value after parsing.
    bool parsingSuccessfulTimezone = reader.parse(buffer_timezone, _timezone_json, false);
    if (!parsingSuccessfulTimezone){
        std::cerr<<"Problem in parsingSuccessfulTimezone"<<std::endl;
        return -1;
    }

    val = _timezone_json["foundLocation"];
    auto timezone_json = val["timezone"].asString();


    date::zoned_time<std::chrono::system_clock::duration> sy = date::make_zoned(timezone_json, std::chrono::system_clock::now());
    auto offset = sy.get_info().offset;
    std::cerr<<"offset count "<<offset.count()<<std::endl;
    timezone = offset.count()/3600;

    size_t index_for_begin_daylength = buffer_keys_and_values.find("dayLength:", 0);
    if (index_for_begin_daylength  == std::string::npos){
        std::cerr<<"Error in index_for_begin_daylength"<<std::endl;
        return -1;
    }

    size_t index_for_end_daylength = buffer_keys_and_values.find("},", index_for_begin_daylength);
    if (index_for_end_daylength  == std::string::npos){
        std::cerr<<"Error in index_for_end_daylength"<<std::endl;
        return -1;
    }

    std::string buffer_1_ = buffer_keys_and_values.substr(index_for_begin_daylength, index_for_end_daylength - index_for_begin_daylength + 1);
    //dayLength:{sunrise:"6:29",sunset:"20:04",lengthofday:"13 h 35 min"}
    //{"dayLength":"{"sunrise":"6":"29","sunset":"20":"04","lengthofday":"13 h 35 min"}}
    /* Convert js-script data to JSON */
    /* Replace substring :00:00 to *00*00 */
    buffer_1_ = std::regex_replace(buffer_1_, std::regex(":\""), "*\"");
    buffer_1_ = std::regex_replace(buffer_1_, std::regex(":"), "|");
    //std::erase(buffer_1, '"');
    buffer_1_.erase(std::remove(buffer_1_.begin(), buffer_1_.end(), '"'), buffer_1_.end());
    buffer_1_.insert(0, "{");
    /* Replace substring , to "," */
    buffer_1_ = std::regex_replace(buffer_1_, std::regex(","), "\",\"");
    /* Replace substring { to {" */
    buffer_1_ = std::regex_replace(buffer_1_, std::regex("\\{"), "{\"");
    /* Replace substring } to "} */
    buffer_1_ = std::regex_replace(buffer_1_, std::regex("\\}"), "\"}");
    /* Replace substring *00*00 to :00:00 */
    buffer_1_ = std::regex_replace(buffer_1_, std::regex("\\*"), "\":\"");
    /* Replace substring : to ":" */
    buffer_1_ = std::regex_replace(buffer_1_, std::regex("\\|"), ":");
    buffer_1_ = std::regex_replace(buffer_1_, std::regex("dayLength:"), "dayLength\":");
    buffer_1_ += "}";

    bool parsingDayLengthSuccessful = reader.parse(buffer_1_, root, false);
    if (!parsingDayLengthSuccessful){
        std::cerr<<"Problem in parsingDayLengthSuccessful";
        return -1;
    }

    sunrise_time = root["dayLength"]["sunrise"].asString();
    sunset_time = root["dayLength"]["sunset"].asString();

    size_t index_for_begin_forecast = buffer_keys_and_values.find("forecastValues:", 0);
    if (index_for_begin_forecast  == std::string::npos){
        std::cerr<<"Error in index_for_begin_forecast"<<std::endl;
        return -1;
    }

    //std::cout<<index_for_begin_forecast<<std::endl;
    size_t index_for_end_forecast = buffer_keys_and_values.find("}],", index_for_begin_forecast);
    if (index_for_end_forecast  == std::string::npos){
        std::cerr<<"Error in index_for_end_forecast"<<std::endl;
        return -1;
    }
    //std::cout<<index_for_end_forecast<<std::endl;

    std::string buffer_1 = buffer_keys_and_values.substr(index_for_begin_forecast, index_for_end_forecast - index_for_begin_forecast + 2);

    //std::cout << buffer_1 << std::endl;


    size_t index_for_begin_symbolDescriptions = buffer_keys_and_values.find("symbolDescriptions:", 0);
    if (index_for_begin_symbolDescriptions  == std::string::npos){
        std::cerr<<"Error in index_for_begin_symbolDescriptions"<<std::endl;
        return -1;
    }
    //std::cout << "index_for_begin_symbolDescriptions "<<index_for_begin_symbolDescriptions << std::endl;
    size_t index_for_end_symbolDescriptions = buffer_keys_and_values.find("}},", index_for_begin_symbolDescriptions);
    if (index_for_end_symbolDescriptions  == std::string::npos){
        std::cerr<<"Error in index_for_end_symbolDescriptions"<<std::endl;
        return -1;
    }
    //std::cout << "index_for_end_symbolDescriptions "<<index_for_end_symbolDescriptions << std::endl;

    std::string descriptions = buffer_keys_and_values.substr(index_for_begin_symbolDescriptions, index_for_end_symbolDescriptions - index_for_begin_symbolDescriptions + 2);

    descriptions.insert(0, "{");
    descriptions += "}";
    //std::cout << descriptions << std::endl;
    /* Replace substring txt_en to "txt_en" */
    descriptions = std::regex_replace(descriptions, std::regex("symbolDescriptions"), "\"symbolDescriptions\"");
    descriptions = std::regex_replace(descriptions, std::regex("txt_en"), "\"txt_en\"");
    descriptions = std::regex_replace(descriptions, std::regex("txt_fi"), "\"txt_fi\"");
    descriptions = std::regex_replace(descriptions, std::regex("txt_sv"), "\"txt_sv\"");
    //std::cout << descriptions << std::endl;

    Json::Value _descriptions_json;   // will contains the root value after parsing.
    bool parsingSuccessfulDesc = reader.parse(descriptions, _descriptions_json, false);
    //std::cout <<  parsingSuccessfulDesc<< std::endl;
    if (!parsingSuccessfulDesc){
        std::cerr<<"Problem in parsingSuccessfulDesc";
        return -1;
    }

    Json::Value descriptions_json;
    descriptions_json = _descriptions_json["symbolDescriptions"];

    //std::cerr<<descriptions_json.size()<<std::endl;

    /* Convert js-script data to JSON */
    //std::erase(buffer_1, '"');
    buffer_1.erase(std::remove(buffer_1.begin(), buffer_1.end(), '"'), buffer_1.end());
    //std::cout << buffer_1 << std::endl;

    buffer_1.insert(0, "{");

    /* Replace substring :00:00 to *00*00 */
    buffer_1 = std::regex_replace(buffer_1, std::regex(":00:00"), "*00*00");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring },{ to }|{ */
    buffer_1 = std::regex_replace(buffer_1, std::regex("\\},\\{"), "}|{");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring , to "," */
    buffer_1 = std::regex_replace(buffer_1, std::regex(","), "\",\"");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring : to ":" */
    buffer_1 = std::regex_replace(buffer_1, std::regex(":"), "\":\"");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring { to {" */
    buffer_1 = std::regex_replace(buffer_1, std::regex("\\{"), "{\"");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring } to "} */
    buffer_1 = std::regex_replace(buffer_1, std::regex("\\}"), "\"}");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring }|{ to },{ */
    buffer_1 = std::regex_replace(buffer_1, std::regex("\\}\\|\\{"), "},{");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring *00*00 to :00:00 */
    buffer_1 = std::regex_replace(buffer_1, std::regex("\\*00\\*00"), ":00:00");
    //std::cout << buffer_1 << std::endl;

    /* Replace substring "[ to [ */
    buffer_1 = std::regex_replace(buffer_1, std::regex("\"\\["), "[");
    //std::cout << buffer_1 << std::endl;



    buffer_1 += "}";
    //std::cout << buffer_1 << std::endl;
    //bool parsingSuccessful = reader.parse(jsonfile, root, false);
    bool parsingSuccessful = reader.parse(buffer_1, root, false);
    if (!parsingSuccessful){
        std::cerr<<"Problem in parsingSuccessful";
        return -1;
    }

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

    //std::cerr<<root["observations"].get(root["observations"].getMemberNames()[0], nullval)<<std::endl;
    //val = root["observations"].get(root["observations"].getMemberNames()[0], nullval);
    val = root["forecastValues"];
    //std::cerr<<val.size();

    /* Forecasts */
    //val = root["forecasts"][0].get("forecast", nullval);
    val = root["forecastValues"];
    for (uint i = 0; i < val.size(); i++){
        std::string _local_time_string;
        std::string _time_string;
        std::string local_time_string;
        int icon = 48;
        time_t offset_time = 0;
        std::string description = "";

        _local_time_string = val[i].get("isolocaltime","").asCString();
        if (dictionary.find(_local_time_string) != dictionary.end()) {
            _local_time_string = dictionary[_local_time_string];
           // std::erase(_local_time_string, '"');
    	   _local_time_string.erase(std::remove(_local_time_string.begin(), _local_time_string.end(), '"'), _local_time_string.end());
        }
            //std::cerr<<"_local_time_string"<<_local_time_string<<std::endl;
        if (_local_time_string != ""){
            tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
            tmp_tm.tm_isdst = time_tm2.tm_isdst;
            setlocale(LC_TIME, "POSIX");
            strptime((const char*)_local_time_string.c_str(), "%Y-%m-%dT%H:%M:%S", &tmp_tm);
            utc_time = mktime(&tmp_tm); 
            setlocale(LC_TIME, "");
            //const std::chrono::zoned_time zt{"Europe\u002FMadrid", std::chrono::system_clock::now()};
            //std::cout <<  "Europe\u002FMadrid" << " - Zoned Time: " << zt << '\n';
            /* get timezone */
            if (!check_timezone){
                fprintf(file_out,"  <timezone>%i</timezone>\n", timezone);
                check_timezone = true;
                first_day = true;
                /* set forecast for whole day */
                if (tmp_tm.tm_hour >=15){
                    offset_time = (tmp_tm.tm_hour - localtimezone - 1)*3600;
                    afternoon = true;
                }else{
                    offset_time = 2*3600;
                }    
            }    
            
            std::string SmartSymbol = "";
            SmartSymbol = val[i].get("SmartSymbol","").asCString();
            if (dictionary.find(SmartSymbol) != dictionary.end()) {
                //std::cerr<<"00000000000000"<<SmartSymbol;
                SmartSymbol = dictionary[SmartSymbol];
                //std::erase(SmartSymbol, '"');
    	   	SmartSymbol.erase(std::remove(SmartSymbol.begin(), SmartSymbol.end(), '"'), SmartSymbol.end());
            }


            /* nan - не бывает */ 
            if (val[i].get("Temperature","").asString() == "nan" && SmartSymbol == "nan" ){
                continue;
            }
            
            if (first_day){
                if (afternoon){
                    fprintf(file_out,"    <period start=\"%li\" hour=\"true\"", utc_time + 3600*localtimezone - 3600*timezone);
                    fprintf(file_out," end=\"%li\">\n", utc_time + offset_time + 3*3600 + 3600*localtimezone - 3600*timezone + 5*3600); 
                }else{    
                    fprintf(file_out,"    <period start=\"%li\" hour=\"true\"", utc_time + 3600*localtimezone - 3600*timezone - 3600) ;
                    fprintf(file_out," end=\"%li\">\n", utc_time + 3600*localtimezone + 3*3600 + offset_time - 3600*timezone + 5*3600);
                }
            }else{
                fprintf(file_out,"    <period start=\"%li\" hour=\"true\"", utc_time + 3600*localtimezone - 3600*timezone);
                fprintf(file_out," end=\"%li\">\n", utc_time + 3600*localtimezone + 3*3600 - 3600*timezone); 
            }    

            if (val[i].get("Temperature","").asString() != "" || val[i].get("Temperature","").asString() != "nan"){
                if (std::isdigit(val[i].get("Temperature","").asString()[0]) ||
                                 val[i].get("Temperature","").asString()[0] == '-' ||
                                 val[i].get("Temperature","").asString()[0] == '+' ||
                                 val[i].get("Temperature","").asString()[0] == '.'){
                    fprintf(file_out,"     <temperature>%.0f</temperature>\n", atof(val[i].get("Temperature","").asCString()));
                }else{
                    if (dictionary.find(val[i].get("Temperature","").asString()) != dictionary.end()) {
                        fprintf(file_out,"     <temperature>%.0f</temperature>\n", atof(dictionary[val[i].get("Temperature","").asString()].c_str()));
                    }
                }
            }    
            std::string description = "";
            if (SmartSymbol != "" ){
                int result = 0;
                result = std::stoi(SmartSymbol);
                description = descriptions_json[SmartSymbol].get("txt_en","").asString();
                switch (result){
                    case 1:
                        icon = 32;
                        description = "Clear";
                        break;
                    case 4:
                        icon = 30;
                        description = "Partly Cloudy";
                        break;
                    case 6:
                        icon = 28;
                        description = "Mostly Cloudy";
                        break;
                    case 2:
                        icon = 30;
                        description = "Partly Cloudy";
                        break;
                    case 3:
                        icon = 26;
                        description = "Cloudy";
                        break;
                    case 7:
                        icon = 26;
                        description = "Overcast";
                        break;
                    case 9:
                        icon = 20;
                        description = "Fog";
                        break;
                    case 11:
                        icon = 9;
                        description = "Drizzle";
                        break;
                    case 21:
                        icon = 39;
                        description = "Light Rain Showers";
                        break;
                    case 22:
                        icon = 39;
                        description = "Rain Showers";
                        break;
                    case 23:
                        icon = 11;
                        description = "Heavy Rain Showers";
                        break;
                    case 24:
                        icon = 39;
                        description = "Scattered Showers";
                        break;
                    case 27:
                        icon = 11;
                        description = "Showers";
                        break;
                    case 31:
                        icon = 11;
                        description = "Light Rain";
                        break;
                    case 32:
                        icon = 12;
                        description = "Rain";
                        break;
                    case 33:
                        icon = 12;
                        description = "Heavy Rain";
                        break;
                    case 34:
                        icon = 39;
                        description = "Mostly Cloudy And Periods Of Light Rain";
                        break;
                    case 35:
                        icon = 39;
                        description = "Mostly Cloudy And Periods Of Moderate Rain";
                        break;
                    case 37:
                        icon = 11;
                        description = "Light Rain";
                        break;
                    case 38:
                        icon = 12;
                        description = "Moderate Rain";
                        break;
                    case 39:
                        icon = 12;
                        description = "Heavy Rain";
                        break;
                    case 41:
                        icon = 41;
                        description = "Light Snow Showers";
                        break;
                    case 42:
                        icon = 41;
                        description = "Snow Showers";
                        break;
                    case 43:
                        icon = 41;
                        description = "Heavy Snow Showers";
                        break;
                    case 44:
                        icon = 6;
                        description = "Scattered Light Sleet Showers";
                        break;
                    case 47:
                        icon = 6;
                        description = "Light Sleet";
                        break;
                    case 48:
                        icon = 6;
                        description = "Moderate Sleet";
                        break;
                    case 51:
                        icon = 14;
                        description = "Light Snowfall";
                        break;
                    case 52:
                        icon = 14;
                        description = "Snowfall";
                        break;
                    case 53:
                        icon = 42;
                        description = "Heavy Snowfall";
                        break;
                    case 54:
                        icon = 41;
                        description = "Scattered Light Snow Showers";
                        break;
                    case 57:
                        icon = 14;
                        description = "Light Snowfall";
                        break;
                    case 58:
                        icon = 14;
                        description = "Moderate Snowfall";
                        break;
                    case 59:
                        icon = 42;
                        description = "Heavy Snowfall";
                        break;
                    case 61:
                        icon = 38;
                        description = "Thundershowers";
                        break;
                    case 62:
                        icon = 38;
                        description = "Strong Thundershowers";
                        break;
                    case 63:
                        icon = 17;
                        description = "Thunder";
                        break;
                    case 64:
                        icon = 4;
                        description = "Heavy Thunder";
                        break;
                    case 72:
                        icon = 6;
                        description = "Sleet Showers";
                        break;
                    case 73:
                        icon = 6;
                        description = "Heavy Sleet Showers";
                        break;
                    case 77:
                        icon = 4;
                        description = "Thundershowers";
                        break;
                    case 81:
                        icon = 5;
                        description = "Light Sleet";
                        break;
                    case 82:
                        icon = 5;
                        description = "Sleet";
                        break;
                    case 83:
                        icon = 5;
                        description = "Heavy Sleet";
                        break;
                    case 101:
                        icon = 31;
                        description = "Clear";
                        break;
                    case 102:
                        icon = 29;
                        description = "Partly Cloudy";
                        break;
                    case 103:
                        icon = 26;
                        description = "Cloudy";
                        break;
                    case 104:
                        icon = 29;
                        description = "Partly Cloudy Night";
                        break;
                    case 106:
                        icon = 27;
                        description = "Mostly Cloudy Night";
                        break;
                    case 107:
                        icon = 26;
                        description = "Overcast";
                        break;
                    case 109:
                        icon = 20;
                        description = "Fog";
                        break;
                    case 111:
                        icon = 9;
                        description = "Drizzle";
                        break;
                    case 114:
                        icon = 8;
                        description = "Freezing Drizzle";
                        break;
                    case 117:
                        icon = 10;
                        description = "Freezing Rain";
                        break;
                    case 121:
                        icon = 45;
                        description = "Light Rain Showers";
                        break;
                    case 122:
                        icon = 45;
                        description = "Rain Showers";
                        break;
                    case 123:
                        icon = 11;
                        description = "Heavy Rain Showers";
                        break;
                    case 124:
                        icon = 45;
                        description = "Scattered Showers";
                        break;
                    case 127:
                        icon = 11;
                        description = "Showers";
                        break;
                    case 131:
                        icon = 11;
                        description = "Light Rain";
                        break;
                    case 132:
                        icon = 12;
                        description = "Rain";
                        break;
                    case 133:
                        icon = 12;
                        description = "Heavy Rain";
                        break;
                    case 134:
                        icon = 45;
                        description = "Mostly Cloudy And Periods Of Light Rain";
                        break;
                    case 135:
                        icon = 45;
                        description = "Mostly Cloudy And Periods Of Moderate Rain";
                        break;
                    case 137:
                        icon = 11;
                        description = "Light Rain";
                        break;
                    case 138:
                        icon = 11;
                        description = "Moderate Rain";
                        break;
                    case 141:
                        icon = 41;
                        description = "Light Snow Showers";
                        break;
                    case 142:
                        icon = 41;
                        description = "Snow Showers";
                        break;
                    case 143:
                        icon = 41;
                        description = "Heavy Snow Showers";
                        break;
                    case 144:
                        icon = 5;
                        description = "Scattered Light Sleet Showers";
                        break;
                    case 147:
                        icon = 5;
                        description = "Light Sleet";
                        break;
                    case 148:
                        icon = 6;
                        description = "Moderate Sleet";
                        break;
                    case 151:
                        icon = 14;
                        description = "Light Snowfall";
                        break;
                    case 152:
                        icon = 14;
                        description = "Snowfall";
                        break;
                    case 153:
                        icon = 42;
                        description = "Heavy Snowfall";
                        break;
                    case 154:
                        icon = 46;
                        description = "Scattered Light Snow Showers";
                        break;
                    case 155:
                        icon = 46;
                        description = "Scattered Moderate Snow Showers";
                        break;
                    case 157:
                        icon = 14;
                        description = "Light Snowfall";
                        break;
                    case 158:
                        icon = 14;
                        description = "Moderate Snowfall";
                        break;
                    case 159:
                        icon = 42;
                        description = "Heavy Snowfall";
                        break;
                    case 161:
                        icon = 47;
                        description = "Thundershowers";
                        break;
                    case 162:
                        icon = 47;
                        description = "Strong Thundershowers";
                        break;
                    case 163:
                        icon = 17;
                        description = "Thunder";
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
                    case 172:
                        icon = 6;
                        description = "Sleet Showers";
                        break;
                    case 173:
                        icon = 6;
                        description = "Heavy Sleet Showers";
                        break;
                    case 177:
                        icon = 17;
                        description = "Thundershowers";
                        break;
                    case 181:
                        icon = 5;
                        description = "Light Sleet";
                        break;
                    case 182:
                        icon = 5;
                        description = "Sleet";
                        break;
                    case 183:
                        icon = 5;
                        description = "Heavy Sleet";
                        break;


                }
                if (icon ==48){
                	std::cerr<<"icon "<<icon<<" - ";
			        std::cerr<<SmartSymbol<<" "<< description<<std::endl;
		        }
                if (i==0){
                    current_description = description;
                    current_icon = icon;
                }
                fprintf(file_out,"     <icon>%i</icon>\n", icon);
                fprintf(file_out, "     <description>%s</description>\n", description.c_str());
            }    
            if (val[i].get("PoP","").asString() != ""){
                fprintf(file_out,"     <ppcp>%i</ppcp>\n", atoi(val[i].get("PoP","").asCString()));
            }    
            if (val[i].get("HourlyMaximumGust","").asString() != ""){
                fprintf(file_out,"     <wind_gust>%i</wind_gust>\n", atoi(val[i].get("HourlyMaximumGust","").asCString()));
            }    
            if (val[i].get("WindSpeedMS","").asString() != ""){
                fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", atoi(val[i].get("WindSpeedMS","").asCString()));
            }    
            if (val[i].get("WindDirection","").asString() != ""){
                float _wind_direction = INT_MAX;
                std::string _wind_direction_ = "";
                _wind_direction_ = val[i].get("WindDirection","").asCString();
                if (dictionary.find(_wind_direction_) != dictionary.end()) {
                    _wind_direction_ = dictionary[_wind_direction_];
                    //std::erase(_wind_direction_, '"');
    	   	    _wind_direction_.erase(std::remove(_wind_direction_.begin(), _wind_direction_.end(), '"'), _wind_direction_.end());
                }

                //std::cout<<"Wind "<<_wind_direction_<<std::endl;

                _wind_direction = atof(_wind_direction_.c_str());
                wind_index = (int)round(_wind_direction/22.5) + 1;
                //std::cout<<"Wind_index "<<wind_index<<std::endl;
                if (wind_index > 16){
                    wind_index = 16;
                }
                //std::cout<<"Wind_direction "<<wind_directions[wind_index].c_str()<<std::endl;
 
                fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",wind_directions[wind_index].c_str());
            }    
            if (val[i].get("Precipitation1h","").asString() != ""){
                fprintf(file_out,"     <precipitation>%.1f</precipitation>\n", atof(val[i].get("Precipitation1h","").asCString()));
            }    
            if (val[i].get("FeelsLike","").asString() != ""){
                fprintf(file_out,"     <flike>%.0f</flike>\n", atof(val[i].get("FeelsLike","").asCString()));
            }    

            fprintf(file_out, "    </period>\n");
            if (first_day){
                if (afternoon){
                    fprintf(file_out,"    <period start=\"%li\" ", utc_time + 3600*localtimezone - 3600*timezone - 3600 - 15*3600);
                    fprintf(file_out," end=\"%li\">\n", utc_time + offset_time + 3*3600 + 3600*localtimezone - 3600*timezone + 5*3600); 
                }else{    
                    fprintf(file_out,"    <period start=\"%li\" ", utc_time + 3600*localtimezone - 3600*timezone - 3600) ;
                    fprintf(file_out," end=\"%li\">\n", utc_time + 3600*localtimezone + 3*3600 + offset_time - 3600*timezone + 5*3600);
                }

                if (val[i].get("Temperature","").asString() != "" || val[i].get("Temperature","").asString() != "nan"){
                    if (std::isdigit(val[i].get("Temperature","").asString()[0])){
                        fprintf(file_out,"     <temperature>%.0f</temperature>\n", atof(val[i].get("Temperature","").asCString()));
                    }else{
                        if (dictionary.find(val[i].get("Temperature","").asString()) != dictionary.end()) {
                            fprintf(file_out,"     <temperature>%.0f</temperature>\n", atof(dictionary[val[i].get("Temperature","").asString()].c_str()));
                        }
                    }
                }    
                fprintf(file_out,"     <icon>%i</icon>\n", icon);
                fprintf(file_out, "     <description>%s</description>\n", description.c_str());
                fprintf(file_out, "    </period>\n");
            }
            if (i==0){
                time_t _current_time_ = utc_time + 3600*localtimezone - 3600*timezone - 3600;
                struct tm *lt = localtime(&_current_time_);
                lt->tm_hour = 0;
                lt->tm_min = 0;
                lt->tm_sec = 0;
                time_t midnight = mktime(lt) + 4*3600;
                time_t midnight_tomorrow = midnight + 24*3600 + 4*3600;

                lt->tm_hour = atoi(sunrise_time.substr(0, sunrise_time.find(":", 0)).c_str());
                lt->tm_min = atoi(sunrise_time.substr(sunrise_time.find(":", 0) + 1, sunrise_time.size()).c_str());
                time_t sunrise = mktime(lt);
                lt->tm_hour = atoi(sunset_time.substr(0, sunset_time.find(":", 0)).c_str());
                lt->tm_min = atoi(sunset_time.substr(sunset_time.find(":", 0) + 1, sunset_time.size()).c_str());
                time_t sunset = mktime(lt);

                fprintf(file_out,"    <period start=\"%li\" ", midnight);
                fprintf(file_out," end=\"%li\">\n", midnight_tomorrow); 

                fprintf(file_out,"      <sunrise>%li</sunrise>\n", sunrise); 
                fprintf(file_out,"      <sunset>%li</sunset>\n", sunset); 
                fprintf(file_out,"    </period>\n");
            }  

            first_day = false;
            afternoon = false;
        }
    }
#if 0
    /* Sun info */
    val = root["suninfo"].get(root["observations"].getMemberNames()[0], nullval);
    /* std::cerr<<val<<std::endl; */
    time_t sunrise_time = 0;
    time_t sunset_time = 0;
    time_t day_begin = 0;
    
    std::string sun_time;
    if (val.get("sunrise","").asString() != ""){
        setlocale(LC_TIME, "POSIX");
        sun_time = val.get("sunrise","").asString();
        tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
        tmp_tm.tm_isdst = time_tm2.tm_isdst;
        strptime((const char*)sun_time.c_str(), "%Y%m%dT%H%M%S", &tmp_tm);
        sunrise_time = mktime(&tmp_tm); 
        tmp_tm.tm_hour = 0; tmp_tm.tm_min = 0; tmp_tm.tm_sec = 0;
        day_begin = mktime(&tmp_tm); 
        setlocale(LC_TIME, "");
    }    
    if (val.get("sunset","").asString() != ""){
        sun_time = val.get("sunset","").asString();
        setlocale(LC_TIME, "POSIX");
        tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
        tmp_tm.tm_isdst = time_tm2.tm_isdst;
        strptime((const char*)sun_time.c_str(), "%Y%m%dT%H%M%S", &tmp_tm);
        sunset_time = mktime(&tmp_tm); 
        setlocale(LC_TIME, "");
    }    
    if ((sunrise_time > 0) && (sunset_time > 0)){
        fprintf(file_out,"    <period start=\"%li\"", day_begin);
        fprintf(file_out," end=\"%li\">\n", day_begin +3600*24 -1);
        fprintf(file_out,"     <sunrise>%li</sunrise>\n", sunrise_time);
        fprintf(file_out,"     <sunset>%li</sunset>\n", sunset_time);
        fprintf(file_out,"    </period>\n");
    }
#endif
    std::ifstream jsonfile(current_data_path, std::ifstream::binary);
    bool parsingCurrentSuccessful = reader.parse(jsonfile, root, false);
    if (!parsingCurrentSuccessful){
        std::cerr<<"Problem in parsingCurrentSuccessful";
        return -1;
    }

    val = root["observations"];
    if (val.size() > 1){
        int index = val.size() -1;
        if (val[index].get("WindSpeedMS","").asString() == "" &&
            val[index].get("Humidity","").asString() == "" &&
            val[index].get("Pressure","").asString() == "" &&
            val[index].get("Precipitation1h","").asString() == ""){
            if (index > 0){
                std::cerr<<"Decrement index"<<std::endl;
                index --;
            }
        }
        std::string _local_time_string;
        _local_time_string = val[index].get("localtime","").asCString();
        if (_local_time_string != ""){
            tmp_tm = {0,0,0,0,0,0,0,0,0,0,0};
            tmp_tm.tm_isdst = time_tm2.tm_isdst;
            setlocale(LC_TIME, "POSIX");
            strptime((const char*)_local_time_string.c_str(), "%Y%m%dT%H%M%S", &tmp_tm);
            utc_time = mktime(&tmp_tm); 
            setlocale(LC_TIME, "");

            fprintf(file_out,"    <period start=\"%li\"", utc_time + 3600*localtimezone - 3600*timezone);
            fprintf(file_out," current=\"true\" ");
            fprintf(file_out," end=\"%li\">\n", utc_time + 3600*localtimezone + 2*3600 - 3600*timezone); 
            fprintf(file_out,"     <icon>%i</icon>\n", current_icon);
            fprintf(file_out,"     <description>%s</description>\n", current_description.c_str());

            if (val[index].get("WindSpeedMS","").asString() != ""){
                if (val[index].get("WindSpeedMS","").asString() == ""){
                    current_wind_speed = INT_MAX;
                }else{
                    current_wind_speed =(int)round(val[index].get("WindSpeedMS","").asDouble());
               }
            }    
            if (current_wind_speed != INT_MAX){
                fprintf(file_out,"     <wind_speed>%i</wind_speed>\n", current_wind_speed);
            }
            if (val[index].get("DewPoint","").asString() != ""){
                if (val[index].get("DewPoint","").asString() == ""){
                    current_dewpoint = INT_MAX;
                }else{
                    current_dewpoint =(int)round(val[index].get("DewPoint","").asDouble());
               }
            }    
            if (current_dewpoint != INT_MAX){
                fprintf(file_out,"     <dewpoint>%i</dewpoint>\n", current_dewpoint);
            }
            if (val[index].get("Humidity","").asString() != ""){
                if (val[index].get("Humidity","").asString() == ""){
                    current_humidity = INT_MAX;
                }else{
                    current_humidity =(int)round(val[index].get("Humidity","").asDouble());
               }
            }    
            if (current_humidity != INT_MAX){
                fprintf(file_out,"     <humidity>%i</humidity>\n", current_humidity);
            }

            if (val[index].get("Precipitation1h","").asString() != ""){
                if (val[index].get("Precipitation1h","").asString() == ""){
                    current_precipitation = INT_MAX;
                }else{
                    current_precipitation =(int)round(val[index].get("Precipitation1h","").asDouble());
               }
            }    
            if (current_precipitation != INT_MAX){
                fprintf(file_out,"     <precipitation>%.1f</precipitation>\n", current_precipitation);
            }

            if (val[index].get("Pressure","").asString() != ""){
                if (val[index].get("Pressure","").asString() == ""){
                    current_pressure = INT_MAX;
                }else{
                    current_pressure =(int)round(val[index].get("Pressure","").asDouble());
               }
            }    
            if (current_pressure != INT_MAX){
                fprintf(file_out,"     <pressure>%i</pressure>\n", current_pressure);
            }

            if (val[index].get("Visibility","").asString() != ""){
                if (val[index].get("Visibility","").asString() == ""){
                    current_visibility = INT_MAX;
                }else{
                    current_visibility =(int)round(val[index].get("Visibility","").asInt());
               }
            }    
            if (current_visibility != INT_MAX){
                fprintf(file_out,"     <visible>%i</visible>\n", current_visibility);
            }

            if (val[index].get("WindDirection","").asString() != ""){
                if (val[index].get("WindDirection","").asString() == ""){
                    current_wind_direction = "";
                }else{
                    int _current_wind_direction =(int)round(val[index].get("WindDirection","").asInt());
                    wind_index = (int)round(_current_wind_direction/22.5) + 1;
                    //std::cout<<"Wind_index "<<wind_index<<std::endl;
                    if (wind_index > 16){
                        wind_index = 16;
                    }
                    //std::cout<<"Wind_direction "<<wind_directions[wind_index].c_str()<<std::endl;
                    fprintf(file_out,"     <wind_direction>%s</wind_direction>\n",wind_directions[wind_index].c_str());

               }
            }    
            if (val[index].get("WindGust","").asString() != ""){
                if (val[index].get("WindGust","").asString() == ""){
                    current_wind_gust = INT_MAX;
                }else{
                    current_wind_gust =(int)round(val[index].get("WindGust","").asDouble());
               }
            }    
            if (current_wind_gust != INT_MAX){
                fprintf(file_out,"     <wind_gust>%i</wind_gust>\n", current_wind_gust);
            }
            if (val[index].get("t2m","").asString() != ""){
                if (val[index].get("t2m","").asString() == ""){
                    current_temperature = INT_MAX;
                }else{
                    current_temperature =(int)round(val[index].get("t2m","").asDouble());
               }
            }    
            if (current_temperature != INT_MAX){
                fprintf(file_out,"     <temperature>%i</temperature>\n", current_temperature);
            }
            fprintf(file_out, "    </period>\n");
        }
    }

    fclose(file_out);
    setlocale(LC_NUMERIC, "");
    count_day=1;
    return count_day;
}


/*******************************************************************************/
int
convert_station_fmi_fi_data(const char *days_data_path, const char *result_file, const char *current_data_path){
 
    int     days_number = -1;
    FILE    *file_out;
    
    if(!days_data_path)
        return -1;

    /* check file accessability */
    if(!access(days_data_path, R_OK)){
        days_number = parse_and_write_days_xml_data(days_data_path, result_file, current_data_path);
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
main_fmi_fi(int argc, char *argv[]){
    int result = -1; 
    if (argc < 2) {
        fprintf(stderr, "fmifi <input_days_file> <output_file> \n");
        return -1;
    }
    if (argc == 3) 
    	result = convert_station_fmi_fi_data(argv[1], argv[2], "");
    if (argc == 4)
    	result = convert_station_fmi_fi_data(argv[1], argv[2], argv[3]);
    //fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
