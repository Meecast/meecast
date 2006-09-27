#include "weather-home_common.h"

extern gchar *_weather_dir_name;

weather_com_parser *weather_parser_new_from_file(const gchar *filename);
int parse_weather_com_xml(void);
extern gchar *_weather_station_id;

