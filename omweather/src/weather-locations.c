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
/*******************************************************************************/
int process_xpath_expression(const char* filename, const xmlChar* xpathExpr,
				const xmlChar* ns, GSList **list){
    xmlDocPtr		doc;
    xmlParserCtxtPtr	ctxt;

    xmlXPathContextPtr	xpathCtx; 
    xmlXPathObjectPtr	xpathObj; 
    /* Init libxml */     
    xmlInitParser();
    LIBXML_TEST_VERSION
    
    if(!filename || !ns || !xpathExpr)
	return 1;

    /* Do the main job */
    ctxt = xmlNewParserCtxt();
    if(ctxt == NULL){
        fprintf(stderr, "Failed to allocate parser context\n");
        return 1;
    }
    doc = xmlCtxtReadFile(ctxt, filename, NULL, XML_PARSE_DTDVALID);
    /* check if parsing suceeded */
    if(doc == NULL){
        fprintf(stderr, "Failed to parse %s\n", filename);
	xmlFreeParserCtxt(ctxt);
	return 2;
    }
    if(ctxt->valid == 0){
        fprintf(stderr, "Failed to validate %s\n", filename);
        /* free up the resulting document */
        xmlFreeDoc(doc);
	xmlFreeParserCtxt(ctxt);
	return 3;
    }
    xmlFreeParserCtxt(ctxt);
    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL){
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc); 
        return 1;
    }
    /* Register namespaces from list (if any) */
    if((ns != NULL) && (register_namespaces(xpathCtx, ns) < 0)) {
        fprintf(stderr,"Error: failed to register namespaces list \"%s\"\n", ns);
        xmlXPathFreeContext(xpathCtx); 
        xmlFreeDoc(doc); 
        return 1;
    }
    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if(xpathObj == NULL){
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        xmlXPathFreeContext(xpathCtx); 
        xmlFreeDoc(doc); 
        return 1;
    }
    /* Print results */
    print_xpath_nodes(xpathObj->nodesetval, list);
    /* Cleanup */
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx); 
    xmlFreeDoc(doc); 
    /* Shutdown libxml */
    xmlCleanupParser();
    return 0;
}
/*******************************************************************************/
int register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar* nsList){
    xmlChar* nsListDup;
    xmlChar* prefix;
    xmlChar* href;
    xmlChar* next;
    
    if(!xpathCtx || !nsList)
	return -1;

    nsListDup = xmlStrdup(nsList);
    if(nsListDup == NULL){
	fprintf(stderr, "Error: unable to strdup namespaces list\n");
	return -1;	
    }
    
    next = nsListDup; 
    while(next != NULL){
	/* skip spaces */
	while((*next) == ' ')
	    next++;
	if((*next) == '\0')
	    break;
	/* find prefix */
	prefix = next;
	next = (xmlChar*)xmlStrchr(next, '=');
	if(next == NULL){
	    fprintf(stderr,"Error: invalid namespaces list format\n");
	    xmlFree(nsListDup);
	    return -1;	
	}
	*(next++) = '\0';	
	/* find href */
	href = next;
	next = (xmlChar*)xmlStrchr(next, ' ');
	if(next != NULL){
	    *(next++) = '\0';	
	}
	/* do register namespace */
	if(xmlXPathRegisterNs(xpathCtx, prefix, href) != 0){
	    fprintf(stderr,"Error: unable to register NS with prefix=\"%s\" and href=\"%s\"\n", prefix, href);
	    xmlFree(nsListDup);
	    return -1;	
	}
    }
    xmlFree(nsListDup);
    return 0;
}
/*******************************************************************************/
void print_xpath_nodes(xmlNodeSetPtr nodes, GSList **list){
    xmlNodePtr				cur;
    int					size, i;
    GSList				*tmp_list = NULL;
    struct station_and_weather_code	*sc = NULL;
    gboolean				enable_add = FALSE;
        
    size = (nodes) ? nodes->nodeNr : 0;
    
    for(i = 0; i < size; ++i){
	if( !(nodes->nodeTab[i]) )
	    break;
	if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE){
	    cur = nodes->nodeTab[i];

	    sc = g_new0(struct station_and_weather_code, 1);
	    if(!sc)
		break;
	    memset(sc, 0, sizeof(struct station_and_weather_code));

	    if(cur && (cur->children) && (cur->children->content)){
		sc->station_name = g_strdup((char*)cur->children->content);
		enable_add = TRUE;
	    }
	    if(cur && (cur->properties) && (cur->properties->children)
				&& (cur->properties->children->content) ){
		sc->station_code = g_strdup((char*)cur->properties->children->content);
		enable_add = TRUE;
	    }
	    if(enable_add)
		tmp_list = g_slist_append(tmp_list, sc);
	    else
		g_free(sc);	    
	}
    }
    *list = tmp_list;
}
/*******************************************************************************/
