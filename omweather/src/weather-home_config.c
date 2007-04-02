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

#include "weather-home_config.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
/*
 * Change the weather cache directory and update dependent variables.
 */
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
    	    retval = (GNOME_VFS_OK == gnome_vfs_make_directory_for_uri(
                    	(GnomeVFSURI*)list->data, 0755));
    	    gnome_vfs_uri_unref((GnomeVFSURI*)list->data);
    	    list = g_list_remove(list, list->data);
    	}
	retval = FALSE;
    }
    else
	retval = TRUE;
    if(retval){
        if(app->weather_dir_name)
            g_free(app->weather_dir_name);
        app->weather_dir_name = new_weather_dir_name;
	gnome_vfs_uri_unref(weather_dir_uri);
    }
    return retval;
}

/* Add time update to list */
void add_time_update_list(gint _between_time, gchar *_time_name){
    struct time_update *tu;
    tu = g_new0(struct time_update, 1);
    tu->between_time = _between_time;	  
    tu->name_between_time = g_strdup(_time_name);	  
    time_update_list = g_slist_append(time_update_list, tu);
}

gboolean fill_station_inform( struct weather_station *ws){
    FILE *stations_file;  
    char state_name[21];
/*  unsigned char out_buffer[1024]; */
    char out_buffer[1024]; /* buffer for work with stations.txt files*/
    char temp_station_name[21];
    char temp_station_code[9];
    int i;
    /* Search state or province on country and add stations to combobox*/
    if((stations_file = fopen(STATIONS_FILE,"r")) != NULL){
	memset(state_name, 0, sizeof(state_name)); /* Clear temporary value */
	while(!feof(stations_file)){
	    memset(out_buffer, 0, sizeof(out_buffer)); /* Clear buffer */
	    fgets(out_buffer, sizeof(out_buffer), stations_file); /* Read Next Line */
	    memset(temp_station_name, 0, sizeof(temp_station_name)); /* Clear buffer */
	    if( (strlen(out_buffer) > 27) && ((char)out_buffer[0] != '!') ){ /* Is it country or state or province name ? */
    		if( out_buffer[19] != ' ' )
		    sprintf(state_name,"%.19s",out_buffer);
	/* Check for wrong string */ 
		if( strlen(out_buffer) > 90 ){
	  /* Prepare strings station_name and code_name for work */
		    for(i = 3; i < 19; i++)
			temp_station_name[i - 3] = out_buffer[i];
		    for(i = 84; i < 92; i++)
			temp_station_code[i - 84] = out_buffer[i];
		    temp_station_code[8] = 0;
	  /* Trim right space */
    		    for(i = 15; i > 0; i--)
    			if(temp_station_name[i] == ' ')
        		    temp_station_name[i] = 0;
    			else
        		    break;
		    if( strcmp(ws->id_station,temp_station_code) == 0 ){
			ws->name_station = g_strdup(temp_station_name);	   
			fclose(stations_file);  
			return TRUE;
		    } 
		}    	  
	    } 
	}
	fclose(stations_file);  
    }
    return FALSE;    
}

/* Reinitialize stations list */
void reinitilize_stations_list(gchar *stations_string){
    struct weather_station *ws;
    gchar	*temp1 = NULL,
		*temp2 = NULL;

    stations_view_list = NULL; /* Initialize value */
    temp1 = g_strdup(stations_string);
 /* Delimit stations Id string */
    if(strlen(temp1) > 0){
	temp2 = strtok(temp1, "@\0"); /* Delimiter between ID - @ */
	if(temp2)  /* Check random error */      
	    do{
		if(strlen(temp2) > 0){
		    ws = g_new0(struct weather_station, 1);
		    ws->id_station = g_strdup(temp2);
		    if(fill_station_inform(ws))
			stations_view_list = g_slist_append(stations_view_list, ws); /* Add station to stations list */
		    else{
			g_free(ws->id_station);
			g_free(ws);
		    }
		}
		temp2 = strtok(NULL, "@\0"); /* Delimiter between ID - @ */
	    }while(temp2);
    }
    g_free(temp1);       
}

