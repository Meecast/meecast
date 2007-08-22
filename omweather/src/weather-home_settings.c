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
#include "weather-home_settings.h"
#include "weather-locations.h"
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
void changed_country(void){
    GtkTreeModel	*model;
    GtkTreeIter		iter;
    gchar		*country_name = NULL;
    long		regions_start = -1,
			regions_end = -1,
			regions_number = 0;
    
    /* clear regions list */
    if(app->regions_list)
	gtk_list_store_clear(app->regions_list);
    /* clear locations list */
    if(app->stations_list)
	gtk_list_store_clear(app->stations_list);    

    if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(countrys), &iter)){
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(countrys));
	gtk_tree_model_get(model, &iter, 0, &country_name,
					 1, &regions_start,
					 2, &regions_end,
					    -1);
	if(app->regions_list)
	    gtk_list_store_clear(app->regions_list);
	app->regions_list = create_items_list(REGIONSFILE, regions_start,
						regions_end, &regions_number);

	gtk_combo_box_set_row_span_column((GtkComboBox*)states, 0);
	gtk_combo_box_set_model((GtkComboBox*)states,
				(GtkTreeModel*)app->regions_list);

	/* if region is one then set it active and disable combobox */
	if(regions_number < 2){
	    gtk_combo_box_set_active((GtkComboBox*)states, 0);
	    gtk_widget_set_sensitive(states, FALSE);
	}
	else{
	    gtk_combo_box_set_active((GtkComboBox*)states, -1);
	    gtk_widget_set_sensitive(states, TRUE);
	}

	g_free(app->config->current_country);
	app->config->current_country = country_name;
    }
}
/*******************************************************************************/
void changed_state(void){
    GtkTreeModel	*model = NULL;
    GtkTreeIter		iter;
    gchar		*state_name = NULL;
    long		stations_start = -1,
			stations_end = -1;

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

	app->stations_list = create_items_list(LOCATIONSFILE, stations_start,
						stations_end, NULL);
	gtk_combo_box_set_row_span_column((GtkComboBox*)stations, 0);
	gtk_combo_box_set_model((GtkComboBox*)stations,
				(GtkTreeModel*)app->stations_list);

	g_free(state_name);
    }
}
/*******************************************************************************/
void changed_stations(void){
    GtkTreeModel	*model = NULL;
    GtkTreeIter		iter;
    gchar		*station_name = NULL,
			*station_id0 = NULL;

    if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(stations), &iter)){
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(stations));
	gtk_tree_model_get(model, &iter, 0, &station_name,
					 1, &station_id0,
					-1);

	_weather_station_id_temp = g_strdup(station_id0);
	g_free(station_name);
	g_free(station_id0);
    }
}
/*******************************************************************************/
/* Fill station list (tree) */
void fill_station_list_view(GtkWidget *station_list_view,
			    GtkListStore *station_list_store){
    GSList *tmplist = NULL; /* Temporary for station list */
    struct weather_station *ws; /* Description Weather station */
    GtkTreeSelection *list_selection;
    GtkTreeIter iter;

    list_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));
    tmplist = stations_view_list;
    while(tmplist){
	ws = tmplist->data;
	gtk_list_store_append(GTK_LIST_STORE
                        	(station_list_store), &iter);
	gtk_list_store_set(GTK_LIST_STORE(station_list_store),
                        	&iter, 0,
                        	ws->name_station, -1);
	if((app->config->current_station_id)&&streq(app->config->current_station_id, ws->id_station))
    	    gtk_tree_selection_select_iter(list_selection, &iter);
	tmplist = g_slist_next(tmplist);
    }
}
/*******************************************************************************/
/* Edit the station */
void weather_window_edit_station(GtkWidget *widget,
            			   GdkEvent *event,
                    		   gpointer user_data){
    
    GtkWidget	*window_edit_station,
		*label,
		*code_label,
		*table,
		*station_name_edit,
		*station_code_edit;
    
    GSList	*tmplist = NULL; /* Temporary for station list */
    struct weather_station *ws; /* Description Weather station */
    GtkTreeIter	iter;
    gchar	*selected_station_name = NULL,
		*selected_station_code = NULL;
    GtkTreeModel	*model;
    GtkTreeSelection	*selection;

    model = gtk_tree_view_get_model(GTK_TREE_VIEW(station_list_view));
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));
    if( !gtk_tree_selection_get_selected(selection, NULL, &iter) )
	return;
    gtk_tree_model_get(model, &iter, 0, &selected_station_name, -1); 
    
    /* Create dialog window */
    window_edit_station = gtk_dialog_new_with_buttons(_("Edit Station"),
        						NULL,
							GTK_DIALOG_MODAL,
							NULL);
    /* Add buttons */
    gtk_dialog_add_button(GTK_DIALOG(window_edit_station),
        		    _("OK"), GTK_RESPONSE_ACCEPT);
    gtk_dialog_add_button(GTK_DIALOG(window_edit_station),
        		    _("Cancel"), GTK_RESPONSE_REJECT);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_edit_station)->vbox),
        		    table = gtk_table_new(2, 2, FALSE), TRUE, TRUE, 0);	    

   /* Add Label and Edit field for station name */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Station:")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f),
        			1, 2, 0, 1);
    station_name_edit = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(station_name_edit), 50);
    gtk_entry_set_text(GTK_ENTRY(station_name_edit), selected_station_name);
    gtk_container_add(GTK_CONTAINER(label), station_name_edit);
    /* Added Label and Edit field for station code */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			code_label = gtk_label_new(_("Code:")),
        			0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			code_label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f),
        			1, 2, 1, 2);
    station_code_edit = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(station_code_edit), 8);
    /* search station code */
    tmplist = stations_view_list;
    while(tmplist){
	ws = tmplist->data;
	if((ws->name_station && selected_station_name &&
		streq(selected_station_name, ws->name_station))||
		(!ws->name_station && !selected_station_name)){
	    selected_station_code = g_strdup(ws->id_station);
	    break;
	}
	tmplist = g_slist_next(tmplist);
    }
    gtk_entry_set_text(GTK_ENTRY(station_code_edit), selected_station_code);
    gtk_container_add(GTK_CONTAINER(code_label), station_code_edit);
    /* set size for dialog */
    gtk_widget_set_size_request(GTK_WIDGET(window_edit_station), 350, -1);
    gtk_widget_show_all(window_edit_station);

    /* start dialog */
    switch(gtk_dialog_run(GTK_DIALOG(window_edit_station))){
	case GTK_RESPONSE_ACCEPT:/* Press Button Ok */
	    tmplist = stations_view_list;
	    while(tmplist){
		ws = tmplist->data;
		if((ws->name_station && selected_station_name &&
				streq(selected_station_name, ws->name_station))||
		  (!ws->name_station && !selected_station_name)){
		    if(ws->name_station)
			g_free(ws->name_station);
   		    ws->name_station = g_strdup(gtk_entry_get_text(GTK_ENTRY(station_name_edit)));
		    if(ws->id_station)
			g_free(ws->id_station);
		    ws->id_station = g_strdup(gtk_entry_get_text(GTK_ENTRY(station_code_edit)));
		    /* Update station list */	    
		    gtk_list_store_clear(station_list_store);
		    fill_station_list_view(station_list_view, station_list_store);
		    /* Update config file */
		    new_config_save(app->config);
		    flag_update_station = TRUE;
		    break;
		}
 		tmplist = g_slist_next(tmplist);  
	    }
	break;
	default:
	break;
    }
    if(selected_station_name)
        g_free(selected_station_name);
    if(selected_station_code)
        g_free(selected_station_code);    
    gtk_widget_destroy(window_edit_station);
}
/*******************************************************************************/
/* Delete station from list */
static gboolean weather_delete_station(GtkWidget *widget,
                    		GdkEvent *event,
                    		gpointer user_data){
    GSList *tmplist = NULL; /* Temporary for station list */
    struct weather_station *ws; /* Description Weather station */
    GtkTreeIter iter;
    gchar *station_selected = NULL;
    GtkTreeModel *model;
    GtkTreeSelection *selection;

#ifndef RELEASE    
    fprintf(stderr,"%s()\n", __PRETTY_FUNCTION__);
#endif
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(station_list_view));
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));
    if( !gtk_tree_selection_get_selected(selection, NULL, &iter) )
	return FALSE;
 
    gtk_tree_model_get(model, &iter, 0, &station_selected, -1); 
    tmplist = stations_view_list;
    while(tmplist){
	ws = tmplist->data;
	if((ws->name_station != NULL && station_selected != NULL && streq(station_selected, ws->name_station))||
	  (ws->name_station == NULL && station_selected == NULL)){
      /* Remove station from the Station List */
	    stations_view_list = g_slist_remove(stations_view_list, ws);
	    g_free(ws->id_station);
	    g_free(ws->name_station);
	    g_free(ws);
	    gtk_list_store_clear(station_list_store);
	    tmplist = stations_view_list;

      /* If not selected station, select first */	    
	    if (!(gtk_tree_selection_get_selected(selection, NULL, &iter)) && (tmplist != NULL )) {
	     ws = tmplist->data;
	     if(app->config->current_station_id)
		g_free(app->config->current_station_id);
   	     app->config->current_station_id = g_strdup(ws->id_station); 
	    }
	    
	    fill_station_list_view (station_list_view,station_list_store);
      /* Update station list */
	    flag_update_station = TRUE;
	    model = gtk_tree_view_get_model(GTK_TREE_VIEW(station_list_view));
	    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));
      /* Search new selected station */    
	    if( gtk_tree_selection_get_selected(selection, NULL, &iter) ){
		if (station_selected)
	            g_free (station_selected);	
		gtk_tree_model_get(model, &iter, 0, &station_selected,-1);
		tmplist = stations_view_list;
		while(tmplist){
		    ws = tmplist->data;
		    if(streq(station_selected, ws->name_station)){
         /* Set New selected station on default on main display*/
    			if(app->config->current_station_id)
			    g_free(app->config->current_station_id);
			app->config->current_station_id = g_strdup(ws->id_station); 
			if(app->config->current_station_name)
			    g_free(app->config->current_station_name);
    			app->config->current_station_name = g_strdup(ws->name_station); 
		    }
		    tmplist = g_slist_next(tmplist);
		} 
	    }
	    else
		app->config->current_station_id = NULL;
      /* Update config file */
	    new_config_save(app->config);
	    break; 
	}
	tmplist = g_slist_next(tmplist);
    }
    if (g_slist_length(stations_view_list) == 0){
       if(app->config->current_station_name)
          g_free(app->config->current_station_name);
       if(app->config->current_station_id)
          g_free(app->config->current_station_id);
       app->config->current_station_name = NULL;
       app->config->current_station_id = NULL;
       weather_frame_update(FALSE);
       /* Update config file */
       new_config_save(app->config);
    }
