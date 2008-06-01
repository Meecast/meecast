/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
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
#ifndef _weather_common_h
#define _weather_common_h 1
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <gtk/gtk.h>
#include <gconf/gconf-client.h>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE /* glibc2 needs this */
#endif
#include <time.h>
#include <libgnomevfs/gnome-vfs.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <string.h> 
#ifdef OS2008 
    #include <hildon/hildon-banner.h>
    #include <libhildondesktop/hildon-desktop-picture.h>
    #include <libhildondesktop/hildon-desktop-home-item.h>
    #include <libhildondesktop/libhildondesktop/hildon-home-area.h>
    #include <X11/extensions/Xcomposite.h>
    #include <X11/extensions/Xdamage.h>
    #include <X11/extensions/Xrender.h>
    #include <location/location-gps-device.h>
#else
    #include <hildon-widgets/hildon-banner.h>
#endif
#include <sys/types.h>
#include <libosso.h>
#include <wchar.h>
#include <stdio.h>
#include <limits.h>
/* localization headers and defines */
#include <libintl.h>
#include <locale.h>
#ifdef USE_CONIC
#include <conic/conic.h>
#endif
/*******************************************************************************/
#define _(String) dgettext (GETTEXT_PACKAGE, String)
#define Max_count_weather_day	5
#define COUNTRIESFILE		"/usr/share/omweather/countries.list"
#define REGIONSFILE		"/usr/share/omweather/regions.list"
#define LOCATIONSFILE		"/usr/share/omweather/locations.list"
#define ICONS_PATH		"/usr/share/omweather/icons/"
#define BUTTON_ICONS		"/usr/share/omweather/button_icons/"
#define SUPER_GIANT_ICON_SIZE	256
#define GIANT_ICON_SIZE		128
#define LARGE_ICON_SIZE		96
#define BIG_ICON_SIZE		80
#define MEDIUM_ICON_SIZE	64
#define SMALL_ICON_SIZE		48
#define TINY_ICON_SIZE		32
#define START_FUNCTION		fprintf(stderr,"\n>>>>>>>>>Start %s()\n", __PRETTY_FUNCTION__);
#define END_FUNCTION		fprintf(stderr,"\n>>>>>>>>>End %s()\n", __PRETTY_FUNCTION__);
/*******************************************************************************/
enum { AUTOUPDATE, DAYTIMEEVENT, DBUSINITEVENT, UPDATE_AFTER_CONNECTED, CHECK_GPS_POSITION};
enum { ONE_ROW, ONE_COLUMN, TWO_ROWS, TWO_COLUMNS, COMBINATION };
enum { METERS, KILOMETERS, MILES, SEA_MILES };
enum { METERS_S, KILOMETERS_H, MILES_H };
enum { CELSIUS, FAHRENHEIT };
enum { UNKNOWN, TINY, SMALL, MEDIUM, LARGE, GIANT };
enum { ICON, STATION_NAME };
enum { MB, INCH };
/*******************************************************************************/
typedef struct weather_day_button_with_image{
    GtkWidget	*button;                                                                                               
    GtkWidget	*label;                                                                                                
    GtkWidget	*box;
    GdkPixbuf   *icon_buffer;                                                                                                             
    GtkWidget   *icon_image; 
}WDB;
/*******************************************************************************/
typedef struct{
    int		icon;
    gchar	title[80];		/* Title */
    gchar	hmid[80];       	/* Humidity */
    gchar	wind_speed[80];       	/* Wind Speed */
    gchar	wind_gust[80];        	/* Wind Gust */
    gchar	vis[80];              	/* Visibilty */
    gchar	wind_title[80];		/* Wind Title */
    gchar	temp[20];		/* Temperature */
    time_t	begin_time;		/* Begin time party of the day */
    float	pressure;		/* Pressure value */
    gchar	pressure_str[80];	/* Pressure direction */
    int		moon_icon;		/* moon icon */
    gchar	moon[60];		/* moon phase */
}part_of_day;
/*******************************************************************************/
typedef struct{
    part_of_day	day;		/* Or current weather */
    part_of_day	night;
    gchar	date[40];	/* Date */     
    time_t	date_time;	/* Date of the year or current time*/  
    time_t      zone;           /* time zone */
    gchar	sunrise[60];	/* sunrise */
    gchar	sunset[60];	/* sunset */
    gchar	dayshname[60];	/* Short name of day */
    gchar	dayfuname[60];	/* Full name of day */
    gchar	hi_temp[20];	/* High temperature of day or real current temperature for current day */
    gchar	low_temp[20];	/* Low temperature  of day or feels like temperature for current day */
    gchar	location[50];	/* Location */
}weather_day;
/*******************************************************************************/
struct event_time{
    time_t	time;          	/* Time event */
    short	type_event; 	/* Type of event: Automatic update event AUTOUPDATE, 
				    time of day event DAYTIMEEVENT */ 
};
/*******************************************************************************/
struct time_update{ 
    gint	between_time;        /* Time in minutes  betwen two update weather */
    gchar	*name_between_time;  /* Human string it time */
};
/*******************************************************************************/
struct weather_station{
    gchar	*id_station;
    gchar	*name_station;
    gboolean    is_gps;
};
/*******************************************************************************/
typedef	struct{
    char	name[50];
    long	start;
    long	end;
}Country_item;
/*******************************************************************************/
typedef	struct{
    char	name[50];
    long	start;
    long	end;
    double	minlat;
    double	minlon;
    double	maxlat;
    double	maxlon;
}Region_item;
/*******************************************************************************/
typedef struct{
    char	name[50];
    char	id0[10];
    double	latitude;
    double	longtitude;
}Station;
/*******************************************************************************/
typedef struct applet_config{
    gchar	*cache_dir_name;
    gchar	*icon_set;
    gchar	*current_country;
    gchar	*current_station_name;
    gchar	*current_station_id;
    gchar 	*iap_http_proxy_host;
    gchar 	*cache_directory;
    gint 	iap_http_proxy_port;
    gint	update_interval;
    gint	switch_time;
    gint	icons_size;
    gint	icons_layout;
    gint	current_settings_page;
    gint	days_to_show;
    gint	previos_days_to_show;
    gint	distance_units;
    gint	wind_units;
    gint	temperature_units;
    gint	pressure_units;
#ifdef OS2008
    gint	display_at;
    gboolean	use_sensor;
    guint	sensor_update_time;
#endif
    guint	data_valid_interval;
    gboolean	transparency;
    gboolean	separate;
    gboolean	swap_hi_low_temperature;
    gboolean	hide_station_name;
    gboolean	hide_arrows;
    gboolean	downloading_after_connecting;
    gboolean    gps_station;
    gboolean	show_wind;
    GdkColor	font_color;
    GdkColor	background_color;
}AppletConfig;
/*******************************************************************************/
typedef struct OMWeatherApplet{
    osso_context_t	*osso;
    GHashTable		*hash;
    GtkWidget		*top_widget;
    GtkWidget		*main_window;
    GtkWidget		*popup_window;
    GtkWidget		*popup_window_more;    
    int			button_pressed;
    AppletConfig	*config;
    gboolean		show_update_window;
    gboolean		iap_connected;
    gboolean		iap_connecting;
    long		iap_connecting_timer;  
    guint		timer;
    guint		timer_for_os2008;
    guint		switch_timer;
    guint		sensor_timer;
    WDB			*buttons[Max_count_weather_day];
    guint		flag_updating;
    gboolean		dbus_is_initialize;
    gboolean		gps_must_be_current;    
    weather_day		weather_current_day;
    weather_day		weather_days[Max_count_weather_day];
    GtkListStore	*countrys_list;
    GtkListStore	*regions_list;
    GtkListStore	*stations_list;
    GtkListStore	*user_stations_list;
    GtkListStore	*time_update_list;
    GtkWidget 		*contextmenu;
    gboolean 		widget_first_start;
    int 		widget_showing;
    gboolean 		home_item_flag_expose;
    gboolean 		area_button_pressed;
    gboolean 		area_button_release;
    gboolean		area_changed;
    GdkPixbuf 		*pixbuf;
    guint		signal_expose;
    guint		signal_area_expose;
    guint		signal_item_expose;
    guint		my_applet_signal_release;
    guint		signal_release;
    guint		signal_press;
    guint		signal_size_request;
    guint		signal_area_changed;
    GtkWidget 		*parent_parent;
    GtkWidget 		*parent;
    gpointer		*child_data;
    int 		ax;
    int			ay;
    int			aw;
    int			ah;
#ifdef OS2008
    guint		gps_id_connection;
    LocationGPSDevice 	*gps_device;
    Station		gps_station;
    gdouble		temporary_station_latitude;
    gdouble		temporary_station_longtitude;
    float		sensor_data;
#endif
    gchar               temperature_string[1024];
    gchar               forecast_string[2048];
#ifdef USE_CONIC    
    ConIcConnection 	*connection;
#endif        
}OMWeatherApp;

/*******************************************************************************/
void free_list_time_event(void);
void time_event_add(time_t time_value, short int type_event);
extern void popup_window_destroy(void);
extern int new_read_config(AppletConfig*);
/*******************************************************************************/
extern OMWeatherApp	*app;
/*******************************************************************************/
#endif
