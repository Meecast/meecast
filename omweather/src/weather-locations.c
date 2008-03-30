/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
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
#include "weather-locations.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
/*******************************************************************************/
GtkListStore* create_items_list(const char *filename, long start, long end,
							    long *items_number){
    FILE		*fh;
    GtkListStore	*list = NULL;
    GtkTreeIter		iter;
    char		buffer[512];
    Region_item		r_item;
    Country_item	c_item;    
    Station		station;
    long		max_bytes = 0,
			readed_bytes = 0,
			count = 0;
    
    max_bytes = end - start;
    fh = fopen(filename, "rt");
    if(!fh){
	fprintf(stderr, "\nCan't read file %s: %s", filename,
		strerror(errno));
	items_number && (*items_number = count);
	list = NULL;
    }
    else{
	if(!strcmp(filename, LOCATIONSFILE))
	    list = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING,
					    G_TYPE_DOUBLE, G_TYPE_DOUBLE);
	else
	    list = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
	if(start > -1)
	    if(fseek(fh, start, SEEK_SET)){
		fprintf(stderr,
			"\nCan't seek to the position %ld on %s file: %s\n",
			start, filename, strerror(errno));
		items_number && (*items_number = count);
		return NULL;
	    }
        while(!feof(fh)){
	    memset(buffer, 0, sizeof(buffer));
	    fgets(buffer, sizeof(buffer) - 1, fh);
	    readed_bytes += strlen(buffer);

	    if(!strcmp(filename, LOCATIONSFILE)){
		if(!parse_station_string(buffer, &station)){
		    gtk_list_store_append(list, &iter);
		    gtk_list_store_set(list, &iter,
					0, station.name,
					1, station.id0,
					2, station.latitude,
					3, station.longtitude,
					-1);
		    count++;
		}
	    }
	    else{
		if(!strcmp(filename,REGIONSFILE)){
		    if(!parse_region_string(buffer, &r_item)){
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter,
					    0, r_item.name,
					    1, r_item.start,
					    2, r_item.end,
					    -1);
			count++;
		    }
		}
		else{
		    if(!parse_country_string(buffer, &c_item)){
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter,
					    0, c_item.name,
					    1, c_item.start,
					    2, c_item.end,
					    -1);
			count++;
		    }	
		}
	    }

	    if(start > -1 && end > -1 && readed_bytes >= max_bytes)
		break;
	}
	fclose(fh);
    }
    items_number && (*items_number = count);
    return list;
}
/*******************************************************************************/
int parse_country_string(const char *string, Country_item *result){
    char	*delimiter = NULL,
		*tmp,
		buffer[32];
    int		res = 0;
    
    tmp = (char*)string;
    delimiter = strchr(tmp, ';');
    if(!delimiter)
	res = 1;
    else{
	memset(result->name, 0, sizeof(result->name));
    	memcpy(result->name, tmp,
		((sizeof(result->name) - 1 > (int)(delimiter - tmp)) ?
		((int)(delimiter - tmp)) : (sizeof(result->name) - 1)));
	tmp = delimiter + 1;
	delimiter = strchr(tmp, ';');
	if(!delimiter){
	    result->start = -1;
	    res = 1;
	}
	else{
	    memset(buffer, 0, sizeof(buffer));
	    memcpy(buffer, tmp, ((sizeof(buffer) - 1 > (int)(delimiter - tmp)) ?
				    ((int)(delimiter - tmp)) : (sizeof(buffer) - 1)));
	    result->start = atol(buffer);
	    tmp = delimiter + 1;
	    delimiter = strchr(tmp, ';');
	    if(!delimiter){
		result->end = -1;
		res = 1;
	    }
	    else{
	    	memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, tmp, ((sizeof(buffer) - 1 > (int)(delimiter - tmp)) ?
				    ((int)(delimiter - tmp)) : (sizeof(buffer) - 1)));
		result->end = atol(buffer);
	    }
	}
    }
    return res;
}

/*******************************************************************************/
int parse_region_string(const char *string, Region_item *result){
    char	*delimiter = NULL,
		*tmp;
    int		res = 0;
    
    tmp = (char*)string;
    delimiter = strchr(tmp, ';');
    setlocale(LC_NUMERIC, "POSIX");
    if(!delimiter)
	res = 1;
    else{
	memset(result->name, 0, sizeof(result->name));
        memcpy(result->name, tmp,
		((sizeof(result->name) - 1 > (int)(delimiter - tmp)) ?
		((int)(delimiter - tmp)) : (sizeof(result->name) - 1)));
	tmp = delimiter + 1;
	delimiter = strchr(tmp, ';');
	if(!delimiter){
	    result->start = -1;
	    res = 1;
	}
	else{
	    *delimiter = 0;
	    result->start = atol(tmp);
	    tmp = delimiter + 1;
	    delimiter = strchr(tmp, ';');
	    if(!delimiter){
		result->end = -1;
		res = 1;
	    }
	    else{
		*delimiter = 0;
		result->end = atol(tmp);
		tmp = delimiter + 1;
		delimiter = strchr(tmp, ';');
		if(!delimiter){
		    result->minlat = 0;
		    res = 1;
		}
		else{
		    *delimiter = 0;
		    result->minlat = atof(tmp);
		    tmp = delimiter + 1;
		    delimiter = strchr(tmp, ';');
		    if(!delimiter){
			  result->minlon = 0;
			  res = 1;
		    }
		    else{
			*delimiter = 0;
			result->minlon = atof(tmp);
			tmp = delimiter + 1;
			delimiter = strchr(tmp, ';');
			if(!delimiter){
			    result->maxlat = 0;
			    res = 1;
			}
			else{
			    *delimiter = 0;
			    result->maxlat = atof(tmp);
			    tmp = delimiter + 1;
			    delimiter = strchr(tmp, ';');
			    if(!delimiter){
				result->maxlon = 0;
				res = 1;
			    }
			    else{
				*delimiter = 0;
				result->maxlon = atof(tmp);
			    }
			}
		    }			    
		}	
	    }
	}
    }
    setlocale(LC_NUMERIC, "");    
    return res;
}
/*******************************************************************************/
int parse_station_string(const char *string, Station *result){
    char	*delimiter = NULL,
		*tmp;
    int		res = 0;
    
    tmp = (char*)string;
    delimiter = strchr(tmp, ';');
    setlocale(LC_NUMERIC, "POSIX");
    if(!delimiter)
	res = 1;
    else{
	memset(result->name, 0, sizeof(result->name));
    	memcpy(result->name, tmp,
		((sizeof(result->name) - 1 > (int)(delimiter - tmp)) ?
		((int)(delimiter - tmp)) : (sizeof(result->name) - 1)));
	tmp = delimiter + 1;
	delimiter = strchr(tmp, ';');
	if(!delimiter)
	    res = 1;
	else{
	    memset(result->id0, 0, sizeof(result->id0));
	    memcpy(result->id0, tmp,
		    ((sizeof(result->id0) - 1 > (int)(delimiter - tmp)) ?
		    ((int)(delimiter - tmp)) : (sizeof(result->id0) - 1)));
	    
	    tmp = delimiter + 1;
	    delimiter = strchr(tmp, ';');
	    if(!delimiter)
		res = 1;
	    else{
                *delimiter = 0;
                result->latitude = atof(tmp);
		tmp = delimiter + 1;
		delimiter = strchr(tmp, ';');
		if(!delimiter)
		    res = 1;
		else{
                     *delimiter = 0;
                     result->longtitude = atof(tmp);
		}
	    }
	}
    }
    setlocale(LC_NUMERIC, "");
    return res;
}
/*******************************************************************************/
