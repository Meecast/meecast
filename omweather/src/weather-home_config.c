/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) Superkaramba's Liquid Weather ++ team
 *	for ALL the artwork (icons)
 *        The maintainer (also the main author I believe)
 *        is Matthew <dmbkiwi@yahoo.com>
 *  http://liquidweather.net/icons.html#iconsets
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
#include "weather-home_config.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
/*******************************************************************************/
/* Change the weather cache directory and update dependent variables. */
gboolean config_set_weather_dir_name(gchar *new_weather_dir_name){
    GnomeVFSURI *weather_dir_uri;
    gboolean retval = FALSE;
    GnomeVFSURI *parent, *curr_uri;
    GList *list = NULL;

    /* Rest of the function devoted to making sure the directory exists. */
    weather_dir_uri = gnome_vfs_uri_new(new_weather_dir_name);
    if(!gnome_vfs_uri_exists(weather_dir_uri)){
        list = g_list_prepend(list, curr_uri = weather_dir_uri);
        while(GNOME_VFS_ERROR_NOT_FOUND == gnome_vfs_make_directory_for_uri(
        	parent = gnome_vfs_uri_get_parent(curr_uri), 0755))
            list = g_list_prepend(list, curr_uri = parent);	
	while(list != NULL){
	    if( GNOME_VFS_OK != gnome_vfs_make_directory_for_uri((GnomeVFSURI*)list->data, 0755) ){
		retval = FALSE;
		break;
	    }
    	    list = g_list_remove(list, list->data);
    	}
	retval = TRUE;
    }
    else
	retval = TRUE;
    if(retval){
        if(app->config->cache_dir_name)
            g_free(app->config->cache_dir_name);
        app->config->cache_dir_name = new_weather_dir_name;
    }    
    gnome_vfs_uri_unref(weather_dir_uri);
    return retval;
}
/*******************************************************************************/
/* The stations data  fill from clock plugin data */
void fill_station_from_clock_plugin_data(void){
    FILE  *clock_file;  
    gchar *home_city = NULL;
    gchar *remote_city = NULL;
    gchar *tmp = NULL;
    char  out_buffer[1024]; /* buffer for work with city_in_clock.txt files*/
    struct weather_station *ws;
    
    GConfClient *gconf_client = gconf_client_get_default();
    /* City name from config file of clock */
    home_city = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_CLOCK_HOME_LOCATION, NULL);
    if(!home_city)
	return;
    remote_city = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_CLOCK_REMOTE_LOCATION, NULL);
    if(!remote_city)
	return;
    if((clock_file = fopen(CLOCK_FILE,"r")) != NULL){
	while(!feof(clock_file)){
	    memset(out_buffer, 0, sizeof(out_buffer));
	    fgets(out_buffer, sizeof(out_buffer) - 1, clock_file);
	    tmp = strchr(out_buffer, ';'); /* Separator changed from | to ; in 0.19 version */
	    if(!tmp)
		tmp = strchr(out_buffer, '|'); /* Finding old separator */
	    if(!tmp)
		continue;
	    if(!strncmp(out_buffer, home_city, tmp - out_buffer) || 
	        !strncmp(out_buffer, remote_city, tmp - out_buffer)){
		/* Prepare struct */
    		ws = g_new0(struct weather_station, 1);
		tmp[ strlen(tmp) - 1 ] = 0;
		ws->id_station = g_strdup(tmp + 1);
		tmp[0] = 0;
		ws->name_station = g_strdup(out_buffer);
		/* Add station to stations list */
		app->stations_view_list = g_slist_append(app->stations_view_list, ws); 
		/* A current station */
		if(!strncmp(out_buffer, home_city, tmp - out_buffer)){
		    app->config->current_station_id = g_strdup(ws->id_station);
		    app->config->current_station_name = g_strdup(ws->name_station);
		}
	    }	    
	}
	fclose(clock_file);
    }
    if(home_city)
	g_free(home_city);
    if(remote_city)
	g_free(remote_city);	
}
/*******************************************************************************/
/* New Reinitialize stations list */
void reinitilize_stations_list2(GSList *stlist){
    struct weather_station *ws = NULL;
    gchar *temp1 = NULL, *temp2 = NULL;
    app->stations_view_list = NULL; /* Initialize value */

    while(stlist != NULL){
	temp1 = strdup(stlist->data);
   /* Delimit Id and name */
	if(strlen(temp1) > 0){
	    temp2 = strtok(temp1,"@\0"); /* Delimiter between ID and name - @ */
	    if(temp2 != NULL)  /* Check random error */      
		if(strlen(temp2) > 0){
		    ws = g_new0(struct weather_station, 1);
		    ws->id_station = g_strdup(temp2); 
		}
	    temp2 = strtok(NULL,"@\0"); /* Delimiter between ID - @ */ 
	    if(temp2 != NULL)
		ws->name_station = g_strdup(temp2); 
	    app->stations_view_list = g_slist_append(app->stations_view_list, ws); /* Add station to stations list */
	}     
	g_free(stlist->data);      
	g_free(temp1);
	stlist = g_slist_next(stlist);
    }	    
}
/*******************************************************************************/
/* Prepare stations ID list  to write config file */
GSList* prepare_idlist(void){
    GSList *stlist = NULL;
    GSList *tmplist = NULL;
    struct weather_station *ws;
    char *str;

    tmplist = app->stations_view_list;
    while(tmplist != NULL){ 
	ws = tmplist->data;
	str = g_strdup_printf("%s@%s", ws->id_station, ws->name_station);
	stlist = g_slist_append(stlist, str);
	tmplist = g_slist_next(tmplist);
    }
    return stlist;
}
/*******************************************************************************/
GtkListStore* create_time_update_list(void){
    GtkListStore	*list = NULL;
    GtkTreeIter         iter;

    list = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
    
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("Never"), 1, 0, -1);
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("every 5 minutes"), 1, 5, -1);
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("every 30 minutes"), 1, 30, -1);
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("every hour"), 1, 60, -1);
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("every 2 hours"), 1, 120, -1);
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("every 4 hours"), 1, 240, -1);
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("every 8 hours"), 1, 480, -1);
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("every 24 hours"), 1, 1440, -1);
    gtk_list_store_append(list, &iter);
    gtk_list_store_set(list, &iter, 0, _("every minute (Debug)"), 1, 1, -1);

    return list;
}
/*******************************************************************************/
int new_read_config(AppletConfig *config){
    GConfValue	*value = NULL;
    GConfClient *gconf_client = NULL;
    int		fd = -1;
    GSList	*stlist = NULL;
    GError	*gerror = NULL;
    GdkColor	DEFAULT_FONT_COLOR = {0, 0xFF00, 0xFF00, 0x0000};
    gchar	tmp_buff[1024],
		*home_dir,
		*tmp = NULL;

    gconf_client = gconf_client_get_default();

    if(!gconf_client){
        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return 1;
    }
    /* Get Weather Cache Directory.  Default is "~/apps/omweather". */
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_DIR_NAME, NULL);
    if(!tmp){
	home_dir = getenv("HOME");
	if(!home_dir){
    	    fprintf(stderr, _("Can not get path to the HOME directory. Quitting.\n"));
    	    return 1;
	}
	memset(tmp_buff, 0, sizeof(tmp_buff));
	snprintf(tmp_buff, sizeof(tmp_buff) - 1, "%s%s",
		    home_dir, "/apps/omweather");
	g_free(home_dir);
    }
    else
	snprintf(tmp_buff, sizeof(tmp_buff) - 1, "%s", tmp);
    if(tmp)
	g_free(tmp);
    tmp = NULL;

    if(!config_set_weather_dir_name(gnome_vfs_expand_initial_tilde(tmp_buff)))
        fprintf(stderr, _("Could not create Weather Cache directory.\n"));
    /* Get Weather Station ID for current station */
    config->current_station_id = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_CURRENT_STATION_ID, NULL);
    /* Get Weather Stations ID and NAME */
    stlist = gconf_client_get_list(gconf_client,
        			    GCONF_KEY_WEATHER_STATIONS_LIST,
				    GCONF_VALUE_STRING, NULL);
    if(stlist){
	reinitilize_stations_list2(stlist);
	g_slist_free(stlist);
    }
    /* Get icon set name */ 
    config->icon_set = gconf_client_get_string(gconf_client,
					    GCONF_KEY_WEATHER_ICON_SET,
					    NULL);
    sprintf(path_large_icon, "%s%s/", ICONS_PATH, config->icon_set);
    if( ( fd = open(path_large_icon, O_RDONLY) ) == -1 ){
	memset(path_large_icon, 0, sizeof(path_large_icon));
	config->icon_set = g_strdup("Crystal");
	sprintf(path_large_icon, "%s%s/", ICONS_PATH, config->icon_set);
    }
    else
	close(fd);
    /* Get Weather Icon Size  */		     
    config->icons_size = gconf_client_get_int(gconf_client,
        				    GCONF_KEY_WEATHER_ICONS_SIZE,
					    NULL);
    if(config->icons_size < LARGE || config->icons_size > SMALL)
        config->icons_size = LARGE;

    /* Get setting tab number  */		     
    config->current_settings_page = gconf_client_get_int(gconf_client,
        				    GCONF_KEY_WEATHER_SETTING_TAB_NUMBER,
					    NULL);
    if(config->current_settings_page < 0 ||
	config->current_settings_page > MAX_SETTINGS_PAGE_NUMBER)
        config->current_settings_page = 0;

    /* Get Weather country name. */    
    config->current_country = gconf_client_get_string(gconf_client,
        					    GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME,
						    NULL);
    /* Get Weather station name. */
    config->current_station_name = gconf_client_get_string(gconf_client,
    							GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
							NULL);
    /* Get Weather periodic update time. */
    config->update_interval = gconf_client_get_int(gconf_client,
        			    GCONF_KEY_WEATHER_UPDATE_INTERVAL,
				    NULL);
    if(config->update_interval < 0 || config->update_interval > 24 * 60 ) /* No more than 24 hours */
	config->update_interval = 0;
    remove_periodic_event();		/* delete event from list */
    add_periodic_event(time(NULL));	/* add new event */

    /* Get Weather font color. */    	
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_FONT_COLOR, NULL);
    if(!tmp || !gdk_color_parse(tmp, &(config->font_color)))
         config->font_color = DEFAULT_FONT_COLOR; 
    g_free(tmp);
    tmp = NULL;

    /* Get background color. */    	
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_BACKGROUND_COLOR, NULL);
    if(!tmp || !gdk_color_parse(tmp, &(config->background_color)))
         config->background_color = DEFAULT_FONT_COLOR; 
    g_free(tmp);
    tmp = NULL;

    /* Get Enable Transparency flag. Default is TRUE. */
    value = gconf_client_get(gconf_client, GCONF_KEY_ENABLE_TRANSPARENCY, NULL);
    if(value){
        config->transparency = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->transparency = TRUE;
    /* Get Split Button State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SEPARATE_DATA, NULL);
    if(value){
        config->separate = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->separate = FALSE;

    /* Get auto_downloading_after_connecting. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_DOWNLOADING_AFTER_CONNECTING, NULL);
    if(value){
        config->downloading_after_connecting = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->downloading_after_connecting = FALSE;

    
    /* Get Swap Temperature Button State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SWAP_TEMPERATURE, NULL);
    if(value){
        config->swap_hi_low_temperature = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->swap_hi_low_temperature = FALSE;

    /* Get Hide Station State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_HIDE_STATION_NAME, NULL);
    if(value){
        config->hide_station_name = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->hide_station_name = FALSE;

    /* Get Temperature Unit  Default Celsius */
    config->temperature_units = gconf_client_get_int(gconf_client,
                    			GCONF_KEY_WEATHER_TEMPERATURE_UNIT, &gerror);
    (config->temperature_units) ? (config->temperature_units = FAHRENHEIT)
				: (config->temperature_units = CELSIUS);
    if(gerror){
    	g_error_free(gerror);
	gerror = NULL;
    }

    /* Get Layout  Default Horizontal */
    config->icons_layout = gconf_client_get_int(gconf_client,
                				GCONF_KEY_ICONS_LAYOUT,
						NULL);
    if(config->icons_layout < ONE_ROW ||
	config->icons_layout > TWO_COLUMNS)
	config->icons_layout = ONE_ROW;

    /* Get number days to show default 5 */
    config->days_to_show = gconf_client_get_int(gconf_client,                                                                                     
                				GCONF_KEY_WEATHER_DAYS,
						&gerror);
    if(gerror || config->days_to_show <= 0
	      || config->days_to_show > Max_count_weather_day){
	config->days_to_show = 5;
	g_error_free(gerror);
    }
    config->previos_days_to_show = config->days_to_show;

    /* Get distance units */
    config->distance_units = gconf_client_get_int(gconf_client,                                                                                     
                				    GCONF_KEY_WEATHER_DISTANCE_UNITS,
						    &gerror);
    if(gerror || config->distance_units > SEA_MILES){
	config->distance_units = METERS;
	g_error_free(gerror);
    }

    /* Get wind units */
    config->wind_units = gconf_client_get_int(gconf_client,                                                                                     
						GCONF_KEY_WEATHER_WIND_UNITS,
						&gerror);
    if(gerror || config->wind_units > MILES_H){
	config->wind_units = METERS_S;
	g_error_free(gerror);
    }

    /* Get valid data time for current weather */
    config->data_valid_interval =
	    gconf_client_get_int(gconf_client,
				    GCONF_KEY_WEATHER_VALID_DATA_TIME,
				    &gerror);
    if(gerror || (config->data_valid_interval != 1 && 
		    config->data_valid_interval != 2 &&
		    config->data_valid_interval != 4 &&
		    config->data_valid_interval != 8) ){
	config->data_valid_interval = 2 * 3600; /* Default value - 2 hours */
	g_error_free(gerror);
    }
    else
	config->data_valid_interval *= 3600;

    /* If this first start then fill default station from clock config */ 
    tmp = gconf_client_get_string(gconf_client,
                		    GCONF_KEY_WEATHER_PROGRAM_VERSION, NULL);
    if(!tmp){
	if(!app->config->current_station_id){
	    fill_station_from_clock_plugin_data();
	    if(app->iap_connected){
		app->show_update_window = TRUE;
		update_weather();
	    }
	}	    
    }
    else{
	g_free(tmp);		     
	tmp = NULL;		
    }    
    gconf_client_clear_cache(gconf_client);
    g_object_unref(gconf_client);
    return 0;
}
/*******************************************************************************/
void config_update_proxy(void){
    GConfClient *gconf_client = gconf_client_get_default();
    if(app->config->iap_http_proxy_host)
        g_free(app->config->iap_http_proxy_host);
    /* Get proxy data */
    if(gconf_client_get_bool(gconf_client,
                GCONF_KEY_HTTP_PROXY_ON, NULL)){
        /* HTTP Proxy is on. */
        app->config->iap_http_proxy_host = gconf_client_get_string(gconf_client,
                GCONF_KEY_HTTP_PROXY_HOST, NULL);
        app->config->iap_http_proxy_port = gconf_client_get_int(gconf_client,
                GCONF_KEY_HTTP_PROXY_PORT, NULL);
    }
    else{/* HTTP Proxy is off. */
        app->config->iap_http_proxy_host = NULL;
        app->config->iap_http_proxy_port = 0;
    }
    g_object_unref(gconf_client);
}
/*******************************************************************************/ 
void new_config_save(AppletConfig *config){
    gchar temp_buffer[16];
    GConfClient *gconf_client;
    GSList *stlist = NULL;
   
    gconf_client = gconf_client_get_default();
    if(!gconf_client){
	fprintf(stderr, _("Failed to initialize GConf. Settings were not saved.\n"));
        return;
    }
    /* Save program version */
    if(VERSION)
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_PROGRAM_VERSION,
				VERSION, NULL);
    /* Save Weather Cache Directory. */
    if(config->cache_dir_name)
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_DIR_NAME,
				config->cache_dir_name, NULL);
    /* Save Weather country name. */
    if(config->current_country)
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME,
				config->current_country, NULL);	    
    /* Save Weather station name. */
    if(config->current_station_name)
    	gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
				config->current_station_name, NULL);
    else	    
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
				"", NULL);
    /* Save Weather station id. */
    if(config->current_station_id)
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_STATION_ID,
				config->current_station_id, NULL);
    else 	    
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_STATION_ID,
				"", NULL);
    /* Save icon set name */
    if(config->icon_set)
	gconf_client_set_string(gconf_client,
				GCONF_KEY_WEATHER_ICON_SET,
				config->icon_set, NULL);
    /* Save Weather Icon Size  */		     	    
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_ICONS_SIZE,
			config->icons_size, NULL);
    /* Save current setting tab number  */		     	    
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_SETTING_TAB_NUMBER,
			config->current_settings_page, NULL);
    /* Save Weather Font Color */
    sprintf(temp_buffer, "#%02x%02x%02x",
            config->font_color.red >> 8,
            config->font_color.green >> 8,
            config->font_color.blue >> 8);
    gconf_client_set_string(gconf_client,
        		    GCONF_KEY_WEATHER_FONT_COLOR,
			    temp_buffer, NULL);
    /* Save Background Color */
    sprintf(temp_buffer, "#%02x%02x%02x",
            config->background_color.red >> 8,
            config->background_color.green >> 8,
            config->background_color.blue >> 8);
    gconf_client_set_string(gconf_client,
        		    GCONF_KEY_WEATHER_BACKGROUND_COLOR,
			    temp_buffer, NULL);
    /* Save Weather Update setting  */
    gconf_client_set_int(gconf_client,
        		    GCONF_KEY_WEATHER_UPDATE_INTERVAL,
			    config->update_interval, NULL);
     /* Save Enable Enable Transparency flag. */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_ENABLE_TRANSPARENCY,
			config->transparency, NULL);
     /* Save Split Button State */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_SEPARATE_DATA,
			config->separate, NULL);	    
     /* Save Downloading after connecting State */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_DOWNLOADING_AFTER_CONNECTING,
			config->downloading_after_connecting, NULL);	    
     /* Save Swap Temperature Button State */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_SWAP_TEMPERATURE,
			config->swap_hi_low_temperature, NULL);	    
     /* Hide Station Name and Arrows */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_HIDE_STATION_NAME,
			config->hide_station_name, NULL);
    /* Save Weather Temperature Unit  */		     	    
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_TEMPERATURE_UNIT,
			config->temperature_units, NULL);
    /* Save Days to show. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_DAYS,
			config->days_to_show, NULL);	    
    /* Save distance units. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_DISTANCE_UNITS,
			config->distance_units, NULL);
    /* Save wind units. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_WIND_UNITS,
			config->wind_units, NULL);
    /* Save valid data time */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_VALID_DATA_TIME,
			config->data_valid_interval / 3600, NULL);
    /* Save Layout type. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_ICONS_LAYOUT,
			config->icons_layout, NULL);	    
    /* Save station list */
    stlist = prepare_idlist();
    gconf_client_set_list(gconf_client,
        		GCONF_KEY_WEATHER_STATIONS_LIST,
			GCONF_VALUE_STRING, stlist, NULL);
    /* Free stlist */	    
    g_slist_foreach(stlist, (GFunc)g_free, NULL);
    g_slist_free(stlist);
    g_object_unref(gconf_client);
}/*******************************************************************************/
