/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
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
#define SOURCES_LIB "/usr/lib/omweather"
/*******************************************************************************/
GtkListStore*
create_sources_list(gchar *sources_path, gint *sources_number, GSList **handles){
    GtkListStore    *list = NULL;
    GSList          *db_set = NULL,
                    *tmp = NULL;
    GtkTreeIter     iter;
    gchar           buffer[255];
    GHashTable      *source = NULL;
    gint            counter = 0;
    gpointer        value = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!sources_path)
        return NULL;/* error */

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
            if(source_name_valid(source)
               && source_library_valid(source, handles)
               && (source_forecast_url_valid(source)
               || source_detail_url_valid(source)) ){
                value = g_hash_table_lookup(source, "name");
                /* add source to list */
                gtk_list_store_append(list, &iter);
//                fprintf(stderr, "\nITER %s\n", iter);
                gtk_list_store_set(list, &iter, 0, value,
                          1, (gpointer)source,
                          -1);

                counter++;
            }
        }
        g_free(tmp->data);
        tmp = g_slist_next(tmp);

    }
    g_slist_free(db_set);

    if(sources_number)
     *sources_number = counter-1;
    return list;
}
/*******************************************************************************/
gpointer
get_source_parser(GtkListStore *data, const gchar *source_name){
    GtkTreeIter iter;
    GHashTable  *source = NULL;
    gpointer    value = NULL;
    gboolean    valid = FALSE;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data && !source_name)
        return NULL;
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(data), &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(data), &iter, 1, &source, -1);
        value = g_hash_table_lookup(source, "name");
        if(source_name && value && !strcmp(source_name, (gchar*)value))
            return g_hash_table_lookup(source, "parser");
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(data), &iter);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return NULL;
}
/*******************************************************************************/
gpointer
get_source_hash(GtkListStore *data, const gchar *source_name){
    GtkTreeIter iter;
    GHashTable  *source = NULL;
    gpointer    value = NULL;
    gboolean    valid = FALSE;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data && !source_name)
        return NULL;
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(data), &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(data), &iter, 1, &source, -1);
        value = g_hash_table_lookup(source, "name");
        if(value && !strcmp(source_name, (gchar*)value))
            return source;
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(data), &iter);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return NULL;
}
/*******************************************************************************/
gpointer
get_first_source_hash(GtkListStore *data){
    GtkTreeIter iter;
    GHashTable  *source = NULL;
    gpointer    value = NULL;
    gboolean    valid = FALSE;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
        return NULL;
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(data), &iter);
    gtk_tree_model_get(GTK_TREE_MODEL(data), &iter, 1, &source, -1);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return source;
}
/*******************************************************************************/
void
unload_parsers(GSList *list){
    GSList  *tmp = list;
    while(tmp){
        dlclose(tmp->data);
        tmp = g_slist_next(tmp);
    }
}
/*******************************************************************************/
gboolean
source_name_valid(GHashTable *data){
    gpointer	value = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
        return FALSE;
    /* check name */
    value = g_hash_table_lookup(data, "name");
    if(!value)
        return FALSE;
    else
        return TRUE;
}
/*******************************************************************************/
gboolean
source_library_valid(GHashTable *data, GSList **handles){
    gpointer        value = NULL,
                    handle = NULL,
                    parser = NULL;
    gchar           buffer[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
        return FALSE;
    /* check libomweather-xxx-source.so file */
    value = g_hash_table_lookup(data, "library");
    if(!value)/* library file does not defined */
        return FALSE;
    else{
        *buffer = 0;
        snprintf(buffer, sizeof(buffer) - 1, "%s/%s", SOURCES_LIB,
                    (gchar*)value);
        if(access(buffer, R_OK))/* file does not exist or no permissions */
            return FALSE;
        handle = dlopen(buffer, RTLD_NOW);
        if(handle){
            dlerror();
            parser = dlsym(handle, "get_station_weather_data");
            if(!dlerror()){
                g_hash_table_insert(data, "parser", parser);
                /* store opening library handle */
                *handles = g_slist_append(*handles, handle);
            }
        }
    }
    return TRUE;
}
/*******************************************************************************/
gboolean
source_forecast_url_valid(GHashTable *data){
    gpointer        value;
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
gboolean
source_detail_url_valid(GHashTable *data){
    gpointer        value;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
        return FALSE;
    /* check detail url*/
    value = g_hash_table_lookup(data, "detail_url");
    if(!(value && strstr(value, "%s")))
        return FALSE;
    else
        return TRUE;
}
/*******************************************************************************/
gboolean
source_search_url_valid(GHashTable *data){
    gpointer        value;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
        return FALSE;
    /* check search url*/
    value = g_hash_table_lookup(data, "search_url");
    if(!(value && strstr(value, "%s")))
        return FALSE;
    else
        return TRUE;
}
/*******************************************************************************/
gboolean
source_stations_database_valid(GHashTable *data){
    gpointer        value;
    gchar           buffer[256];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data)
        return FALSE;
    /* check *.db file */
    value = g_hash_table_lookup(data, "base");
    
    if(!value)/* database file does not defined */
        return FALSE;
    else{
        *buffer = 0;
        snprintf(buffer, sizeof(buffer) - 1, "%s%s", DATABASEPATH,
                    (gchar*)value);
        if(access(buffer, R_OK))/* file does not exist or no permissions */
            return FALSE;
    }
    return TRUE;
}
/*******************************************************************************/
gboolean
source_logo_file_valid(GHashTable *data){
    gpointer        value;
    gchar           buffer[256];
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
GHashTable*
parse_source_file(const gchar *filename, const gchar *encoding){
    xmlDoc     *document = NULL;
    xmlNode    *root_node = NULL,
               *current_node = NULL;
    GHashTable *object = NULL;
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
          xmlFreeDoc(document);
          return object;
      } /* if(document) */
    } /* if(!access()) */
    return object;
}
/*******************************************************************************/
void
free_source_field(gpointer key, gpointer val, gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* "parser" is not *gchar */
    if(strcmp ("parser",key)){
        g_free(val);
        val = NULL;
    }
}
/*******************************************************************************/
void
free_hashtable_with_source(GHashTable* hashtable){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    g_hash_table_foreach(hashtable, free_source_field, NULL);
    g_hash_table_destroy(hashtable);
}
/*******************************************************************************/
void
parse_children(xmlNode *node, GHashTable *object){
    xmlChar     *value = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    while(node){
        if(node->type == XML_ELEMENT_NODE){
            /* name */
            if(!xmlStrcmp(node->name, (const xmlChar*)"name")){
                value = xmlNodeGetContent(node);
                g_hash_table_insert(object, "name", g_strdup((gchar*)value));
                xmlFree(value);
            }
            /* logo */
            if(!xmlStrcmp(node->name, (const xmlChar*)"logo")){
                value = xmlNodeGetContent(node);
                g_hash_table_insert(object, "logo", g_strdup((gchar*)value));
                xmlFree(value);
            }
            /* forecast_url */
            if(!xmlStrcmp(node->name, (const xmlChar*)"forecast")){
                value = xmlGetProp(node, (const xmlChar*)"url");
                g_hash_table_insert(object, "forecast_url", g_strdup((gchar*)value));
                xmlFree(value);
            }
            /* detail_url */
            if(!xmlStrcmp(node->name, (const xmlChar*)"detail")){
                value = xmlGetProp(node, (const xmlChar*)"url");
                g_hash_table_insert(object, "detail_url", g_strdup((gchar*)value));
                xmlFree(value);
            }
            /* search_url */
            if(!xmlStrcmp(node->name, (const xmlChar*)"search")){
                value = xmlGetProp(node, (const xmlChar*)"url");
                g_hash_table_insert(object, "search_url", g_strdup((gchar*)value));
                xmlFree(value);
            }
            /* stations_db */
            if(!xmlStrcmp(node->name, (const xmlChar*)"base")){
                value = xmlNodeGetContent(node);
                g_hash_table_insert(object, "base", g_strdup((gchar*)value));
                xmlFree(value);
            }
            /* library */
            if(!xmlStrcmp(node->name, (const xmlChar*)"library")){
                value = xmlNodeGetContent(node);
                g_hash_table_insert(object, "library", g_strdup((gchar*)value));
                xmlFree(value);
            }
        }
        node = node->next;
    }
}
/*******************************************************************************/
gchar*
get_source_forecast_url(GtkListStore *data, const gchar *source_name){
    GtkTreeIter iter;
    GHashTable  *source = NULL;
    gpointer    value = NULL;
    gboolean    valid = FALSE;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data && !source_name)
        return NULL;
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(data), &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(data), &iter, 1, &source, -1);
        value = g_hash_table_lookup(source, "name");
        if(value && !strcmp(source_name, (gchar*)value) &&
                source_forecast_url_valid(source))
            return g_hash_table_lookup(source, "forecast_url");
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(data), &iter);
    }
    return NULL;
}
/*******************************************************************************/
gchar*
get_source_detail_url(GtkListStore *data, const gchar *source_name){
    GtkTreeIter iter;
    GHashTable  *source = NULL;
    gpointer    value = NULL;
    gboolean    valid = FALSE;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data && !source_name)
        return NULL;
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(data), &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(data), &iter, 1, &source, -1);
        value = g_hash_table_lookup(source, "name");
        if(value && !strcmp(source_name, (gchar*)value) &&
                source_detail_url_valid(source))
            return g_hash_table_lookup(source, "detail_url");
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(data), &iter);
    }
    return NULL;
}
/*******************************************************************************/
gchar*
get_source_logo(GtkListStore *data, const gchar *source_name){
    GtkTreeIter iter;
    GHashTable  *source = NULL;
    gpointer    value = NULL;
    gboolean    valid = FALSE;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!data && !source_name)
        return NULL;
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(data), &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(data), &iter, 1, &source, -1);
        value = g_hash_table_lookup(source, "name");
        if(source_name && value && !strcmp(source_name, (gchar*)value) &&
                source_logo_file_valid(source))
            return g_hash_table_lookup(source, "logo");
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(data), &iter);
    }
    return NULL;
}
/*******************************************************************************/
