/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
 * Copyright (C) 2008      Andrew Olmsted
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
#include "weather-config.h"
#include "weather-alerts.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#define CLOCK_FILE	"/usr/share/omweather/city_in_clock.txt"
#define	DEFAULT_FONT	"Sans 12"
/*******************************************************************************/
/* Change the weather cache directory and update dependent variables. */
gboolean
config_set_weather_dir_name(gchar *new_weather_dir_name){
    GnomeVFSURI *weather_dir_uri;
    gboolean retval = FALSE;
    GnomeVFSURI *parent, *curr_uri;
    GList *list = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
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
void
fill_user_stations_list_from_clock(GtkListStore** list){
    FILE	*clock_file;  
    gchar	*home_city = NULL,
		*remote_city = NULL,
		*tmp = NULL,
		*station_name = NULL,
		*station_code = NULL,
		buffer[1024];
    GtkTreeIter	iter;
    GConfClient *gconf_client = gconf_client_get_default();
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif        
    /* City name from config file of clock */
    home_city = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_CLOCK_HOME_LOCATION, NULL);
    if(!home_city)
	return;
    remote_city = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_CLOCK_REMOTE_LOCATION, NULL);
    if(!remote_city)
	return;
    /* This is code for the first start during installation of deb packet */
    if ((clock_file = fopen(CLOCK_FILE, "r")) == NULL)
	sleep (25);
    else
	fclose(clock_file);
	
    if((clock_file = fopen(CLOCK_FILE, "r"))){
	while(!feof(clock_file)){
	    memset(buffer, 0, sizeof(buffer));
	    fgets(buffer, sizeof(buffer) - 1, clock_file);
	    tmp = strchr(buffer, ';'); /* Separator changed from | to ; in 0.19 version */
	    if(!tmp)
		tmp = strchr(buffer, '|'); /* Finding old separator */
	    if(!tmp)
		continue;
	    if(!strncmp(buffer, home_city, tmp - buffer) || 
	        !strncmp(buffer, remote_city, tmp - buffer)){
		/* Prepare struct */
		tmp[ strlen(tmp) - 1 ] = 0;
		station_code = g_strdup(tmp + 1);
		tmp[0] = 0;
		station_name = g_strdup(buffer);
		/* Add station to the user stations list */
                gtk_list_store_append(*list, &iter);
                gtk_list_store_set(*list, &iter,
                                    0, station_name,
                                    1, station_code,
				    3, app->config->weather_source,
                                    -1);
		/* A current station */
		if(!strncmp(buffer, home_city, tmp - buffer)){
		    app->config->current_station_id = station_code;
		    app->config->current_station_name = station_name;
		    app->config->current_station_source = app->config->weather_source;
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
void
fill_user_stations_list(GSList *source_list, GtkListStore** list){
    GtkTreeIter iter;
    gchar	*temp1 = NULL,
		*temp2 = NULL,
		*temp3 = NULL,
		*station_name = NULL,
		*station_code = NULL;
    guint	station_source = 0;
#ifdef OS2008
    gboolean	is_gps = FALSE;
#endif
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    while(source_list){
	temp1 = strdup((gchar*)source_list->data);
   /* Delimit Id and name */
	if(strlen(temp1) > 0){
	    /* station name */
	    temp2 = strtok(temp1, "@"); /* Delimiter - @ */
	    if(temp2)  /* Check random error */      
		if(strlen(temp2) > 0)
		    station_code = g_strdup(temp2); 
	    /* station code */
	    temp2 = strtok(NULL,"@"); /* Delimiter - @ */ 
	    if(temp2)
		station_name = g_strdup(temp2);
	    /* station source */
	    temp3 = strtok(NULL, "@");
	    if(temp3)
		station_source = (guint)atoi(temp3);
	    
#ifdef OS2008		
	    if(app->gps_station.id0 && app->gps_station.name &&
            	    station_code && station_name &&
            	    !strcmp(app->gps_station.id0, station_code) &&
		    !strcmp(app->gps_station.name, station_name))
		is_gps = TRUE;
	    else
		is_gps = FALSE;
#endif		
	    if(station_name && station_code){
		/* Add station to stations list */
        	gtk_list_store_append(*list, &iter);
        	gtk_list_store_set(*list, &iter,
#ifdef OS2008
                            	    0, station_name,
                            	    1, station_code,
				    2, is_gps,
				    3, station_source,
#else
                            	    0, station_name,
                            	    1, station_code,
				    3, station_source,
#endif
                            	    -1);
            }
            if(!strcmp(station_name, app->config->current_station_name) &&
			!strcmp(station_code, app->config->current_station_id)){
		app->config->current_station_id = g_strdup(station_code);
		app->config->current_station_name = g_strdup(station_name);
		app->config->current_station_source = station_source;
	    }
	    station_name && (g_free(station_name) , station_name = NULL);
	    station_code && (g_free(station_code) , station_code = NULL);
	}
	g_free(temp1);
	source_list = g_slist_next(source_list);
    }
}
/*******************************************************************************/
GSList*
create_stations_string_list(void){
    GSList      *stlist = NULL;
    GtkTreeIter	iter;
    gboolean	valid;
    gchar	*station_name = NULL,
		*station_code = NULL,
		*str = NULL;
    guint	station_source = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif    
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
					    &iter);
    while(valid){
	gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
			    &iter, 
                    	    0, &station_name,
                    	    1, &station_code,
			    3, &station_source,
                    	    -1);
	str = g_strdup_printf("%s@%s@%d", station_code, station_name,
				station_source);
	stlist = g_slist_append(stlist, str);
	g_free(station_name);
	g_free(station_code);
	valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
					    &iter);
    }
    return stlist;
}
/*******************************************************************************/
GtkListStore*
create_time_update_list(void){
    GtkListStore	*list = NULL;
    GtkTreeIter         iter;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
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
gint
read_config(AppletConfig *config){
    GConfValue	*value = NULL;
    GConfClient *gconf_client = NULL;
    int		fd = -1;
    GSList	*stlist = NULL;
    GError	*gerror = NULL;
    GdkColor	DEFAULT_FONT_COLOR = {0, 0xFF00, 0xFF00, 0x0000};
    gchar	tmp_buff[1024],
		*home_dir,
		*tmp = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
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
	home_dir = NULL;
	tmp = tmp_buff;
    }
    else{
	snprintf(tmp_buff, sizeof(tmp_buff) - 1, "%s", tmp);
	if(tmp)
	    g_free(tmp);
	tmp = NULL;            
    }
    config->cache_directory = g_strdup(tmp_buff);

    if(!config_set_weather_dir_name(gnome_vfs_expand_initial_tilde(tmp_buff)))
        fprintf(stderr, _("Could not create weather cache directory.\n"));

    /* Get Weather country name. */    
    config->current_country = gconf_client_get_string(gconf_client,
        					    GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME,
						    NULL);
    /* Get current station id */
    config->current_station_id = NULL;
    config->current_station_id = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_CURRENT_STATION_ID, NULL);
    if(config->current_station_id && strlen(config->current_station_id) == 0)
	config->current_station_id = NULL;
    /* Get Weather current station name. */
    config->current_station_name = NULL;
    config->current_station_name = gconf_client_get_string(gconf_client,
							GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
							NULL);
    if(config->current_station_name && strlen(config->current_station_name) == 0)
	config->current_station_name = NULL;
    /* Get weather data source */ 
    config->weather_source = gconf_client_get_int(gconf_client,
					    GCONF_KEY_WEATHER_DATA_SOURCE,
					    NULL);
    if(config->weather_source < WEATHER_COM1 &&
	    config->weather_source > RP5_RU)
	config->weather_source = WEATHER_COM2;
    /* Get GPS station name and id */
#ifdef OS2008
    app->gps_station.name[0] = 0;
    tmp = NULL;
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_GPS_STATION_NAME, NULL);
    if(tmp){
	snprintf(app->gps_station.name, sizeof(app->gps_station.name) - 1, "%s", tmp);
    	g_free(tmp);
    }	
    app->gps_station.id0[0] = 0;
    tmp = NULL;
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_GPS_STATION_ID, NULL);
    if(tmp){
    	snprintf(app->gps_station.id0, sizeof(app->gps_station.id0) - 1, "%s", tmp);
	g_free(tmp);
	tmp = NULL;
    }
