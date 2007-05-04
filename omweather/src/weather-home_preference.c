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

#include "weather-home_preference.h"
#if defined (BSD) && !_POSIX_SOURCE
    #include <sys/dir.h>
    typedef struct dirent Dirent;
#else
    #include <dirent.h>
    #include <linux/fs.h>
    typedef struct dirent Dirent;
#endif
/* Hack for Maemo SDK 2.0 */
#ifndef DT_DIR
#define DT_DIR 4
#endif


/* Compare station name */
gint compare_station(gconstpointer a, gconstpointer b){
    gint result;
    struct station_and_weather_code *sca, *scb;
    sca = (struct station_and_weather_code*)a;
    scb = (struct station_and_weather_code*)b;
 
    if(strlen(sca->station_name) < strlen(scb->station_name))
	result = strncmp(sca->station_name, scb->station_name,
			    strlen(scb->station_name));
    else 
	result = strncmp(sca->station_name, scb->station_name,
		    strlen(sca->station_name));
    return result;
}


/* Free memory allocated for stations list */
void free_list_stations(void){
    static GSList *stations_list_temp = NULL;
    struct station_and_weather_code *sc;
 
    if(stations_list_in_state){ 
	stations_list_temp = stations_list_in_state; 
	while(stations_list_temp){
	    sc = stations_list_temp->data;
	    g_free(sc->station_name);
	    g_free(sc->station_code);
	    g_free(sc);
	    stations_list_temp = g_slist_next(stations_list_temp);
	}
	g_slist_free(stations_list_in_state);
	stations_list_in_state = NULL;
    } 
}

/* Select item on country combobox */
void changed_country(void){
    GtkTreeModel *model;
    char flag; /* Flag for country processing */
    char flag_new_state; /* Flag for new country or province or state */
    char out_buffer[1024]; /* buffer for work with stations.txt files */
    static gchar *gcountry_name = NULL;
    FILE *stations_file, *iso3166_file; 
    char country_name[52];
    char country_code [3];
    char temp_state_name[20];
    int count_state = 0; /* Count state of file iso3166 */

/* Search Country defined ComboBox in ISO file */
    flag = FALSE;    
/* Clear the list. */
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(states));
    gtk_list_store_clear(GTK_LIST_STORE(model));
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(stations));
    gtk_list_store_clear(GTK_LIST_STORE(model));
    gcountry_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(countrys));
/* Search country code */    
    if((iso3166_file = fopen(COUNTRYS_FILE,"r")) != NULL ){
	while(!feof(iso3166_file)){
	    memset(out_buffer, 0, sizeof(out_buffer)); /* Clear buffer */
	    fgets(out_buffer, sizeof(out_buffer), iso3166_file);/* Read Next Line */
	    if(strlen(out_buffer) > 0){
		if(streq("----------------------------------------------------------------------\n",out_buffer))
    		    flag = (flag == TRUE) ? FALSE : TRUE ;
		else
		    if(flag == TRUE){
    			if(strcmp("\n",out_buffer) != 0){
    			    sprintf(country_name, "%.38s", out_buffer);
			    if(streq(gcountry_name, country_name)){
	    /* Write country code */
				country_code[0] = out_buffer[48];
				country_code[1] = out_buffer[49];	    
				country_code[2] = 0;
			    }
			}
		    }
	    }	
	}
    }
/* Search state or province on country and add it to combobox state */
    if((stations_file = fopen(STATIONS_FILE,"r")) != NULL){
	memset(temp_state_name, 0, sizeof(temp_state_name));
	flag_new_state = FALSE;
	while(!feof(stations_file)){
	    memset(out_buffer, 0, sizeof(out_buffer));
	    fgets(out_buffer, sizeof(out_buffer), stations_file);
	    if( (strlen(out_buffer)>0) && ((char)out_buffer[0] != '!') ){   
/* Is it country or state or province name ? */
    		if( out_buffer[19] != ' ' ){
		    sprintf(temp_state_name, "%.19s", out_buffer);
		    flag_new_state = TRUE;
		}  
		if( (flag_new_state == TRUE) &&
			(country_code[0] == out_buffer[81]) &&
			(country_code[1] == out_buffer[82]) ){
		    flag_new_state = FALSE;	
        	    gtk_combo_box_append_text(GTK_COMBO_BOX(states), temp_state_name);
		    count_state++;
/*		    if(app->current_state_name &&
			    (streq(temp_state_name, app->current_state_name)))
			index_state = count_state; */
		}    	  
	    } 
	}
    }    
    g_free(app->current_country);    
    app->current_country = gcountry_name;
    free_list_stations();
}

