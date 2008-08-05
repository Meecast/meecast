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
#include "weather-settings.h"
#include "weather-locations.h"
#include "weather-help.h"
#include "weather-utils.h"
#include <errno.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
#if defined (BSD) && !_POSIX_SOURCE
    #include <sys/dir.h>
    typedef struct dirent Dirent;
#else
    #include <dirent.h>
    #include <linux/fs.h>
    typedef struct dirent Dirent;
#endif
/*******************************************************************************/
/* Hack for Maemo SDK 2.0 */
#ifndef DT_DIR
#define DT_DIR 4
#endif
/*******************************************************************************/
void add_station_to_user_list(gchar *weather_station_name,
				gchar *weather_station_id, gboolean is_gps,
								guint source){
    GtkTreeIter		iter;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif    
    /* Add station to stations list */
    gtk_list_store_append(app->user_stations_list, &iter);
    gtk_list_store_set(app->user_stations_list, &iter,
#ifdef OS2008
                                0, weather_station_name,
                                1, weather_station_id,
                                2, is_gps,
#else
                                0, weather_station_name,
                                1, weather_station_id,
#endif
				3, source,
                                -1);
    /* Set it station how current (for GPS stations) */				
    if(is_gps && app->gps_must_be_current){
	if(app->config->current_station_id != NULL)
	    g_free(app->config->current_station_id);
	app->config->current_station_id = g_strdup(weather_station_id);
	if(app->config->current_station_name)
	    g_free(app->config->current_station_name);
	app->config->current_station_name = g_strdup(weather_station_name);
    }
}
/*******************************************************************************/
void changed_country_handler(GtkWidget *widget, gpointer user_data){
    struct lists_struct	*list = NULL;
    GtkWidget		*config = GTK_WIDGET(user_data),
			*countries = NULL,
			*states = NULL;
    GtkTreeModel	*model;
    GtkTreeIter		iter;
    gchar		*country_name = NULL;
    long		regions_start = -1,
			regions_end = -1,
			regions_number = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    list = (struct lists_struct*)g_object_get_data(G_OBJECT(config), "list");
    if(list){
	countries = list->countries;
	states = list->states;
    }
    else
	return;
    /* clear regions list */
    if(app->regions_list)
	gtk_list_store_clear(app->regions_list);
    /* clear locations list */
    if(app->stations_list)
	gtk_list_store_clear(app->stations_list);    

    if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(countries), &iter)){
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(countries));
	gtk_tree_model_get(model, &iter, 0, &country_name,
					 1, &regions_start,
					 2, &regions_end,
					    -1);
	if(app->regions_list)
	    gtk_list_store_clear(app->regions_list);
	app->regions_list
	    = create_items_list(weather_sources[app->config->weather_source].db_path,
				REGIONSFILE, regions_start, regions_end,
				&regions_number);

	gtk_combo_box_set_row_span_column(GTK_COMBO_BOX(states), 0);
	gtk_combo_box_set_model(GTK_COMBO_BOX(states),
				(GtkTreeModel*)app->regions_list);

	/* if region is one then set it active and disable combobox */
	if(regions_number < 2){
	    gtk_combo_box_set_active(GTK_COMBO_BOX(states), 0);
	    gtk_widget_set_sensitive(GTK_WIDGET(states), FALSE);
	}
	else{
	    gtk_combo_box_set_active(GTK_COMBO_BOX(states), -1);
	    gtk_widget_set_sensitive(GTK_WIDGET(states), TRUE);
	}
	g_free(app->config->current_country);
	app->config->current_country = country_name;
    }
}
/*******************************************************************************/
void changed_state_handler(GtkWidget *widget, gpointer user_data){
    struct lists_struct	*list = NULL;
    GtkWidget		*config = GTK_WIDGET(user_data),
			*states = NULL,
			*stations = NULL;
    GtkTreeModel	*model = NULL;
    GtkTreeIter		iter;
    gchar		*state_name = NULL;
    long		stations_start = -1,
			stations_end = -1;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    list = (struct lists_struct*)g_object_get_data(G_OBJECT(config), "list");
    if(list){
	states = list->states;
	stations = list->stations;
    }
    else
	return;
/* clear locations list */
    if(app->stations_list)
	gtk_list_store_clear(app->stations_list);
	
    if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(states), &iter)){
    	model = gtk_combo_box_get_model(GTK_COMBO_BOX(states));
	gtk_tree_model_get(model, &iter, 0, &state_name,
					 1, &stations_start,
					 2, &stations_end,
					    -1);
	/* clear locations list */
	if(app->stations_list)
	    gtk_list_store_clear(app->stations_list);

	app->stations_list
	    = create_items_list(weather_sources[app->config->weather_source].db_path,
				LOCATIONSFILE, stations_start, stations_end,
				NULL);
	gtk_combo_box_set_row_span_column(GTK_COMBO_BOX(stations), 0);
	gtk_combo_box_set_model(GTK_COMBO_BOX(stations),
				(GtkTreeModel*)app->stations_list);
	g_free(state_name);
    }
}
/*******************************************************************************/
void changed_stations_handler(GtkWidget *widget, gpointer user_data){
    struct lists_struct	*list = NULL;
    GtkWidget		*config = GTK_WIDGET(user_data),
			*stations = NULL,
			*add_button = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    list = (struct lists_struct*)g_object_get_data(G_OBJECT(config), "list");
    if(list)
	stations = list->stations;
    else
	return;
    add_button = lookup_widget(config, "add_from_list");
    if(add_button && gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations)))
	gtk_widget_set_sensitive(add_button, TRUE);
}
/*******************************************************************************/
/* Delete station from list */
void delete_station_handler(GtkButton *button, gpointer user_data){
    GtkWidget		*dialog = NULL,
			*config = GTK_WIDGET(user_data),
			*rename_entry = NULL;
    GtkTreeView		*station_list_view = NULL;
    GtkTreeIter		iter;
    gchar		*station_selected = NULL,
			*station_name = NULL,
			*station_code = NULL;
    GtkTreeModel	*model;
    GtkTreeSelection	*selection;
    gboolean		valid = FALSE;
    gint		result = GTK_RESPONSE_NONE;
    GtkTreePath		*path;
    guint		station_source = -1;
#ifdef OS2008
    gboolean 		is_gps = FALSE;
#endif
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    station_list_view = (GtkTreeView*)lookup_widget(config, "station_list_view");
    rename_entry = lookup_widget(config, "rename_entry");
/* create confirm dialog */
    dialog = gtk_message_dialog_new(NULL,
                            	    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                            	    GTK_MESSAGE_QUESTION,
                            	    GTK_BUTTONS_NONE,
                            	    _("Are you sure to want delete this station ?"));
    gtk_dialog_add_button(GTK_DIALOG(dialog),
        		    _("Yes"), GTK_RESPONSE_YES);
    gtk_dialog_add_button(GTK_DIALOG(dialog),
        		    _("No"), GTK_RESPONSE_NO);
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    if(result != GTK_RESPONSE_YES)
	return;
    if(!station_list_view)
	return;
/* search station for delete */
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(station_list_view));
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));
    if( !gtk_tree_selection_get_selected(selection, NULL, &iter) )
	return;

    gtk_tree_model_get(model, &iter, 0, &station_selected, -1);
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                                  &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter,
#ifdef OS2008
                            0, &station_name,
                            1, &station_code,
			    2, &is_gps,
#else
                            0, &station_name,
                            1, &station_code,
#endif
                            -1);
	if(!strcmp(station_name, station_selected)){
	    path = gtk_tree_model_get_path(GTK_TREE_MODEL(app->user_stations_list),
					    &iter);
#ifdef OS2008
	    if(is_gps){
		/* Reset gps station */
		app->gps_station.id0[0] = 0;
		app->gps_station.name[0] = 0;
		app->gps_station.latitude = 0;
		app->gps_station.longtitude = 0;
	    }
#endif	    	    
	    /* delete selected station */
	    gtk_list_store_remove(app->user_stations_list, &iter);
	    g_free(station_name);
    	    g_free(station_code);
	    /* try to get previos station data */
	    if(gtk_tree_path_prev(path)){
		valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
						&iter,
						path);
		if(valid){
		    /* set current station */
		    gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                        		&iter,
                        		0, &station_name,
                        		1, &station_code,
					3, &station_source,
                        		-1);
		    /* update current station code */
        	    if(app->config->current_station_id)
            		g_free(app->config->current_station_id);
        	    app->config->current_station_id = station_code;
        	    /* update current station name */
        	    if(app->config->current_station_name)
            		g_free(app->config->current_station_name);
        	    app->config->current_station_name = station_name;
        	    app->config->previos_days_to_show = app->config->days_to_show;
		    app->config->current_station_source = station_source;
		    break;
		}
		else
		    gtk_tree_path_free(path);
	    }
	    else{/* try to get next station */
		valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
						    &iter, path);
		if(valid){
		    /* set current station */
		    gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                        		    &iter,
                        		    0, &station_name,
                        		    1, &station_code,
					    3, &station_source,
                        		    -1);
		    /* update current station code */
        	    if(app->config->current_station_id)
            	        g_free(app->config->current_station_id);
        	    app->config->current_station_id = station_code;
        	    /* update current station name */
        	    if(app->config->current_station_name)
            	        g_free(app->config->current_station_name);

        	    app->config->current_station_name = station_name;
        	    app->config->previos_days_to_show = app->config->days_to_show;
		    app->config->current_station_source = station_source;
		    break;
		}
		else{/* if no next station than set current station to NO STATION */
		    /* update current station code */
		    gtk_tree_path_free(path);
        	    if(app->config->current_station_id)
            		g_free(app->config->current_station_id);
        	    app->config->current_station_id = NULL;
        	    /* update current station name */
        	    if(app->config->current_station_name)
            		g_free(app->config->current_station_name);
        	    app->config->current_station_name = NULL;
        	    app->config->previos_days_to_show = app->config->days_to_show;
		    app->config->current_station_source = -1;
		    /* clear rename field */
		    if(rename_entry)
			gtk_entry_set_text((GtkEntry*)rename_entry, "");
		    break;
		}
	    }
	}
	else{
	    g_free(station_name);
    	    g_free(station_code);
	}
	valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                                        &iter);
    }
    g_free(station_selected);
    redraw_home_window(FALSE);
    /* Update config file */
    config_save(app->config);
    highlight_current_station(GTK_TREE_VIEW(station_list_view));
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
/* get icon set names */
int create_icon_set_list(GSList **store){
    Dirent	*dp;
    DIR		*dir_fd;
    int		sets_number = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif    
    dir_fd = opendir(ICONS_PATH);
    if(dir_fd){
	while( (dp = readdir(dir_fd)) ){
	    if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
		continue;
	    if(dp->d_type == DT_DIR){
		*store = g_slist_append(*store, g_strdup(dp->d_name));
		sets_number++;
	    }
	}
	closedir(dir_fd);
    }
    else{
    	*store = g_slist_append(*store, app->config->icon_set);
	sets_number++;
    }
    return sets_number;
}
/*******************************************************************************/
void station_list_view_select_handler(GtkTreeView *tree_view,
                                    			    gpointer user_data){
    GtkTreeIter		iter;
    gchar		*station_selected = NULL,
			*station_name = NULL,
			*station_code = NULL;
    gboolean		valid = FALSE;
    GtkTreeSelection	*selected_line = NULL;
    GtkTreeModel	*model = NULL;
    guint		station_source = -1;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    selected_line = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
    if( !gtk_tree_selection_get_selected(selected_line, NULL, &iter) )
        return;
    gtk_tree_model_get(model, &iter, 0, &station_selected, -1);

    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                                  &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter,
                            0, &station_name,
                            1, &station_code,
			    3, &station_source,
                            -1);
        if(!strcmp(station_selected, station_name)){
        /* update current station code */
            if(app->config->current_station_id)
                g_free(app->config->current_station_id);
            app->config->current_station_id = station_code;
            /* update current station name */
            if(app->config->current_station_name)
                g_free(app->config->current_station_name);
            app->config->current_station_name = station_name;
	    /* add selected station name to the rename entry */
	    gtk_entry_set_text(GTK_ENTRY(user_data), station_name);
	    app->config->current_station_source = station_source;
            break;
        }
	else{
	    g_free(station_name);
	    g_free(station_code);
	}
	valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                    	    &iter);
    }
    g_free(station_selected);
    redraw_home_window(FALSE);
    config_save(app->config);
}
/*******************************************************************************/
void update_iterval_changed_handler(GtkComboBox *widget, gpointer user_data){
    time_t		update_time = 0;
    GtkTreeModel	*model;
    GtkTreeIter		iter;
    gchar		*temp_string,
			tmp_buff[100];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(gtk_combo_box_get_active_iter(widget, &iter)){
	model = gtk_combo_box_get_model(widget);
	gtk_tree_model_get(model, &iter, 1, &update_time, -1);
	/* fill next update field */
	if(!update_time)
	    temp_string = _("Never");
	else{
	    update_time *= 60;
	    update_time += time(NULL);
	    tmp_buff[0] = 0;
	    strftime(tmp_buff, sizeof(tmp_buff) - 1, "%X %x",
	    			    localtime(&update_time));
	    temp_string = tmp_buff;
	}
	gtk_label_set_text(GTK_LABEL(user_data), temp_string);
    }
}
/*******************************************************************************/
int get_active_item_index(GtkTreeModel *list, int time, const gchar *text,
						gboolean use_index_as_result){
    int		result = 0,
		index = 0;
    gboolean	valid = FALSE;
    GtkTreeIter	iter;
    gchar	*str_data = NULL;
    gint	int_data;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    valid = gtk_tree_model_get_iter_first((GtkTreeModel*)list, &iter);
    while(valid){
	gtk_tree_model_get(list, &iter, 
                    	    0, &str_data,
                    	    1, &int_data,
			    -1);
	if(text){ /* if parameter is string */
	    if(!strcmp((char*)text, str_data)){
		if(use_index_as_result)
		    result = index;
		else
		    result = int_data;
		break;
	    }
	}
	else{/* if parameter is int */
	    if(time == int_data){
		result = index;
		break;
	    }
	}
	g_free(str_data);
	str_data = NULL;
	index++;
	valid = gtk_tree_model_iter_next(list, &iter);
    }
    if(str_data)
	g_free(str_data);
    return result;
}
/*******************************************************************************/
void transparency_button_toggled_handler(GtkToggleButton *togglebutton,
                                        		    gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(gtk_toggle_button_get_active(togglebutton))
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
    else
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
}
/*******************************************************************************/
gboolean check_station_code(const gint source, const gchar *station_code){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gint	min_length = 0;
    switch(source){
	case WEATHER_COM1:
	case WEATHER_COM2:
	    min_length = 5;
	break;
	case RP5_RU:
	    min_length = 2;
	break;
    }
    if(strlen((char*)station_code) < min_length)
	return TRUE;
    return FALSE;
}
/*******************************************************************************/
void up_key_handler(GtkButton *button, gpointer list){
    GtkTreeView		*stations = (GtkTreeView*)list;
    GtkTreeIter		iter,
			prev_iter;
    GtkTreeSelection	*selected_line;
    GtkTreeModel	*model;
    GtkTreePath		*path;

    selected_line = gtk_tree_view_get_selection(GTK_TREE_VIEW(stations));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(stations));
    if( !gtk_tree_selection_get_selected(selected_line, NULL, &iter) )
        return;
    path = gtk_tree_model_get_path(model, &iter);
    if(!gtk_tree_path_prev(path)){
	gtk_tree_path_free(path);
	return;
    }
    else{
	if(gtk_tree_model_get_iter(model, &prev_iter, path))
	    gtk_list_store_move_before(GTK_LIST_STORE(model), &iter, &prev_iter);
    }
    gtk_tree_path_free(path);
}
/*******************************************************************************/
void down_key_handler(GtkButton *button, gpointer list){
    GtkTreeView		*stations = (GtkTreeView*)list;
    GtkTreeIter		iter,
			next_iter;
    GtkTreeSelection	*selected_line;
    GtkTreeModel	*model;
    GtkTreePath		*path;

    selected_line = gtk_tree_view_get_selection(GTK_TREE_VIEW(stations));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(stations));
    if( !gtk_tree_selection_get_selected(selected_line, NULL, &iter) )
        return;
    path = gtk_tree_model_get_path(model, &iter);
    gtk_tree_path_next(path);
    if(gtk_tree_model_get_iter(model, &next_iter, path))
	gtk_list_store_move_after(GTK_LIST_STORE(model), &iter, &next_iter);
    gtk_tree_path_free(path);
}
/*******************************************************************************/
void highlight_current_station(GtkTreeView *tree_view){
    GtkTreeIter		iter;
    gchar		*station_name = NULL,
			*station_code = NULL;
    gboolean		valid;
    GtkTreePath		*path;
    GtkTreeModel	*model;
    guint		station_source = -1;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                                  &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter,
                            0, &station_name,
                            1, &station_code,
			    3, &station_source,
                            -1);
	if(!app->config->current_station_name){
	    app->config->current_station_name = station_name;
	    app->config->current_station_id = station_code;
	    app->config->current_station_source = station_source;
	    break;
	}
	else{
    	    if(app->config->current_station_name && station_name &&                    
        	!strcmp(app->config->current_station_name, station_name)){
		model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
		path = gtk_tree_model_get_path(model, &iter);
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(tree_view), path, NULL, FALSE);
		gtk_tree_path_free(path);
    		break;
	    }
	    else{
		g_free(station_name);
		g_free(station_code);
	    }
	}
	valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                    	    &iter);
    }
}
/*******************************************************************************/
void weather_window_settings(GtkWidget *widget, GdkEvent *event,
							    gpointer user_data){
    gint	day_number = (gint)user_data;/* last looking day on detail window */

    GtkWidget	*window_config = NULL,
		*notebook = NULL,
		*vbox = NULL,
#if defined(OS2008) || defined(DEBUGTEMP)
		*sensor_page = NULL,
#endif
		*buttons_box = NULL,
		*help_button = NULL,
		*apply_button = NULL,
		*close_button = NULL,
		*back_button = NULL;
    GtkStyle    *style = NULL;
#ifndef RELEASE
    char	tmp_buff[1024];
#endif
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* kill popup window :-) */
    if(app->popup_window)
        gtk_widget_destroy(app->popup_window);