#ifndef RELEASE
    fprintf(stderr,"End %s()\n", __PRETTY_FUNCTION__);    
#endif
    if(station_selected)
        g_free (station_selected);
    return TRUE;
}
/*******************************************************************************/
static GtkListStore* create_station_list_store(void){
    GtkListStore *station_list = NULL;
    station_list = gtk_list_store_new(1, G_TYPE_STRING);
    return station_list;
}
/*******************************************************************************/
static GtkWidget* create_tree_view(GtkListStore * list){
    GtkWidget *tree_view = NULL;
    GtkTreeSelection *list_selection = NULL;
    GtkCellRenderer *list_renderer = NULL;
    GtkTreeViewColumn *list_column = NULL;

/* create the tree view model LIST */
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list));
/* make the list component single selectable */
    list_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    gtk_tree_selection_set_mode(list_selection, GTK_SELECTION_SINGLE);
/* add name column to the view */
    list_renderer = gtk_cell_renderer_text_new();
    list_column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_sizing(list_column, GTK_TREE_VIEW_COLUMN_FIXED);
    g_object_set(G_OBJECT(list_renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    gtk_tree_view_column_set_expand(list_column, TRUE);

    gtk_tree_view_column_pack_start(list_column, list_renderer, TRUE);
    gtk_tree_view_column_set_attributes(list_column, list_renderer,
                                      "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), list_column);
/* return widget to caller */
    return tree_view;
}
/*******************************************************************************/
#define ZERO(type, name) type name; memset(&name, 0, sizeof name)
#define SIG_TIMER_EXPIRATION SIGRTMIN
#define CLOCK_TYPE CLOCK_MONOTONIC
/*******************************************************************************/
void weather_window_add_custom_station(void){
    struct weather_station *ws;       /* Temp struct for station */
    GtkWidget *window_add_custom_station;
    GtkWidget *label;
    GtkWidget *table;

/* Create dialog window */
    window_add_custom_station = gtk_dialog_new_with_buttons(_("Add Custom Station"),
        							NULL, GTK_DIALOG_MODAL,
								NULL);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_add_custom_station)->vbox),
        		    table = gtk_table_new(4, 2, FALSE), TRUE, TRUE, 0);	    
    gtk_dialog_add_button(GTK_DIALOG(window_add_custom_station),
        		    _("OK"), GTK_RESPONSE_ACCEPT);
    gtk_dialog_add_button(GTK_DIALOG(window_add_custom_station),
        		    _("Cancel"), GTK_RESPONSE_REJECT);