#endif    
    /* Get user Weather Stations list */
    stlist = gconf_client_get_list(gconf_client,
				    GCONF_KEY_WEATHER_STATIONS_LIST,
				    GCONF_VALUE_STRING, NULL);
    if(stlist){
	fill_user_stations_list(stlist, &app->user_stations_list);
	g_slist_free(stlist);
    }
    /* Get user alerts list */
    stlist = gconf_client_get_list(gconf_client,
				    GCONF_KEY_WEATHER_ALERTS_LIST,
				    GCONF_VALUE_STRING, NULL);
    if(stlist){
	fill_user_alerts_list(stlist, &app->user_alerts_list);
	g_slist_free(stlist);
    }
    /* Get icon set name */ 
    config->icon_set = gconf_client_get_string(gconf_client,
					    GCONF_KEY_WEATHER_ICON_SET,
					    NULL);
    sprintf(path_large_icon, "%s%s/", ICONS_PATH, config->icon_set);
    if( ( fd = open(path_large_icon, O_RDONLY) ) == -1 ){
	memset(path_large_icon, 0, sizeof(path_large_icon));
	config->icon_set = g_strdup("Glance");
	sprintf(path_large_icon, "%s%s/", ICONS_PATH, config->icon_set);
    }
    else
	close(fd);


    /* Get Weather Icon Size  */
    config->icons_size = gconf_client_get_int(gconf_client,
        				    GCONF_KEY_WEATHER_ICONS_SIZE,
					    NULL);
    if(config->icons_size < TINY || config->icons_size > GIANT)
        config->icons_size = LARGE;    
    /* Get setting tab number  */		     
    config->current_settings_page = gconf_client_get_int(gconf_client,
        				    GCONF_KEY_WEATHER_SETTING_TAB_NUMBER,
					    NULL);
    if(config->current_settings_page < 0 ||
	config->current_settings_page > MAX_SETTINGS_PAGE_NUMBER)
        config->current_settings_page = 0;



    /* Get Weather periodic update time. */
    config->update_interval = gconf_client_get_int(gconf_client,
        			    GCONF_KEY_WEATHER_UPDATE_INTERVAL,
				    NULL);
    if(config->update_interval < 0 || config->update_interval > 24 * 60 ) /* No more than 24 hours */
	config->update_interval = 0;
    remove_periodic_event();		/* delete event from list */
    add_periodic_event(time(NULL));	/* add new event */
    