/* Main window */
    window_config = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GLADE_HOOKUP_OBJECT_NO_REF(window_config, window_config, "window_config");
    g_object_set_data(G_OBJECT(window_config), "day_number", (gpointer)day_number);
    gtk_window_fullscreen(GTK_WINDOW(window_config));
    if (app->config->ui_background_color_on)
       set_background_color(window_config, &(app->config->ui_background_color));
    gtk_widget_show(window_config);
    /* create frame vbox */
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window_config), vbox);
/* Bottom buttons box */
    buttons_box = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(buttons_box, -1, 60);
    /* Back buton */
    back_button = create_button_with_image(BUTTON_ICONS, "back", 40, FALSE, FALSE);
    g_signal_connect(G_OBJECT(back_button), "button_press_event",
                        G_CALLBACK(back_button_handler),
			(gpointer)window_config);
    /* Help buton */
    help_button = create_button_with_image(BUTTON_ICONS, "about", 40, FALSE, FALSE);
    g_signal_connect(G_OBJECT(help_button), "button_press_event",
                        G_CALLBACK(help_button_handler),
			(gpointer)window_config);
    /* Apply button */
    apply_button = create_button_with_image(BUTTON_ICONS, "apply", 40, FALSE, FALSE);
    GLADE_HOOKUP_OBJECT(window_config, apply_button, "apply_button");
    g_signal_connect(G_OBJECT(apply_button), "button_press_event",
                        G_CALLBACK(apply_button_handler),
			(gpointer)window_config);
    gtk_widget_set_sensitive(apply_button, FALSE);
    /* Close button */
    close_button = create_button_with_image(BUTTON_ICONS, "close", 40, FALSE, FALSE);
    g_signal_connect(G_OBJECT(close_button), "button_press_event",
                        G_CALLBACK(close_button_handler),
			(gpointer)window_config);
/* Change background color on buttons */
     if (app->config->ui_background_color_on){
       gtk_widget_modify_bg(apply_button, GTK_STATE_INSENSITIVE, &(app->config->ui_background_color));
       set_background_color(close_button, &(app->config->ui_background_color));
       set_background_color(apply_button, &(app->config->ui_background_color));
       set_background_color(help_button, &(app->config->ui_background_color));
       set_background_color(back_button, &(app->config->ui_background_color));
     }

/* Pack buttons to the buttons box */
    gtk_box_pack_start(GTK_BOX(buttons_box), back_button, TRUE, TRUE, 25);
    gtk_box_pack_start(GTK_BOX(buttons_box), apply_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(buttons_box), help_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(buttons_box), close_button, TRUE, TRUE, 25);

/* create tabs widget */
    notebook = gtk_notebook_new();
    GLADE_HOOKUP_OBJECT(window_config, notebook, "notebook");
    if (app->config->ui_background_color_on){
	set_background_color(notebook, &(app->config->ui_background_color));
	style = gtk_widget_get_style(notebook);
	gtk_widget_set_style(notebook,style);    
    }
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
/* add pages to the notebook */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
		    create_locations_tab(window_config),
        	    gtk_label_new(_("Stations")));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
		    create_visuals_tab(window_config),
        	    gtk_label_new(_("Visuals")));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_display_tab(window_config),
        			gtk_label_new(_("Display")));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			 create_units_tab(window_config) ,
        			gtk_label_new(_("Units")));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_update_tab(window_config),
        			gtk_label_new(_("Update")));
#if defined(OS2008) || defined(DEBUGTEMP)
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			sensor_page = create_sensor_page(window_config),
        			gtk_label_new(_("Sensor")));
#endif
#ifndef RELEASE
/* Events list tab */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    print_list(tmp_buff, sizeof(tmp_buff) - 1);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_scrolled_window_with_text(tmp_buff,
						    GTK_JUSTIFY_LEFT),
        			gtk_label_new("Events"));
#endif
    gtk_widget_show(notebook);
/* Pack items to config window */
    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), buttons_box, FALSE, FALSE, 0);

    gtk_widget_show_all(window_config);
/* set current page and show it for notebook */
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook),
				    app->config->current_settings_page);
}
/*******************************************************************************/
void apply_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
    struct lists_struct	*list = NULL;
    GtkTreeModel	*model;
    GtkTreeIter		iter;
    time_t		updatetime = 0;
    GtkWidget		*config_window = GTK_WIDGET(user_data),
			*visible_items_number = NULL,
			*icon_size = NULL,
			*separate = NULL,
			*font = NULL,
#ifdef OS2008
			*enable_gps = NULL,
#endif
			*swap_temperature = NULL,
			*show_wind = NULL,
			*show_station_name = NULL,
			*show_arrows = NULL,
			*transparency = NULL,
			*background_color = NULL,
			*font_color = NULL,
			*time2switch = NULL,
			*validtime = NULL,
			*update_time = NULL,
			*weather_source = NULL,
			*download_after_connection = NULL,
#if defined(OS2008) || defined(DEBUGTEMP)
			*use_sensor = NULL,
			*display_at = NULL,
			*sensor_update_time = NULL,
#endif
			*temperature = NULL,
			*pressure = NULL,
			*meters = NULL,
			*kilometers = NULL,
			*miles = NULL,
			*wind_meters = NULL,
			*wind_kilometers = NULL,
			*wind_miles = NULL,
			*row = NULL,
			*column = NULL,
			*two_rows = NULL,
			*two_columns = NULL,
			*countries = NULL,
			*states = NULL,
			*theme_override = NULL,
			*selected_icon_set = NULL,
			*stations = NULL;
    GSList		*icon_set = NULL;
#ifndef OS2008
    gboolean		need_correct_layout_for_OS2007 = FALSE;
#endif
    gint		active_index = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* visible items */
    visible_items_number = lookup_widget(config_window, "visible_items_number");
    if(visible_items_number){
	if(app->config->days_to_show - 1
		!= hildon_controlbar_get_value(HILDON_CONTROLBAR(visible_items_number))){
	    /* store previos number of icons */
	    app->config->previos_days_to_show = app->config->days_to_show;
	    app->config->days_to_show
		= hildon_controlbar_get_value(HILDON_CONTROLBAR(visible_items_number)) + 1;
#ifndef OS2008
	    need_correct_layout_for_OS2007 = TRUE;
#endif
	}
    }
/* layout type */
    row = lookup_widget(config_window, "one_row");
    column = lookup_widget(config_window, "one_column");
    two_rows = lookup_widget(config_window, "two_rows");
    two_columns = lookup_widget(config_window, "two_columns");
    if(row && column && two_rows && two_columns){
    	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(row)))
            app->config->icons_layout = ONE_ROW;
	else{
	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(column)))
        	app->config->icons_layout = ONE_COLUMN;
	    else{
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(two_rows)))
            	    app->config->icons_layout = TWO_ROWS;
		else{
		    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(two_columns)))		
    			app->config->icons_layout = TWO_COLUMNS;
		    else
			app->config->icons_layout = COMBINATION;
		}
	    }
	}
#ifndef OS2008
	need_correct_layout_for_OS2007 = TRUE;
#endif
    }
/* icon set */	
    icon_set = (GSList*)g_object_get_data(G_OBJECT(config_window), "iconsetlist");
    if(icon_set){
	while(icon_set){
	    selected_icon_set = lookup_widget(config_window, (gchar*)icon_set->data);
	    if(selected_icon_set){
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(selected_icon_set))){
		    if(app->config->icon_set)
			g_free(app->config->icon_set);
		    app->config->icon_set = g_strdup((gchar*)icon_set->data);
		    break;
		}
	    }
	    icon_set = g_slist_next(icon_set);
	}
	memset(path_large_icon, 0, sizeof(path_large_icon));
	snprintf(path_large_icon, sizeof(path_large_icon) - 1,
		    "%s%s/", ICONS_PATH, app->config->icon_set);
#ifndef OS2008
	need_correct_layout_for_OS2007 = TRUE;
#endif
    }
/* icon size */
    icon_size = lookup_widget(config_window, "icon_size");
    if(icon_size){
	if(app->config->icons_size - 1
		!= hildon_controlbar_get_value(HILDON_CONTROLBAR(icon_size))){
	    app->config->icons_size
		= hildon_controlbar_get_value(HILDON_CONTROLBAR(icon_size)) + 1;
#ifndef OS2008
	    need_correct_layout_for_OS2007 = TRUE;
#endif
	}
    }
/* distance units */
    meters = lookup_widget(config_window, "meters");
    kilometers = lookup_widget(config_window, "kilometers");
    miles = lookup_widget(config_window, "miles");
    if(meters && kilometers && miles){
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(meters)))
            app->config->distance_units = METERS;
	else{
	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(kilometers)))
        	app->config->distance_units = KILOMETERS;
	    else{
	    	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(miles)))
        	    app->config->distance_units = MILES;
		else
		    app->config->distance_units = SEA_MILES;
	    }
	}
    }
/* wind units */
    wind_meters = lookup_widget(config_window, "wind_meters");
    wind_kilometers = lookup_widget(config_window, "wind_kilometers");
    wind_miles = lookup_widget(config_window, "wind_miles");
    if(wind_meters && wind_kilometers && wind_miles){
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wind_meters)))
            app->config->wind_units = METERS_S;
	else{
	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wind_kilometers)))
        	app->config->wind_units = KILOMETERS_H;
	    else
		app->config->wind_units = MILES_H;
	}
    }
/* pressure */
    pressure = lookup_widget(config_window, "mb_pressure");
    if(pressure){
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pressure)))
            app->config->pressure_units = MB;
	else
            app->config->pressure_units = INCH;
    }
