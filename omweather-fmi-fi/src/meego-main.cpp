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

int station_timezone = 0;
/*******************************************************************************/

int
parse_and_write_days_xml_data(const char *days_data_path, const char *result_file){
    int count_day = -1;
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    FILE   *file_out;
    char buffer  [4096],
         buffer2 [4096],
         *delimiter = NULL;

    std::ifstream jsonfile(days_data_path, std::ifstream::binary);
    bool parsingSuccessful = reader.parse(jsonfile, root, false);
    fprintf(stderr, "Result %i\n", parsingSuccessful);
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
    fprintf(file_out,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<station name=\"Station name\" id=\"%s\" xmlns=\"http://omweather.garage.maemo.org/schemas\">\n", buffer);
    fprintf(file_out," <units>\n  <t>C</t>\n  <ws>m/s</ws>\n  <wg>m/s</wg>\n  <d>km</d>\n");
    fprintf(file_out,"  <h>%%</h>  \n  <p>mmHg</p>\n </units>\n");

    Json::Value nullval ;
    std::string sss = root["observations"].getMemberNames()[0];
    std::cerr<<sss<<std::endl;
    //std::cerr<<root["observations"].get(root["observations"].getMemberNames()[0], nullval)<<std::endl;
    Json::Value val = root["observations"].get(root["observations"].getMemberNames()[0], nullval);

    double min_distance = 32000;
    int max_count_of_parameters = 0;
    for (int i = 0; i < val.size(); i++){
        std::cerr<<"size "<<val[i].size()<<std::endl;
        if (atof(val[i].get("distance","").asCString()) < min_distance && (val[i].size()>max_count_of_parameters && atof(val[i].get("distance","").asCString()) - min_distance < 10)){
            std::cerr<<val[i].get("distance", nullval)<<std::endl;
            min_distance = atof(val[i].get("distance","").asCString());
            max_count_of_parameters = val[i].size();
        }
//        std::cerr<<val[i]<<std::endl;
//        std::cerr<<"qqqqqqqqqqqqq"<<std::endl;
    }
    //std::cout << root["observations"];

    fclose(file_out);

    count_day=1;
    return count_day;
}


/*******************************************************************************/
int
convert_station_fmi_fi_data(const char *days_data_path, const char *result_file){
 
    int     days_number = -1;
    char    buffer[2048],
            *delimiter = NULL;
    FILE    *file_out;
    
    if(!days_data_path)
        return -1;

    hash_for_icons = hash_icons_fmifi_table_create();
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
