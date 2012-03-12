/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen
 *
 * Copyright (C) 2012 Vlad Vasilyeu
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
#include "main.h"
/*******************************************************************************/

/* Global section */
FILE *file;
#define WIDTH  (480)
#define HEIGHT (800)

#define DATA_XSD_PATH "/opt/com.meecast.omweather/share/xsd/data.xsd"

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
        std::cerr << "count station:" << config->stationsList().size() << std::endl;
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config = new Core::Config();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config = new Core::Config();
    }
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

static void
_on_delete(Ecore_Evas *ee)
{
   ecore_main_loop_quit();
}

int
main(void)
{
    int x, y, w, h;
    int i = 0, success = 0;
    struct _App app;

    app.dp = NULL;

    if (!ecore_evas_init())
        return EXIT_FAILURE;

    if (!edje_init())
        return EXIT_FAILURE;

    /* this will give you a window with an Evas canvas under the first
     * engine available */
    app.ee = ecore_evas_new(NULL, 0, 0, WIDTH, HEIGHT, NULL);
    if (!app.ee)
      goto error;

    ecore_evas_callback_delete_request_set(app.ee, _on_delete);
    ecore_evas_title_set(app.ee, "MeeCast");

    app.config = create_and_fill_config();
    /* Check time for previous updating */
    app.dp = current_data(app.config->stationsList().at(app.config->current_station_id())->fileName());

    /* 25*60 = 30 minutes - minimal time between updates */ 
    if (app.dp && (abs(time(NULL) - app.dp->LastUpdate()) > 25*60)){
        /*update weather forecast*/
        for (i=0; i < app.config->stationsList().size();i++){
            if (app.config->stationsList().at(i)->updateData(true)){
                success ++;
            }
        }
    }

   create_main_window(&app);
   ecore_evas_show(app.ee);

   ecore_main_loop_begin();

   ecore_evas_free(app.ee);
   ecore_evas_shutdown();
   edje_shutdown();
   return 0;

error:
   fprintf(stderr, "You got to have at least one evas engine built"
                   " and linked up to ecore-evas for this example to run"
                   " properly.\n");
   ecore_evas_shutdown();
   return -1;

error_edj:
   fprintf(stderr, "Failed to load basic.edj!\n");

   ecore_evas_shutdown();
   return -2;
}
#if 0
EAPI_MAIN int
elm_main(int argc, char **argv)
{
  
    create_main_window(config, dp);

    // run the mainloop and process events and callbacks
    elm_run();
    elm_shutdown();
    return 0;
}
ELM_MAIN()
#endif
#if 0
int
main(int argc, char *argv[]){
    int result = 0; 
    int i, success;
    Core::DataParser* dp = NULL;
    Core::Data *temp_data = NULL;
    config = create_and_fill_config();
    fprintf(stderr, "\nresult = %d\n", result);
    /* Check time for previous updating */
    dp = current_data(config->stationsList().at(config->current_station_id())->fileName());

    /* 25*60 = 30 minutes - minimal time between updates */ 
    if (dp && (abs(time(NULL) - dp->LastUpdate()) > 25*60)){
        /*update weather forecast*/
        for (i=0; i < config->stationsList().size();i++){
            if (config->stationsList().at(i)->updateData(true)){
                success ++;
            }
        }
    }

    if (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL)))) {
        std::cerr<<"Temperature "<<  temp_data->temperature_hi().value(true)<<std::endl;
    }
    config->saveConfig();
    return result;
}
#endif