/* temperature */
    temperature = lookup_widget(config_window, "temperature");
    if(temperature){
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(temperature)))
            app->config->temperature_units = CELSIUS;
	else
            app->config->temperature_units = FAHRENHEIT;
    }
#ifdef OS2008
/* enable gps */
    enable_gps = lookup_widget(config_window, "enable_gps");
    if(enable_gps){
	app->config->gps_station = 
	    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(enable_gps));
	if(app->config->gps_station)
            add_gps_event(1);
	else{/* Reset gps station */
	    app->gps_station.id0[0] = 0;
	    app->gps_station.name[0] = 0;
	    app->gps_station.latitude = 0;
	    app->gps_station.longtitude = 0;		
	}
    }
#endif
/* download after connection */
    download_after_connection = lookup_widget(config_window,
						"download_after_connection");
    app->config->downloading_after_connecting 
	= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(download_after_connection));
/* swap temperature */
    swap_temperature = lookup_widget(config_window, "swap_temperature");
    if(swap_temperature)
	app->config->swap_hi_low_temperature = 
	    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(swap_temperature));
/* show wind */
    show_wind = lookup_widget(config_window, "show_wind");
    if(show_wind)
	app->config->show_wind = 
	    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_wind));
/* separate */
    separate = lookup_widget(config_window, "separate");
    if(separate)
	app->config->separate = 
		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(separate));
/* hide station name */
    show_station_name = lookup_widget(config_window, "show_station_name");
    if(show_station_name){
	if(app->config->show_station_name
		!= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_station_name))){
	    app->config->show_station_name
		= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_station_name));
#ifndef OS2008
	    need_correct_layout_for_OS2007 = TRUE;
#endif
	}
    }
#if defined(OS2008) || defined(DEBUGTEMP)
/* use sensor */
    use_sensor = lookup_widget(config_window, "use_sensor");
    if(use_sensor)
	app->config->use_sensor = 
	    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(use_sensor));
    if(app->config->use_sensor) /* if enabled sensor */
	read_sensor(1);		/* immediately read and display sensor data */
/* display sensor at */
    display_at = lookup_widget(config_window, "display_at");
    if(display_at){
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(display_at)))
	    app->config->display_at = STATION_NAME;
	else
	    app->config->display_at = ICON;
    }
/* sensor update time */
    sensor_update_time = lookup_widget(config_window, "sensor_update_time");
    if(sensor_update_time){
	if(app->config->use_sensor){
	    app->config->sensor_update_time
		= get_time_from_index(gtk_combo_box_get_active(GTK_COMBO_BOX(sensor_update_time)));
	    app->sensor_timer = g_timeout_add(app->config->sensor_update_time * 1000 * 60,
                                            (GtkFunction)read_sensor,
                                            GINT_TO_POINTER(1));
	}
    }
#endif
/* Show arrows */
    show_arrows = lookup_widget(config_window, "show_arrows");
    if(show_arrows){
	if(app->config->show_arrows
		!= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_arrows))){
	    app->config->show_arrows
		= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_arrows));
#ifndef OS2008
	    need_correct_layout_for_OS2007 = TRUE;
#endif
	}
    }
/* transparency */
    transparency = lookup_widget(config_window, "transparency");
#ifdef OS2008
    if(transparency){
	app->config->alpha_comp =
		hildon_controlbar_get_value(HILDON_CONTROLBAR(transparency));
    }
#else
    if(transparency)
	app->config->transparency = 
		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(transparency));
#endif
/* UI Theme Override */
    theme_override = lookup_widget(config_window, "theme_override");
    if(theme_override){
	app->config->ui_background_color_on =
	    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(theme_override));
    }
/* background color */
    background_color = lookup_widget(config_window, "background_color");
    if(background_color)
    	gtk_color_button_get_color(GTK_COLOR_BUTTON(background_color),
					&(app->config->background_color));
/* font */
    font = lookup_widget(config_window, "font");
    if(font){
	if(app->config->font)
	    g_free(app->config->font);
	app->config->font
	    = g_strdup((gchar*)gtk_font_button_get_font_name(GTK_FONT_BUTTON(font)));
    }
/* font color */
    font_color = lookup_widget(config_window, "font_color");
    if(font_color)
    	gtk_color_button_get_color(GTK_COLOR_BUTTON(font_color),
					&(app->config->font_color));
/* Switch time */
    time2switch = lookup_widget(config_window, "time2switch");
    if(time2switch){
	if( gtk_combo_box_get_active((GtkComboBox*)time2switch) != app->config->switch_time / 10){
	    app->config->switch_time = 10 * gtk_combo_box_get_active((GtkComboBox*)time2switch);
		g_source_remove(app->switch_timer);
		if(app->config->switch_time > 0)
		    app->switch_timer = g_timeout_add(app->config->switch_time * 1000,
                    					(GtkFunction)switch_timer_handler,
                        				app->main_window);
	}
    }
/* Data valid time */
    validtime = lookup_widget(config_window, "valid_time");
    if(validtime){
	if( (1 << gtk_combo_box_get_active((GtkComboBox*)validtime)) != app->config->data_valid_interval / 3600 ){
	    app->config->data_valid_interval = 3600 * (1 << gtk_combo_box_get_active((GtkComboBox*)validtime));
	}
    }
/* Update time */
    update_time = lookup_widget(config_window, "update_time");
    if(update_time){
	if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(update_time), &iter)){
	    model = gtk_combo_box_get_model(GTK_COMBO_BOX(update_time));
	    gtk_tree_model_get(model, &iter, 1, &updatetime, -1);
	    if(app->config->update_interval != updatetime){
		app->config->update_interval = updatetime;
		remove_periodic_event();
		add_periodic_event(time(NULL));
	    }
	}
    }
/* Weather data source */	
    weather_source = lookup_widget(config_window, "weather_source");
    list = (struct lists_struct*)g_object_get_data(G_OBJECT(config_window), "list");
    if(weather_source && list){
        countries = list->countries;
        states = list->states;
	stations = list->stations;

	active_index = gtk_combo_box_get_active(GTK_COMBO_BOX(weather_source));
	if(app->config->weather_source != active_index)
	    app->config->weather_source = active_index;
	/* clear and delete old countries list */
	if(app->countrys_list){
            gtk_list_store_clear(app->countrys_list);
            g_object_unref(app->countrys_list);
        }
	/* create new countries list */
	app->countrys_list
	    = create_items_list(weather_sources[app->config->weather_source].db_path,
				    COUNTRIESFILE, -1, -1, NULL);
	gtk_combo_box_set_model(GTK_COMBO_BOX(countries),
				(GtkTreeModel*)app->countrys_list);
	/* set active item for all lists to nothing */
	gtk_combo_box_set_active(GTK_COMBO_BOX(countries), -1);
	gtk_combo_box_set_active(GTK_COMBO_BOX(states), -1);
	gtk_combo_box_set_active(GTK_COMBO_BOX(stations), -1);
    }
#ifndef OS2008
/* add param for close button handler */
    if(need_correct_layout_for_OS2007)
	g_object_set_data(G_OBJECT(config_window),
			    "need_correct_layout_for_OS2007",
			    (gpointer)1 );
#endif
/* save settings */
    config_save(app->config);
    redraw_home_window(FALSE);
/* disable button */
    gtk_widget_set_sensitive(button, FALSE);
    if(app->config->ui_background_color_on)
	set_background_color(button, &(app->config->ui_background_color));
/* store current settings state */
    app->stations_tab_start_state = app->stations_tab_current_state;
    app->visuals_tab_start_state = app->visuals_tab_current_state;
    app->display_tab_start_state = app->display_tab_current_state;
    app->units_tab_start_state = app->units_tab_current_state;
    app->update_tab_start_state = app->update_tab_current_state;
#ifdef OS2008
    app->sensor_tab_start_state = app->sensor_tab_current_state;
#endif
}
/*******************************************************************************/
void close_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
    GtkWidget	*config_window = GTK_WIDGET(user_data),
		*notebook = NULL;
    guint	current_page = 0;
    gboolean	need_update_weather = FALSE;
    GSList	*iconset = NULL,
		*tmp = NULL;
#ifndef OS2008
    gboolean	need_correct_layout_for_OS2007 = FALSE;
#endif
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* free memory used by iconset list */
    iconset = g_object_get_data(G_OBJECT(config_window), "iconsetlist");
    if(iconset){
        tmp = (GSList*)iconset;
	while(tmp){
	    if(tmp->data)
		g_free((gchar*)(tmp->data));
	    tmp = g_slist_next(tmp);
	}
    }
/* get settings tab number */
    notebook = lookup_widget(config_window, "notebook");
    if(notebook)
	current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    
    if(g_object_get_data(G_OBJECT(user_data), "need_update_weather"))
	need_update_weather = TRUE;
#ifndef OS2008
    if(g_object_get_data(G_OBJECT(user_data), "need_correct_layout_for_OS2007"))
	need_correct_layout_for_OS2007 = TRUE;
#endif
    gtk_widget_destroy(config_window);
/* check if update is needed */
    if(need_update_weather){
	update_weather(TRUE);
	redraw_home_window(FALSE);
    }
#ifndef OS2008
/* check if correct layout needed */
    if(need_correct_layout_for_OS2007)
        hildon_banner_show_information(app->main_window,
                                        NULL,
                                        _("Use Edit layout \nfor tuning images of applet"));
#endif
    app->config->current_settings_page = current_page;
/* save config */
    config_save(app->config);
}
/*******************************************************************************/
void help_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
    GtkWidget	*config = GTK_WIDGET(user_data),
		*notebook = NULL;
    gint	page_number = -1;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    notebook = lookup_widget(config, "notebook");
    if(notebook){
	page_number = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	switch(page_number){
	    case SETTINGS_STATIONS_PAGE:
		help_activated_handler(NULL, OMWEATHER_SETTINGS_STATIONS_HELP_ID);
	    break;
	    case SETTINGS_VISUALS_PAGE:
		help_activated_handler(NULL, OMWEATHER_SETTINGS_VISUALS_HELP_ID);
	    break;
	    case SETTINGS_DISPLAY_PAGE:
		help_activated_handler(NULL, OMWEATHER_SETTINGS_DISPLAY_HELP_ID);
	    break;
	    case SETTINGS_UNITS_PAGE:
		help_activated_handler(NULL, OMWEATHER_SETTINGS_UNITS_HELP_ID);
	    break;
	    case SETTINGS_UPDATE_PAGE:
		help_activated_handler(NULL, OMWEATHER_SETTINGS_UPDATE_HELP_ID);
	    break;
	    case SETTINGS_SENSOR_PAGE:
		help_activated_handler(NULL, OMWEATHER_SETTINGS_SENSOR_HELP_ID);
	    break;
	}
    }
}
/*******************************************************************************/
void back_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
    gint day_number
	    = (gint)g_object_get_data(G_OBJECT(user_data), "day_number");
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gtk_widget_destroy(GTK_WIDGET(user_data));
    weather_window_popup(NULL, NULL, (gpointer)day_number);
}
/*******************************************************************************/
void entry_changed_handler(GtkWidget *entry, gpointer user_data){
    gchar	*changed_entry_name = NULL;
    GtkWidget	*button = NULL,
		*config_window = GTK_WIDGET(user_data);
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* get pressed gtkedit name */    
    changed_entry_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(entry));

    if(!changed_entry_name)
        return;

    if( !strcmp(changed_entry_name, "rename_entry") ){/* check rename entry */
	button = lookup_widget(config_window, "apply_rename_button_name");
	if(button){
	    if(strlen(gtk_entry_get_text(GTK_ENTRY(entry))) > 0)
		if(strcmp((char*)gtk_entry_get_text(GTK_ENTRY(entry)),
			    app->config->current_station_name))
		    gtk_widget_set_sensitive(button, TRUE);
		else
		    gtk_widget_set_sensitive(button, FALSE);
	}
    }
    else{
	if( !strcmp(changed_entry_name, "station_code") )/* check code entry */
	    button = lookup_widget(config_window, "add_code_button_name");
	else
	    if( !strcmp(changed_entry_name, "station_name") )/* check name entry */
		button = lookup_widget(config_window, "add_station_button_name");
	/* Change sensitive of button */
	if(strlen(gtk_entry_get_text(GTK_ENTRY(entry))) > 0)
	    gtk_widget_set_sensitive(button, TRUE);
	else
	    gtk_widget_set_sensitive(button, FALSE);
    }
}
/*******************************************************************************/
int lookup_and_select_station(gchar *db_path, gchar *station_name,
								Station *result){

    FILE		*fh_region, *fh_station;
    Region_item  	region;
    GtkListStore	*list = NULL;
    GtkTreeIter		iter;
    char		buffer[512];
    char		buffer_full_name[2048];
    gchar		buff[512];
    Station		station;
    GtkWidget		*window_select_station = NULL,
			*station_list_view = NULL,
			*scrolled_window = NULL,
			*label = NULL,
			*table = NULL;
    gchar		*selected_station_name = NULL;
    long		max_bytes = 0,
			readed_bytes = 0;
    gboolean		valid;
    GtkTreeModel	*model;
    GtkTreeSelection	*selection;
    gchar        	*station_full_name = NULL,
			*station_name_temp = NULL,
                	*station_id0 = NULL;
    double       	station_latitude,
        		station_longtitude;
    int 		return_code = 0;

    /* Prepare */
    memset(result->name, 0, sizeof(result->name));
    memset(result->id0, 0, sizeof(result->id0));
/* prepare file name with path */
    buff[0] = 0;
    snprintf(buff, sizeof(buff) - 1, "%s%s", db_path, REGIONSFILE);
    fh_region = fopen(buff, "rt");
    if(!fh_region){
	fprintf(stderr, "\nCan't read file %s: %s", buff,
		strerror(errno));
	return -1;	
    }
    list = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING,
				 G_TYPE_DOUBLE, G_TYPE_DOUBLE,
				 G_TYPE_STRING);
