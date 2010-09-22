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
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include "omweather-home.hpp"
#include "omweather-config.hpp"
#include <libosso.h>
/*******************************************************************************/
#if !defined(MAEMO4) && !defined(MAEMO5) && !defined(NONMAEMO) && !defined(APPLICATION)
#define MAEMO3
#else
#undef MAEMO3
#endif
/*******************************************************************************/
#ifdef MAEMO4
HD_DEFINE_PLUGIN(OMWeather, omweather, HILDON_DESKTOP_TYPE_HOME_ITEM)
struct _OMWeatherPrivate{
    GtkWidget   *desktop;
};
#endif
/*
#ifdef MAEMO5
HD_DEFINE_PLUGIN_MODULE(OmweatherDesktopWidget, omweather_plugin, HD_TYPE_HOME_PLUGIN_ITEM)
#endif
*/
/*******************************************************************************/
#if defined(MAEMO5) /* for Maemo 5 */
static void
omweather_plugin_init(OmweatherDesktopWidget *applet){
    HDHomePluginItem    *myparent;
    OS2009Config        config;
#elif defined(MAEMO4) /* for Maemo 4.x */
void
omweather_init(OMWeather *applet){
    GtkSettings *settings;
    GdkColormap *cm;
    OS2008Config        config;
#else   /* for Maemo 3.x and early */
void*
hildon_home_applet_lib_initialize(void *state_data, int *state_size,
                                    GtkWidget **widget){
    Config        config;
#endif
#if !defined(MAEMO5) &&  !defined(NONMAEMO) && !defined(APPLICATION)
    osso_context_t      *osso = NULL;
    osso = osso_initialize(PACKAGE, VERSION, TRUE, NULL);
    if(!osso){
        fprintf(stderr, _("Error initializing the OMWeather applet"));
#if defined(NONMAEMO) || defined(APPLICATION)
        return FALSE;
#elif defined(MAEMO4) || defined(MAEMO5)
        return;
#else
        return NULL;
#endif
    }
#endif
}
/*******************************************************************************/
#if defined(MAEMO3)
int
hildon_home_applet_lib_save_state(void *raw_data, void **state_data,
                                                                int *state_size){
    (*state_data) = NULL;
    if(state_size)
        (*state_size) = 0;
    return 1;
}
/*******************************************************************************/
void
hildon_home_applet_lib_background(void *raw_data){
}
/*******************************************************************************/
void
hildon_home_applet_lib_foreground(void *raw_data){
}
/*******************************************************************************/
GtkWidget*
hildon_home_applet_lib_settings(void *applet_data, GtkWindow *parent){
    GtkWidget    *menu_item = NULL;
/*
    menu_item = gtk_menu_item_new_with_label(_("OMWeather settings"));
    g_signal_connect(menu_item, "activate",G_CALLBACK(weather_window_settings), NULL);
*/
    return menu_item;
}
/*******************************************************************************/
#endif
#if !defined(MAEMO3)
void
omweather_destroy(GtkObject *widget){
#else
void 
hildon_home_applet_lib_deinitialize(void *applet_data){
#endif
}
/*******************************************************************************/