/* Select item on state combobox */
void changed_state(void){
    GtkTreeModel	*model = NULL;
    GSList		*current = NULL;
    char		flag,			/* Flag for country processing */
			flag_necessary_state,	/* Flag for finding country or province or state */
			out_buffer[1024],	/* buffer for work with stations.txt files */
			state_name[21],
			temp_station_name[21],
			temp_station_code[9];
    static gchar	*gstate_name = NULL;
    FILE		*stations_file = NULL; 
    struct station_and_weather_code *sc;
    int			count_station = 0,	/* Count station of state or region */
			i;
/* Search Country in the ComboBox*/
    flag = FALSE;    
/* Clear the list. */
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(stations));
    gtk_list_store_clear(GTK_LIST_STORE(model));
    free_list_stations();
    
    gstate_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(states));
    /* Search state or province on country and add stations to combobox*/
    if((stations_file = fopen(STATIONS_FILE,"r")) != NULL){
	memset(state_name, 0, sizeof(state_name));
	flag_necessary_state = FALSE;
	while(!feof(stations_file)){
	    memset(out_buffer, 0, sizeof(out_buffer));
	    fgets(out_buffer, sizeof(out_buffer), stations_file);
	    memset(temp_station_name, 0, sizeof(temp_station_name));
	    if( (strlen(out_buffer)>27) && ((char)out_buffer[0] != '!') ){  
        /* Is it country or state or province name ? */
    		if( out_buffer[19] != ' ' ){
		    sprintf(state_name, "%.19s", out_buffer);
		    flag_necessary_state = (streq(state_name,gstate_name)) ? TRUE : FALSE;
		} 
	/* Check for wrong string */ 
		if( (flag_necessary_state == TRUE) &&
			(strlen(out_buffer)>90) ){
	  /* Prepare strings station_name and code_name for work */
		    for(i = 3; i < 19; i++)
			temp_station_name[i - 3] = out_buffer[i];
		    for(i = 84; i < 92; i++)
			temp_station_code[i - 84] = out_buffer[i];
		    temp_station_code[8] = 0;
	  /* Trim right space */
		    for(i = 15; i > 0; i--){
			if(temp_station_name[i] == ' ') 
        		    temp_station_name[i] = 0;
    			else
        		    break;
		    }
    		    count_station++;
		    if((app->current_station_name) &&
			    (streq(temp_station_name, app->current_station_name)))
			index_station = count_station;
	  /* Add station and station code to list */	  
		    sc = g_new0(struct station_and_weather_code, 1);
		    sc->station_name = g_strdup(temp_station_name);	  
		    sc->station_code = g_strdup(temp_station_code);
		    stations_list_in_state = g_slist_append(stations_list_in_state, sc); /* Necessary free list  beyond !!! */
		}    	  
	    }
	}
    }    
/* Sort list */    
    stations_list_in_state = g_slist_sort(stations_list_in_state, compare_station);
/* Fill  gtk_combo_box */
    for(current = stations_list_in_state; current; current = current->next){
	sc = current->data;
/* Copy name station to combobox */
	gtk_combo_box_append_text(GTK_COMBO_BOX(stations), sc->station_name);
    }    
    g_free(gstate_name);
}

