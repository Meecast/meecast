
#define APPLET_X_SIZE 200
#define APPLET_Y_SIZE 40
#define FONT_MAIN_SIZE_LARGE 14
#define FONT_MAIN_SIZE_SMALL 9
 GdkColor DEFAULT_COLOR_MARK = {0, 0x0000, 0x0000, 0xc000};



#include "weather-home_common.h"

void hello_world_dialog_show (void);

#include <libosso.h>
//#include <pthread.h>
#include <gconf/gconf-client.h>


#define HTTP_RESPONSE_OK "200"



void weather_buttons_init(void);
weather_com_parser *weather_com_parser_new( const gchar * weather_com );
int parse_weather_com_xml(void);
void update_weather(GtkWidget *widget,
		    GdkEvent *event, 
                    gpointer user_data);
void weather_window_preference (GtkWidget *widget,GdkEvent *event,gpointer user_data);

void weather_com_destroy_parser( weather_com_parser *weather_com );


gchar  path_large_icon[]="/usr/share/weather/icons/Crystal/large_icon/";

/* Default values */
GdkColor DEFAULT_FONT_COLOR = {0, 0x0d00, 0x2a00, 0xc000};

/* CONFIGURATION INFORMATION. */
gchar *_weather_dir_name = NULL;
gchar *_weather_country_name = NULL;
gchar *_weather_state_name = NULL;
gchar *_weather_station_name = NULL;
gchar *_weather_station_id = NULL;
gchar *_weather_icon_size = NULL;
GdkColor _weather_font_color = {0,0,0,0};
