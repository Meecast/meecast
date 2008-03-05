/* This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
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
#include "weather-home_common.h"
#include "weather-gps.h"
/*******************************************************************************/
#if HILDON == 1

void
get_nearest_station( double lat, double lon, Station *result)
{
    FILE		*fh;
    char		buffer[512];
//#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
//#endif     
    float        min_lat,max_lat,min_lon,max_lon;
    gchar        region_string[4096];
    Region_item  region;
    GtkListStore *stations_list = NULL;
    GtkTreeIter  iter;
    gboolean     valid;
    GtkTreeModel *model;
    gchar        *station_name = NULL,
                 *station_id0 = NULL;
    double       station_latitude,
                 station_longitude,
                 distance,
		 min_distance = 40000;

    FILE *file_log;
    file_log=fopen("/tmp/omw.log","a+");

    fh = fopen(REGIONSFILE, "rt");
    if(!fh){
	fprintf(stderr, "\nCan't read file %s: %s", REGIONSFILE,
		strerror(errno));
	return NULL;
    }
    /* Reading region settings */
    while(!feof(fh)){
	memset(buffer, 0, sizeof(buffer));
	fgets(buffer, sizeof(buffer) - 1, fh);
        parse_region_string(buffer,&region);
	/* Checking insiding point in to region */
        if ( lat >= region.minlat && lat <= region.maxlat && lon >= region.minlon && lon <= region.maxlon){
	    
            stations_list = create_items_list(LOCATIONSFILE, region.start,region.end, NULL);
            valid =  gtk_tree_model_get_iter_first(GTK_TREE_MODEL(stations_list), &iter);
            
            while (valid){
                gtk_tree_model_get(GTK_TREE_MODEL(stations_list),
				                &iter, 
                                                0, &station_name,
					        1, &station_id0,
					        2, &station_latitude,
					        3, &station_longitude,
                		                -1);
		/* Calculating distance */				
		distance = calculate_distance(lat,lon,station_latitude,station_longitude);

		if (distance<min_distance){
		    /* New minimal distance */
		    min_distance = distance;
		    /* Copying to result */
    		    memset(result->name, 0, sizeof(result->name));
    		    memcpy(result->name, station_name,
		    	    ((sizeof(result->name) - 1) > (int)(strlen(station_name)) ?
			    (int)(strlen(station_name)) : (sizeof(result->name) - 1)));
    		    memset(result->name, 0, sizeof(result->name));
    		    memcpy(result->name, station_id0,
		    	    ((sizeof(result->id0) - 1) > (int)(strlen(station_id0)) ?
			    (int)(strlen(station_id0)) : (sizeof(result->id0) - 1)));			    
		    result->latitude = station_latitude;
		    result->longitude = station_longitude;
		}
		                
            	valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(stations_list),&iter);
            }
	    
	    /* Clearing station list */
            if(stations_list){
	          gtk_list_store_clear(stations_list);
	          stations_list = NULL;
	    }	    
	    fprintf(file_log,"Minimal station: %s %f \n",result->name,min_distance);
        }          
    }
    fclose(fh);
    fclose(file_log);
//#ifndef RELEASE
    fprintf(stderr,"END %s(): \n", __PRETTY_FUNCTION__);
//#endif 
}
/*******************************************************************************/
static void
location_changed (LocationGPSDevice *device, gpointer userdata)
{
    if (!app->config->gps_station)
        return;
    FILE *file_log;
    file_log=fopen("/tmp/omw.log","a+");
    if (device->fix->fields & LOCATION_GPS_DEVICE_LATLONG_SET){
	fprintf (file_log,"Latitude: %.2f\nLongitude: %.2f\nAltitude: %.2f\n",
	device->fix->latitude, device->fix->longitude, device->fix->altitude);
	app->temporary_station_latitude = device->fix->latitude;
	app->temporary_station_longtitude = device->fix->longitude;	
	fclose(file_log);
//	get_nearest_station(device->fix->latitude,device->fix->longitude,&app->temporary_gps_station);
    }
}
/*******************************************************************************/ 
void
initial_gps_connect(void)
{ 
#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
#endif 
    get_nearest_station(55.28,30.23,&app->gps_station);
    app->gps_device = g_object_new (LOCATION_TYPE_GPS_DEVICE, NULL);
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
    app->gps_id_connection = g_signal_connect (app->gps_device, "changed", G_CALLBACK (location_changed), NULL);
    FILE *file_log;
    file_log=fopen("/tmp/omw.log","a+");
    fprintf(file_log,"Begin GPS \n");
    fclose(file_log);
#ifndef RELEASE
    fprintf(stderr,"END %s(): \n", __PRETTY_FUNCTION__);
#endif 
    
}
/*******************************************************************************/
void
deinitial_gps_connect(void)
{
    g_signal_handler_disconnect (app->gps_device,app->gps_id_connection);
    g_object_unref(app->gps_device);
    FILE *file_log;    
    file_log=fopen("/tmp/omw.log","a+");
    fprintf(file_log,"END GPS \n");
    fclose(file_log);
}
/*******************************************************************************/
#endif