/* Select item on station combobox */
void changed_stations(void){
    struct station_and_weather_code *sc;
    static GSList *stations_list_temp = NULL;
    static char *temp_string = NULL;
  
    stations_list_temp = stations_list_in_state;
    while(stations_list_temp){
	sc = stations_list_temp->data;
	temp_string = gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations));
	if(streq(sc->station_name,temp_string)){
	    if(_weather_station_id_temp)
		g_free(_weather_station_id_temp);
	    _weather_station_id_temp = g_strdup(sc->station_code);
	    g_free(temp_string);
	    break;    
	}
	g_free(temp_string);
	stations_list_temp = g_slist_next(stations_list_temp);
    }
}

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
	if((app->current_station_id)&&streq(app->current_station_id, ws->id_station))
    	    gtk_tree_selection_select_iter(list_selection, &iter);
	tmplist = g_slist_next(tmplist);
    }
}
/* Rename the station name */
void 
weather_window_rename_station(GtkWidget *widget,
            			   GdkEvent *event,
                    		   gpointer user_data){
    
    GtkWidget *window_rename_station;
    GtkWidget *label;
    GtkWidget *table;
    GtkWidget *station_name_edit;
    
    GSList *tmplist = NULL; /* Temporary for station list */
    struct weather_station *ws; /* Description Weather station */
    GtkTreeIter iter;
    gchar *station_selected = NULL;
    GtkTreeModel *model;
    GtkTreeSelection *selection;

    model = gtk_tree_view_get_model(GTK_TREE_VIEW(station_list_view));
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(station_list_view));
    if( !gtk_tree_selection_get_selected(selection, NULL, &iter) )
	return;
    gtk_tree_model_get(model, &iter, 0, &station_selected, -1); 
    
    /* Create dialog window */
    window_rename_station = gtk_dialog_new_with_buttons(_("Rename Station"),
        						NULL,
							GTK_DIALOG_MODAL,
        						GTK_STOCK_OK,
							GTK_RESPONSE_ACCEPT, NULL);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_rename_station)->vbox),
        		    table = gtk_table_new(2, 2, FALSE), TRUE, TRUE, 0);	    
    gtk_dialog_add_button(GTK_DIALOG(window_rename_station),
        		    GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT);
   /* Add Label and Edit */
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Station:")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0.f, 0.f, 0.f, 0.f),
        			1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label),station_name_edit = gtk_entry_new());
    gtk_entry_set_max_length(GTK_ENTRY(station_name_edit),16);
    gtk_entry_set_text(GTK_ENTRY(station_name_edit),station_selected);
    gtk_widget_set_size_request(GTK_WIDGET(window_rename_station), 350, -1);
    gtk_widget_show_all(window_rename_station);   

    /* start dialog */
    switch(gtk_dialog_run(GTK_DIALOG(window_rename_station))){
	case GTK_RESPONSE_ACCEPT:/* Press Button Ok */
	    tmplist = stations_view_list;
	    while(tmplist){
		ws = tmplist->data;
		if((ws->name_station != NULL && station_selected != NULL && streq(station_selected, ws->name_station))||
		  (ws->name_station == NULL && station_selected == NULL)){
		    if(ws->name_station)
			g_free(ws->name_station);
   		    ws->name_station = g_strdup(gtk_entry_get_text(GTK_ENTRY(station_name_edit))); 
		    /* Update station list */	    
		    gtk_list_store_clear(station_list_store);
		    fill_station_list_view (station_list_view,station_list_store);
		    /* Update config file */
		    config_save();
		    flag_update_station = TRUE;
		}
 		tmplist = g_slist_next(tmplist);  
	    }
	break;
	default:
	break;
    }
    if (station_selected)
        g_free(station_selected);
    gtk_widget_destroy(window_rename_station);
}

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
    
    fprintf(stderr,"%s()\n", __PRETTY_FUNCTION__);
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
	     if(app->current_station_id)
		g_free(app->current_station_id);
   	     app->current_station_id = g_strdup(ws->id_station); 
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
    			if(app->current_station_id)
			    g_free(app->current_station_id);
			app->current_station_id = g_strdup(ws->id_station); 
			if(app->current_station_name)
			    g_free(app->current_station_name);
    			app->current_station_name = g_strdup(ws->name_station); 
		    }
		    tmplist = g_slist_next(tmplist);
		} 
	    }
	    else
		app->current_station_id = NULL;
      /* Update config file */
	    config_save();       
	    break; 
	}
	tmplist = g_slist_next(tmplist);
    }
    if (g_slist_length(stations_view_list) == 0){
       if(app->current_station_name)
          g_free(app->current_station_name);
       if(app->current_station_id)
          g_free(app->current_station_id);
       app->current_station_name = NULL;
       app->current_station_id = NULL;
       weather_frame_update(FALSE);
       /* Update config file */
       config_save();
    }
    fprintf(stderr,"End %s()\n", __PRETTY_FUNCTION__);    
    if (station_selected)
        g_free (station_selected);
    return TRUE;
}

static GtkListStore* create_station_list_store(void){
    GtkListStore *station_list = NULL;
    station_list = gtk_list_store_new(1, G_TYPE_STRING);
    return station_list;
}


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