/* New Reinitialize stations list */
void reinitilize_stations_list2(GSList *stlist){
    struct weather_station *ws = NULL;
    gchar *temp1 = NULL, *temp2 = NULL;
    stations_view_list = NULL; /* Initialize value */

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
	    stations_view_list = g_slist_append(stations_view_list, ws); /* Add station to stations list */
	}     
	g_free(stlist->data);      
	g_free(temp1);
	stlist = g_slist_next(stlist);
    }	    
}

/* Prepare stations ID list in one string to write config file */
gchar* prepare_idlist_string(void){
    GString *result_string;
    GSList *tmplist = NULL;
    struct weather_station *ws;

   /* Initialize value */
    tmplist = stations_view_list;
    result_string = g_string_new(NULL);
    while(tmplist != NULL){
	ws = tmplist->data;
	g_string_append(result_string, ws->id_station);
	g_string_append_c(result_string,'@'); /* @ - delimiter */
	tmplist = g_slist_next(tmplist);
    }      
    return g_string_free(result_string, FALSE);
}

/* Prepare stations ID list  to write config file */
GSList* prepare_idlist(void){
    GSList *stlist = NULL;
    GSList *tmplist = NULL;
    struct weather_station *ws;
    char *str;

    tmplist = stations_view_list;
    while(tmplist != NULL){ 
	ws = tmplist->data;
	str = g_strdup_printf("%s@%s", ws->id_station, ws->name_station);
	stlist = g_slist_append(stlist, str);
	tmplist = g_slist_next(tmplist);
    }
    return stlist;
}

/* Initialize all configuration from GCONF.  This should not be called more
 * than once during execution. */
