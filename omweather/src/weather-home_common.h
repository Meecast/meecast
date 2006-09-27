#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include <libgnomevfs/gnome-vfs.h>
#define Max_count_web_button 10


typedef struct
{
  int icon;
  gchar title[26]; /* Title */
  gchar hmid[5]; /* Humidity */
  int wind_speed; /* Wind Speed */
  gchar wind_title[11]; /* Wind Title */
}part_of_day;


typedef struct
{
  part_of_day day;
  part_of_day night;
  gchar date[10];     /* Date */     
  gchar dayshname[3]; /* Short name of day */
  gchar dayfuname[20];/* Full name of day */
  gchar hi_temp[4];   /* High temperature of day */
  gchar low_temp[4];  /* Low temperature  of day */
  gchar location[50]; /* Location */
}weather_day;

typedef struct
{
	gint    error;
//	weather_channel *channel_list;
	xmlDoc	    *doc;
	xmlNode     *weather_com_root;
} weather_com_parser;


weather_day weather_days[Max_count_web_button];