/* Add Custom Station Name  */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Station name:")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
        			1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label),custom_station_name = gtk_entry_new());
    gtk_entry_set_max_length((GtkEntry*)custom_station_name, 16);
    gtk_entry_set_width_chars((GtkEntry*)custom_station_name, 16);
/* Add Custom Station Code  */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Station code\n (ZIP Code):")),
        			0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0.f, 1.f, 0.f, 1.f) ,
        			1, 2, 1, 2);
    gtk_container_add(GTK_CONTAINER(label),custom_station_code = gtk_entry_new());	    
    gtk_entry_set_max_length((GtkEntry*)custom_station_code, 8);
    gtk_entry_set_width_chars((GtkEntry*)custom_station_code, 8);
  
    gtk_widget_show_all(window_add_custom_station);   
/* start dialog */
    switch(gtk_dialog_run(GTK_DIALOG(window_add_custom_station))){
	case GTK_RESPONSE_ACCEPT:/* Press Button Ok */
		ws = g_new0(struct weather_station, 1);
		if(app->config->current_station_id != NULL)
		    g_free(app->config->current_station_id);
		app->config->current_station_id = g_strdup(gtk_entry_get_text((GtkEntry*)custom_station_code));
		ws->id_station = g_strdup(app->config->current_station_id);
		if(app->config->current_station_name)
		    g_free(app->config->current_station_name);
		app->config->current_station_name = g_strdup(gtk_entry_get_text((GtkEntry*)custom_station_name));
		ws->name_station = g_strdup(app->config->current_station_name);
	    /* Add station to stations list */
		stations_view_list = g_slist_append(stations_view_list, ws); 
	    /* Add station to View List(Tree) */
		gtk_list_store_clear(station_list_store);
		fill_station_list_view (station_list_view,station_list_store);
	    /* Update config file */
		new_config_save(app->config);
		flag_update_station = TRUE;
	break;
	default:
	break;
    }
    gtk_widget_destroy(window_add_custom_station);
}
/*******************************************************************************/
void weather_window_add_station(GtkWidget *widget,
            			GdkEvent *event,
                    		gpointer user_data){
    struct		weather_station *ws;   	/* Temp struct for station */
    GtkWidget		*label,
			*table;

/* Create dialog window */
    window_add_station = gtk_dialog_new_with_buttons(_("Add Station"),
        						NULL,
							GTK_DIALOG_MODAL,
							NULL);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_add_station)->vbox),
        		table = gtk_table_new(4, 2, FALSE), TRUE, TRUE, 0);
    gtk_dialog_add_button(GTK_DIALOG(window_add_station),
                    		_("Add Custom Station"), OMW_RESPONSE_ADD_CUSTOM_STATION);
    gtk_dialog_add_button(GTK_DIALOG(window_add_station),
        			_("OK"), GTK_RESPONSE_ACCEPT);
    gtk_dialog_add_button(GTK_DIALOG(window_add_station),
        			_("Cancel"), GTK_RESPONSE_REJECT);
/* Add Country */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Country:")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f),
        			1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label),countrys = gtk_combo_box_new_text());
/* Add State */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("State(Province):")),
        			0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f),
        			1, 2, 2, 3);
  gtk_container_add(GTK_CONTAINER(label),states = gtk_combo_box_new_text());
/* Add Station */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Station(Place):")),
        			0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f) ,
        			1, 2, 3, 4);
    gtk_container_add(GTK_CONTAINER(label),stations = gtk_combo_box_new_text());
 
    gtk_widget_show_all(window_add_station);

    gtk_combo_box_set_row_span_column((GtkComboBox*)countrys, 0);
    gtk_combo_box_set_model((GtkComboBox*)countrys, (GtkTreeModel*)app->countrys_list);