#define ZERO(type, name) type name; memset(&name, 0, sizeof name)
#define SIG_TIMER_EXPIRATION SIGRTMIN
#define CLOCK_TYPE CLOCK_MONOTONIC
struct timespec get_time_stamp(void){
    ZERO(struct timespec, now);
    if(clock_gettime(CLOCK_TYPE, &now) != 0)
	perror("clock_gettime");
    fprintf(stderr, "NOW=%ld.%09ld\n", now.tv_sec, now.tv_nsec);
    return now;
}

void weather_window_add_custom_station(){
    struct weather_station *ws;       /* Temp struct for station */
    GtkWidget *window_add_custom_station;
    GtkWidget *label;
    GtkWidget *table;

/* Create dialog window */
    window_add_custom_station = gtk_dialog_new_with_buttons(_("Add Custom Station"),
        							NULL, GTK_DIALOG_MODAL,
        							GTK_STOCK_OK,
								GTK_RESPONSE_ACCEPT, NULL);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_add_custom_station)->vbox),
        		    table = gtk_table_new(4, 2, FALSE), TRUE, TRUE, 0);	    
    gtk_dialog_add_button(GTK_DIALOG(window_add_custom_station),
        		    GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT);
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
		if(app->current_station_id != NULL)
		    g_free(app->current_station_id);
		app->current_station_id = g_strdup(gtk_entry_get_text((GtkEntry*)custom_station_code));
		ws->id_station = g_strdup(app->current_station_id);
		if(app->current_station_name)
		    g_free(app->current_station_name);
		app->current_station_name = g_strdup(gtk_entry_get_text((GtkEntry*)custom_station_name));
		ws->name_station = g_strdup(app->current_station_name);
	    /* Add station to stations list */
		stations_view_list = g_slist_append(stations_view_list, ws); 
	    /* Add station to View List(Tree) */
		gtk_list_store_clear(station_list_store);
		fill_station_list_view (station_list_view,station_list_store);
	    /* Update config file */
		config_save();
		flag_update_station = TRUE;
	break;
	default:
	break;
    }
    gtk_widget_destroy(window_add_custom_station);
}

void weather_window_add_station(GtkWidget *widget,
            			GdkEvent *event,
                    		gpointer user_data){
    FILE  *iso3166_file;
    char country_name[52];
    char out_buffer[1024];   /* Buffer for work with stations.txt files */
    char flag;                        /* Flag for country processing */
    int count_country = 0;            /* Count country of file iso3166 */
    int index_country = 0;            /* Position country of the list */
    struct weather_station *ws;       /* Temp struct for station */
    GtkTreeIter iter;                 /* Temp for gtk_combo_box */
    GtkListStore *country_list_store; /* Country List store */
    GtkWidget *label, *table;

/* Create dialog window */
    window_add_station = gtk_dialog_new_with_buttons(_("Add Station"),
        						NULL,
							GTK_DIALOG_MODAL,
        						GTK_STOCK_OK,
							GTK_RESPONSE_ACCEPT, NULL);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window_add_station)->vbox),
        		table = gtk_table_new(4, 2, FALSE), TRUE, TRUE, 0);
    gtk_dialog_add_button(GTK_DIALOG(window_add_station),
                    		_("Add Custom Station"), OMW_RESPONSE_ADD_CUSTOM_STATION);
    gtk_dialog_add_button(GTK_DIALOG(window_add_station),
        			GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT);
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
 
    country_list_store = create_station_list_store();
  
    gtk_widget_show_all(window_add_station);   
/* Inserting Countrys to ComboBox from ISO file*/
    flag = FALSE;
    if((iso3166_file = fopen(COUNTRYS_FILE,"r")) != NULL){
	while(!feof(iso3166_file)){
	    memset(out_buffer, 0, sizeof(out_buffer)); /* Clear buffer */
	    fgets(out_buffer, sizeof(out_buffer), iso3166_file); /* Read Next Line */
	    if(strlen(out_buffer)>0){
		if(streq("----------------------------------------------------------------------\n",out_buffer))
    		    flag = (flag == TRUE) ? FALSE : TRUE ;
		else
		    if(flag == TRUE){
    			if(strcmp("\n",out_buffer) != 0){
    			    sprintf(country_name,"%.38s",out_buffer);
			    gtk_list_store_append(GTK_LIST_STORE(country_list_store), &iter);
    			    gtk_list_store_set(GTK_LIST_STORE(country_list_store), &iter,
                        			0, country_name,-1);
			    count_country++;
    			    if((app->current_country) &&(streq(country_name, app->current_country)))
    				index_country = count_country;
			}
		    }
	    }
	}
	fclose(iso3166_file);
    }
    gtk_combo_box_set_model((GtkComboBox*)countrys, (GtkTreeModel*) country_list_store);  