#ifdef OS2008
    /* Get gps_station. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_USE_GPS_STATION, NULL);
    if(value){
        config->gps_station = gconf_value_get_bool(value);
        gconf_value_free(value);
        add_gps_event(1);
    }
    else
        config->gps_station = FALSE;
#endif

    /* Get Weather font color. */
    tmp = NULL;
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_FONT_COLOR, NULL);
    if(!tmp || !gdk_color_parse(tmp, &(config->font_color)))
         config->font_color = DEFAULT_FONT_COLOR; 
    g_free(tmp);

    /* Get font description. */
    tmp = NULL;
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_FONT, NULL);
    if(!tmp)
        config->font = g_strdup(DEFAULT_FONT);
    else{
	config->font = g_strdup(tmp);
	g_free(tmp);
    }

    
    tmp = NULL;
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_BACKGROUND_COLOR, NULL);
    if(!tmp || !gdk_color_parse(tmp, &(config->background_color)))
         config->background_color = DEFAULT_FONT_COLOR; 
    g_free(tmp);
    tmp = NULL;
    #ifndef OS2008
	/* Get Enable Transparency flag. Default is TRUE. */
	value = gconf_client_get(gconf_client, GCONF_KEY_ENABLE_TRANSPARENCY, NULL);
	if(value){
    	    config->transparency = gconf_value_get_bool(value);
    	    gconf_value_free(value);
	}
	else
    	    config->transparency = TRUE;
    #endif 
    /*Get Enable Weather For Two Hours. Default is TRUE.*/
    value = gconf_client_get(gconf_client, GCONF_KEY_SHOW_WEATHER_FOR_TWO_HOURS,
                                                                                NULL);
    if(value){
        config->show_weather_for_two_hours = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->show_weather_for_two_hours = TRUE;
    /* Get Split Button State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SEPARATE_DATA, NULL);
    if(value){
        config->separate = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->separate = FALSE;
#ifdef OS2008
    /* Get Use sensor Button State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_USE_SENSOR, NULL);
    if(value){
        config->use_sensor = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->use_sensor = FALSE;
    /* Get display sensor at  */
    config->display_at = gconf_client_get_int(gconf_client,
        				    GCONF_KEY_DISPLAY_SENSOR_AT,
					    NULL);
    if(config->display_at < ICON || config->display_at > STATION_NAME)
        config->display_at = STATION_NAME;
    /* Get sensor update time  */
    config->sensor_update_time = gconf_client_get_int(gconf_client,
        				    GCONF_KEY_SENSOR_UPDATE_TIME,
					    NULL);
    if(config->sensor_update_time < 0 || config->sensor_update_time > 30)
        config->sensor_update_time = 0;
    /* start timer for read data from device temperature sensor */
    if(config->use_sensor){
	read_sensor(0);
	app->sensor_timer = g_timeout_add(config->sensor_update_time * 1000 * 60,
                                            (GtkFunction)read_sensor,
                                            GINT_TO_POINTER(1));
    }
    /* Get value of transparence default 0 */
    config->alpha_comp = gconf_client_get_int(gconf_client,
                			    GCONF_KEY_ALPHA_COMPONENT,
					    &gerror);
    if(gerror || config->alpha_comp <= 0){
	config->alpha_comp = 0;
	if(gerror)
	    g_error_free(gerror);
    }
    /* Get value of corner radius. Default is 10 */
    config->corner_radius = gconf_client_get_int(gconf_client,
							GCONF_KEY_CORNER_RADIUS,
							&gerror);
    if(gerror || config->corner_radius <= 0){
	config->corner_radius = 10;
	if(gerror)
	    g_error_free(gerror);
    }
#endif
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

    /* Get Show wind Button State. Default is FALSE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SHOW_WIND, NULL);
    if(value){
        config->show_wind = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->show_wind = FALSE;

    /* Get Show Station Name. Default is TRUE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SHOW_STATION_NAME, NULL);
    if(value){
        config->show_station_name = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->show_station_name = TRUE;
    /* Get Show Arrows State. Default is TRUE */
    value = gconf_client_get(gconf_client, GCONF_KEY_SHOW_ARROWS, NULL);
    if(value){
        config->show_arrows = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        config->show_arrows = TRUE;

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
	    config->icons_layout > COMBINATION)
	config->icons_layout = ONE_ROW;

    /* Get number days to show default 5 */
    config->days_to_show = gconf_client_get_int(gconf_client,                                                                                     
                				GCONF_KEY_WEATHER_DAYS,
						&gerror);
    if(gerror || config->days_to_show <= 0
	      || config->days_to_show > Max_count_weather_day){
	config->days_to_show = 5;
	if(gerror)
	    g_error_free(gerror);
    }
    config->previos_days_to_show = config->days_to_show;

    /* Get distance units */
    config->distance_units = gconf_client_get_int(gconf_client,                                                                                     
                				    GCONF_KEY_WEATHER_DISTANCE_UNITS,
						    &gerror);
    if(gerror || config->distance_units > SEA_MILES){
	config->distance_units = METERS;
	if(gerror)
	    g_error_free(gerror);
    }
    /* Get pressure units */
    config->pressure_units = gconf_client_get_int(gconf_client,                                                                                     
                				    GCONF_KEY_WEATHER_PRESSURE_UNITS,
						    &gerror);
    if(gerror || config->pressure_units < MB || config->pressure_units > MM){
	config->pressure_units = MB;
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
    /* Get switch time */
    config->switch_time = gconf_client_get_int(gconf_client,
				    GCONF_KEY_WEATHER_SWITCH_TIME,
				    &gerror);
    if(gerror || (config->switch_time != 0 &&
		    config->switch_time != 10 &&
		    config->switch_time != 20 &&
		    config->switch_time != 30 &&
		    config->switch_time != 40 &&
		    config->switch_time != 50 &&
		    config->switch_time != 60) ){
	config->switch_time = 0;	/* Default value - Never */
	if(gerror)
	    g_error_free(gerror);
    }
    else{
	if(config->switch_time > 0){
	    g_source_remove(app->switch_timer);
	    app->switch_timer = g_timeout_add(config->switch_time * 1000,
                    		(GtkFunction)switch_timer_handler,
                        	app->main_window);
	}
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
	if(gerror)
	    g_error_free(gerror);
    }
    else
	config->data_valid_interval *= 3600;

    /* If this first start then fill default station from clock config */ 
    tmp = NULL;
    tmp = gconf_client_get_string(gconf_client,
                		    GCONF_KEY_WEATHER_PROGRAM_VERSION, NULL);
    if(!tmp){
	if(!app->config->current_station_id){
	    fill_user_stations_list_from_clock(&app->user_stations_list);
	    if(app->iap_connected)
		update_weather(TRUE);
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
void
config_update_proxy(void){
    GConfClient *gconf_client = gconf_client_get_default();
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
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
void
config_save(AppletConfig *config){
    gchar temp_buffer[16];
    GConfClient *gconf_client;
    GSList *stlist = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif   
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
    /* Save Weather Data Source  */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_DATA_SOURCE,
			config->weather_source, NULL);
    /* Save current setting tab number  */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_SETTING_TAB_NUMBER,
			config->current_settings_page, NULL);
#ifdef OS2008
     /* Save Use sensor Button State */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_USE_SENSOR,
			config->use_sensor, NULL);
    /* Save display sensor at  */		     	    
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_DISPLAY_SENSOR_AT,
			config->display_at, NULL);
    /* Save sensor update time */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_SENSOR_UPDATE_TIME,
			config->sensor_update_time, NULL);
#endif
    /* Save Weather Font Color */
    sprintf(temp_buffer, "#%02x%02x%02x",
            config->font_color.red >> 8,
            config->font_color.green >> 8,
            config->font_color.blue >> 8);
    gconf_client_set_string(gconf_client,
        		    GCONF_KEY_WEATHER_FONT_COLOR,
			    temp_buffer, NULL);
    /* Save Font Description */
    gconf_client_set_string(gconf_client,
        		    GCONF_KEY_WEATHER_FONT,
			    config->font, NULL);
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
    /*Save Enable Weather For Two Hours*/
    gconf_client_set_bool(gconf_client,
                                    GCONF_KEY_SHOW_WEATHER_FOR_TWO_HOURS,
                                    config->show_weather_for_two_hours, NULL);
     /* Save Split Button State */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_SEPARATE_DATA,
			config->separate, NULL);

     /* Save Downloading after connecting State */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_DOWNLOADING_AFTER_CONNECTING,
			config->downloading_after_connecting, NULL);
