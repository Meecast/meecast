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
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gconf/gconf-client.h>
#include <libgnomevfs/gnome-vfs.h>
#include <sys/types.h>
#include <libosso.h>
#include <wchar.h>
#include <limits.h>
#include <libintl.h>
#include <locale.h> 
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

#ifdef USE_CONIC
#include <conic/conic.h>
#endif
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
/*******************************************************************************/
#define _(String) dgettext (GETTEXT_PACKAGE, String)
#define Max_count_weather_day	10
#define COUNTRIESFILE		"countries.list"
#define REGIONSFILE		"regions.list"
#define LOCATIONSFILE		"locations.list"
#define ICONS_PATH		"/usr/share/omweather/icons/"
#define BUTTON_ICONS		"/usr/share/omweather/button_icons/"
#define START_FUNCTION		fprintf(stderr,"\n>>>>>>>>>Start %s()\n", __PRETTY_FUNCTION__);
#define END_FUNCTION		fprintf(stderr,"\n>>>>>>>>>End %s()\n", __PRETTY_FUNCTION__);
/*******************************************************************************/
enum { AUTOUPDATE, CHANGE_DAY_PART, DBUSINITEVENT, UPDATE_AFTER_CONNECTED, CHECK_GPS_POSITION};
enum { ONE_ROW, ONE_COLUMN, TWO_ROWS, TWO_COLUMNS, COMBINATION };
enum { METERS, KILOMETERS, MILES, SEA_MILES };
enum { METERS_S, KILOMETERS_H, MILES_H };
enum { CELSIUS, FAHRENHEIT };
enum { TINY, SMALL, MEDIUM, LARGE, GIANT };
enum { ICON, STATION_NAME };
enum { MB, INCH };
enum { WEATHER_COM1, WEATHER_COM2, RP5_RU, MAX_WEATHER_SOURCE_NUMBER };
enum { TINY_ICON_SIZE = 32, SMALL_ICON_SIZE = 48, MEDIUM_ICON_SIZE = 64,
	BIG_ICON_SIZE = 80, LARGE_ICON_SIZE = 96, GIANT_ICON_SIZE = 128,
	SUPER_GIANT_ICON_SIZE = 256 };
enum { SETTINGS_STATIONS_PAGE, SETTINGS_INTERFACE_PAGE, SETTINGS_UNITS_PAGE,
	SETTINGS_UPDATE_PAGE, SETTINGS_SENSOR_PAGE, ABOUT_PAGE };
/*******************************************************************************/
typedef struct{
    gint	error;
    xmlDoc	*doc;
    xmlNode	*weather_com_root;
}weather_com_parser;
/*******************************************************************************/
typedef struct{
    GSList	*location;
    GSList	*current;
    GSList	*days;
    gboolean	current_data_is_invalid;
}WeatherStationData;
/*******************************************************************************/
typedef struct weather_data_source{
    gchar	*name;
    gchar	*db_path;
    gchar	*url;
    gint 	(*parser)(const gchar *station_id, weather_com_parser *parser,
			    WeatherStationData *wsd);
}WeatherSource;
/*******************************************************************************/
typedef struct weather_day_button_with_image{
    GtkWidget	*button;                                                                                               
    GtkWidget	*label;                                                                                                
    GtkWidget	*box;
    GdkPixbuf   *icon_buffer;                                                                                                             
    GtkWidget   *icon_image; 
}WDB;
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
    gchar	*font;
    gchar	*current_country;
    gchar	*current_station_name;
    gchar	*current_station_id;
    guint	current_station_source;
    gchar 	*iap_http_proxy_host;
    gchar 	*cache_directory;
    gint	weather_source;
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
#if defined(OS2008) || defined(DEBUGTEMP)
    gint	display_at;
    gboolean	use_sensor;
    guint	sensor_update_time;
    guint 	alpha_comp;
    guint	corner_radius;
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
    GdkColor	ui_background_color;
    gboolean	ui_background_color_on;
}AppletConfig;
/*******************************************************************************/
typedef struct OMWeatherApplet{
    osso_context_t	*osso;
    GHashTable		*hash;
    GtkWidget		*top_widget;
    GtkWidget		*main_window;
    GtkWidget		*popup_window;
    GtkWidget		*popup_window_more;    
    gint		button_pressed;
    AppletConfig	*config;
    gboolean		show_update_window;
    gboolean		iap_connected;
    gboolean		iap_connecting;
    glong		iap_connecting_timer;  
    guint		timer;
    guint		timer_for_os2008;
    guint		switch_timer;
    guint		sensor_timer;
    guint		count_day;		/* days number from parser */
    GSList		*buttons;
    guint		flag_updating;
    gboolean		dbus_is_initialize;
    gboolean		gps_must_be_current;    
    GtkListStore	*countrys_list;
    GtkListStore	*regions_list;
    GtkListStore	*stations_list;
    GtkListStore	*user_stations_list;
    GtkListStore	*time_update_list;
    GtkWidget 		*contextmenu;
    gboolean 		widget_first_start;
    gint 		widget_showing;
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
    gint 		ax;
    gint		ay;
    gint		aw;
    gint		ah;
#ifdef OS2008
    guint		gps_id_connection;
    LocationGPSDevice 	*gps_device;
    Station		gps_station;
    gdouble		temporary_station_latitude;
    gdouble		temporary_station_longtitude;
#endif
#if defined(OS2008) || defined(DEBUGTEMP)
    gfloat		sensor_data;
#endif
#ifdef USE_CONIC    
    ConIcConnection 	*connection;
#endif
    WeatherStationData	wsd;
}OMWeatherApp;
/*******************************************************************************/
extern	OMWeatherApp	*app;
/*******************************************************************************/
#ifdef OS2008 
G_BEGIN_DECLS

#define TYPE_OMWEATHER (omweather_get_type())
#define OMWEATHER(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
			TYPE_OMWEATHER, OMWeather))
#define OMWEATHER_CLASS(klass) \
		(G_TYPE_CHECK_CLASS_CAST((klass), TYPE_OMWEATHER, OMWeather))
#define IS_OMWEATHERT(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
			TYPE_OMWEATHER))
#define IS_OMWEATHERT_CLASS(klass) \
		(G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_OMWEATHER))
#define IS_OMWEATHER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), \
			TYPE_OMWEATHER, OMWeather))
#define OMWEATHER_GET_PRIVATE(obj) \
		(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_OMWEATHER, \
				OMWeather))

typedef struct _OMWeatherPrivate OMWeatherPrivate;

/* Instance struct */
typedef struct {
	HildonDesktopHomeItem parent;
	GtkWidget			   *layout;
	GnomeVFSMonitorHandle  *fileMon;
	guint                  updateTimeout;
	gboolean			   queueRefresh;
	XRectangle			   clip;
	OMWeatherPrivate *priv;
} OMWeather;

/* Class struct */
typedef struct {
	HildonDesktopHomeItemClass parent_class;
} OMWeatherClass;


GType personal_launcher_get_type(void);


G_END_DECLS
#endif
#endif
