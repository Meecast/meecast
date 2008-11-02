/* # vim: sw=4 ts=4 expandtab ai */
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
#include "weather-units.h"
/*******************************************************************************/
GHashTable* parse_units_file(const gchar *filename, const gchar *encoding){
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
    xmlChar	*key = NULL,
		*value = NULL;

    while(node){
	if(node->type == XML_ELEMENT_NODE){
	    fprintf(stderr, "\n>>>>Name %s\n", node->name);
	    /* source */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"source")){
		key = xmlGetProp(node, (const xmlChar*)"name");
		something = g_hash_table_new(g_str_hash, g_str_equal);
		g_hash_table_insert(object, (gpointer)key, something);
		xmlFree(key);
	    }
	    /* temperature */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"temperature")){
		value = xmlNodeGetContent(node);
		g_hash_table_insert(object, "temperature", (gpointer)value);
		xmlFree(value);
	    }
	    /* pressure */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"pressure")){
		value = xmlNodeGetContent(node);
		g_hash_table_insert(object, "pressure", (gpointer)value);
		xmlFree(value);
	    }
	    /* wind */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"wind")){
		value = xmlNodeGetContent(node);
		g_hash_table_insert(object, "wind", (gpointer)value);
		xmlFree(value);
	    }
	    /* humidity */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"humidity")){
		value = xmlNodeGetContent(node);
		g_hash_table_insert(object, "humidity", (gpointer)value);
		xmlFree(value);
	    }
	    parse_children(node->children, something);
	}
	node = node->next;
    }
}
/*******************************************************************************/