/* Set default value to country combo_box */
    gtk_combo_box_set_active(GTK_COMBO_BOX(countrys), -1);

    g_signal_connect((gpointer)countrys, "changed",
            		G_CALLBACK (changed_country), NULL);
    g_signal_connect((gpointer)states, "changed",
                	G_CALLBACK (changed_state), NULL);
    g_signal_connect((gpointer) stations, "changed",
            		G_CALLBACK (changed_stations), NULL);

    switch(gtk_dialog_run(GTK_DIALOG(window_add_station))){
	default:
	case GTK_RESPONSE_REJECT:/* Press Cancel  */
	break;
	case OMW_RESPONSE_ADD_CUSTOM_STATION:/* Press Custom station add  */
	    weather_window_add_custom_station();
	break;
	case GTK_RESPONSE_ACCEPT:/* Press Button Ok */
	    if (gtk_combo_box_get_active(GTK_COMBO_BOX(stations)) == -1) /* Item not selected */
		break;
	    flag_update_station = TRUE;
	    ws = g_new0(struct weather_station,1);
	    if(app->config->current_station_id != NULL)
	        g_free(app->config->current_station_id);
	    app->config->current_station_id = g_strdup(_weather_station_id_temp);
	    ws->id_station = g_strdup(_weather_station_id_temp);
	    if(app->config->current_station_name)
		g_free(app->config->current_station_name);
	    app->config->current_station_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations));
	    ws->name_station = g_strdup(app->config->current_station_name);
/* Add station to stations list */
	    stations_view_list = g_slist_append(stations_view_list, ws); 
/* Update config file */
	    new_config_save(app->config);
/* Add station to View List(Tree) */
	    gtk_list_store_clear(station_list_store);
	    fill_station_list_view (station_list_view,station_list_store);
	    break;
    }
    gtk_widget_destroy(window_add_station); 
}
/*******************************************************************************/
/* Main preference window */
void weather_window_settings(GtkWidget *widget,
				GdkEvent *event,
				gpointer user_data){

    GtkWidget	*window_config = NULL,
		*notebook = NULL,
		*label = NULL,
		*time_update_label = NULL,
		*table = NULL,
		*font_color = NULL,
		*background_color = NULL,
		*chk_transparency = NULL,
		*chk_downloading_after_connection = NULL,
		*separate_button = NULL,
		*swap_temperature_button = NULL,
		*hide_station_name = NULL,
		*scrolled_window = NULL,
		*button_add = NULL,
		*button_del = NULL,
		*button_ren = NULL;		
    char	flag_update_icon = '\0'; /* Flag update main weather icon of desktop */
    gboolean	flag_tuning_warning; /* Flag for show the warnings about tuning images of applet */
    GdkColor	_weather_font_color_temp, /* Temporary for font color */
		background_color_temp;
    static char *temp_string; /* Temporary for the results differnet strdup functions */
    static int result_gtk_dialog_run; /* Temporary for the gtk_dialog_run result */
    char	tmp_buff[2048];

    not_event = TRUE;
    flag_update_station = FALSE;
    flag_update_icon = FALSE;
    flag_tuning_warning = FALSE; 

    if(!app->dbus_is_initialize)
	weather_initialize_dbus();

    window_config = gtk_dialog_new_with_buttons(_("Other Maemo Weather Settings"),
        				NULL,
					GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
					NULL);
    /* add OK button */
    gtk_dialog_add_button(GTK_DIALOG(window_config),
        		    _("OK"), GTK_RESPONSE_ACCEPT);
    /* add CANCEL button */
    gtk_dialog_add_button(GTK_DIALOG(window_config),
        		    _("Cancel"), GTK_RESPONSE_REJECT);
    /* add Help button */
    gtk_dialog_add_button(GTK_DIALOG(window_config),
        		    _("About"), GTK_RESPONSE_HELP);
/* Create Notebook widget */
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_config)->vbox),
        		notebook = gtk_notebook_new(), TRUE, TRUE, 0);
/* Locations tab */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        	    table = gtk_table_new(4, 2, FALSE),
        	    label = gtk_label_new(_("Locations")));
	    
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        	    label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f),
        	    0, 1, 0, 6);
  
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window), GTK_SHADOW_OUT);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 250, 180);

    station_list_store = create_station_list_store();
    station_list_view = create_tree_view(station_list_store);
    gtk_container_add(GTK_CONTAINER(scrolled_window),
                	GTK_WIDGET(station_list_view));
    fill_station_list_view(station_list_view,station_list_store);
    gtk_container_add(GTK_CONTAINER(label), scrolled_window);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(" "),
        			1, 2, 0, 1);
    button_add = gtk_button_new_with_label(_(" Add "));
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			button_add,
        			1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(" "),
        			1, 2, 2, 3);
    button_ren = gtk_button_new_with_label(_("Edit"));
    gtk_table_attach_defaults(GTK_TABLE(table),
        			button_ren,
        			1, 2, 3, 4);				
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(" "),
        			1, 2, 4, 5);				
    button_del = gtk_button_new_with_label(_("Delete"));
    gtk_table_attach_defaults(GTK_TABLE(table),
        			button_del,
        			1, 2, 5, 6);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(" "),
        			1, 2, 6, 7);
    g_signal_connect(station_list_view, "cursor-changed",
                	G_CALLBACK(station_list_view_select_handler), NULL);
    g_signal_connect(button_ren, "clicked",
                	G_CALLBACK(weather_window_edit_station), NULL);				
    g_signal_connect(button_del, "clicked",
                	G_CALLBACK(weather_delete_station), NULL);
    g_signal_connect(button_add, "clicked",
                	G_CALLBACK(weather_window_add_station),
                	NULL);
/* Interface tab */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			table = gtk_table_new(1, 8, FALSE),
        			label = gtk_label_new(_("Interface")));
