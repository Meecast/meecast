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
#include "weather-common.h"
#include "weather-help.h"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
#include "build"
/*******************************************************************************/
void help_activated_handler(GtkWidget * window, gchar * help_id){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!help_id)
        return;
#if defined (OS2008) || defined (OS2009) || defined(NONMAEMO)
    hildon_help_show(app->osso, help_id, HILDON_HELP_SHOW_DIALOG);
#else
    ossohelp_show(app->osso, help_id, OSSO_HELP_SHOW_DIALOG);
#endif
}
/*******************************************************************************/
void create_about_dialog(void){
    GtkWidget	*help_dialog,
		*notebook;
    char	tmp_buff[2048];
    gint	result;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

/* Test 
GdkPixbuf *pixbuf;
pixbuf = gdk_pixbuf_get_from_drawable (NULL, app->top_widget->window, NULL,
app->top_widget->allocation.x, app->top_widget->allocation.y, 0, 0,
app->top_widget->allocation.width, app->top_widget->allocation.height);
gdk_pixbuf_save (pixbuf , "screenie.png", "png", NULL, NULL);
*/

    help_dialog =
        gtk_dialog_new_with_buttons(_("Other Maemo Weather Info"), NULL,
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    _("OK"), GTK_RESPONSE_ACCEPT, NULL);
/* Create Notebook widget */
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(help_dialog)->vbox),
                       notebook = gtk_notebook_new(), TRUE, TRUE, 0);
/* About tab */
    snprintf(tmp_buff, sizeof(tmp_buff) - 1,
#ifdef DISPLAY_BUILD
             "%s%s%s%s%s%s%s",
#else
             "%s%s%s%s%s",
#endif
             _("\nHildon desktop applet\n"
               "for Nokia 770/N800/N810\n"
               "to show weather forecasts.\n" "Version "), VERSION,
#ifdef DISPLAY_BUILD
             _(" Build: "), BUILD,
#endif
             _("\nCopyright(c) 2006-2008\n"
               "Vlad Vasiliev, Pavel Fialko"),
             _("\nCopyright(c) 2008\n"
               "for default icon set (Glance) and design of interface\n"
               "Andrew Zhilin"),
             _("\nCopyright(c) 2008\n"
               "Andrew Olmsted\n"
               "for Semi-transparency and coloured backgrounds\n"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                             create_scrolled_window_with_text(tmp_buff,
                                                              GTK_JUSTIFY_CENTER),
                             gtk_label_new(_("About")));
/* Authors tab */
    snprintf(tmp_buff, sizeof(tmp_buff) - 1, "%s",
             _("\nAuthor and maintenance:\n"
               "\tVlad Vasiliev, vlad@gas.by\n"
               "Maintenance:\n\tPavel Fialko, pavelnf@gmail.com\n"
               "Documentation:\n\tMarko Vertainen\n"
               "Design of default iconset and design of interface:\n\tAndrew Zhilin"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                             create_scrolled_window_with_text(tmp_buff,
                                                              GTK_JUSTIFY_LEFT),
                             gtk_label_new(_("Authors")));
/* Thanks tab */
    snprintf(tmp_buff, sizeof(tmp_buff) - 1, "%s",
             _("\nEd Bartosh - for more feature requests,\n"
               "\t\t\t\tsupport and criticism\n"
               "Eugen Kaluta aka tren - for feature requests\n"
               "\t\t\t\tand support\n"
               "Maxim Kalinkevish aka spark for testing\n"
               "Yuri Komyakov - for Nokia 770 device \n"
               "Greg Thompson for support stations.txt file\n"
               "Frank Persian - for idea of new layout\n"
               "Brian Knight - for idea of iconset, criticism \n"
               "\t\t\t\tand donation ;-)\n"));
    strcat(tmp_buff,
           _("Andrew aka Tabster - for testing and ideas\n"
             "Brad Jones aka kazrak - for testing\n"
             "Alexis Iglauer - for testing\n"
             "Eugene Roytenberg - for testing\n"
             "Jarek Szczepanski aka Imrahil - for testing\n"
             "Vladimir Shakhov aka Mendoza - for testing \n"
             "Marc Dilon - for spell/stylecheck text of English\n"));
    strcat(tmp_buff,
           _("Arkady Glazov aka Globster - for testing\n"
             "Alexander Savchenko aka dizel - for testing\n"));
    strcat(tmp_buff, _("Eric Link - for feature request and donation\n"));

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                             create_scrolled_window_with_text(tmp_buff,
                                                              GTK_JUSTIFY_LEFT),
                             gtk_label_new(_("Thanks")));
/* Translators tab */
    snprintf(tmp_buff, sizeof(tmp_buff) - 1, "%s",
             _("French - Nicolas Granziano, Philippe \n"
               "Russian - Pavel Fialko, Vlad Vasiliev, Ed Bartosh\n"
               "Finnish - Marko Vertainen\n"
               "German - Claudius Henrichs\n"
               "Italian - Pavel Fialko, Alessandro Pasotti, Salvatore Fiorenzo\n"
               "Spanish - Alejandro Lopez\n"));

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                             create_scrolled_window_with_text(tmp_buff,
                                                              GTK_JUSTIFY_LEFT),
                             gtk_label_new(_("Translators")));
    gtk_widget_show_all(help_dialog);
/* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(help_dialog));
    gtk_widget_destroy(help_dialog);
}
/*******************************************************************************/
