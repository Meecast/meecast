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
#if HILDON == 1		         
/*******************************************************************************/
#include "weather-home_common.h"
#include "weather-gps.h"
/*******************************************************************************/
gchar *
get_region( float lat, float lon)
{
    FILE		*fh;
    char		buffer[512];
//#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
//#endif     
    float min_lat,max_lat,min_lon,max_lon;
    int i,r_start,r_end;
    gchar region_string[4096];
    Region_item result;

    FILE *file_log;
    file_log=fopen("/tmp/omw.log","a+");

    fh = fopen(REGIONSFILE, "rt");
    if(!fh){
	fprintf(stderr, "\nCan't read file %s: %s", REGIONSFILE,
		strerror(errno));
	return NULL;
    }
    while(!feof(fh)){
	memset(buffer, 0, sizeof(buffer));
	fgets(buffer, sizeof(buffer) - 1, fh);
	parse_region_string(buffer,&result);
	fprintf(file_log,"Count %s %f %f %f %f\n",result.name,result.minlat,result.minlon,result.maxlat,result.maxlon);
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
    FILE *file_log;
    file_log=fopen("/tmp/omw.log","a+");
    if (device->fix->fields & LOCATION_GPS_DEVICE_LATLONG_SET){
	fprintf (file_log,"Latitude: %.2f\nLongitude: %.2f\nAltitude: %.2f\n",
	device->fix->latitude, device->fix->longitude, device->fix->altitude);
	fclose(file_log);
	get_region(device->fix->latitude,device->fix->longitude);
    }
}
/*******************************************************************************/ 
void
initial_gps_connect(void)
{ 
//#ifndef RELEASE
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
//#endif 
    get_region(55.28,30.23);
//    app->gps_device = g_object_new (LOCATION_TYPE_GPS_DEVICE, NULL);
    fprintf(stderr,"BEGIN %s(): \n", __PRETTY_FUNCTION__);
//    app->gps_id_connection = g_signal_connect (app->gps_device, "changed", G_CALLBACK (location_changed), NULL);
    FILE *file_log;
    file_log=fopen("/tmp/omw.log","a+");
    fprintf(file_log,"Begin GPS \n");
    fclose(file_log);
//#ifndef RELEASE
    fprintf(stderr,"END %s(): \n", __PRETTY_FUNCTION__);
//#endif 
    
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
