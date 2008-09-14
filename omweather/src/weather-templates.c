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
#include "weather-templates.h"
/*******************************************************************************/
GtkWidget* parse_template_from_file(const gchar *filename, const gchar *encoding){
    xmlDoc	*document = NULL;
    xmlNode	*root_node = NULL,
		*current_node = NULL;
    GtkWidget	*object = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* check file accessibility */
    if(!access(filename, R_OK | F_OK)){
	document = xmlReadFile(filename, encoding, 0);
	if(document){
	    root_node = xmlDocGetRootElement(document);
	    current_node = root_node->children;
	    parse_children(current_node, object);
	    return object;
	} /* if(document) */
    } /* if(!access()) */
    return object;
}
/*******************************************************************************/
void parse_children(xmlNode *node, GtkWidget *object){
    GtkWidget	*something = NULL,
		*label = NULL;
    xmlChar	*temp_xml_string = NULL;
    gint	size = 0;
    while(node){
	if(node->type == XML_ELEMENT_NODE){
	    fprintf(stderr, "\n>>>>Name %s\n", node->name);
	    /* line */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"line")){
		something = gtk_hbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(object), something, TRUE, TRUE, 0);
	    }
	    /* column */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"column")){
		something = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(object), something, TRUE, TRUE, 0);
	    }
	    /* text */
	    if(!xmlStrcmp(node->name, (const xmlChar *)"text")){
		temp_xml_string = xmlNodeGetContent(node);
		label = gtk_label_new((gchar*)hash_table_find(temp_xml_string, FALSE));
		xmlFree(temp_xml_string);
		gtk_box_pack_start(GTK_BOX(object), label, TRUE, TRUE, 0);
		temp_xml_string = xmlGetProp(node, (const xmlChar*)"fontsize");
		size = atoi((gchar*)temp_xml_string);
		xmlFree(temp_xml_string);
		set_font(label, NULL, size);
	    }
	    parse_children(node->children, something);
	}
	node = node->next;
    }
}
/*******************************************************************************/