void read_config(void){
    gchar	*tmp;
    GConfValue	*value;
    int		fd = -1;
    GSList	*stlist = NULL;
    GError	*gerror = NULL;
    GdkColor	DEFAULT_FONT_COLOR = {0, 0x0d00, 0x2a00, 0xc000};
    
    GConfClient *gconf_client = gconf_client_get_default();

    if(!gconf_client){
        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        exit(1);
    }
    /* Get Weather Cache Directory.  Default is "~/apps/omweather". */
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_DIR_NAME, NULL);
    if(!tmp)
        tmp = g_strdup("~/apps/omweather");
    if(!config_set_weather_dir_name(gnome_vfs_expand_initial_tilde(tmp)))
        fprintf(stderr, _("Could not create Weather Cache directory.\n"));
    g_free(tmp);
    /* Get Weather Station ID for current station */
    app->current_station_id = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_CURRENT_STATION_ID, NULL);
    
    /* Depricated, after 0.18 not used */				    
    if (!app->current_station_id)
	app->current_station_id = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_STATION_ID, NULL);
	
    /* Get Weather Stations ID  */ /* DEPRICATED !!! */
    /*
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_STATION_IDS, NULL);
    if(tmp){
	reinitilize_stations_list(tmp);
	g_free(tmp);
    }
    else
	if(app->current_station_id)
	    reinitilize_stations_list(app->current_station_id);
*/	    
    /* Get Weather Stations ID and NAME */
    stlist = gconf_client_get_list(gconf_client,
        			    GCONF_KEY_WEATHER_STATIONS_LIST,
				    GCONF_VALUE_STRING, NULL);
    if(stlist){
	reinitilize_stations_list2(stlist);
	g_slist_free(stlist);
    }
	/* Get icon set name */ 
    app->icon_set = gconf_client_get_string(gconf_client,
					    GCONF_KEY_WEATHER_ICON_SET,
					    NULL);
    sprintf(path_large_icon, "%s%s/", ICONS_PATH, app->icon_set);
    if( ( fd = open(path_large_icon, O_RDONLY) ) == -1 ){
	memset(path_large_icon, 0, sizeof(path_large_icon));
	app->icon_set = g_strdup("Crystal");
	sprintf(path_large_icon, "%s%s/", ICONS_PATH, app->icon_set);
    }
    else
	close(fd);
    /* Get Weather Icon Size  */		     
    app->icons_size = gconf_client_get_int(gconf_client,
        				    GCONF_KEY_WEATHER_ICONS_SIZE,
					    NULL);
    if(app->icons_size < 0)
        app->icons_size = LARGE;
    /* Get Weather country name. */    
    app->current_country = gconf_client_get_string(gconf_client,
        					    GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME,
						    NULL);
    /* Get Weather station name. */    
    app->current_station_name = gconf_client_get_string(gconf_client,
    							GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
							NULL);
    /* Get Weather periodic update time . */    
    app->update_interval = gconf_client_get_int(gconf_client,
        			    GCONF_KEY_WEATHER_UPDATE_INTERVAL,
				    NULL);
    if(app->update_interval < 0)
	app->update_interval = 0;
    /* Get Weather font color. */    	
    tmp = gconf_client_get_string(gconf_client,
        			    GCONF_KEY_WEATHER_FONT_COLOR, NULL);
    if(!tmp || !gdk_color_parse(tmp, &_weather_font_color))
         _weather_font_color = DEFAULT_FONT_COLOR;
    g_free(tmp);
    /* Get Enable Transparency flag. Default is TRUE. */
    value = gconf_client_get(gconf_client, GCONF_KEY_ENABLE_TRANSPARENCY, NULL);
    if(value){
        _enable_transparency = gconf_value_get_bool(value);
        gconf_value_free(value);
    }
    else
        _enable_transparency = TRUE;      
    /* Get Temperature Unit  Default Celsius */
    _weather_temperature_unit = gconf_client_get_int(gconf_client,
                    			GCONF_KEY_WEATHER_TEMPERATURE_UNIT, NULL);
    (_weather_temperature_unit) ? (_weather_temperature_unit = FAHRENHEIT)
				: (_weather_temperature_unit = CELSIUS);
    /* Get Layout  Default Horizontal */
    app->icons_layout = gconf_client_get_int(gconf_client,
                			    GCONF_KEY_ICONS_LAYOUT,
					    &gerror);
    if(gerror)
	app->icons_layout = ONE_ROW;
    /* Get number days to show */
    days_to_show = gconf_client_get_int(gconf_client,                                                                                     
                	    GCONF_KEY_WEATHER_DAYS, &gerror);
    if(gerror || !days_to_show)
	days_to_show = 5;
    /* Get distance units */
    distance_units = gconf_client_get_int(gconf_client,                                                                                     
                	    GCONF_KEY_WEATHER_DISTANCE_UNITS, &gerror);
    if(gerror)
	distance_units = METERS;
    /* Get wind units */
    wind_units = gconf_client_get_int(gconf_client,                                                                                     
			GCONF_KEY_WEATHER_WIND_UNITS, &gerror);
    if(gerror)
	wind_units = 0;
    /* Fill time update list */
    if(!time_update_list){
	add_time_update_list(0, _("Never"));	
	add_time_update_list(1 * 60, _("1 hour"));
	add_time_update_list(2 * 60, _("2 hours"));
	add_time_update_list(4 * 60, _("4 hours"));
	add_time_update_list(8 * 60, _("8 hours"));
	add_time_update_list(24 * 60, _("24 hours"));
	add_time_update_list(1, _("1 minute (DEBUG)"));    
    }
    app->show_update_window = FALSE;
    gconf_client_clear_cache(gconf_client);
}

void config_update_proxy(void){
    GConfClient *gconf_client = gconf_client_get_default();
    if(app->iap_http_proxy_host)
        g_free(app->iap_http_proxy_host);
    /* Get proxy data */
    if(gconf_client_get_bool(gconf_client,
                GCONF_KEY_HTTP_PROXY_ON, NULL)){
        /* HTTP Proxy is on. */
        app->iap_http_proxy_host = gconf_client_get_string(gconf_client,
                GCONF_KEY_HTTP_PROXY_HOST, NULL);
        app->iap_http_proxy_port = gconf_client_get_int(gconf_client,
                GCONF_KEY_HTTP_PROXY_PORT, NULL);
    }
    else{/* HTTP Proxy is off. */
        app->iap_http_proxy_host = NULL;
        app->iap_http_proxy_port = 0;
    }
    g_object_unref(gconf_client);
}