/* prepare file name and path */
    buff[0] = 0;
    snprintf(buff, sizeof(buff) - 1, "%s%s", db_path, LOCATIONSFILE);
    /* Reading region settings */
    while(!feof(fh_region)){
	memset(buffer, 0, sizeof(buffer));
	fgets(buffer, sizeof(buffer) - 1, fh_region);
        parse_region_string(buffer,&region);
	fh_station = fopen(buff, "rt");
	if(!fh_station){
	    fprintf(stderr, "\nCan't read file %s: %s", buff, 
		    strerror(errno));
	    return -1;	
	}
	max_bytes = region.end - region.start;
	readed_bytes = 0;
	if(region.start > -1)
	    if(fseek(fh_station, region.start, SEEK_SET)){
		fprintf(stderr,
			"\nCan't seek to the position %ld on LOCATIONSFILE file: %s\n",
			region.start, strerror(errno));
		return -1;
	    }
	
	while(!feof(fh_station)){
	    memset(buffer, 0, sizeof(buffer));
	    fgets(buffer, sizeof(buffer) - 1, fh_station);
	    readed_bytes += strlen(buffer);
	    if(!parse_station_string(buffer, &station)){
		if (strcasestr(station.name,station_name)){
		    gtk_list_store_append(list, &iter);
		    snprintf(buffer_full_name,sizeof(buffer_full_name) - 1,
					    "%s,%s",region.name,station.name);
		    gtk_list_store_set(list, &iter,
					0, buffer_full_name,
					1, station.id0,
					2, station.latitude,
					3, station.longtitude,
					4, station.name,
					-1);
		}
	    }
	    if(region.start > -1 && region.end > -1 && readed_bytes >= max_bytes)
		break;
	}
	fclose(fh_station);
    }
    fclose(fh_region);
    /* Create dialog window */
    window_select_station = gtk_dialog_new_with_buttons(_("Select Station"),
        						NULL,
							GTK_DIALOG_MODAL,
							NULL);
    /* Add buttons */
    gtk_dialog_add_button(GTK_DIALOG(window_select_station),
        		    _("OK"), GTK_RESPONSE_ACCEPT);
    gtk_dialog_add_button(GTK_DIALOG(window_select_station),
        		    _("Cancel"), GTK_RESPONSE_REJECT);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_select_station)->vbox),
        		    table = gtk_table_new(2, 2, FALSE), TRUE, TRUE, 0);	    

   /* Add Label and Edit field for station name */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("List of the found station(s):")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f),
        			0, 1, 1, 2);
    /* Stations list */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window),
					GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 500, 280);

    station_list_view = create_tree_view(list);
    gtk_container_add(GTK_CONTAINER(scrolled_window),
                	GTK_WIDGET(station_list_view));

    gtk_container_add(GTK_CONTAINER(label), scrolled_window);
    /* set size for dialog */
    gtk_widget_set_size_request(GTK_WIDGET(window_select_station), 550, -1);
    gtk_widget_show_all(window_select_station);

    /* start dialog */
    switch(gtk_dialog_run(GTK_DIALOG(window_select_station))){
	case GTK_RESPONSE_ACCEPT:/* Press Button Ok */
	    /* Lookup selected item */
	    model = gtk_tree_view_get_model(GTK_TREE_VIEW(station_list_view));
	    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));
	    if( !gtk_tree_selection_get_selected(selection, NULL, &iter) )
		return -1;
	    gtk_tree_model_get(model, &iter, 0, &selected_station_name, -1); 
	    
	    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list),
                                                  &iter);
	    while(valid){
    		gtk_tree_model_get(GTK_TREE_MODEL(list),
                        	    &iter,
                        	    0, &station_full_name,
				    1, &station_id0,
				    2, &station_latitude,
				    3, &station_longtitude,
				    4, &station_name_temp,
                        	    -1);
    		if(!strcmp(selected_station_name, station_full_name)){
    		    /* copy selected station to result */
    		    memcpy(result->name, station_name_temp, ((sizeof(result->name) - 1) > ((int)strlen(station_name_temp)) ?
				    ((int)strlen(station_name_temp)) : (sizeof(result->name) - 1)));
    		    memcpy(result->id0, station_id0, ((sizeof(result->id0) - 1) > ((int)strlen(station_id0)) ?
				    ((int)strlen(station_id0)) : (sizeof(result->id0) - 1)));
		}
		valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list),
                                                        &iter);
    	    }
    	    return_code = 0;
	break;
	default: 
	    return_code = -1;
	break;
    }
    if(selected_station_name)
        g_free(selected_station_name);
    gtk_widget_destroy(window_select_station);

    return return_code;
}
/*******************************************************************************/
void add_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
    GtkWidget		*config = GTK_WIDGET(user_data),
			*station_name_entry = NULL,
		        *station_code_entry = NULL,
			*stations = NULL,
			*stations_list_view = NULL;
    gchar		*pressed_button = NULL;
    GtkTreeModel	*model = NULL;
    GtkTreeIter		iter;
    gchar		*station_name = NULL,
			*station_code = NULL;
    gboolean            station_code_invalid = TRUE;
    Station		select_station;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* get pressed button name */    
    pressed_button = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));

    if(!pressed_button)
        return;

    if( !strcmp((char*)pressed_button, "add_name") ){
	station_name_entry = lookup_widget(config, "station_name_entry");
	if(!lookup_and_select_station(weather_sources[app->config->weather_source].db_path,
					(gchar*)gtk_entry_get_text((GtkEntry*)station_name_entry), &select_station)){
	    add_station_to_user_list(g_strdup(select_station.name),
	                                g_strdup(select_station.id0),
	                                FALSE, app->config->weather_source);
	    config_save(app->config);
	    gtk_entry_set_text(((GtkEntry*)station_name_entry),"");
	    /* set need update weather flag for close button handler */
	    g_object_set_data(G_OBJECT(config),	"need_update_weather",
				(gpointer) 1);
	}
	else{/* no one station found */
	    hildon_banner_show_information(app->main_window,
					    NULL,
					    _("No one station found! May be you don't have installed omweather-*-station-db packet"));
	    gtk_entry_set_text((GtkEntry*)station_name_entry, "");
	}
    }
    else{
        if(!strcmp((char*)pressed_button, "add_code")){
	    station_code_entry = lookup_widget(config, "station_code_entry");
	    station_code_invalid
		= check_station_code(app->config->weather_source,
					gtk_entry_get_text((GtkEntry*)station_code_entry));
	    if(!station_code_invalid){
	        add_station_to_user_list(g_strdup(gtk_entry_get_text((GtkEntry*)station_code_entry)),
	                                 g_strdup(gtk_entry_get_text((GtkEntry*)station_code_entry)), 
	                                 FALSE, app->config->weather_source);
	        config_save(app->config);
	        gtk_entry_set_text(((GtkEntry*)station_code_entry),"");
	        /* disable add button */
		gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
		/* set need update weather flag for close button handler */
		g_object_set_data(G_OBJECT(config), "need_update_weather",
				    (gpointer) 1);
	    }else{
	    /* Need Error window */
	    }
        }
        else{
	    stations = lookup_widget(config, "stations");
	    if(stations){
	        if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(stations), &iter)){
		    model = gtk_combo_box_get_model(GTK_COMBO_BOX(stations));
		    gtk_tree_model_get(model, &iter, 0, &station_name,
					   1, &station_code, -1);
		    add_station_to_user_list(station_name, station_code,
						FALSE, app->config->weather_source);
		    g_free(station_name);
		    g_free(station_code);
		    config_save(app->config);
		    /* set selected station to nothing */
		    gtk_combo_box_set_active((GtkComboBox*)stations, -1);
		    /* disable add button */
		    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
		    /* set need update weather flag for close button handler */
		    g_object_set_data(G_OBJECT(config), "need_update_weather",
					(gpointer) 1);
		}
	    }
	}
    }
    stations_list_view = lookup_widget(config, "station_list_view");
    if(stations_list_view){
	highlight_current_station(GTK_TREE_VIEW(stations_list_view));
	redraw_home_window(FALSE);
    }
}
/*******************************************************************************/
void rename_button_handler(GtkWidget *button, GdkEventButton *event,
							    gpointer user_data){
    GtkWidget		*config_window = GTK_WIDGET(user_data),
		        *rename_entry = NULL;
    gboolean		valid = FALSE;
    GtkTreeIter		iter;
    gchar		*new_station_name = NULL,
			*station_name = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* check where the station name is changed */
    rename_entry = lookup_widget(config_window, "rename_entry");
    if(rename_entry){
	new_station_name = (gchar*)gtk_entry_get_text(GTK_ENTRY(rename_entry));
	if(strcmp(app->config->current_station_name, new_station_name)){
    	    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                                  &iter);
	    while(valid){
    		gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                        	    &iter, 0, &station_name, -1);
    		if(!strcmp(app->config->current_station_name, station_name)){
		    /* update current station name */
		    g_free(station_name);
		    gtk_list_store_remove(app->user_stations_list, &iter);
                    add_station_to_user_list(g_strdup(new_station_name),
					    app->config->current_station_id,
					    FALSE, app->config->current_station_source);
		    if(app->config->current_station_name)
			g_free(app->config->current_station_name);
		    app->config->current_station_name = g_strdup(new_station_name);
		    gtk_widget_set_sensitive(button, FALSE);
        	    break;
		}
		else
		    g_free(station_name);
		valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                    	    &iter);
	    }
	}
    }