/* Days to show */
    app->config->days_to_show--; /* count down, because combobox items start with 0 */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
				label = gtk_label_new(_("Visible items:")),
				0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
				1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label), days_number = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "1");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "2");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "3");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "4");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "5");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "6");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "7");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "8");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "9");
    gtk_combo_box_append_text(GTK_COMBO_BOX(days_number), "10");
    switch(app->config->days_to_show){
	case 0:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 0);break;
	case 1:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 1);break;
	case 2:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 2);break;
	case 3:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 3);break;
	default:
	case 4:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 4);break;
	case 5:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 5);break;
	case 6:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 6);break;
	case 7:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 7);break;
	case 8:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 8);break;
	case 9:  gtk_combo_box_set_active(GTK_COMBO_BOX(days_number), 9);break;
    }    
    app->config->days_to_show++; /* count up to return to real value */
/* Layout */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Layout:")),
        			0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
        			1, 2, 1, 2);
    gtk_container_add(GTK_CONTAINER(label),layout_type = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(layout_type), _("One row"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(layout_type), _("One column"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(layout_type), _("Two rows"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(layout_type), _("Two columns"));
    switch(app->config->icons_layout){
	default:
	case ONE_ROW: 	  gtk_combo_box_set_active(GTK_COMBO_BOX(layout_type),0);break;
	case ONE_COLUMN:  gtk_combo_box_set_active(GTK_COMBO_BOX(layout_type),1);break;
	case TWO_ROWS:    gtk_combo_box_set_active(GTK_COMBO_BOX(layout_type),2);break;
	case TWO_COLUMNS: gtk_combo_box_set_active(GTK_COMBO_BOX(layout_type),3);break;
    }
/* Icon set */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Icon set:")),
        			0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 2, 3);
    gtk_container_add(GTK_CONTAINER(label), iconset = gtk_combo_box_new_text());
/* add icons set to list */
    create_icon_set_list(iconset);
/* Icon size */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Icon size:")),
        			0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 3, 4);
    gtk_container_add(GTK_CONTAINER(label),icon_size = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(icon_size), _("Large"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(icon_size), _("Medium"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(icon_size), _("Small"));
    switch(app->config->icons_size){
	default:
	case LARGE: gtk_combo_box_set_active(GTK_COMBO_BOX(icon_size),0);break;
	case MEDIUM: gtk_combo_box_set_active(GTK_COMBO_BOX(icon_size),1);break;
	case SMALL: gtk_combo_box_set_active(GTK_COMBO_BOX(icon_size),2);break;
    }
/* Font color */   
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Font color:")),
        			0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 4, 5);
    font_color = gtk_color_button_new();
    gtk_container_add(GTK_CONTAINER(label), font_color);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(font_color), &(app->config->font_color));
/* Background color */   
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Background color:")),
        			0, 1, 5, 6);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 5, 6);
    background_color = gtk_color_button_new();
    gtk_container_add(GTK_CONTAINER(label), background_color);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(background_color), &(app->config->background_color));
    if(app->config->transparency)
    	gtk_widget_set_sensitive(background_color, FALSE);
    else
	gtk_widget_set_sensitive(background_color, TRUE);
/* Transparency */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Transparency:")),
        			0, 1, 6, 7);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
        			1, 2, 6, 7);
    gtk_container_add(GTK_CONTAINER(label), chk_transparency = gtk_check_button_new());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chk_transparency),
        			    app->config->transparency);
    g_signal_connect(GTK_TOGGLE_BUTTON(chk_transparency), "toggled",
            		    G_CALLBACK(transparency_button_toggled_handler), background_color);
/* Split */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Separate current weather data:")),
        			0, 1, 7, 8);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
        			1, 2, 7, 8);
    gtk_container_add(GTK_CONTAINER(label), separate_button = gtk_check_button_new());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(separate_button),
        			    app->config->separate);
/* Swap temperature */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Swap hi/low temperature:")),
        			0, 1, 8, 9);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
        			1, 2, 8, 9);
    gtk_container_add(GTK_CONTAINER(label), swap_temperature_button = gtk_check_button_new());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(swap_temperature_button),
        			    app->config->swap_hi_low_temperature);
/* Hide station name button */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Hide station name and arrows:")),
        			0, 1, 9, 10);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 9, 10);
    gtk_container_add(GTK_CONTAINER(label), hide_station_name = gtk_check_button_new());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(hide_station_name),
        			    app->config->hide_station_name);
/* Units tab */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			table = gtk_table_new(1, 3, FALSE),
        			label = gtk_label_new(_("Units")));
/* Temperature units */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Temperature units:")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label),temperature_unit = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(temperature_unit), _("Celsius (Metric)"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(temperature_unit), _("Fahrenheit (Imperial)"));
    switch(app->config->temperature_units){
	default:
	case CELSIUS: gtk_combo_box_set_active(GTK_COMBO_BOX(temperature_unit), 0); break;
	case FAHRENHEIT: gtk_combo_box_set_active(GTK_COMBO_BOX(temperature_unit), 1); break;
    }
/* Distance units */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
				label = gtk_label_new(_("Distance units:")),
				0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
				1, 2, 1, 2);
    gtk_container_add(GTK_CONTAINER(label), units = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(units), _("Meters"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(units), _("Kilometers"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(units), _("Miles"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(units), _("Miles (Sea)"));
    switch(app->config->distance_units){
	case METERS:			gtk_combo_box_set_active(GTK_COMBO_BOX(units), METERS);break;
	default:
	case KILOMETERS:	 	gtk_combo_box_set_active(GTK_COMBO_BOX(units), KILOMETERS);break;
	case MILES:			gtk_combo_box_set_active(GTK_COMBO_BOX(units), MILES);break;
	case SEA_MILES:			 gtk_combo_box_set_active(GTK_COMBO_BOX(units), SEA_MILES);break;
    }    
/* Wind units */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
				label = gtk_label_new(_("Wind speed units:")),
				0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
				1, 2, 2, 3);
    gtk_container_add(GTK_CONTAINER(label), wunits = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("m/s"));
/*    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("km/s"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("mi/s"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("m/h"));
*/    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("km/h"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("mi/h"));
    switch(app->config->wind_units){
	default:
	case METERS_S:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), METERS_S);break;
/*	case KILOMETERS_S:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), KILOMETERS_S);break;
	case MILES_S:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), MILES_S);break;
	case METERS_H:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), METERS_H);break;
*/	case KILOMETERS_H:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), KILOMETERS_H);break;
	case MILES_H:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), MILES_H);break;
    }    
