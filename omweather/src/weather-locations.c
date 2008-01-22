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
    Item		item;
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
					    G_TYPE_STRING, G_TYPE_STRING);
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
					2, station.lattitude,
					3, station.longitude,
					-1);
		    count++;
		}
	    }
	    else{
		if(!parse_item_string(buffer, &item)){
		    gtk_list_store_append(list, &iter);
		    gtk_list_store_set(list, &iter,
					0, item.name,
					1, item.start,
					2, item.end,
					-1);
		    count++;
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
int parse_item_string(const char *string, Item *result){
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
int parse_station_string(const char *string, Station *result){
    char	*delimiter = NULL,
		*tmp;
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
	    res = 1;
	}
	else{
	    memset(result->id0, 0, sizeof(result->id0));
	    memcpy(result->id0, tmp,
		    ((sizeof(result->id0) - 1 > (int)(delimiter - tmp)) ?
		    ((int)(delimiter - tmp)) : (sizeof(result->id0) - 1)));
	    
	    tmp = delimiter + 1;
	    delimiter = strchr(tmp, ';');
	    if(!delimiter){
		res = 1;
	    }
	    else{
		memset(result->lattitude, 0, sizeof(result->lattitude));
		memcpy(result->lattitude, tmp,
			((sizeof(result->lattitude) - 1 > (int)(delimiter - tmp)) ?
			((int)(delimiter - tmp)) : (sizeof(result->lattitude) - 1)));

		tmp = delimiter + 1;
		delimiter = strchr(tmp, ';');
		if(!delimiter){
		    res = 1;
		}
		else{
		    memset(result->longitude, 0, sizeof(result->longitude));
		    memcpy(result->longitude, tmp,
			    ((sizeof(result->longitude) - 1 > (int)(delimiter - tmp)) ?
			    ((int)(delimiter - tmp)) : (sizeof(result->longitude) - 1)));
		}
	    }
	}
    }
    return res;
}
/*******************************************************************************/
gboolean find_iterator(GtkListStore *store, gint field_number, GType type,
					gchar *field_value, GtkTreeIter *result){
    GtkTreeIter	iter;
    gboolean	valid;
    void	*field_data;
    
    if(field_number < 0 || !field_value)
	return FALSE;

    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(store),
                            &iter,
                            field_number, &field_data,
                            -1);
	switch(type){
	    default:
	    case G_TYPE_STRING:
		if(!strcmp((gchar*)field_data, field_value)){
		    g_free((gchar*)field_data);
		    memcpy(&iter, result, sizeof(GtkTreeIter));
		    return TRUE;
		}
	    break;
	    case G_TYPE_INT:
		if(*((gint*)field_data) == atoi(field_value)){
		    g_free((gint*)field_data);
		    memcpy(&iter, result, sizeof(GtkTreeIter));
		    return TRUE;
		}
	    break;
	}
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
    return FALSE;
}
/*******************************************************************************/
