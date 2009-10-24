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
#include "omweather-sources.hpp"
#include <fstream>
#include <dlfcn.h>
/*******************************************************************************/
Source::Source(const std::string filename){
    source_is_invalid = true;
    std::ifstream    file(filename.c_str());
    if(file.is_open()){
        file.close();
        xmlDoc *document = xmlReadFile(filename.c_str(), NULL, 0);
        if(document){
            xmlNode *root_node = xmlDocGetRootElement(document);
            xmlNode *current_node = root_node->children;
            parse_children(current_node);
            xmlFreeDoc(document);
            if(!source_name.empty() && !source_forecast_url.empty() &&
                    !source_detail_url.empty() && load_library())
                source_is_invalid = false;
        }
    }
}
/*******************************************************************************/
void Source::parse_children(xmlNode *node){
    xmlChar     *value = NULL;

    while(node){
        if(node->type == XML_ELEMENT_NODE){
            /* name */
            if(!xmlStrcmp(node->name, (const xmlChar*)"name")){
                value = xmlNodeGetContent(node);
                source_name = (char*)value;
                xmlFree(value);
                node = node->next;
                continue;
            }
            /* logo */
            if(!xmlStrcmp(node->name, (const xmlChar*)"logo")){
                value = xmlNodeGetContent(node);
                source_logo = (char*)value;
                xmlFree(value);
                node = node->next;
                continue;
            }
            /* forecast_url */
            if(!xmlStrcmp(node->name, (const xmlChar*)"forecast")){
                value = xmlGetProp(node, (const xmlChar*)"url");
                source_forecast_url = (char*)value;
                xmlFree(value);
                node = node->next;
                continue;
            }
            /* detail_url */
            if(!xmlStrcmp(node->name, (const xmlChar*)"detail")){
                value = xmlGetProp(node, (const xmlChar*)"url");
                source_detail_url = (char*)value;
                xmlFree(value);
                node = node->next;
                continue;
            }
            /* search_url */
            if(!xmlStrcmp(node->name, (const xmlChar*)"search")){
                value = xmlGetProp(node, (const xmlChar*)"url");
                source_search_url = (char*)value;
                xmlFree(value);
                node = node->next;
                continue;
            }
            /* stations_db */
            if(!xmlStrcmp(node->name, (const xmlChar*)"base")){
                value = xmlNodeGetContent(node);
                source_database_name = (char*)value;
                xmlFree(value);
                node = node->next;
                continue;
            }
            /* library */
            if(!xmlStrcmp(node->name, (const xmlChar*)"library")){
                value = xmlNodeGetContent(node);
                source_library_name = (char*)value;
                xmlFree(value);
                node = node->next;
                continue;
            }
        }
        node = node->next;
    }
}
/*******************************************************************************/
bool Source::load_library(){
    std::string name = SOURCES_LIB;
    name += source_library_name;

    std::fstream file(name.c_str());
    if(file.is_open()){
        file.close();
        void* handle = dlopen(name.c_str(), RTLD_NOW);
        if(handle){
            dlerror();
            void* parser = dlsym(handle, "get_station_weather_data");
            if(!dlerror()){
                source_parser = parser;
                source_handle = handle;
                return true;
            }
        }
    }
    return false;
}
/*******************************************************************************/
Source::~Source(){
    if(source_handle)
        dlclose(source_handle);
}
/*******************************************************************************/
bool Source::check_database(){
    std::string name = DATABASEPATH;
    name += source_database_name;

    std::fstream file(name.c_str());
    if(file.is_open()){
        file.close();
        return true;
    }
    return false;
}
/*******************************************************************************/
bool Source::is_valid() const{
    return source_is_invalid;
}
/*******************************************************************************/
SourcesList::SourcesList(){
    list.clear();
    std::string name = SOURCESPATH;
    DIR *dir_fd = opendir(name.c_str());
    if(dir_fd){
        Dirent *dp;
        while((dp = readdir(dir_fd))){
            if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
                continue;
            if(dp->d_type == DT_REG){
                std::string source_file_description = name + dp->d_name;
                list.push_back(Source(source_file_description));
            }
        }
        closedir(dir_fd);
    }
}
/*******************************************************************************/
int SourcesList::size() const{
    return list.size();
}
/*******************************************************************************/
