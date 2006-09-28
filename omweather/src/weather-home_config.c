#include "weather-home_config.h"
/*
 * Change the weather cache directory and update dependent variables.
 */
gboolean
config_set_weather_dir_name(gchar *new_weather_dir_name)
{
    GnomeVFSURI *weather_dir_uri;
    gboolean retval = FALSE;
    fprintf(stderr,"%s()\n", __PRETTY_FUNCTION__);

    /* Rest of the function devoted to making sure the directory exists. */
    weather_dir_uri = gnome_vfs_uri_new(new_weather_dir_name);
    if(!gnome_vfs_uri_exists(weather_dir_uri))
    {
        GnomeVFSURI *parent, *curr_uri;
        GList *list = NULL;

        list = g_list_prepend(list, curr_uri = weather_dir_uri);
        while(GNOME_VFS_ERROR_NOT_FOUND == gnome_vfs_make_directory_for_uri(
                        parent = gnome_vfs_uri_get_parent(curr_uri), 0755))
            list = g_list_prepend(list, curr_uri = parent);

        while(list != NULL)
        {
            retval = (GNOME_VFS_OK == gnome_vfs_make_directory_for_uri(
                        (GnomeVFSURI*)list->data, 0755));
            gnome_vfs_uri_unref((GnomeVFSURI*)list->data);
            list = g_list_remove(list, list->data);
        }
        /* Retval now equals result of last make-dir attempt. */
    }
    else
        retval = TRUE;

    if(retval)
    {
        if(_weather_dir_name)
            g_free(_weather_dir_name);
        _weather_dir_name = new_weather_dir_name;
    }

    fprintf(stderr,"%s(): return %d\n", __PRETTY_FUNCTION__, retval);
    return retval;
}

/*
 * Initialize all configuration from GCONF.  This should not be called more
 * than once during execution.
 */
void
config_init()
{
    gchar *tmp;

    GConfClient *gconf_client = gconf_client_get_default();
    fprintf(stderr,"%s()\n", __PRETTY_FUNCTION__);

    if(!gconf_client)
    {
        fprintf(stderr,"Failed to initialize GConf.  Quitting.\n");
        exit(1);
    }
    /* Get Weather Cache Directory.  Default is "~/apps/omweather". */
        tmp = gconf_client_get_string(gconf_client,
                GCONF_KEY_WEATHER_DIR_NAME, NULL);
        if(!tmp)
            tmp = g_strdup("~/apps/omweather");
        if(!config_set_weather_dir_name(gnome_vfs_expand_initial_tilde(tmp)))
        {
            fprintf(stderr,"Could not create Weather Cache directory.\n");
        }
        g_free(tmp);    
	/* Get Weather Station ID  */
        _weather_station_id = gconf_client_get_string(gconf_client,
                     GCONF_KEY_WEATHER_STATION_ID, NULL);
	/* Get Weather Icon Size  */		     
        _weather_icon_size = gconf_client_get_string(gconf_client,
              GCONF_KEY_WEATHER_ICON_SIZE, NULL);
        if(!_weather_icon_size)
          _weather_icon_size = g_strdup("Large");
       /* Get Weather country name. */    
       _weather_country_name = gconf_client_get_string(gconf_client,
              GCONF_KEY_WEATHER_COUNTRY_NAME, NULL);
       /* Get Weather state name. */    
       _weather_state_name = gconf_client_get_string(gconf_client,
              GCONF_KEY_WEATHER_STATE_NAME, NULL);
       /* Get Weather station name. */    
       _weather_station_name = gconf_client_get_string(gconf_client,
              GCONF_KEY_WEATHER_STATION_NAME, NULL);
       /* Get Weather font color. */    	
	tmp = gconf_client_get_string(gconf_client,
              GCONF_KEY_WEATHER_FONT_COLOR, NULL);
        if(!tmp || !gdk_color_parse(tmp, &_weather_font_color))
         _weather_font_color = DEFAULT_FONT_COLOR;
      

}
 
/**
 * Save all configuration data to GCONF.
 */
void
config_save()
{
    gchar temp_buffer[16];
    GConfClient *gconf_client = gconf_client_get_default();
    
    if(!gconf_client)
    {
        fprintf(stderr,"Failed to initialize GConf.  Settings were not saved.\n");
        return;
    }

    /* Save Weather Cache Directory. */
    if(_weather_dir_name)
        gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_DIR_NAME, _weather_dir_name, NULL);
    /* Save Weather country name. */
    if(_weather_country_name)
        gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_COUNTRY_NAME, _weather_country_name, NULL);
    /* Save Weather state or province name. */
    if(_weather_country_name)
        gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_STATE_NAME, _weather_state_name, NULL);
    /* Save Weather station name. */
    if(_weather_station_name)
        gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_STATION_NAME, _weather_station_name, NULL);
    /* Save Weather station id. */
    if(_weather_station_id)
        gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_STATION_ID, _weather_station_id, NULL);
    /* Save Weather Icon Size  */		     	    
    if(_weather_icon_size)
        gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_ICON_SIZE, _weather_icon_size, NULL);
    /* Save Weather Font Color */
    sprintf(temp_buffer, "#%02x%02x%02x",
            _weather_font_color.red >> 8,
            _weather_font_color.green >> 8,
            _weather_font_color.blue >> 8);
     gconf_client_set_string(gconf_client,
            GCONF_KEY_WEATHER_FONT_COLOR, temp_buffer, NULL);
     
	    
     g_object_unref(gconf_client);

	    
}