/* Set default value to country combo_box */
    if(index_country != 0){
	gtk_combo_box_set_active (GTK_COMBO_BOX(countrys),index_country-1);
	changed_country();
    }
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
	    if(app->current_station_id != NULL)
	        g_free(app->current_station_id);
	    app->current_station_id = g_strdup(_weather_station_id_temp);
	    ws->id_station = g_strdup(_weather_station_id_temp);
	    if(app->current_station_name)
		g_free(app->current_station_name);
	    app->current_station_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(stations));
	    ws->name_station = g_strdup(app->current_station_name);
/* Add station to stations list */
	    stations_view_list = g_slist_append(stations_view_list, ws); 
/* Update config file */
	    config_save();
/* Add station to View List(Tree) */
	    gtk_list_store_clear(station_list_store);
	    fill_station_list_view (station_list_view,station_list_store);
	    break;
    }
    gtk_widget_destroy(window_add_station); 
}

/* Main preference window */
void weather_window_preference(GtkWidget *widget,
				GdkEvent *event,
				gpointer user_data){

    GtkWidget	*window_config = NULL,
		*notebook = NULL,
		*label = NULL,
		*table = NULL,
		*font_color = NULL,
		*chk_transparency = NULL,
		*scrolled_window = NULL,
		*button_add = NULL,
		*button_del = NULL,
		*button_ren = NULL;		

    char flag_update_icon = '\0'; /* Flag update main weather icon of desktop */
    gboolean flag_tuning_warning; /* Flag for show the warnings about tuning images of applet */
    int index_update_time = 0; /* Position active update time of the list */
    GdkColor _weather_font_color_temp; /* Temporary for font color */
    static GSList *time_update_list_temp = NULL; /* Temporary list for time update */
    struct time_update *tu; /* Temporary for time update list */
    static char *temp_string; /* Temporary for the results differnet strdup functions */
    static int result_gtk_dialog_run; /* Temporary for the gtk_dialog_run result */
    time_t	next_update_time = 0;
/*#ifdef PC_EMULATOR*/
    char	tmp_buff[2048];
/*#endif*/
    not_event = TRUE;
    flag_update_station = FALSE;
    flag_update_icon = FALSE;
    flag_tuning_warning = FALSE; 

    if (!app->dbus_is_initialize)
			weather_initialize_dbus();


    window_config = gtk_dialog_new_with_buttons(_("Other Maemo Weather Settings"),
        				NULL,
					GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        				GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
					NULL);
    /* add CANCEL button */
    gtk_dialog_add_button(GTK_DIALOG(window_config),
            GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT);
    /* add Help button */
    gtk_dialog_add_button(GTK_DIALOG(window_config),
            GTK_STOCK_ABOUT, GTK_RESPONSE_HELP);
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
    button_ren = gtk_button_new_with_label(_("Rename"));
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
                	G_CALLBACK(weather_window_rename_station), NULL);				
    g_signal_connect(button_del, "clicked",
                	G_CALLBACK(weather_delete_station), NULL);
    g_signal_connect(button_add, "clicked",
                	G_CALLBACK(weather_window_add_station),
                	NULL);
/* Interface tab */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			table = gtk_table_new(1, 6, FALSE),
        			label = gtk_label_new(_("Interface")));