/* Update tab */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			table = gtk_table_new(4, 2, FALSE),
        			label = gtk_label_new(_("Update")));
			
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Automatically updating data after connection:")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label), chk_downloading_after_connection = gtk_check_button_new());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chk_downloading_after_connection),
        			    app->config->downloading_after_connecting);
				    
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Valid time for current weather:")),
        			0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 1, 2);
    gtk_container_add(GTK_CONTAINER(label), valid_time_list = gtk_combo_box_new_text());
    gtk_combo_box_append_text(GTK_COMBO_BOX(valid_time_list), _("1 hour"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(valid_time_list), _("2 hours"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(valid_time_list), _("4 hours"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(valid_time_list), _("8 hours"));
    switch((guint)(app->config->data_valid_interval / 3600)){
	case 1:  gtk_combo_box_set_active(GTK_COMBO_BOX(valid_time_list), 0);break;
	default:
	case 2:  gtk_combo_box_set_active(GTK_COMBO_BOX(valid_time_list), 1);break;
	case 4:  gtk_combo_box_set_active(GTK_COMBO_BOX(valid_time_list), 2);break;
	case 8:  gtk_combo_box_set_active(GTK_COMBO_BOX(valid_time_list), 3);break;
    }

    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Updating of weather data:")),
        			0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 2, 3);
    gtk_container_add(GTK_CONTAINER(label), update_time = gtk_combo_box_new_text());
    
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Next update:")),
        			0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 3, 4);

    time_update_label = gtk_label_new(NULL);
    gtk_container_add(GTK_CONTAINER(label), time_update_label);
    g_signal_connect(update_time, "changed",
                	G_CALLBACK(update_iterval_changed_handler), time_update_label);
/* Fill update time box */
    gtk_combo_box_set_row_span_column(GTK_COMBO_BOX(update_time), 0);
    gtk_combo_box_set_model(GTK_COMBO_BOX(update_time),
				(GtkTreeModel*)app->time_update_list);
    gtk_combo_box_set_active(GTK_COMBO_BOX(update_time),
	    get_active_time_update((GtkTreeModel*)app->time_update_list,
				    app->config->update_interval, NULL));
//#ifndef RELEASE
/* Evetns list tab */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    print_list(tmp_buff, sizeof(tmp_buff) - 1);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_scrolled_window_with_text(tmp_buff,
						    GTK_JUSTIFY_LEFT),
        			label = gtk_label_new(_("Events")));
//#endif
    gtk_widget_show_all(window_config);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), app->config->current_settings_page);
/* kill popup window :-) */
    if(app->popup_window){
	gtk_widget_destroy(app->popup_window);
	app->popup_window = NULL;
    }

    while (0 != (result_gtk_dialog_run = gtk_dialog_run(GTK_DIALOG(window_config)))){
/* start dialog window */
      switch(result_gtk_dialog_run){
	case GTK_RESPONSE_ACCEPT:/* Pressed Button Ok */
/* icon set */	
	    temp_string = gtk_combo_box_get_active_text(GTK_COMBO_BOX(iconset));
	    if(strcmp(app->config->icon_set, temp_string)){
		if(app->config->icon_set)
		    g_free(app->config->icon_set);
	        app->config->icon_set = g_strdup(temp_string);
		memset(path_large_icon, 0, sizeof(path_large_icon));
		sprintf(path_large_icon, "%s%s/", ICONS_PATH, app->config->icon_set);
		flag_update_icon = TRUE;
	    }
	    g_free(temp_string);
/* icon size */	    
	    if(app->config->icons_size != gtk_combo_box_get_active(GTK_COMBO_BOX(icon_size))){
		app->config->icons_size = gtk_combo_box_get_active(GTK_COMBO_BOX(icon_size));
		flag_update_icon = TRUE;
		flag_tuning_warning = TRUE;
	    }
/* Temperature units */
	    if(app->config->temperature_units != gtk_combo_box_get_active(GTK_COMBO_BOX(temperature_unit))){
		app->config->temperature_units = gtk_combo_box_get_active(GTK_COMBO_BOX(temperature_unit));
		flag_update_icon = TRUE;
	    }
/* Font color */
	    gtk_color_button_get_color(GTK_COLOR_BUTTON(font_color), &_weather_font_color_temp);
	    if(( _weather_font_color_temp.red - app->config->font_color.red ) ||
		    ( _weather_font_color_temp.green - app->config->font_color.green ) ||
		    ( _weather_font_color_temp.blue - app->config->font_color.blue )){
		memcpy(&(app->config->font_color), &_weather_font_color_temp, sizeof(app->config->font_color));
    		flag_update_icon = TRUE;
	    }
/* Background color */
	    gtk_color_button_get_color(GTK_COLOR_BUTTON(background_color), &background_color_temp);
	    if(( background_color_temp.red - app->config->background_color.red ) ||
		    ( background_color_temp.green - app->config->background_color.green ) ||
		    ( background_color_temp.blue - app->config->background_color.blue )){
		memcpy(&(app->config->background_color), &background_color_temp, sizeof(app->config->background_color));
    		flag_update_icon = TRUE;
	    }
/* Days to show */
	    if( gtk_combo_box_get_active((GtkComboBox*)days_number)!= app->config->days_to_show - 1){
		app->config->previos_days_to_show = app->config->days_to_show;/* store previos number of icons */
		app->config->days_to_show = gtk_combo_box_get_active((GtkComboBox*)days_number);
		app->config->days_to_show++;
    		flag_update_icon = TRUE;
		flag_tuning_warning = TRUE;
	    }
/* Layout Type */
	    if( gtk_combo_box_get_active((GtkComboBox*)layout_type) != app->config->icons_layout ){
		app->config->icons_layout = gtk_combo_box_get_active((GtkComboBox*)layout_type);
    		flag_update_icon = TRUE;
		flag_tuning_warning = TRUE;
	    }
/* Transparency mode */
    	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chk_transparency)) != app->config->transparency){
		app->config->transparency = 
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chk_transparency));
    		flag_update_icon = TRUE;
	    }
