/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2010-2011 Tanya Makova
 * 	for the code
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

#include "meego-netbookUX-omweather.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#ifdef LOCALDEBUG
    #define CONFIG_FILE "config.xml"
    #define CONFIG_XSD_PATH "../core/data/config.xsd"
#else
//    #define CONFIG_PATH "~/.config/omweather/config.xml"
    #define CONFIG_FILE "config.xml"
    #define CONFIG_XSD_PATH "/usr/share/omweather/xsd/config.xsd"
    #define DATA_XSD_PATH "/usr/share/omweather/xsd/data.xsd"
#endif

/*******************************************************************************/
GHashTable *hash_table_create(void) {
    GHashTable *hash;
    hash = g_hash_table_new(g_str_hash, g_str_equal);
/*
 * WARNING!
 * Do not insert new lines to this file, use file hash.data
 * To add new reserved word from data (xml) file you can use
 * script get_reserved_word.pl like this:
 * cat BOXX0014.xml USCA0001.xml > |./get_reserved_word.pl
*/
#include "hash.data"
    return hash;
}

void
change_path(ClutterPath *path, gint need_size)
{
    ClutterKnot knot;
    int i;
    ClutterPathNode *node;
    GSList  *path_list;
    i = clutter_path_get_n_nodes((path));
    for (path_list = clutter_path_get_nodes(path); path_list != NULL; path_list = path_list->next){
        node = (ClutterPathNode *)path_list->data;
        knot = node->points[0];
        node->points[0].x = (((need_size*100)/128) * knot.x/100);
        node->points[0].y = (((need_size*100)/128) * knot.y/100);
        
        knot = node->points[1];
        node->points[1].x = (((need_size*100)/128) * knot.x/100);
        node->points[1].y = (((need_size*100)/128) * knot.y/100);
        knot = node->points[2];
        node->points[2].x = (((need_size*100)/128) * knot.x/100);
        node->points[2].y = (((need_size*100)/128) * knot.y/100);
    
    }
} 
//////////////////////////////////////////////////////////////////////////////
void
change_actor_size_and_position(ClutterActor *actor, gint need_size)
{
    guint h,w;
    gint x,y;
    if (!actor)
        return;
    w = clutter_actor_get_width(actor);
    h = clutter_actor_get_height(actor);
    x = clutter_actor_get_x(actor);
    y = clutter_actor_get_y(actor);
    clutter_actor_set_width(actor,(((need_size*100)/128) * w/100)); /* 128 must be 128 */ 
    clutter_actor_set_height(actor,(((need_size*100)/128) * h/100)); /* 128 must be 128 */ 
    clutter_actor_set_x(actor,(((need_size*100)/128) * x/100)); /* 128 must be 128 */ 
    clutter_actor_set_y(actor,(((need_size*100)/128) * y/100)); /* 128 must be 128 */ 
    
} 

Core::Config *
create_and_fill_config(){
    Core::Config *config;
    std::cerr<<"Create Config class: " << Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd"<< std::endl;
    try{
        config = new Core::Config(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
        std::cerr << config->stationsList().size() << std::endl;
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config = new Core::Config();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config = new Core::Config();
    }
    //std::cerr<<"End of creating Config class: " <<std::endl;
    config->saveConfig();
    std::cerr<<"End of creating Config class: " <<std::endl;

    return config;
}

Core::DataParser*
current_data(std::string& str){
  Core::DataParser* dp;
  try{
        dp = new Core::DataParser(str, DATA_XSD_PATH);
    }
    catch(const std::string &str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return NULL;
    }
    catch(const char *str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return NULL;
    }
    return dp;
}

void
init_omweather_core(void){
    config = create_and_fill_config();
}



//////////////////////////////////////////////////////////////////////////////
static void
on_new_frame (ClutterTimeline *timeline,
          gint             elapsed_msecs,
          ClutterActor    *actor)
{
  gdouble angle = 360 * clutter_timeline_get_progress (timeline);

  clutter_actor_set_rotation (actor, CLUTTER_Z_AXIS,
                              angle,
                              clutter_actor_get_width (actor) / 2,
                  clutter_actor_get_height (actor) / 2,
                              0);
}

ClutterTimeline*
create_update_animation(ClutterActor *actor)
{
  ClutterTimeline *timeline;

  timeline = clutter_timeline_new (1000); /* one second */
  clutter_timeline_set_loop (timeline, TRUE);
  g_signal_connect (timeline, "new-frame", G_CALLBACK (on_new_frame), actor);
  return timeline;
}
//////////////////////////////////////////////////////////////////////////////
/*
static void*
//update_weather_forecast(Core::Config *config){
update_weather_forecast(void* data){
    int i;
    int success = 0;
    Core::Station* station;
    for (i=0; i < config->stationsList().size();i++){
        station = config->stationsList().at(i);
        if (station->updateData(true))
            success ++;
    }
    //return success;
}*/