/* Days to show */
    app->days_to_show--; /* count down, because combobox items start with 0 */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
				label = gtk_label_new(_("Days to show:")),
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
    switch(app->days_to_show){
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
    app->days_to_show++; /* count up to return to real value */
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
    switch(app->icons_layout){
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
    switch(app->icons_size){
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
    gtk_color_button_set_color(GTK_COLOR_BUTTON(font_color), &(app->font_color));
    /* Transparency */
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_label_new(_("Transparency")),
        			0, 1, 5, 6);
    gtk_table_attach_defaults(GTK_TABLE(table),	    
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f) ,
        			1, 2, 5, 6);
    gtk_container_add(GTK_CONTAINER(label),chk_transparency = gtk_check_button_new());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chk_transparency),
        			    app->transparency);
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
    switch(app->temperature_units){
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
    gtk_combo_box_append_text(GTK_COMBO_BOX(units), _("Miles (International)"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(units), _("Miles (Imperial)"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(units), _("Miles (Sea)"));
    switch(app->distance_units){
	default:
	case METERS:			gtk_combo_box_set_active(GTK_COMBO_BOX(units), METERS);break;
	case KILOMETERS:	 	gtk_combo_box_set_active(GTK_COMBO_BOX(units), KILOMETERS);break;
	case INTERNATIONAL_MILES:	gtk_combo_box_set_active(GTK_COMBO_BOX(units), INTERNATIONAL_MILES);break;
	case IMPERIAL_MILES:		gtk_combo_box_set_active(GTK_COMBO_BOX(units), IMPERIAL_MILES);break;
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
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("km/s"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("mi/s"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("m/h"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("km/h"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(wunits), _("mi/h"));
    switch(app->wind_units){
	default:
	case METERS_S:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), METERS_S);break;
	case KILOMETERS_S:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), KILOMETERS_S);break;
	case MILES_S:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), MILES_S);break;
	case METERS_H:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), METERS_H);break;
	case KILOMETERS_H:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), KILOMETERS_H);break;
	case MILES_H:  gtk_combo_box_set_active(GTK_COMBO_BOX(wunits), MILES_H);break;
    }    
/* Update tab */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			table = gtk_table_new(2, 2, FALSE),
        			label = gtk_label_new(_("Update")));
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Updating of weather data:")),
        			0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 0, 1);
    gtk_container_add(GTK_CONTAINER(label), update_time = gtk_combo_box_new_text());
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_label_new(_("Next update:")),
        			0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),
        			label = gtk_alignment_new(0, 0.5, 0.f, 0.f),
        			1, 2, 1, 2);
    next_update_time = next_update();
    if(!next_update_time)
	temp_string = _("Never");
    else{
	temp_string = ctime(&next_update_time);
	temp_string[strlen(temp_string) - 1] = 0;
    }
    gtk_container_add(GTK_CONTAINER(label), gtk_label_new(temp_string));
/* Fill update time box */
    time_update_list_temp = time_update_list;
    while(time_update_list_temp != NULL){
	tu = time_update_list_temp->data;
	gtk_combo_box_append_text(GTK_COMBO_BOX(update_time), tu->name_between_time);
	if(tu->between_time == app->update_interval)
	    gtk_combo_box_set_active(GTK_COMBO_BOX(update_time), index_update_time);
	time_update_list_temp = g_slist_next(time_update_list_temp);
	index_update_time++;
    }
/*#ifdef PC_EMULATOR*/
/* Evetns list tab */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    print_list(tmp_buff, sizeof(tmp_buff) - 1);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_scrolled_window_with_text(tmp_buff,
						    GTK_JUSTIFY_LEFT),
        			label = gtk_label_new(_("Events")));
/*#endif*/
    gtk_widget_show_all(window_config);