/* Split data mode */
    	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(separate_button)) != app->config->separate){
		app->config->separate = 
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(separate_button));
    		flag_update_icon = TRUE;
	    }
/* Swap temperature data mode */
    	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(swap_temperature_button)) != app->config->swap_hi_low_temperature){
		app->config->swap_hi_low_temperature = 
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(swap_temperature_button));
    		flag_update_icon = TRUE;
	    }
/* Hide station name and arrows */
    	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hide_station_name)) != app->config->hide_station_name){
		app->config->hide_station_name = 
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hide_station_name));
    		flag_update_icon = TRUE;
	    }
/* Distance units */
	    if( gtk_combo_box_get_active((GtkComboBox*)units) != app->config->distance_units ){
		app->config->distance_units = gtk_combo_box_get_active((GtkComboBox*)units);
    		flag_update_icon = TRUE;
	    }
/* Wind units */
	    if( gtk_combo_box_get_active((GtkComboBox*)wunits) != app->config->wind_units ){
		app->config->wind_units = gtk_combo_box_get_active((GtkComboBox*)wunits);
    		flag_update_icon = TRUE;
	    }
/* Data valid time */
	    if( (1 << gtk_combo_box_get_active((GtkComboBox*)valid_time_list)) != app->config->data_valid_interval / 3600 ){
		app->config->data_valid_interval = 3600 * (1 << gtk_combo_box_get_active((GtkComboBox*)valid_time_list));
    		flag_update_icon = TRUE;
	    }
/* Downloading after connection */	    
	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chk_downloading_after_connection)))
		app->config->downloading_after_connecting = TRUE;
	    else
		app->config->downloading_after_connecting = FALSE;
	    
/* Current tab number */
	    if(app->config->current_settings_page != gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook))){
		app->config->current_settings_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
		flag_update_icon = TRUE;
	    }
/* Update interval
 * saved in update_iterval_changed_handler
*/
    	    new_config_save(app->config);
	    if(flag_update_icon){
    		weather_frame_update(FALSE);
		app->config->previos_days_to_show = app->config->days_to_show;/* store previos number of icons */
	    }	
	    if(flag_update_station){
		if(app->iap_connected){
		    if( g_slist_length(stations_view_list) > 0 ){
			app->show_update_window = TRUE;
			update_weather();
		    }
		}    
		else    
    		    weather_frame_update(TRUE);
	    }
	    /* clear regions list */
	    if(app->regions_list)
		gtk_list_store_clear(app->regions_list);
	    /* clear locations list */
	    if(app->stations_list)
		gtk_list_store_clear(app->stations_list);    
	break;
	case GTK_RESPONSE_HELP:/* Pressed Button Help */
	    create_help_dialog();
	break;
	default:/* Pressed CANCEL */
	    if( flag_update_station && app->iap_connected ){
		if( g_slist_length(stations_view_list) > 0 ){
		    app->show_update_window = TRUE;
		    update_weather();
		}
		weather_buttons_init();
	    }
	break;
      }
	if(result_gtk_dialog_run !=  GTK_RESPONSE_HELP) 
    	    break; /* We  leave a cycle WHILE */
    }
    not_event = FALSE;
    if(flag_tuning_warning)
	hildon_banner_show_information(app->main_window,
					NULL,
					_("Use Edit layout \nfor tuning images of applet"));
    gtk_widget_destroy(window_config);
}
/*******************************************************************************/
/* get icon set names */
void create_icon_set_list(GtkWidget *store){
    Dirent	*dp;
    DIR		*dir_fd;
    gint	i = 0;
    char 	*temp_string = NULL;
    
    dir_fd	= opendir(ICONS_PATH);
    if(dir_fd){
	while( (dp = readdir(dir_fd)) ){
	    if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
		continue;
	    if(dp->d_type == DT_DIR){
		gtk_combo_box_append_text(GTK_COMBO_BOX(store), dp->d_name);
		if(!strcmp(app->config->icon_set, dp->d_name))
		    gtk_combo_box_set_active(GTK_COMBO_BOX(store), i);
		i++;
	    }
	}
	closedir(dir_fd);
	/* check if selected icon set not found */
	temp_string = gtk_combo_box_get_active_text(GTK_COMBO_BOX(iconset));
	if(!temp_string)
	    gtk_combo_box_set_active(GTK_COMBO_BOX(store), 0);
	else 
	    g_free(temp_string);
    }
    else{
    	gtk_combo_box_append_text(GTK_COMBO_BOX(store), app->config->icon_set);
	gtk_combo_box_set_active(GTK_COMBO_BOX(store), 0);
    }
}
/*******************************************************************************/
void create_help_dialog(void){
    GtkWidget	*help_dialog,
		*notebook,
		*title;
    char	tmp_buff[2048];
    gint	result;

    help_dialog = gtk_dialog_new_with_buttons(_("Other Maemo Weather Info"),
        				NULL,
					GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        				_("OK"), GTK_RESPONSE_ACCEPT,
					NULL);
/* Create Notebook widget */
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(help_dialog)->vbox),
        	    notebook = gtk_notebook_new(), TRUE, TRUE, 0);