/* Save current station position configuration data to GCONF. */
void config_save_current_station(){
    GConfClient *gconf_client = gconf_client_get_default();
    
    if(!gconf_client){
        fprintf(stderr, _("Failed to initialize GConf. Settings were not saved.\n"));
        return;
    }
    /* Save Weather station name. */
    if(app->current_station_name)
        gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_CURRENT_STATION_NAME, app->current_station_name, NULL);
    /* Save Weather station id. */
    if(app->current_station_id)
        gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_CURRENT_STATION_ID, app->current_station_id, NULL);
    g_object_unref(gconf_client);
}
 
/* Save all configuration data to GCONF. */
void config_save(){
    gchar temp_buffer[16];
    gchar *idlist_string;
    GConfClient *gconf_client;
    GSList *stlist = NULL;
   
    gconf_client = gconf_client_get_default();
    if(!gconf_client){
	fprintf(stderr, _("Failed to initialize GConf. Settings were not saved.\n"));
        return;
    }
    /* Save Weather Cache Directory. */
    if(app->weather_dir_name)
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_DIR_NAME,
				app->weather_dir_name, NULL);
    /* Save Weather country name. */
    if(app->current_country)
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_COUNTRY_NAME,
				app->current_country, NULL);	    
    /* Save Weather station name. */
    if(app->current_station_name)
    	gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
				app->current_station_name, NULL);
    else	    
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_STATION_NAME,
				"", NULL);
    /* Save Weather station id. */
    if(app->current_station_id)
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_STATION_ID,
				app->current_station_id, NULL);
    else 	    
        gconf_client_set_string(gconf_client,
        			GCONF_KEY_WEATHER_CURRENT_STATION_ID,
				"", NULL);
     /* Temporary in release 0.1 8 deleted */
    idlist_string = g_strdup("");
    gconf_client_set_string(gconf_client,
        		    GCONF_KEY_WEATHER_STATION_IDS,
			    idlist_string, NULL);
    g_free(idlist_string);
    /* Save icon set name */
    if(app->icon_set)
	gconf_client_set_string(gconf_client,
				GCONF_KEY_WEATHER_ICON_SET,
				app->icon_set, NULL);
    /* Save Weather Icon Size  */		     	    
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_ICONS_SIZE,
			app->icons_size, NULL);
    /* Save Weather Font Color */
    sprintf(temp_buffer, "#%02x%02x%02x",
            _weather_font_color.red >> 8,
            _weather_font_color.green >> 8,
            _weather_font_color.blue >> 8);
    gconf_client_set_string(gconf_client,
        		    GCONF_KEY_WEATHER_FONT_COLOR,
			    temp_buffer, NULL);
    /* Save Weather Update setting  */
    gconf_client_set_int(gconf_client,
        		    GCONF_KEY_WEATHER_UPDATE_INTERVAL,
			    app->update_interval, NULL);
     /* Save Enable Enable Transparency flag. */
    gconf_client_set_bool(gconf_client,
        		GCONF_KEY_ENABLE_TRANSPARENCY,
			_enable_transparency, NULL);	    
    /* Save Weather Temperature Unit  */		     	    
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_TEMPERATURE_UNIT,
			_weather_temperature_unit, NULL);
    /* Save Days to show. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_DAYS,
			days_to_show, NULL);	    
    /* Save distance units. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_DISTANCE_UNITS,
			distance_units, NULL);
    /* Save wind units. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_WEATHER_WIND_UNITS,
			wind_units, NULL);
    /* Save Layout type. */
    gconf_client_set_int(gconf_client,
        		GCONF_KEY_ICONS_LAYOUT,
			app->icons_layout, NULL);	    
    /* Save station list */
    stlist = prepare_idlist();
    gconf_client_set_list(gconf_client,
        		GCONF_KEY_WEATHER_STATIONS_LIST,
			GCONF_VALUE_STRING, stlist, NULL);
    /* Free stlist */	    
    g_slist_foreach(stlist, (GFunc)g_free, NULL);
    g_slist_free(stlist);
    g_object_unref(gconf_client);
}