/* kill popup window :-) */
    if (app->popup_window){
	gtk_widget_destroy(app->popup_window);
	app->popup_window = NULL;
    }

    while (0 != (result_gtk_dialog_run = gtk_dialog_run(GTK_DIALOG(window_config)))){
/* start dialog window */
      switch(result_gtk_dialog_run){
	case GTK_RESPONSE_ACCEPT:/* Pressed Button Ok */
/* icon set */	
	    temp_string = gtk_combo_box_get_active_text(GTK_COMBO_BOX(iconset));
	    if(strcmp(app->icon_set, temp_string)){
		if(app->icon_set)
		    g_free(app->icon_set);
/*	        app->icon_set = g_strdup(gtk_combo_box_get_active_text(GTK_COMBO_BOX(iconset))); */
	        app->icon_set = g_strdup(temp_string);
		memset(path_large_icon, 0, sizeof(path_large_icon));
		sprintf(path_large_icon, "%s%s/", ICONS_PATH, app->icon_set);
		flag_update_icon = TRUE;
	    }
	    g_free(temp_string);
/* icon size */	    
	    if(app->icons_size != gtk_combo_box_get_active(GTK_COMBO_BOX(icon_size))){
		app->icons_size = gtk_combo_box_get_active(GTK_COMBO_BOX(icon_size));
		flag_update_icon = TRUE;
		flag_tuning_warning = TRUE;
	    }
/* Temperature units */
	    if(app->temperature_units != gtk_combo_box_get_active(GTK_COMBO_BOX(temperature_unit))){
		app->temperature_units = gtk_combo_box_get_active(GTK_COMBO_BOX(temperature_unit));
		flag_update_icon = TRUE;
	    }
/* Font color */
	    gtk_color_button_get_color(GTK_COLOR_BUTTON(font_color), &_weather_font_color_temp);
	    if(( _weather_font_color_temp.red - app->font_color.red ) ||
		    ( _weather_font_color_temp.green - app->font_color.green ) ||
		    ( _weather_font_color_temp.blue - app->font_color.blue )){
		memcpy(&(app->font_color), &_weather_font_color_temp, sizeof(app->font_color));
    		flag_update_icon = TRUE;
	    }
/* Days to show */
	    if( gtk_combo_box_get_active((GtkComboBox*)days_number)!= app->days_to_show - 1){
		app->previos_days_to_show = app->days_to_show;/* store previos number of icons */
		app->days_to_show = gtk_combo_box_get_active((GtkComboBox*)days_number);
		app->days_to_show++;
    		flag_update_icon = TRUE;
		flag_tuning_warning = TRUE;
	    }
/* Layout Type */
	    if( gtk_combo_box_get_active((GtkComboBox*)layout_type) != app->icons_layout ){
		app->icons_layout = gtk_combo_box_get_active((GtkComboBox*)layout_type);
    		flag_update_icon = TRUE;
		flag_tuning_warning = TRUE;
	    }
/* Transparency mode */
    	    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chk_transparency)) != app->transparency){
		app->transparency = 
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chk_transparency));
    		flag_update_icon = TRUE;
	    }
/* Distance units */
	    if( gtk_combo_box_get_active((GtkComboBox*)units) != app->distance_units ){
		app->distance_units = gtk_combo_box_get_active((GtkComboBox*)units);
    		flag_update_icon = TRUE;
	    }
/* Wind units */
	    if( gtk_combo_box_get_active((GtkComboBox*)wunits) != app->wind_units ){
		app->wind_units = gtk_combo_box_get_active((GtkComboBox*)wunits);
    		flag_update_icon = TRUE;
	    }
/* Find select element of update time box and save time value */
	    time_update_list_temp = time_update_list;
	    while(time_update_list_temp){
    		tu = time_update_list_temp->data;
		temp_string =gtk_combo_box_get_active_text(GTK_COMBO_BOX(update_time));
		if(!strcmp(tu->name_between_time,temp_string)){
		    app->update_interval = tu->between_time;
		    remove_periodic_event();
		    add_periodic_event(time(NULL));
		    g_free(temp_string);
		    break;
		}    	  
		g_free(temp_string);
    		time_update_list_temp = g_slist_next(time_update_list_temp);
    	    }
    	    config_save();
	    if(flag_update_icon){
    		weather_frame_update(FALSE);
		app->previos_days_to_show = app->days_to_show;/* store previos number of icons */
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
	    free_list_stations();
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
/* ???		config_save(); */
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
		if(!strcmp(app->icon_set, dp->d_name))
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
    	gtk_combo_box_append_text(GTK_COMBO_BOX(store), app->icon_set);
	gtk_combo_box_set_active(GTK_COMBO_BOX(store), 0);
    }
}

void create_help_dialog(void){
    GtkWidget	*help_dialog,
		*notebook,
		*title;
    char	tmp_buff[2048];
    gint	result;

    help_dialog = gtk_dialog_new_with_buttons(_("Other Maemo Weather Info"),
        				NULL,
					GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        				GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
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
	    "Eugene Roytenberg - for testing\n"
	    "Jarek Szczepanski aka Imrahil - for testing\n"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        			create_scrolled_window_with_text(tmp_buff,
						    GTK_JUSTIFY_LEFT),
        			title = gtk_label_new(_("Thanks")));

    gtk_widget_show_all(help_dialog);
/* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(help_dialog));
    gtk_widget_destroy(help_dialog);
}

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
	    if(app->current_station_name)
		g_free(app->current_station_name);
	    app->current_station_name = g_strdup(ws->name_station);
	    if(app->current_station_id)
		g_free(app->current_station_id);
	    app->current_station_id = g_strdup(ws->id_station);
	    break;
	}
	tmplist = g_slist_next(tmplist);
    }
    g_free(station_selected);
    weather_frame_update(TRUE);
    config_save_current_station();
}