/* About tab */
    snprintf(tmp_buff, sizeof(tmp_buff), "%s%s%s",
	    _("\nHildon desktop applet\n"
	    "for Nokia 770/N800\n"
	    "to show weather forecasts.\n"
	    "Version "), VERSION, 
	    _("\nCopyright(c) 2006-2007\n"
	    "Vlad Vasiliev, Pavel Fialko"));
	    
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_scrolled_window_with_text(tmp_buff,
						    GTK_JUSTIFY_CENTER),
				title = gtk_label_new(_("About")));
/* Authors tab */
    snprintf(tmp_buff, sizeof(tmp_buff), "%s",
		_("\nAuthor and maintenance:\n"
		"\tVlad Vasiliev, vlad@gas.by\n"
		"Maintenance:\n\tPavel Fialko, pavelnf@gmail.com"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_scrolled_window_with_text(tmp_buff,
						    GTK_JUSTIFY_LEFT),
				title = gtk_label_new(_("Authors")));
/* Thanks tab */
    snprintf(tmp_buff, sizeof(tmp_buff), "%s",
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
	    "Jarek Szczepanski aka Imrahil - for testing\n"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_scrolled_window_with_text(tmp_buff,
						    GTK_JUSTIFY_LEFT),
        			title = gtk_label_new(_("Thanks")));
/* Translators tab */
    snprintf(tmp_buff, sizeof(tmp_buff), "%s",
	    _("French - Nicolas Granziano\n"
	      "Russian - Pavel Fialko, Vlad Vasiliev,\n\t    Ed Bartosh\n"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_scrolled_window_with_text(tmp_buff,
						    GTK_JUSTIFY_LEFT),
        			title = gtk_label_new(_("Translators")));

    gtk_widget_show_all(help_dialog);
/* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(help_dialog));
    gtk_widget_destroy(help_dialog);
}
/*******************************************************************************/
GtkWidget* create_scrolled_window_with_text(const char* text,
				GtkJustification justification){

    GtkWidget	*text_view,
		*scrolled_window;
    GtkTextBuffer	*text_buffer;

    text_view = gtk_text_view_new();
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_buffer), text, -1);
    /* set params of text view */
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_justification(GTK_TEXT_VIEW(text_view),
				    justification);
    gtk_text_view_set_overwrite(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_NONE);
    /* scrolled window */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window),
					GTK_SHADOW_OUT);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 550, 200);
    /* pack childs to the scrolled window */
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(text_view));
    return scrolled_window;
}
/*******************************************************************************/
void station_list_view_select_handler(GtkTreeView *tree_view,
                                        gpointer user_data){
    struct weather_station *ws;
    GSList		*tmplist = NULL;
    GtkTreeIter		iter;
    gchar		*station_selected = NULL;
    GtkTreeSelection	*selected_line;
    GtkTreeModel	*model;


    selected_line = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
    if( !gtk_tree_selection_get_selected(selected_line, NULL, &iter) )
        return;
    gtk_tree_model_get(model, &iter, 0, &station_selected, -1);

    tmplist = stations_view_list;
    while(tmplist){
        ws = tmplist->data;
        if( ws->name_station &&
		station_selected && 
		!strcmp(ws->name_station, station_selected) ){
	    if(app->config->current_station_name)
		g_free(app->config->current_station_name);
	    app->config->current_station_name = g_strdup(ws->name_station);
	    if(app->config->current_station_id)
		g_free(app->config->current_station_id);
	    app->config->current_station_id = g_strdup(ws->id_station);
	    break;
	}
	tmplist = g_slist_next(tmplist);
    }
    g_free(station_selected);
    weather_frame_update(TRUE);
    new_config_save(app->config);
}
/*******************************************************************************/
void update_iterval_changed_handler(GtkComboBox *widget, gpointer user_data){
    time_t		update_time = 0;
    GtkLabel		*label;
    GtkTreeModel	*model;
    GtkTreeIter		iter;
    gchar		*temp_string,
			tmp_buff[100];

    label = GTK_LABEL(user_data);

    if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(widget), &iter)){
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
	gtk_tree_model_get(model, &iter, 0, &temp_string,
					 1, &update_time,
    					    -1);
	g_free(temp_string);
	app->config->update_interval = update_time;
	remove_periodic_event();
	add_periodic_event(time(NULL));
	
	/* fill next update field */
	update_time = next_update();
	if(!update_time)
	    temp_string = _("Never");
	else{
	    tmp_buff[0] = 0;
	    strftime(tmp_buff, sizeof(tmp_buff) - 1, "%X %x", localtime(&update_time));
	    temp_string = tmp_buff;
	}
	gtk_label_set_text(label, temp_string);
    }
}
/*******************************************************************************/
int get_active_time_update(GtkTreeModel *list, int time, const gchar *text){

    int		update_time = 0,
		index = 0;
    gboolean	valid = FALSE;
    GtkTreeIter	iter;
    gchar	*str_data = NULL;
    gint	int_data;

    valid = gtk_tree_model_get_iter_first((GtkTreeModel*)list, &iter);
    while(valid){
	gtk_tree_model_get(list, &iter, 
                    	    0, &str_data,
                    	    1, &int_data, -1);
	if(text){ /* if parameter is string */
	    if(!strcmp((char*)text, str_data)){
		update_time = int_data;
		break;
	    }
	}
	else{/* if parameter is int */
	    if(time == int_data){
		update_time = index;
		break;
	    }
	}
	g_free(str_data);
	index++;
	valid = gtk_tree_model_iter_next(list, &iter);
    }
    if(str_data)
	g_free(str_data);
    return update_time;
}
/*******************************************************************************/
void transparency_button_toggled_handler(GtkToggleButton *togglebutton,
                                        		    gpointer user_data){
    GtkWidget	*background_color;
    
    background_color = GTK_WIDGET(user_data);
    
    if(gtk_toggle_button_get_active(togglebutton))
	gtk_widget_set_sensitive(background_color, FALSE);
    else
	gtk_widget_set_sensitive(background_color, TRUE);
}
/*******************************************************************************/

