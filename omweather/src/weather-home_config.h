#include <gtk/gtk.h>
#include <libgnomevfs/gnome-vfs.h>
#include <gconf/gconf-client.h>


#define GCONF_KEY_PREFIX "/apps/maemo/omweather"
#define GCONF_KEY_WEATHER_DIR_NAME      GCONF_KEY_PREFIX"/weather-dir"
#define GCONF_KEY_WEATHER_COUNTRY_NAME  GCONF_KEY_PREFIX"/country-name"
#define GCONF_KEY_WEATHER_STATE_NAME    GCONF_KEY_PREFIX"/state-name"
#define GCONF_KEY_WEATHER_STATION_NAME  GCONF_KEY_PREFIX"/station-name"
#define GCONF_KEY_WEATHER_STATION_ID    GCONF_KEY_PREFIX"/station-id"
#define GCONF_KEY_WEATHER_ICON_SIZE     GCONF_KEY_PREFIX"/icon-size"
#define GCONF_KEY_WEATHER_FONT_COLOR    GCONF_KEY_PREFIX"/font-color"

extern gchar *_weather_dir_name;
extern gchar *_weather_country_name;
extern gchar *_weather_state_name;
extern gchar *_weather_station_name;
extern gchar *_weather_station_id;
extern gchar *_weather_icon_size;
extern GdkColor _weather_font_color;
extern GdkColor DEFAULT_FONT_COLOR;


void config_save();
void config_init();
gboolean config_set_weather_dir_name(gchar *new_weather_dir_name);