#ifdef OS2008
     /* Save Use GPS station */
    gconf_client_set_bool(gconf_client,
        		    GCONF_KEY_USE_GPS_STATION,
			    config->gps_station, NULL);
    /* Save Alpha component */
    gconf_client_set_int(gconf_client,
        		    GCONF_KEY_ALPHA_COMPONENT,
			    config->alpha_comp, NULL);
	/* Save Corner Radius */
	gconf_client_set_int(gconf_client,
					GCONF_KEY_CORNER_RADIUS,
				config->corner_radius, NULL);
#endif
     /* Save Swap Temperature Button State */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_SWAP_TEMPERATURE,
			config->swap_hi_low_temperature, NULL);	    
     /* Save Show Wind Button State */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_SHOW_WIND,
			config->show_wind, NULL);	    
     /* Show Station Name */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_SHOW_STATION_NAME,
			config->show_station_name, NULL);
     /* Show Arrows */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_SHOW_ARROWS,
			config->show_arrows, NULL);
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
    /* Save pressure units. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_PRESSURE_UNITS,
			config->pressure_units, NULL);
    /* Save wind units. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_WIND_UNITS,
			config->wind_units, NULL);
    /* Save switch time */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_SWITCH_TIME,
			config->switch_time, NULL);
    /* Save valid data time */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_VALID_DATA_TIME,
			config->data_valid_interval / 3600, NULL);
    /* Save Layout type. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_ICONS_LAYOUT,
			config->icons_layout, NULL);	    
    /* Save station list */
    stlist = create_stations_string_list();
    gconf_client_set_list(gconf_client,
        		GCONF_KEY_WEATHER_STATIONS_LIST,
			GCONF_VALUE_STRING, stlist, NULL);
    /* Free stlist */	    
    g_slist_foreach(stlist, (GFunc)g_free, NULL);
    g_slist_free(stlist);
    /* Save alerts list */
    stlist = create_list_of_user_alerts(app->user_alerts_list);
    gconf_client_set_list(gconf_client,
        		GCONF_KEY_WEATHER_ALERTS_LIST,
			GCONF_VALUE_STRING, stlist, NULL);
    /* Free stlist */	    
    g_slist_foreach(stlist, (GFunc)g_free, NULL);
    g_slist_free(stlist);
#ifdef OS2008    
    /* Save current GPS station */
    if(config->gps_station){
	if(app->gps_station.name)
	    gconf_client_set_string(gconf_client,
        		    GCONF_KEY_GPS_STATION_NAME,
			    app->gps_station.name, NULL);
	if(app->gps_station.id0)		    
	    gconf_client_set_string(gconf_client,
        		    GCONF_KEY_GPS_STATION_ID,
			    app->gps_station.id0, NULL);
    }
#endif    
    g_object_unref(gconf_client);
}
/*******************************************************************************/
