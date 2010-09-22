/* vim: set sw=4 ts=4 et: */
/* This file is part of Other Maemo Weather(omweather)
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
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
*/
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "weather-main.h"
gboolean key = FALSE;
FILE *file_out = NULL;
/*******************************************************************************/
run_creating_of_animations(void){
    GSList              *list_temp = NULL, *list_of_icons = NULL;
    GSList              *clutter_objects_in_main_form = NULL;
    gchar path[2048];
    gchar c;
    
    list_of_icons = g_list_append(list_of_icons,"0"); 
    list_of_icons = g_list_append(list_of_icons,"1"); 
    list_of_icons = g_list_append(list_of_icons,"2"); 
    list_of_icons = g_list_append(list_of_icons,"3"); 
    list_of_icons = g_list_append(list_of_icons,"4"); 
    list_of_icons = g_list_append(list_of_icons,"5"); 
    list_of_icons = g_list_append(list_of_icons,"6"); 
    list_of_icons = g_list_append(list_of_icons,"7"); 
    list_of_icons = g_list_append(list_of_icons,"8"); 
    list_of_icons = g_list_append(list_of_icons,"9"); 
    list_of_icons = g_list_append(list_of_icons,"10"); 
    list_of_icons = g_list_append(list_of_icons,"11"); 
    list_of_icons = g_list_append(list_of_icons,"12"); 
    list_of_icons = g_list_append(list_of_icons,"13"); 
    list_of_icons = g_list_append(list_of_icons,"14"); 
    list_of_icons = g_list_append(list_of_icons,"15"); 
    list_of_icons = g_list_append(list_of_icons,"16"); 
    list_of_icons = g_list_append(list_of_icons,"17"); 
    list_of_icons = g_list_append(list_of_icons,"18"); 
    list_of_icons = g_list_append(list_of_icons,"19"); 
    list_of_icons = g_list_append(list_of_icons,"20"); 
    list_of_icons = g_list_append(list_of_icons,"21"); 
    list_of_icons = g_list_append(list_of_icons,"22"); 
    list_of_icons = g_list_append(list_of_icons,"23"); 
    list_of_icons = g_list_append(list_of_icons,"24"); 
    list_of_icons = g_list_append(list_of_icons,"25"); 
    list_of_icons = g_list_append(list_of_icons,"26"); 
    list_of_icons = g_list_append(list_of_icons,"27"); 
    list_of_icons = g_list_append(list_of_icons,"28"); 
    list_of_icons = g_list_append(list_of_icons,"29"); 
    list_of_icons = g_list_append(list_of_icons,"30"); 
    list_of_icons = g_list_append(list_of_icons,"31"); 
    list_of_icons = g_list_append(list_of_icons,"32"); 
    list_of_icons = g_list_append(list_of_icons,"33"); 
    list_of_icons = g_list_append(list_of_icons,"34"); 
    list_of_icons = g_list_append(list_of_icons,"35"); 
    list_of_icons = g_list_append(list_of_icons,"36"); 
    list_of_icons = g_list_append(list_of_icons,"37"); 
    list_of_icons = g_list_append(list_of_icons,"38"); 
    list_of_icons = g_list_append(list_of_icons,"39"); 
    list_of_icons = g_list_append(list_of_icons,"40"); 
    list_of_icons = g_list_append(list_of_icons,"41"); 
    list_of_icons = g_list_append(list_of_icons,"42"); 
    list_of_icons = g_list_append(list_of_icons,"43"); 
    list_of_icons = g_list_append(list_of_icons,"44"); 
    list_of_icons = g_list_append(list_of_icons,"45"); 
    list_of_icons = g_list_append(list_of_icons,"46"); 
    list_of_icons = g_list_append(list_of_icons,"47"); 
    list_of_icons = g_list_append(list_of_icons,"48"); 
    list_temp = list_of_icons;
    while(list_temp != NULL){
        key = TRUE;
        sprintf(path,"/usr/share/omweather/icons/Glance/%s.png", list_temp->data);
        clutter_objects_in_main_form = NULL;
        create_hildon_clutter_icon_animation(path, 128, &clutter_objects_in_main_form);
        show_hildon_animation(clutter_objects_in_main_form, NULL);
        while (key){
            gtk_main_iteration();
        }
        fprintf(file_out, "</icon>\n");
        fflush(file_out);
 
        list_temp = g_slist_next(list_temp);
    }
    fprintf(file_out,"</animation>\n");
    fflush(file_out);
    gtk_main_quit();
return FALSE;
}
/*******************************************************************************/
int
main(int argc, char *argv[]){
    GtkWidget* window;
#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif
    file_out = fopen("./1.xml","w");
    fprintf(file_out, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    fprintf(file_out, "<animation>\n");
    gtk_init(&argc, &argv);
    clutter_init(NULL, NULL);
    g_timeout_add(100, run_creating_of_animations, NULL );
    fprintf(stderr,"Begin");
    gtk_main();
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return 0;
}

