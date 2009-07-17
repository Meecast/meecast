/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 * Copyryght (C) 2008 Andrew Olmsted 
 *  for the code
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
#include "weather-simple_settings.h"
/*******************************************************************************/
void
weather_simple_window_settings(GtkWidget *widget, gpointer user_data){
GtkWidget *window = NULL,
          *main_table = NULL,
          *collapsed_button = NULL,
          *expanded_button = NULL,
          *settings_button = NULL;

#if defined OS2009
    window = hildon_stackable_window_new ();
#else
    window = hildon_window_new();
#endif
    gtk_widget_show(window);

    main_table = gtk_table_new(4,3, TRUE);

    collapsed_button = gtk_button_new_with_label (_("Collapsed"));
    gtk_widget_show (collapsed_button);
    gtk_table_attach_defaults (GTK_TABLE(main_table), collapsed_button, 0, 1, 1, 2);
    expanded_button = gtk_button_new_with_label (_("Expanded"));
    gtk_widget_show (expanded_button);
    gtk_table_attach_defaults (GTK_TABLE(main_table), expanded_button, 1, 2, 1, 2);
    settings_button = gtk_button_new_with_label (_("Settings"));
    gtk_widget_show (settings_button);
    gtk_table_attach_defaults (GTK_TABLE(main_table), settings_button, 2, 4, 1, 2);
    gtk_container_add (GTK_CONTAINER (window), main_table);
    gtk_widget_show(main_table);
}
/*******************************************************************************/