/* save settings */
    config_save(app->config);
    redraw_home_window(FALSE);
}
/*******************************************************************************/
void check_buttons_changed_handler(GtkToggleButton *button, gpointer user_data){
    gchar	*button_name = NULL,
		*iconset_name = NULL;
    GtkWidget	*config_window = NULL,
		*apply_button = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    config_window = GTK_WIDGET(user_data);
    apply_button = lookup_widget(config_window, "apply_button");
    button_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
/* layout */
    if(!strcmp(button_name, "one_row")){
    	if(gtk_toggle_button_get_active(button))
	    app->visuals_tab_current_state |= STATE_ONE_ROW;
	else
	    app->visuals_tab_current_state &= ~STATE_ONE_ROW;
	goto check;
    }
    if(!strcmp(button_name, "one_column")){
    	if(gtk_toggle_button_get_active(button))
	    app->visuals_tab_current_state |= STATE_ONE_COLUMN;
	else
	    app->visuals_tab_current_state &= ~STATE_ONE_COLUMN;
	goto check;
    }
    if(!strcmp(button_name, "two_rows")){
    	if(gtk_toggle_button_get_active(button))
	    app->visuals_tab_current_state |= STATE_TWO_ROWS;
	else
	    app->visuals_tab_current_state &= ~STATE_TWO_ROWS;
	goto check;
    }
    if(!strcmp(button_name, "two_columns")){
    	if(gtk_toggle_button_get_active(button))
	    app->visuals_tab_current_state |= STATE_TWO_COLUMNS;
	else
	    app->visuals_tab_current_state &= ~STATE_TWO_COLUMNS;
	goto check;
    }
    if(!strcmp(button_name, "combination")){
    	if(gtk_toggle_button_get_active(button))
	    app->visuals_tab_current_state |= STATE_COMBINATION;
	else
	    app->visuals_tab_current_state &= ~STATE_COMBINATION;
	goto check;
    }
/* iconset */
    if(!strcmp(button_name, "iconset")){
	iconset_name = g_object_get_data(G_OBJECT(button), "name");
        if(iconset_name){
	    if(strcmp(iconset_name, app->config->icon_set))
		app->visuals_tab_current_state |= STATE_ICONSET;
	    else
		app->visuals_tab_current_state &= ~STATE_ICONSET;
	    goto check;
	}
    }
/* theme override */
    if(!strcmp(button_name, "theme_override")){
	if(gtk_toggle_button_get_active(button))
	    app->visuals_tab_current_state |= STATE_THEME_OVERRIDE;
	else
	    app->visuals_tab_current_state &= ~STATE_THEME_OVERRIDE;
	goto check;
    }
#ifndef OS2008
/* transparency */
    if(!strcmp(button_name, "transparency")){
	if(gtk_toggle_button_get_active(button))
	    app->visuals_tab_current_state |= STATE_TRANSPARENCY;
	else
	    app->visuals_tab_current_state &= ~STATE_TRANSPARENCY;
	goto check;
    }
#endif
/* temperature */
    if(!strcmp(button_name, "celcius")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_CELCIUS;
	else
	    app->units_tab_current_state &= ~STATE_CELCIUS;
	goto check;
    }
    if(!strcmp(button_name, "fahrenheit")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_FAHRENHEIT;
	else
	    app->units_tab_current_state &= ~STATE_FAHRENHEIT;
	goto check;
    }
/* distance units */
    if(!strcmp(button_name, "meters")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_METERS;
	else
	    app->units_tab_current_state &= ~STATE_METERS;
	goto check;
    }
    if(!strcmp(button_name, "kilometers")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_KILOMETERS;
	else
	    app->units_tab_current_state &= ~STATE_KILOMETERS;
	goto check;
    }
    if(!strcmp(button_name, "sea_miles")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_SEA_MILES;
	else
	    app->units_tab_current_state &= ~STATE_SEA_MILES;
	goto check;
    }
    if(!strcmp(button_name, "miles")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_MILES;
	else
	    app->units_tab_current_state &= ~STATE_MILES;
	goto check;
    }
    if(!strcmp(button_name, "wind_meters")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_METERS_S;
	else
	    app->units_tab_current_state &= ~STATE_METERS_S;
	goto check;
    }
    if(!strcmp(button_name, "wind_kilometers")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_KILOMETERS_H;
	else
	    app->units_tab_current_state &= ~STATE_KILOMETERS_H;
	goto check;
    }
    if(!strcmp(button_name, "wind_miles")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_MILES_H;
	else
	    app->units_tab_current_state &= ~STATE_MILES_H;
	goto check;
    }
/* pressure */
    if(!strcmp(button_name, "pressure")){
	if(gtk_toggle_button_get_active(button))
	    app->units_tab_current_state |= STATE_PRESSURE_MB;
	else
	    app->units_tab_current_state &= ~STATE_PRESSURE_MB;
	goto check;
    }

#ifdef OS2008
    if(!strcmp(button_name, "enable_gps")){
	if(gtk_toggle_button_get_active(button))
	    app->stations_tab_current_state |= STATE_ENABLE_GPS;
	else
	    app->stations_tab_current_state &= ~STATE_ENABLE_GPS;
	goto check;
    }
#endif
    if(!strcmp(button_name, "separate")){
	if(gtk_toggle_button_get_active(button))
	    app->display_tab_current_state |= STATE_SHOW_CURRENT;
	else
	    app->display_tab_current_state &= ~STATE_SHOW_CURRENT;
	goto check;
    }
    if(!strcmp(button_name, "swap_temperature")){
	if(gtk_toggle_button_get_active(button))
	    app->display_tab_current_state |= STATE_SWAP_TEMPERATURE;
	else
	    app->display_tab_current_state &= ~STATE_SWAP_TEMPERATURE;
	goto check;
    }
    if(!strcmp(button_name, "show_wind")){
	if(gtk_toggle_button_get_active(button))
	    app->display_tab_current_state |= STATE_SHOW_WIND;
	else
	    app->display_tab_current_state &= ~STATE_SHOW_WIND;
	goto check;
    }
    if(!strcmp(button_name, "show_station_name")){
	if(gtk_toggle_button_get_active(button))
	    app->display_tab_current_state |= STATE_SHOW_STATION_NAME;
	else
	    app->display_tab_current_state &= ~STATE_SHOW_STATION_NAME;
	goto check;
    }
    if(!strcmp(button_name, "show_arrows")){
	if(gtk_toggle_button_get_active(button))
	    app->display_tab_current_state |= STATE_SHOW_ARROWS;
	else
	    app->display_tab_current_state &= ~STATE_SHOW_ARROWS;
	goto check;
    }

    if(!strcmp(button_name, "download_after_connection")){
	if(gtk_toggle_button_get_active(button))
	    app->update_tab_current_state |= STATE_AUTO_CONNECT;
	else
	    app->update_tab_current_state &= ~STATE_AUTO_CONNECT;
	goto check;
    }
#if defined(OS2008) || defined(DEBUGTEMP)
/* use sensor */
    if(!strcmp(button_name, "use_sensor")){
	if(gtk_toggle_button_get_active(button))
	    app->sensor_tab_current_state |= STATE_USE_SENSOR;
	else
	    app->sensor_tab_current_state &= ~STATE_USE_SENSOR;
	goto check;
    }
/* display at name */
    if(!strcmp(button_name, "display_at_name")){
	if(gtk_toggle_button_get_active(button))
	    app->sensor_tab_current_state |= STATE_SHOW_AT_NAME;
	else
	    app->sensor_tab_current_state &= ~STATE_SHOW_AT_NAME;
	goto check;
    } 
/* display at icon */
    if(!strcmp(button_name, "display_at_icon")){
	if(gtk_toggle_button_get_active(button))
	    app->sensor_tab_current_state |= STATE_SHOW_AT_ICON;
	else
	    app->sensor_tab_current_state &= ~STATE_SHOW_AT_ICON;
	goto check;
    } 
#endif
    return;
/* if previos state not equal current state than enable apply button */
check:
    if((app->stations_tab_current_state != app->stations_tab_start_state) ||
    	    (app->visuals_tab_current_state != app->visuals_tab_start_state) ||
	    (app->display_tab_current_state != app->display_tab_start_state) ||
	    (app->units_tab_current_state != app->units_tab_start_state) ||
	    (app->update_tab_current_state != app->update_tab_start_state)
#ifdef OS2008
									 ||
	    (app->sensor_tab_current_state != app->sensor_tab_start_state)
#endif
									    )
	gtk_widget_set_sensitive(GTK_WIDGET(apply_button), TRUE);
    else
	gtk_widget_set_sensitive(GTK_WIDGET(apply_button), FALSE);
}
/*******************************************************************************/
void color_buttons_changed_handler(GtkColorButton *button, gpointer user_data){
    gchar	*button_name = NULL;
    GdkColor	tmp_color;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    button_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
    if(!strcmp(button_name, "font_color")){
	gtk_color_button_get_color(button, &tmp_color);
	if( (app->config->font_color.red - tmp_color.red) ||
		(app->config->font_color.green - tmp_color.green) ||
		(app->config->font_color.blue - tmp_color.blue) )
	    app->visuals_tab_current_state |= STATE_FONT_COLOR;
	else
	    app->visuals_tab_current_state &= ~STATE_FONT_COLOR;
	goto check;
    }
    if(!strcmp(button_name, "background_color")){
	gtk_color_button_get_color(button, &tmp_color);
	if( (app->config->background_color.red - tmp_color.red) ||
		(app->config->background_color.green - tmp_color.green) ||
		(app->config->background_color.blue - tmp_color.blue) )
	    app->visuals_tab_current_state |= STATE_BACKGROUND_COLOR;
	else
	    app->visuals_tab_current_state &= ~STATE_BACKGROUND_COLOR;
	goto check;
    }
    return;
check:
/* enable or disable apply button */
    if((app->stations_tab_current_state != app->stations_tab_start_state) ||
    	    (app->visuals_tab_current_state != app->visuals_tab_start_state) ||
	    (app->display_tab_current_state != app->display_tab_start_state) ||
	    (app->units_tab_current_state != app->units_tab_start_state) ||
	    (app->update_tab_current_state != app->update_tab_start_state)
									    ||
#ifdef OS2008
	    (app->sensor_tab_current_state != app->sensor_tab_start_state)
#endif
									    )
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
    else
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
}
/*******************************************************************************/
void combo_boxs_changed_handler(GtkComboBox *combobox, gpointer user_data){
    gchar	*combobox_name = NULL;
    gint	something = -1,
		count = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    combobox_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(combobox));
/* weather source */
    if(!strcmp(combobox_name, "weather_source")){
	if(gtk_combo_box_get_active(combobox) != app->config->weather_source)
	    app->stations_tab_current_state |= STATE_SOURCE;
	else
	    app->stations_tab_current_state &= ~STATE_SOURCE;
	goto check;
    }
/* switch to next station */
    if(!strcmp(combobox_name, "time2switch")){
	if(gtk_combo_box_get_active(combobox) != app->config->switch_time / 10)
	    app->update_tab_current_state |= STATE_SWITCH_TO_NEXT;
	else
	    app->update_tab_current_state &= ~STATE_SWITCH_TO_NEXT;
	goto check;
    }
/* valid time */
    if(!strcmp(combobox_name, "valid_time")){
	something = app->config->data_valid_interval / 3600;
	while(something >>= 1)
	    count++;
	something = count;
	if(gtk_combo_box_get_active(combobox) != something)
	    app->update_tab_current_state |= STATE_VALID_TIME;
	else
	    app->update_tab_current_state &= ~STATE_VALID_TIME;
	goto check;
    }
/* update time */
    if(!strcmp(combobox_name, "update_time")){
        something = get_active_item_index((GtkTreeModel*)app->time_update_list,
					    app->config->update_interval,
					    NULL, TRUE);
	if(gtk_combo_box_get_active(combobox) != something)
	    app->update_tab_current_state |= STATE_UPDATING_TIME;
	else
	    app->update_tab_current_state &= ~STATE_UPDATING_TIME;
	goto check;
    }
/* sensor read time */
    if(!strcmp(combobox_name, "sensor_update_time")){
	if(gtk_combo_box_get_active(combobox) !=
		get_index_from_time(app->config->sensor_update_time))
	    app->sensor_tab_current_state |= STATE_SENSOR_READ_TIME;
	else
	    app->sensor_tab_current_state &= ~STATE_SENSOR_READ_TIME;
	goto check;
    }
    return;
check:
/* if previos state not equal current state than enable apply button */
    if((app->stations_tab_current_state != app->stations_tab_start_state) ||
    	    (app->visuals_tab_current_state != app->visuals_tab_start_state) ||
	    (app->display_tab_current_state != app->display_tab_start_state) ||
	    (app->units_tab_current_state != app->units_tab_start_state) ||
	    (app->update_tab_current_state != app->update_tab_start_state)
									    ||
#ifdef OS2008
	    (app->sensor_tab_current_state != app->sensor_tab_start_state)
#endif
									    )
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
    else
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
}
/*******************************************************************************/
void control_bars_changed_handler(HildonControlbar *control, gpointer user_data){
    gchar	*control_name = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    control_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(control));
    if(!strcmp(control_name, "visible_items_number")){
	if(hildon_controlbar_get_value(control) != app->config->days_to_show - 1)
	    app->display_tab_current_state |= STATE_VISIBLE_ITEMS;
	else
	    app->display_tab_current_state &= ~STATE_VISIBLE_ITEMS;
	goto check;
    }
    if(!strcmp(control_name, "icon_size")){
	if(hildon_controlbar_get_value(control) != app->config->icons_size - 1)
	    app->display_tab_current_state |= STATE_ICON_SIZE;
	else
	    app->display_tab_current_state &= ~STATE_ICON_SIZE;
	goto check;
    }
#ifdef OS2008
    if(!strcmp(control_name, "transparency")){
	if(hildon_controlbar_get_value(control) != app->config->alpha_comp)
	    app->visuals_tab_current_state |= STATE_TRANSPARENCY;
	else
	    app->visuals_tab_current_state &= ~STATE_TRANSPARENCY;
	goto check;
    }
#endif
    return;
check:
/* if previos state not equal current state than enable apply button */
    if((app->stations_tab_current_state != app->stations_tab_start_state) ||
    	    (app->visuals_tab_current_state != app->visuals_tab_start_state) ||
	    (app->display_tab_current_state != app->display_tab_start_state) ||
	    (app->units_tab_current_state != app->units_tab_start_state) ||
	    (app->update_tab_current_state != app->update_tab_start_state)
									    ||
#ifdef OS2008
	    (app->sensor_tab_current_state != app->sensor_tab_start_state)
#endif
									    )
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
    else
	gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
gboolean changed_country_process(gpointer window)
{
	changed_country_handler(NULL,window);
	return FALSE;
}
/*******************************************************************************/
gboolean changed_state_process(gpointer window)
{
	changed_state_handler(NULL,window);
	return FALSE;
}
/*******************************************************************************/
GtkWidget* create_locations_tab(GtkWidget *window){
    static struct lists_struct	list = { NULL, NULL, NULL };
    GtkWidget	*countries = NULL,
		*states = NULL,
		*stations = NULL,
#ifdef OS2008
                *chk_gps = NULL,
#endif
		*left_table = NULL,
		*weather_source = NULL,
		*right_table = NULL,
		*left_right_hbox = NULL,
		*scrolled_window = NULL,
		*apply_rename_button = NULL,
		*up_station_button = NULL,
		*down_station_button = NULL,
		*delete_station_button = NULL,
		*station_list_view = NULL,
		*up_down_delete_buttons_vbox = NULL,
		*station_name = NULL,
		*station_code = NULL,
		*add_station_button = NULL,
		*add_station_button1 = NULL,
		*add_station_button2 = NULL,
		*apply_button = NULL,
		*rename_entry = NULL;
    gint	i = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->stations_tab_start_state = 0;
    g_object_set_data(G_OBJECT(window), "list", (gpointer)&list);
    left_right_hbox = gtk_hbox_new(FALSE, 0);
/* Locations tab */
    /* left side */
    left_table = gtk_table_new(3, 2, FALSE);
    gtk_box_pack_start(GTK_BOX(left_right_hbox), left_table,
			TRUE, TRUE, 0);
/* Rename station entry */
    rename_entry = gtk_entry_new();
    GLADE_HOOKUP_OBJECT(window, rename_entry, "rename_entry");
    gtk_widget_set_name(rename_entry, "rename_entry");
    g_signal_connect(G_OBJECT(rename_entry), "changed",
			G_CALLBACK(entry_changed_handler),
			(gpointer)window);

    gtk_table_attach_defaults(GTK_TABLE(left_table), 
				rename_entry,
				0, 1, 0, 1);
/* Rename apply button */
    apply_rename_button =
	    create_button_with_image(BUTTON_ICONS, "apply", 30, FALSE, FALSE);
    GLADE_HOOKUP_OBJECT(window, apply_rename_button , "apply_rename_button_name");
    gtk_widget_set_name(apply_rename_button, "apply_rename_button");
    g_signal_connect(G_OBJECT(apply_rename_button), "button_press_event",
                	G_CALLBACK(rename_button_handler),
			(gpointer)window);
    gtk_table_attach_defaults(GTK_TABLE(left_table), 
				apply_rename_button,
				1, 2, 0, 1);
    gtk_widget_set_sensitive(GTK_WIDGET(apply_rename_button), FALSE);

    /* Stations list */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window),
					GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 220, 280);

    station_list_view = create_tree_view(app->user_stations_list);
    GLADE_HOOKUP_OBJECT(window, station_list_view, "station_list_view");
    gtk_container_add(GTK_CONTAINER(scrolled_window),
                	GTK_WIDGET(station_list_view));
    gtk_table_attach_defaults(GTK_TABLE(left_table), 
				scrolled_window,
                    		0, 1, 1, 2);
    
/* Up Station and Down Station and Delete Station Buttons */
    up_down_delete_buttons_vbox = gtk_vbox_new(FALSE, 5);
    gtk_table_attach_defaults(GTK_TABLE(left_table), 
				up_down_delete_buttons_vbox,
                    		1, 2, 1, 2);
/* prepare up_station_button */    
    up_station_button = create_button_with_image(NULL, "qgn_indi_arrow_up", 16, TRUE, FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(up_station_button), 30, -1);
    g_signal_connect(up_station_button, "clicked",
			G_CALLBACK(up_key_handler),
			(gpointer)station_list_view);
/* prepare down_station_button */    
    down_station_button = create_button_with_image(NULL, "qgn_indi_arrow_down", 16, TRUE, FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(down_station_button), 30, -1);
    g_signal_connect(down_station_button, "clicked",
		    	G_CALLBACK(down_key_handler),
			(gpointer)station_list_view);
