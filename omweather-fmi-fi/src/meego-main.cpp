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

static xmlHashTablePtr hash_for_icons;
#define buff_size 2048

int station_timezone = 0;
/*******************************************************************************/

int
parse_and_write_days_xml_data(htmlDocPtr doc, const char *result_file){
    int count_day;
    return count_day;
}


/*******************************************************************************/
int
convert_station_fmi_fi_data(const char *days_data_path, const char *result_file){
 
    xmlDoc  *doc = NULL;
    xmlNode *root_node = NULL;
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
        /* check that the file containe valid data */
        doc =  xmlReadFile(days_data_path, "UTF-8", 0);
        if(!doc){
            xmlHashFree(hash_for_icons, NULL);
            return -1;
        }
        root_node = xmlDocGetRootElement(doc);
        if(root_node && root_node->type == XML_ELEMENT_NODE &&
                strstr((char*)root_node->name, "err")){
            xmlFreeDoc(doc);
            xmlCleanupParser();
            xmlHashFree(hash_for_icons, NULL);
            return -2;
        }else{
            days_number = parse_and_write_days_xml_data(doc, result_file);
            xmlFreeDoc(doc);
            xmlCleanupParser();
            if (days_number > 0){
            
                file_out = fopen(result_file, "a");
                if (file_out){
                    fprintf(file_out,"</station>");
                    fclose(file_out);
                }
            }
        }
    }else{
        xmlHashFree(hash_for_icons, NULL);
        return -1;/* file isn't accessability */
    }

    xmlHashFree(hash_for_icons, NULL);
    return days_number;
}

/*******************************************************************************/
int
main(int argc, char *argv[]){
    int result; 
    if (argc < 4) {
        fprintf(stderr, "fmifi <input_days_file> <output_file> <input_hours_file> <input_current_file>\n");
        return -1;
    }
    result = convert_station_fmi_fi_data(argv[1], argv[2]);
    //fprintf(stderr, "\nresult = %d\n", result);
    return result;
}
