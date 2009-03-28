/* vim: set sw=4 ts=4 et: */
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
#include "weather-sources.h"
#include "weather-utils.h"
#include <string.h>
#define		SOURCES_LIB	"/usr/lib/omweather"
/*******************************************************************************/
GtkListStore* create_sources_list(gchar *sources_path, gint *sources_number){
    GtkListStore	*list = NULL;
    GSList		*db_set = NULL,
			*tmp = NULL;
    GtkTreeIter		iter;
    gchar		buffer[255],
			*dot = NULL;
    GHashTable		*source;
    gint		counter = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!sources_path)
	return NULL;	/* error */

    counter = create_icon_set_list(sources_path, &db_set, "file");
    if(counter < 1)
	return NULL;
    list = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);

    tmp = db_set;
    while(tmp){
	*buffer = 0;
	/* parse source description file */
	snprintf(buffer, sizeof(buffer) - 1, "%s%s", sources_path,
		    (gchar*)(tmp->data));
	source = parse_source_file(buffer, "UTF-8");
	if(source){
//	    if(source_params_valid(source)){ /* temporaly */
		/* add source to list */
		*buffer = 0;
		snprintf(buffer, sizeof(buffer) - 1, "%s", (gchar*)(tmp->data));
		dot = strstr(buffer, ".xml");
		if(dot){
		    *dot = 0;	/* delete .xml */
		    gtk_list_store_append(list, &iter);
		    gtk_list_store_set(list, &iter, 0, buffer,
						    1, (gpointer)source,
						    -1);
		    counter++;
		}
//	    }
	}
	tmp = g_slist_next(tmp);
    }
    g_slist_free(db_set);

    *sources_number = counter;
    return list;
}
/*******************************************************************************/
gboolean source_params_valid(GHashTable *data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
	return FALSE;
    if(!source_library_valid(data) ||
	    (!source_forecast_url_valid(data) && !source_detail_url_valid(data)))
	return FALSE;
    return TRUE;
}
/*******************************************************************************/
gboolean source_library_valid(GHashTable *data){
    gpointer	value;
    gchar	buffer[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
	return FALSE;
    /* check *-source.so file */
    value = g_hash_table_lookup(data, "library");
    if(!value)/* library file does not defined */
	return FALSE;
    else{
	*buffer = 0;
	snprintf(buffer, sizeof(buffer) - 1, "%s%s",
		    SOURCES_LIB, (gchar*)value);
	if(access(buffer, R_OK))/* file does not exist or no permissions */
	    return FALSE;
    }
    return TRUE;
}
/*******************************************************************************/
gboolean source_forecast_url_valid(GHashTable *data){
    gpointer	value;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
	return FALSE;
    /* check forecast url*/
    value = g_hash_table_lookup(data, "forecast_url");
    if(!(value && strstr(value, "%s")))
	return FALSE;
    else
	return TRUE;
}
/*******************************************************************************/
gboolean source_detail_url_valid(GHashTable *data){
    gpointer	value;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
	return FALSE;
    /* check forecast url*/
    value = g_hash_table_lookup(data, "detail_url");
    if(!(value && strstr(value, "%s")))
	return FALSE;
    else
	return TRUE;
}
/*******************************************************************************/
gboolean source_stations_database_valid(GHashTable *data){
    gpointer	value;
    gchar	buffer[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
	return FALSE;
    /* check *.db file */
    value = g_hash_table_lookup(data, "stations_db");
    if(!value)/* database file does not defined */
	return FALSE;
    else{
	*buffer = 0;
	snprintf(buffer, sizeof(buffer) - 1, "%s%s",
		    DATABASEPATH, (gchar*)value);
	if(access(buffer, R_OK))/* file does not exist or no permissions */
	    return FALSE;
    }
    return TRUE;
}
/*******************************************************************************/
gboolean source_logo_file_valid(GHashTable *data){
    gpointer	value;
    gchar	buffer[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
	return FALSE;
    /* check image file */
    value = g_hash_table_lookup(data, "logo");
    if(!value)/* logo file does not defined */
	return FALSE;
    else{
	*buffer = 0;
	snprintf(buffer, sizeof(buffer) - 1, "%s%s",
		    COPYRIGHT_ICONS, (gchar*)value);
	if(access(buffer, R_OK))/* file does not exist or no permissions */
	    return FALSE;
    }
    return TRUE;
}
/*******************************************************************************/
GHashTable* parse_source_file(const gchar *filename, const gchar *encoding){
    xmlDoc	*document = NULL;
    xmlNode	*root_node = NULL,
		*current_node = NULL;
    GHashTable	*object = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* check file accessibility */
    if(!access(filename, R_OK | F_OK)){
	document = xmlReadFile(filename, encoding, 0);
	if(document){
	    root_node = xmlDocGetRootElement(document);
	    current_node = root_node->children;
	    object = g_hash_table_new(g_str_hash, g_str_equal);
	    parse_children(current_node, object);
	    return object;
	} /* if(document) */
    } /* if(!access()) */
    return object;
}
/*******************************************************************************/
void parse_children(xmlNode *node, GHashTable *object){
    GHashTable	*something = NULL;
    xmlChar	*value = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    while(node){
	if(node->type == XML_ELEMENT_NODE){
	    /* name */
/*
	    if(!xmlStrcmp(node->name, (const xmlChar *)"name")){
		value = xmlNodeGetContent(node);
		g_hash_table_insert(object, "name", (gpointer)value);
		xmlFree(value);
	    }
*/
	    /* logo */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"logo")){
		value = xmlNodeGetContent(node);
		g_hash_table_insert(object, "logo", (gpointer)value);
		xmlFree(value);
	    }
	    /* forecast_url */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"forecast")){
		value = xmlGetProp(node, (const xmlChar*)"url");
		g_hash_table_insert(object, "forecast_url", (gpointer)value);
		xmlFree(value);
	    }
	    /* detail_url */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"detail")){
		value = xmlGetProp(node, (const xmlChar*)"url");
		g_hash_table_insert(object, "detail_url", (gpointer)value);
		xmlFree(value);
	    }
	    /* search_url */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"search")){
		value = xmlGetProp(node, (const xmlChar*)"url");
		g_hash_table_insert(object, "search_url", (gpointer)value);
		xmlFree(value);
	    }
	    /* stations_db */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"stations_db")){
		value = xmlNodeGetContent(node);
		g_hash_table_insert(object, "stations_db", (gpointer)value);
		xmlFree(value);
	    }
	    /* library */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"library")){
		value = xmlNodeGetContent(node);
		g_hash_table_insert(object, "library", (gpointer)value);
		xmlFree(value);
	    }
	    parse_children(node->children, something);
	}
	node = node->next;
    }
}
/*******************************************************************************/