/* prepare delete_station_button */    
    delete_station_button = create_button_with_image(BUTTON_ICONS, "red", 30, TRUE, FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(delete_station_button), 30, -1);
    g_signal_connect(delete_station_button, "clicked",
                	G_CALLBACK(delete_station_handler),
			(gpointer)window);
/* Pack Up, Down and Delete buttons */
    gtk_box_pack_start(GTK_BOX(up_down_delete_buttons_vbox), up_station_button,
                        TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(up_down_delete_buttons_vbox), delete_station_button,
                        FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(up_down_delete_buttons_vbox), down_station_button,
                        TRUE, TRUE, 0);
#ifdef OS2008
/* GPS */
    gtk_table_attach_defaults(GTK_TABLE(left_table),
				chk_gps = gtk_check_button_new_with_label(_("Enable GPS")),
				0, 1, 2, 3);
    GLADE_HOOKUP_OBJECT(window, chk_gps, "enable_gps");
    gtk_widget_set_name(chk_gps, "enable_gps");
    g_signal_connect(chk_gps, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			(gpointer)window);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chk_gps),
        			    app->config->gps_station);
    if(app->config->gps_station)
	app->stations_tab_start_state |= STATE_ENABLE_GPS;
    else
	app->stations_tab_start_state &= ~STATE_ENABLE_GPS;
#endif
    /* right side */
    right_table = gtk_table_new(10, 3, FALSE);
    gtk_box_pack_start(GTK_BOX(left_right_hbox), right_table,
                        TRUE, TRUE, 0);
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				gtk_label_new(_("Add")),
        			1, 2, 0, 1);
    /* source label */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				gtk_label_new(_("Source:")),
        			0, 1, 1, 2);
    /* source list */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				weather_source = gtk_combo_box_new_text(),
        			1, 2, 1, 2);
    for(i = 0; i < MAX_WEATHER_SOURCE_NUMBER; i++)
	gtk_combo_box_append_text(GTK_COMBO_BOX(weather_source),
				    weather_sources[i].name);
    gtk_combo_box_set_active(GTK_COMBO_BOX(weather_source),
				app->config->weather_source);
/*  delete on 0.22 release */
    gtk_widget_set_sensitive(weather_source, FALSE);

    GLADE_HOOKUP_OBJECT(window, weather_source, "weather_source");
    gtk_widget_set_name(weather_source, "weather_source");
    apply_button = lookup_widget(window, "apply_button");    
    g_signal_connect(weather_source, "changed",
            		G_CALLBACK(combo_boxs_changed_handler),
			apply_button);
    /* label By name */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				gtk_label_new(_("Name:")),
                    		0, 1, 2, 3);
    /* entry for station name */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				station_name = gtk_entry_new(),
                    		1, 2, 2, 3);
    GLADE_HOOKUP_OBJECT(window, station_name, "station_name_entry");
    gtk_widget_set_name(station_name, "station_name");
    g_signal_connect(G_OBJECT(station_name), "changed",
			G_CALLBACK(entry_changed_handler),
			(gpointer)window);
    /* add station button */
    add_station_button = create_button_with_image(BUTTON_ICONS, "add", 30, FALSE, FALSE);
    gtk_widget_set_size_request(add_station_button, 30, 30);
    gtk_widget_set_name(add_station_button, "add_name");
    GLADE_HOOKUP_OBJECT(window, add_station_button, "add_station_button_name");
    gtk_widget_set_sensitive(add_station_button, FALSE);
    g_signal_connect(G_OBJECT(add_station_button), "button_press_event",
			G_CALLBACK(add_button_handler),
			(gpointer)window);
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				add_station_button,
				2, 3, 2, 3);
    /* label By (Zip) code */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				gtk_label_new(_("Code (Zip):")),
                    		0, 1, 3, 4);
    /* entry for station name */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				station_code = gtk_entry_new(),
                    		1, 2, 3, 4);
    gtk_widget_set_name(station_code, "station_code");
    GLADE_HOOKUP_OBJECT(window, station_code, "station_code_entry");
    g_signal_connect(G_OBJECT(station_code), "changed",
			G_CALLBACK(entry_changed_handler),
			(gpointer)window);

    /* add button */
    add_station_button1 = create_button_with_image(BUTTON_ICONS, "add", 30, FALSE, FALSE);
    gtk_widget_set_size_request(add_station_button1, 30, 30);
    gtk_widget_set_name(add_station_button1, "add_code");
    GLADE_HOOKUP_OBJECT(window, add_station_button1, "add_code_button_name");
    gtk_widget_set_sensitive(add_station_button1, FALSE);
    g_signal_connect(G_OBJECT(add_station_button1), "button_press_event",
			G_CALLBACK(add_button_handler),
			(gpointer)window);
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				add_station_button1,
				2, 3, 3, 4);

    /* Label */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				gtk_label_new(_("From the list:")),
                    		1, 2, 5, 6);
    /* Countries label */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				gtk_label_new(_("Country:")),
                    		0, 1, 6, 7);
    /* countries list  */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				countries = gtk_combo_box_new_text(),
				1, 2, 6, 7);
    list.countries = countries;
    gtk_combo_box_set_row_span_column(GTK_COMBO_BOX(countries), 0);
    gtk_combo_box_set_model(GTK_COMBO_BOX(countries),
			    (GtkTreeModel*)app->countrys_list);
    gtk_widget_show(countries);
    /* States label */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				gtk_label_new(_("State:")),
                    		0, 1, 7, 8);
    /* states list */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				states = gtk_combo_box_new_text(),
				1, 2, 7, 8);
    list.states = states;
    gtk_widget_show(states);
    /* Stations label */
    gtk_table_attach_defaults(GTK_TABLE(right_table), 
				gtk_label_new(_("City:")),
                    		0, 1, 8, 9);
    /* stations list */
    gtk_table_attach_defaults(GTK_TABLE(right_table),
				stations = gtk_combo_box_new_text(),
				1, 2, 8, 9);
    list.stations = stations;
    gtk_widget_show(stations);
    GLADE_HOOKUP_OBJECT(window, GTK_WIDGET(stations), "stations");
    /* add button */
    add_station_button2 = create_button_with_image(BUTTON_ICONS, "add", 30, FALSE, FALSE);
    gtk_widget_set_size_request(add_station_button2, 30, 30);
    gtk_widget_set_name(add_station_button2, "add_from_list");
    GLADE_HOOKUP_OBJECT(window, add_station_button2, "add_from_list");
    gtk_widget_set_sensitive(add_station_button2, FALSE);
    g_signal_connect(G_OBJECT(add_station_button2), "button_press_event",
			G_CALLBACK(add_button_handler),
			(gpointer)window);
    gtk_table_attach_defaults(GTK_TABLE(right_table),
				add_station_button2,
				2, 3, 8, 9);
    /* Set size */
    gtk_widget_set_size_request(countries, 300, -1);
    gtk_widget_set_size_request(states, 300, -1);
    gtk_widget_set_size_request(stations, 300, -1);
/* Set default value to country combo_box */
    gtk_combo_box_set_active(GTK_COMBO_BOX(countries),
				get_active_item_index((GtkTreeModel*)app->countrys_list,
				-1, app->config->current_country, TRUE));
    /* fill states list */
		g_idle_add(changed_country_process,(gpointer)window);
    /* fill stations list */
		g_idle_add(changed_state_process,(gpointer)window);
		g_signal_connect(countries, "changed",
            		G_CALLBACK(changed_country_handler),
			(gpointer)window);
    g_signal_connect(states, "changed",
                	G_CALLBACK(changed_state_handler),
			(gpointer)window);
    g_signal_connect(stations, "changed",
            		G_CALLBACK(changed_stations_handler),
			(gpointer)window);
    g_signal_connect(station_list_view, "cursor-changed",
                        G_CALLBACK(station_list_view_select_handler),
			rename_entry);
/* Highlight current station */
    highlight_current_station(GTK_TREE_VIEW(station_list_view));
/* Filling rename entry */
    gtk_entry_set_text(GTK_ENTRY(rename_entry),
			app->config->current_station_name);

    app->stations_tab_current_state = app->stations_tab_start_state;
    return left_right_hbox;
}
/*******************************************************************************/
GtkWidget* create_visuals_tab(GtkWidget *window){
    GtkWidget	*visuals_page = NULL,
		*apply_button = NULL,
		*button = NULL,
		*first_line = NULL,
		*layouts_hbox = NULL,
		*second_line = NULL,
		*iconsets_hbox = NULL,
		*third_line = NULL,
		*theme_override = NULL,
		*fourth_line = NULL,
		*transparency = NULL,
		*fifth_line = NULL,
		*font = NULL,
		*sixth_line = NULL,
		*font_color = NULL,
		*background_color = NULL,
		*one_row_button = NULL,
		*one_column_button = NULL,
		*two_rows_button = NULL,
		*two_columns_button = NULL,
		*combination_button = NULL;
    GSList	*group = NULL,
		*icon_set = NULL,
		*tmp = NULL;
    gchar	buffer[256];
/* Visuals tab */
    app->visuals_tab_start_state = 0;
    visuals_page = gtk_vbox_new(FALSE, 0);
    apply_button = lookup_widget(window, "apply_button");
/* first line */
    first_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(first_line), gtk_label_new(_("Layout:")),
			FALSE, FALSE, 20);
    layouts_hbox = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_end(GTK_BOX(first_line), layouts_hbox, FALSE, FALSE, 20);
    /* make buttons */
    /* one row */
    one_row_button = create_button_with_image(BUTTON_ICONS, "one_row", 40, TRUE, TRUE);
    GLADE_HOOKUP_OBJECT(window, one_row_button, "one_row");
    gtk_widget_set_name(one_row_button, "one_row");
    gtk_box_pack_start(GTK_BOX(layouts_hbox), one_row_button, FALSE, FALSE, 0);
    group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(one_row_button));
    g_signal_connect(one_row_button, "clicked", G_CALLBACK(check_buttons_changed_handler),
                        (gpointer)window);
    /* one column */
    one_column_button = create_button_with_image(BUTTON_ICONS, "one_column", 40, TRUE, TRUE);
    GLADE_HOOKUP_OBJECT(window, one_column_button, "one_column");
    gtk_widget_set_name(one_column_button, "one_column");
    g_object_set_data(G_OBJECT(one_column_button), "number", GINT_TO_POINTER(ONE_COLUMN));
    gtk_box_pack_start(GTK_BOX(layouts_hbox), one_column_button, FALSE, FALSE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(one_column_button), group);
    g_signal_connect(one_column_button, "clicked", G_CALLBACK(check_buttons_changed_handler),
                        (gpointer)window);
    /* two rows */
    two_rows_button = create_button_with_image(BUTTON_ICONS, "two_rows", 40, TRUE, TRUE);
    GLADE_HOOKUP_OBJECT(window, two_rows_button, "two_rows");
    gtk_widget_set_name(two_rows_button, "two_rows");
    gtk_box_pack_start(GTK_BOX(layouts_hbox), two_rows_button, FALSE, FALSE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(two_rows_button),
	    gtk_radio_button_get_group(GTK_RADIO_BUTTON(one_column_button)));
    g_signal_connect(two_rows_button, "clicked", G_CALLBACK(check_buttons_changed_handler),
                        (gpointer)window);
    /* two columns */
    two_columns_button = create_button_with_image(BUTTON_ICONS, "two_columns", 40, TRUE, TRUE);
    GLADE_HOOKUP_OBJECT(window, two_columns_button, "two_columns");
    gtk_widget_set_name(two_columns_button, "two_columns");
    gtk_box_pack_start(GTK_BOX(layouts_hbox), two_columns_button, FALSE, FALSE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(two_columns_button),
	    gtk_radio_button_get_group(GTK_RADIO_BUTTON(two_rows_button)));
    g_signal_connect(two_columns_button, "clicked", G_CALLBACK(check_buttons_changed_handler),
                        (gpointer)window);
    /* combination */
    combination_button = create_button_with_image(BUTTON_ICONS, "combination", 40, TRUE, TRUE);
    GLADE_HOOKUP_OBJECT(window, combination_button, "combination");
    gtk_widget_set_name(combination_button, "combination");
    gtk_box_pack_start(GTK_BOX(layouts_hbox), combination_button, FALSE, FALSE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(combination_button),
	    gtk_radio_button_get_group(GTK_RADIO_BUTTON(two_columns_button)));
    g_signal_connect(combination_button, "clicked", G_CALLBACK(check_buttons_changed_handler),
                        (gpointer)window);
    switch(app->config->icons_layout){
	default:
	case ONE_ROW:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(one_row_button), TRUE);
	    app->visuals_tab_start_state |= STATE_ONE_ROW;
	break;
	case ONE_COLUMN:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(one_column_button), TRUE);
	    app->visuals_tab_start_state |= STATE_ONE_COLUMN;
	break;
	case TWO_ROWS:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(two_rows_button), TRUE);
	    app->visuals_tab_start_state |= STATE_TWO_ROWS;
	break;
	case TWO_COLUMNS:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(two_columns_button), TRUE);
	    app->visuals_tab_start_state |= STATE_TWO_COLUMNS;
	break;
	case COMBINATION:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(combination_button), TRUE);
	    app->visuals_tab_start_state |= STATE_COMBINATION;
	break;
    }
/* second line */
    second_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(second_line), gtk_label_new(_("Icon set:")),
			FALSE, FALSE, 20);
    iconsets_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_end(GTK_BOX(second_line), iconsets_hbox, FALSE, FALSE, 20);
    /* Icon sets */
    group = NULL;
    create_icon_set_list(&icon_set);
    tmp = icon_set;
    while(tmp){
	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer) - 1, "%s%s", ICONS_PATH,
		    (gchar*)(tmp->data));
	button = create_button_with_image(buffer, "44", 40, TRUE, TRUE);
	gtk_radio_button_set_group(GTK_RADIO_BUTTON(button), group);
	group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(button));
	/* store button name */
	gtk_widget_set_name(button, "iconset");
	g_object_set_data(G_OBJECT(button), "name", (gchar*)(tmp->data));
	GLADE_HOOKUP_OBJECT(window, button, (gchar*)(tmp->data));
	gtk_box_pack_start(GTK_BOX(iconsets_hbox), button, FALSE, FALSE, 0);
	if(!strcmp(tmp->data, app->config->icon_set))
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
	g_signal_connect(button, "clicked",
            		    G_CALLBACK(check_buttons_changed_handler),
			    window);
	tmp = g_slist_next(tmp);
    }
    g_object_set_data(G_OBJECT(window), "iconsetlist", icon_set);
/* thrid line */
    third_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(third_line),
		theme_override = gtk_check_button_new_with_label(_("UI Theme Override")),
			FALSE, FALSE, 20);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(theme_override),
        			    app->config->ui_background_color_on);
    if(app->config->ui_background_color_on)
	app->visuals_tab_start_state |= STATE_THEME_OVERRIDE;
    gtk_widget_set_name(theme_override, "theme_override");
    GLADE_HOOKUP_OBJECT(window, theme_override, "theme_override");
    g_signal_connect(theme_override, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
/* fourth line */
    fourth_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(fourth_line),
			gtk_label_new(_("Transparency:")),
			FALSE, FALSE, 20);
#ifdef OS2008
    transparency = hildon_controlbar_new();
    hildon_controlbar_set_min(HILDON_CONTROLBAR(transparency), 0);
    hildon_controlbar_set_max(HILDON_CONTROLBAR(transparency), 100);
    hildon_controlbar_set_value(HILDON_CONTROLBAR(transparency),
				app->config->alpha_comp);
    gtk_scale_set_value_pos(GTK_SCALE(transparency), GTK_POS_LEFT);
#ifndef RELEASE
    fprintf(stderr,"test %i %i %i\n",
	    app->config->alpha_comp,
	    (int)app->config->alpha_comp,
	    hildon_controlbar_get_value(HILDON_CONTROLBAR(transparency)));
#endif
    gtk_widget_set_size_request(transparency, 350, -1);
#else
    transparency = gtk_check_button_new_with_label(_("Transparency:"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(transparency),
        			    app->config->transparency);
    if(app->config->transparency)
	app->visuals_tab_start_state |= STATE_TRANSPARENCY;
    g_signal_connect(transparency, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
#endif
    GLADE_HOOKUP_OBJECT(window, transparency, "transparency");
    gtk_widget_set_name(transparency, "transparency");
    gtk_box_pack_end(GTK_BOX(fourth_line), transparency, FALSE, FALSE, 20);
/* fifth line */
    fifth_line = gtk_hbox_new(FALSE, 0);
/* Font family */
    gtk_box_pack_start(GTK_BOX(fifth_line),
			gtk_label_new(_("Font:")),
			FALSE, FALSE, 20);
    font = gtk_font_button_new_with_font(app->config->font);
    GLADE_HOOKUP_OBJECT(window, font, "font");
/* disable displaying font style at button */
    gtk_font_button_set_show_style(GTK_FONT_BUTTON(font), FALSE);
    gtk_box_pack_start(GTK_BOX(fifth_line), font, FALSE, FALSE, 20);
    g_signal_connect(font, "font-set", G_CALLBACK(font_changed_handler),
			apply_button);
    /* Font color button */
    font_color = gtk_color_button_new();
    GLADE_HOOKUP_OBJECT(window, font_color, "font_color");
    gtk_widget_set_name(font_color, "font_color");
    g_signal_connect(font_color, "color-set",
            		G_CALLBACK(color_buttons_changed_handler),
			apply_button);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(font_color),
				&(app->config->font_color));
    gtk_button_set_relief(GTK_BUTTON(font_color), GTK_RELIEF_NONE);
    gtk_button_set_focus_on_click(GTK_BUTTON(font_color), FALSE);
    gtk_box_pack_end(GTK_BOX(fifth_line), font_color, FALSE, FALSE, 20);
    gtk_box_pack_end(GTK_BOX(fifth_line),
		    gtk_label_new(_("Font color:")),
			FALSE, FALSE, 0);
/* sixth line */
    sixth_line = gtk_hbox_new(FALSE, 0);
    /* Background color */
    background_color = gtk_color_button_new();
    GLADE_HOOKUP_OBJECT(window, background_color, "background_color");
    gtk_widget_set_name(background_color, "background_color");
    g_signal_connect(background_color, "color-set",
            		G_CALLBACK(color_buttons_changed_handler),
			apply_button);
#ifdef OS2008
    g_signal_connect(transparency, "value-changed",
            		G_CALLBACK(control_bars_changed_handler),
			apply_button);
#else
    g_signal_connect(GTK_TOGGLE_BUTTON(transparency), "toggled",
            		    G_CALLBACK(transparency_button_toggled_handler),
			    background_color);
			    
#endif
    gtk_color_button_set_color(GTK_COLOR_BUTTON(background_color),
				&(app->config->background_color));
#ifdef OS2008    
    gtk_widget_set_sensitive(background_color, TRUE);
#else
    if(background_color && app->config->transparency)
        gtk_widget_set_sensitive(background_color, FALSE);	
    else
        gtk_widget_set_sensitive(background_color, TRUE);
#endif
    gtk_button_set_relief(GTK_BUTTON(background_color), GTK_RELIEF_NONE);
    gtk_button_set_focus_on_click(GTK_BUTTON(background_color), FALSE);
    gtk_box_pack_end(GTK_BOX(sixth_line), background_color, FALSE, FALSE, 20);

    gtk_box_pack_end(GTK_BOX(sixth_line),
				gtk_label_new(_("Background color:")),
				FALSE, FALSE, 0);
/* pack lines */
    gtk_box_pack_start(GTK_BOX(visuals_page), first_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(visuals_page), second_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(visuals_page), third_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(visuals_page), fourth_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(visuals_page), fifth_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(visuals_page), sixth_line, TRUE, TRUE, 0);

    app->visuals_tab_current_state = app->visuals_tab_start_state;
    return visuals_page;
}
/*******************************************************************************/
GtkWidget* create_display_tab(GtkWidget *window){
    GtkWidget	*interface_page = NULL,
		*first_line = NULL,
		*second_line = NULL,
		*third_line = NULL,
		*fourth_line = NULL,
		*fifth_line = NULL,
    		*visible_items_number = NULL,
		*icon_size = NULL,
		*show_station_name = NULL,
		*show_arrows = NULL,
		*separate = NULL,
		*swap_temperature = NULL,
		*apply_button = NULL,
		*show_wind = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->display_tab_start_state = 0;
    apply_button = lookup_widget(window, "apply_button");
/* Interface tab */
    interface_page = gtk_vbox_new(FALSE, 0);
/* first line */
    first_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(interface_page), first_line, TRUE, TRUE, 0);
    /* Visible items */
    gtk_box_pack_start(GTK_BOX(first_line),
			gtk_label_new(_("Visible items:")),
			FALSE, FALSE, 20);
    /* Visible items number */
    visible_items_number = hildon_controlbar_new();
    GLADE_HOOKUP_OBJECT(window, visible_items_number, "visible_items_number");
    gtk_widget_set_name(visible_items_number, "visible_items_number");
    g_signal_connect(visible_items_number, "value-changed",
            		G_CALLBACK(control_bars_changed_handler),
			apply_button);
    hildon_controlbar_set_min(HILDON_CONTROLBAR(visible_items_number), 0);
    hildon_controlbar_set_max(HILDON_CONTROLBAR(visible_items_number),
				Max_count_weather_day - 1);
    hildon_controlbar_set_value(HILDON_CONTROLBAR(visible_items_number),
				    app->config->days_to_show - 1);
    gtk_box_pack_end(GTK_BOX(first_line),
			visible_items_number,
			FALSE, FALSE, 20);
    gtk_widget_set_size_request(visible_items_number, 350, -1);
/* second line */
    second_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(interface_page), second_line, TRUE, TRUE, 0);
    /* Icon size */
    gtk_box_pack_start(GTK_BOX(second_line),
			gtk_label_new(_("Icon size:")),
			FALSE, FALSE, 20);
    icon_size = hildon_controlbar_new();
    GLADE_HOOKUP_OBJECT(window, icon_size, "icon_size");
    gtk_widget_set_name(icon_size, "icon_size");
    g_signal_connect(icon_size, "value-changed",
            		G_CALLBACK(control_bars_changed_handler),
			apply_button);
    hildon_controlbar_set_min(HILDON_CONTROLBAR(icon_size), TINY);
    hildon_controlbar_set_max(HILDON_CONTROLBAR(icon_size), GIANT);
    switch(app->config->icons_size - 1){
	case TINY:
	    hildon_controlbar_set_value(HILDON_CONTROLBAR(icon_size), TINY);
	break;
	case SMALL:
	    hildon_controlbar_set_value(HILDON_CONTROLBAR(icon_size), SMALL);
	break;
	case MEDIUM:
	    hildon_controlbar_set_value(HILDON_CONTROLBAR(icon_size), MEDIUM);
	break;
	default:
	case LARGE:
	    hildon_controlbar_set_value(HILDON_CONTROLBAR(icon_size), LARGE);
	break;
	case GIANT:
	    hildon_controlbar_set_value(HILDON_CONTROLBAR(icon_size), GIANT);
	break;
    }
    gtk_box_pack_end(GTK_BOX(second_line), icon_size, FALSE, FALSE, 20);
    gtk_widget_set_size_request(icon_size, 350, -1);
/* third line */
    third_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(interface_page), third_line, TRUE, TRUE, 0);
    /* Separate weather */
    separate = gtk_check_button_new_with_label(_("Show only current weather on first icon"));
    GLADE_HOOKUP_OBJECT(window, separate, "separate");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(separate),
        			    app->config->separate);
    if(app->config->separate)
	app->display_tab_start_state |= STATE_SHOW_CURRENT;
    gtk_widget_set_name(separate, "separate");
    g_signal_connect(separate, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_box_pack_start(GTK_BOX(third_line), separate, FALSE, FALSE, 20);
/* fourth line */
    fourth_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(interface_page), fourth_line, TRUE, TRUE, 0);

    /* Swap temperature */
    gtk_box_pack_start(GTK_BOX(fourth_line),
			swap_temperature = gtk_check_button_new_with_label(_("Swap hi/low temperature")),
			FALSE, FALSE, 20);
    GLADE_HOOKUP_OBJECT(window, swap_temperature, "swap_temperature");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(swap_temperature),
        			    app->config->swap_hi_low_temperature);
    if(app->config->swap_hi_low_temperature)
	app->display_tab_start_state |= STATE_SWAP_TEMPERATURE;
    gtk_widget_set_name(swap_temperature, "swap_temperature");
    g_signal_connect(swap_temperature, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    /* Show wind */
    gtk_box_pack_end(GTK_BOX(fourth_line),
			show_wind = gtk_check_button_new(),
			FALSE, FALSE, 20);
    GLADE_HOOKUP_OBJECT(window, show_wind, "show_wind");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(show_wind),
        			    app->config->show_wind);
    if(app->config->show_wind)
	app->display_tab_start_state |= STATE_SHOW_WIND;
    gtk_widget_set_name(show_wind, "show_wind");
    g_signal_connect(show_wind, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_box_pack_end(GTK_BOX(fourth_line),
        		gtk_label_new(_("Show wind")),
        		FALSE, FALSE, 0);
/* fifth line */
    fifth_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(interface_page), fifth_line, TRUE, TRUE, 0);
    /* Show station name */
    show_station_name = gtk_check_button_new_with_label(_("Show station name"));
    GLADE_HOOKUP_OBJECT(window, show_station_name, "show_station_name");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(show_station_name),
        			    app->config->show_station_name);
    if(app->config->show_station_name)
	app->display_tab_start_state |= STATE_SHOW_STATION_NAME;
    gtk_widget_set_name(show_station_name, "show_station_name");
    g_signal_connect(show_station_name, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_box_pack_start(GTK_BOX(fifth_line),
			show_station_name, FALSE, FALSE, 20);
    /* Show arrows */
    show_arrows = gtk_check_button_new();
    GLADE_HOOKUP_OBJECT(window, show_arrows, "show_arrows");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(show_arrows),
        			    app->config->show_arrows);
    if(app->config->show_arrows)
	app->display_tab_start_state |= STATE_SHOW_ARROWS;
    gtk_widget_set_name(show_arrows, "show_arrows");
    g_signal_connect(show_arrows, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_box_pack_end(GTK_BOX(fifth_line),
			show_arrows, FALSE, FALSE, 20);
    gtk_box_pack_end(GTK_BOX(fifth_line),
			gtk_label_new(_("Show arrows")),
			FALSE, FALSE, 0);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    app->display_tab_current_state = app->display_tab_start_state;
    return interface_page;
}
/*******************************************************************************/
GtkWidget* create_units_tab(GtkWidget *window){
    GSList	*temperature_group = NULL,
		*distance_group = NULL,
		*wind_group = NULL,
		*pressure_group = NULL;
    GtkWidget	*units_page = NULL,
		*celcius_temperature = NULL,
		*fahrenheit_temperature = NULL,
		*distance_meters = NULL,
		*distance_kilometers = NULL,
		*distance_miles = NULL,
		*distance_sea_miles = NULL,
		*wind_meters = NULL,
		*wind_kilometers = NULL,
		*wind_miles = NULL,
		*mb_pressure = NULL,
		*inch_pressure = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    units_page = gtk_table_new(7, 3, FALSE);
/* Units tab */
    app->units_tab_start_state = 0U;
    /* temperature */
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				gtk_label_new(_("Temperature units:")),
				0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				celcius_temperature
				    = gtk_radio_button_new_with_label(NULL,
									_("Celcius")),
				1, 2, 0, 1);
    GLADE_HOOKUP_OBJECT(window, celcius_temperature, "temperature");
    gtk_widget_set_name(celcius_temperature, "celcius");
    g_signal_connect(celcius_temperature, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    temperature_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(celcius_temperature));
    gtk_button_set_focus_on_click(GTK_BUTTON(celcius_temperature), FALSE);
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				fahrenheit_temperature
				    = gtk_radio_button_new_with_label(temperature_group,
									_("Fahrenheit")),
				2, 3, 0, 1);
    gtk_widget_set_name(fahrenheit_temperature, "fahrenheit");
    g_signal_connect(fahrenheit_temperature, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_button_set_focus_on_click(GTK_BUTTON(fahrenheit_temperature), FALSE);
    if(app->config->temperature_units == CELSIUS){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(celcius_temperature), TRUE);
	app->units_tab_start_state |= STATE_CELCIUS;
    }
    else{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fahrenheit_temperature), TRUE);
	app->units_tab_start_state |= STATE_FAHRENHEIT;
    }
    /* distance */
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				gtk_label_new(_("Distance units:")),
				0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				distance_meters
				    = gtk_radio_button_new_with_label(NULL, _("Meters")),
				1, 2, 2, 3);
    GLADE_HOOKUP_OBJECT(window, distance_meters, "meters");
    gtk_widget_set_name(distance_meters, "meters");
    g_signal_connect(distance_meters, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    distance_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(distance_meters));
    gtk_button_set_focus_on_click(GTK_BUTTON(distance_meters), FALSE);

    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				distance_kilometers
				    = gtk_radio_button_new_with_label(distance_group,
									_("Kilometers")),
				2, 3, 2, 3);
    GLADE_HOOKUP_OBJECT(window, distance_kilometers, "kilometers");
    gtk_widget_set_name(distance_kilometers, "kilometers");
    g_signal_connect(distance_kilometers, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_button_set_focus_on_click(GTK_BUTTON(distance_kilometers), FALSE);

    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				distance_miles
				    = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(distance_kilometers)),
									_("Miles")),
				1, 2, 3, 4);
    GLADE_HOOKUP_OBJECT(window, distance_miles, "miles");
    gtk_widget_set_name(distance_miles, "miles");
    g_signal_connect(distance_miles, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_button_set_focus_on_click(GTK_BUTTON(distance_miles), FALSE);

    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				distance_sea_miles
				    = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(distance_miles)),
									_("Sea miles")),
				2, 3, 3, 4);
    gtk_widget_set_name(distance_sea_miles, "sea_miles");
    g_signal_connect(distance_sea_miles, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
				
    gtk_button_set_focus_on_click(GTK_BUTTON(distance_sea_miles), FALSE);
    switch(app->config->distance_units){
	default:
	case METERS:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(distance_meters), TRUE);
	    app->units_tab_start_state |= STATE_METERS;
	break;
	case KILOMETERS:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(distance_kilometers), TRUE);
	    app->units_tab_start_state |= STATE_KILOMETERS;
	break;
	case MILES:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(distance_miles), TRUE);
	    app->units_tab_start_state |= STATE_MILES;
	break;
	case SEA_MILES:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(distance_sea_miles), TRUE);
	    app->units_tab_start_state |= STATE_SEA_MILES;
	break;
    }
    /* wind */
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				gtk_label_new(_("Wind speed units:")),
				0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				wind_meters
				    = gtk_radio_button_new_with_label(NULL, _("m/s")),
				1, 2, 4, 5);
    wind_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(wind_meters));
    GLADE_HOOKUP_OBJECT(window, wind_meters, "wind_meters");
    gtk_widget_set_name(wind_meters, "wind_meters");
    g_signal_connect(wind_meters, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_button_set_focus_on_click(GTK_BUTTON(wind_meters), FALSE);

    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				wind_kilometers
				    = gtk_radio_button_new_with_label(wind_group, _("km/h")),
				2, 3, 4, 5);
    GLADE_HOOKUP_OBJECT(window, wind_kilometers, "wind_kilometers");
    gtk_widget_set_name(wind_kilometers, "wind_kilometers");
    g_signal_connect(wind_kilometers, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_button_set_focus_on_click(GTK_BUTTON(wind_kilometers), FALSE);

    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				wind_miles
				    = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(wind_kilometers)),
									_("mi/h")),
				1, 2, 5, 6);
    GLADE_HOOKUP_OBJECT(window, wind_miles, "wind_miles");
    gtk_widget_set_name(wind_miles, "wind_miles");
    g_signal_connect(wind_miles, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
				
    gtk_button_set_focus_on_click(GTK_BUTTON(wind_miles), FALSE);
    switch(app->config->wind_units){
	default:
	case METERS_S:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wind_meters), TRUE);
	    app->units_tab_start_state |= STATE_METERS_S;
	break;
	case KILOMETERS_H:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wind_kilometers), TRUE);
	    app->units_tab_start_state |= STATE_KILOMETERS_H;
	break;
	case MILES_H:
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wind_miles), TRUE);
	    app->units_tab_start_state |= STATE_MILES_H;
	break;
    }
    /* pressure */
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				gtk_label_new(_("Pressure units:")),
				0, 1, 6, 7);
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				mb_pressure
				    = gtk_radio_button_new_with_label(NULL,
									_("mb")),
				1, 2, 6, 7);
    GLADE_HOOKUP_OBJECT(window, mb_pressure, "mb_pressure");
    gtk_widget_set_name(mb_pressure, "pressure");
    g_signal_connect(mb_pressure, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    pressure_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(mb_pressure));
    gtk_button_set_focus_on_click(GTK_BUTTON(mb_pressure), FALSE);
    gtk_table_attach_defaults(GTK_TABLE(units_page), 
				inch_pressure
				    = gtk_radio_button_new_with_label(pressure_group,
									_("inHg")),
				2, 3, 6, 7);
    gtk_button_set_focus_on_click(GTK_BUTTON(inch_pressure), FALSE);
    if(app->config->pressure_units == MB){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mb_pressure), TRUE);
	app->units_tab_start_state |= STATE_PRESSURE_MB;
    }
    else{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(inch_pressure), TRUE);
	app->units_tab_start_state |= STATE_PRESSURE_IN;
    }
    app->units_tab_current_state = app->units_tab_start_state;
    return units_page;
}
/*******************************************************************************/
GtkWidget* create_update_tab(GtkWidget *window){
    GtkWidget	*time_update_label = NULL,
		*update_page = NULL,
		*chk_downloading_after_connection = NULL,
		*update_time = NULL,
		*valid_time_list = NULL,
		*apply_button = NULL,
		*time_2switch_list = NULL,
		*first_line = NULL,
		*second_line = NULL,
		*third_line = NULL,
		*fourth_line = NULL,
		*fifth_line = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    apply_button = lookup_widget(window, "apply_button");
/* Update tab */
    app->update_tab_start_state = 0;
    update_page = gtk_vbox_new(FALSE, 0);
    first_line = gtk_hbox_new(FALSE, 0);
    second_line = gtk_hbox_new(FALSE, 0);
    third_line = gtk_hbox_new(FALSE, 0);
    fourth_line = gtk_hbox_new(FALSE, 0);
    fifth_line = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(update_page), first_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(update_page), second_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(update_page), third_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(update_page), fourth_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(update_page), fifth_line, TRUE, TRUE, 0);
/* auto download when connect */
    gtk_box_pack_start(GTK_BOX(first_line),
			chk_downloading_after_connection = gtk_check_button_new(),
        		FALSE, FALSE, 5);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chk_downloading_after_connection),
        			    app->config->downloading_after_connecting);
    if(app->config->downloading_after_connecting)
	app->update_tab_start_state |= STATE_AUTO_CONNECT;
    GLADE_HOOKUP_OBJECT(window, chk_downloading_after_connection,
			    "download_after_connection");
    gtk_widget_set_name(chk_downloading_after_connection,
			    "download_after_connection");
    g_signal_connect(chk_downloading_after_connection, "toggled",
            		G_CALLBACK(check_buttons_changed_handler),
			window);
    gtk_box_pack_start(GTK_BOX(first_line),
			gtk_label_new(_("Automatically update data when connecting to the Internet")),
                        FALSE, FALSE, 0);
/* Switch time to the next station */
    gtk_box_pack_start(GTK_BOX(second_line),
        		gtk_label_new(_("Switch to the next station after:")),
        		FALSE, FALSE, 20);
    gtk_box_pack_end(GTK_BOX(second_line),
			time_2switch_list = gtk_combo_box_new_text(),
        		FALSE, TRUE, 20);
    GLADE_HOOKUP_OBJECT(window, time_2switch_list, "time2switch");
    gtk_widget_set_name(time_2switch_list, "time2switch");
    gtk_widget_set_size_request(time_2switch_list, 300, -1);
    g_signal_connect(time_2switch_list, "changed",
            		G_CALLBACK(combo_boxs_changed_handler),
			apply_button);
    gtk_combo_box_append_text(GTK_COMBO_BOX(time_2switch_list), _("Never"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(time_2switch_list), _("10 seconds"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(time_2switch_list), _("20 seconds"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(time_2switch_list), _("30 seconds"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(time_2switch_list), _("40 seconds"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(time_2switch_list), _("50 seconds"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(time_2switch_list), _("60 seconds"));

    switch((guint)(app->config->switch_time / 10)){
	default:
	case 0: gtk_combo_box_set_active(GTK_COMBO_BOX(time_2switch_list), 0); break;
	case 1: gtk_combo_box_set_active(GTK_COMBO_BOX(time_2switch_list), 1); break;
	case 2: gtk_combo_box_set_active(GTK_COMBO_BOX(time_2switch_list), 2); break;
	case 3: gtk_combo_box_set_active(GTK_COMBO_BOX(time_2switch_list), 3); break;
	case 4: gtk_combo_box_set_active(GTK_COMBO_BOX(time_2switch_list), 4); break;
	case 5: gtk_combo_box_set_active(GTK_COMBO_BOX(time_2switch_list), 5); break;
	case 6: gtk_combo_box_set_active(GTK_COMBO_BOX(time_2switch_list), 6); break;
    }
/* Valid time */
    gtk_box_pack_start(GTK_BOX(third_line),
        		gtk_label_new(_("Valid time for current weather:")),
        		FALSE, FALSE, 20);
    gtk_box_pack_end(GTK_BOX(third_line),
			valid_time_list = gtk_combo_box_new_text(),
        		FALSE, TRUE, 20);
    GLADE_HOOKUP_OBJECT(window, valid_time_list, "valid_time");
    gtk_widget_set_name(valid_time_list, "valid_time");
    gtk_widget_set_size_request(valid_time_list, 300, -1);
    g_signal_connect(valid_time_list, "changed",
            		G_CALLBACK(combo_boxs_changed_handler),
			apply_button);
    gtk_combo_box_append_text(GTK_COMBO_BOX(valid_time_list), _("1 hour"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(valid_time_list), _("2 hours"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(valid_time_list), _("4 hours"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(valid_time_list), _("8 hours"));
    switch((guint)(app->config->data_valid_interval / 3600)){
	case 1:  gtk_combo_box_set_active(GTK_COMBO_BOX(valid_time_list), 0); break;
	default:
	case 2:  gtk_combo_box_set_active(GTK_COMBO_BOX(valid_time_list), 1); break;
	case 4:  gtk_combo_box_set_active(GTK_COMBO_BOX(valid_time_list), 2); break;
	case 8:  gtk_combo_box_set_active(GTK_COMBO_BOX(valid_time_list), 3); break;
    }
/* Update interval */
    gtk_box_pack_start(GTK_BOX(fourth_line),
        		gtk_label_new(_("Updating of weather data:")),
        		FALSE, FALSE, 20);
    gtk_box_pack_end(GTK_BOX(fourth_line),
			update_time = gtk_combo_box_new_text(),
        		FALSE, TRUE, 20);
    GLADE_HOOKUP_OBJECT(window, update_time, "update_time");
    gtk_widget_set_name(update_time, "update_time");
    gtk_widget_set_size_request(update_time, 300, -1);
    g_signal_connect(update_time, "changed",
            		G_CALLBACK(combo_boxs_changed_handler),
			apply_button);
    gtk_box_pack_start(GTK_BOX(fifth_line),
        		gtk_label_new(_("Next update:")),
        		FALSE, FALSE, 20);
    gtk_box_pack_end(GTK_BOX(fifth_line),
			time_update_label = gtk_label_new(NULL),
        		FALSE, TRUE, 20);
    gtk_widget_set_size_request(time_update_label, 300, -1);
    g_signal_connect(update_time, "changed",
                	G_CALLBACK(update_iterval_changed_handler), time_update_label);
/* Fill update time box */
    gtk_combo_box_set_row_span_column(GTK_COMBO_BOX(update_time), 0);
    gtk_combo_box_set_model(GTK_COMBO_BOX(update_time),
				(GtkTreeModel*)app->time_update_list);
    gtk_combo_box_set_active(GTK_COMBO_BOX(update_time),
    get_active_item_index((GtkTreeModel*)app->time_update_list,
				    app->config->update_interval, NULL, FALSE));

    app->update_tab_current_state = app->update_tab_start_state;
    return update_page;
}
/*******************************************************************************/
void font_changed_handler(GtkFontButton *widget, gpointer user_data){
    if(strcmp(app->config->font, (gchar*)gtk_font_button_get_font_name(widget)))
	app->visuals_tab_current_state |= STATE_FONT;
    else
	app->visuals_tab_current_state &= ~STATE_FONT;
    if((app->stations_tab_current_state != app->stations_tab_start_state) ||
            (app->visuals_tab_current_state != app->visuals_tab_start_state) ||
            (app->display_tab_current_state != app->display_tab_start_state) ||
            (app->units_tab_current_state != app->units_tab_start_state) ||
            (app->update_tab_current_state != app->update_tab_start_state)
									    ||
#ifdef OS2008
            (app->sensor_tab_current_state != app->sensor_tab_start_state)
#endif
									    )
        gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
    else
        gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
}
/*******************************************************************************/
