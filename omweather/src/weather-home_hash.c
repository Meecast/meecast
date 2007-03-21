#include <glib.h>
#include "weather-home_hash.h"


GHashTable* hash_table_create(void){
    GHashTable *hash;
    
    hash = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(hash, "AM Clouds / PM Sun", _("AM Clouds / PM Sun"));
    g_hash_table_insert(hash, "AM Light Snow", _("AM Light Snow"));
    g_hash_table_insert(hash, "AM Light Wintry Mix", _("AM Light Wintry Mix"));
    g_hash_table_insert(hash, "AM Rain / Snow Showers", _("AM Rain / Snow Showers"));
    g_hash_table_insert(hash, "AM Showers", _("AM Showers"));
    g_hash_table_insert(hash, "AM Snow Showers", _("AM Snow Showers"));
    g_hash_table_insert(hash, "CALM", _("CALM"));
    g_hash_table_insert(hash, "Clear", _("Clear"));
    g_hash_table_insert(hash, "Clear / Wind", _("Clear / Wind"));
    g_hash_table_insert(hash, "Clouds Early / Clearing Late", _("Clouds Early / Clearing Late"));
    g_hash_table_insert(hash, "Cloudy and Windy", _("Cloudy and Windy"));
    g_hash_table_insert(hash, "Cloudy", _("Cloudy"));
    g_hash_table_insert(hash, "Drizzle", _("Drizzle"));
    g_hash_table_insert(hash, "E", _("E"));
    g_hash_table_insert(hash, "ENE", _("ENE"));
    g_hash_table_insert(hash, "ESE", _("ESE"));
    g_hash_table_insert(hash, "Fair", _("Fair"));
    g_hash_table_insert(hash, "falling", _("falling"));
    g_hash_table_insert(hash, "Few Showers", _("Few Showers"));
    g_hash_table_insert(hash, "Few Showers / Wind", _("Few Showers / Wind"));
    g_hash_table_insert(hash, "Few Snow", _("Few Snow"));
    g_hash_table_insert(hash, "Few Snow Showers", _("Few Snow Showers"));
    g_hash_table_insert(hash, "Fog", _("Fog"));
    g_hash_table_insert(hash, "Friday", _("Friday"));
    g_hash_table_insert(hash, "Fr", _("Fr"));
    g_hash_table_insert(hash, "Full", _("Full"));
    g_hash_table_insert(hash, "Haze", _("Haze"));
    g_hash_table_insert(hash, "Heavy Rain / Wind", _("Heavy Rain / Wind"));
    g_hash_table_insert(hash, "Heavy Snow", _("Heavy Snow"));
    g_hash_table_insert(hash, "High", _("High"));
    g_hash_table_insert(hash, "Isolated T-Storms", _("Isolated T-Storms"));
    g_hash_table_insert(hash, "Light Drizzle", _("Light Drizzle"));
    g_hash_table_insert(hash, "Light Rain and Windy", _("Light Rain and Windy"));
    g_hash_table_insert(hash, "Light Rain", _("Light Rain"));
    g_hash_table_insert(hash, "Light Snow", _("Light Snow"));
    g_hash_table_insert(hash, "Light Snow Shower", _("Light Snow Shower"));
    g_hash_table_insert(hash, "Low", _("Low"));
    g_hash_table_insert(hash, "Mist", _("Mist"));
    g_hash_table_insert(hash, "Monday", _("Monday"));
    g_hash_table_insert(hash, "Mo", _("Mo"));
    g_hash_table_insert(hash, "Mostly Clear", _("Mostly Clear"));
    g_hash_table_insert(hash, "Mostly Cloudy", _("Mostly Cloudy"));
    g_hash_table_insert(hash, "Mostly Sunny", _("Mostly Sunny"));
    g_hash_table_insert(hash, "N/A", _("N/A"));
    g_hash_table_insert(hash, "NE", _("NE"));
    g_hash_table_insert(hash, "N", _("N"));
    g_hash_table_insert(hash, "NNE", _("NNE"));
    g_hash_table_insert(hash, "NNW", _("NNW"));
    g_hash_table_insert(hash, "NW", _("NW"));
    g_hash_table_insert(hash, "Overcast", _("Overcast"));
    g_hash_table_insert(hash, "Partly Cloudy", _("Partly Cloudy"));
    g_hash_table_insert(hash, "Partly Cloudy / Wind", _("Partly Cloudy / Wind"));
    g_hash_table_insert(hash, "PM Light Rain", _("PM Light Rain"));
    g_hash_table_insert(hash, "PM Showers", _("PM Showers"));
    g_hash_table_insert(hash, "PM Snow Showers", _("PM Snow Showers"));
    g_hash_table_insert(hash, "Rain and Snow", _("Rain and Snow"));
    g_hash_table_insert(hash, "Rain Early", _("Rain Early"));
    g_hash_table_insert(hash, "Rain", _("Rain"));
    g_hash_table_insert(hash, "Rain Shower and Windy", _("Rain Shower and Windy"));
    g_hash_table_insert(hash, "Rain Shower", _("Rain Shower"));
    g_hash_table_insert(hash, "Rain / Snow", _("Rain / Snow"));
    g_hash_table_insert(hash, "Rain / Snow Showers Late", _("Rain / Snow Showers Late"));
    g_hash_table_insert(hash, "Rain / Snow Showers", _("Rain / Snow Showers"));
    g_hash_table_insert(hash, "Rain / Snow Showers / Wind", _("Rain / Snow Showers / Wind"));
    g_hash_table_insert(hash, "Rain to Wintry Mix", _("Rain to Wintry Mix"));
    g_hash_table_insert(hash, "Rain to Wintry Mix / Wind", _("Rain to Wintry Mix / Wind"));
    g_hash_table_insert(hash, "Rain / Wind Early", _("Rain / Wind Early"));
    g_hash_table_insert(hash, "Rain / Wind", _("Rain / Wind"));
    g_hash_table_insert(hash, "rising", _("rising"));
    g_hash_table_insert(hash, "Saturday", _("Saturday"));
    g_hash_table_insert(hash, "Sa", _("Sa"));
    g_hash_table_insert(hash, "Scattered Showers", _("Scattered Showers"));
    g_hash_table_insert(hash, "Scattered Snow", _("Scattered Snow"));
    g_hash_table_insert(hash, "Scattered Snow Showers", _("Scattered Snow Showers"));
    g_hash_table_insert(hash, "Scattered T-Storms", _("Scattered T-Storms"));
    g_hash_table_insert(hash, "SE", _("SE"));
    g_hash_table_insert(hash, "Showers Early", _("Showers Early"));
    g_hash_table_insert(hash, "Showers in the Vicinity", _("Showers in the Vicinity"));
    g_hash_table_insert(hash, "Showers Late", _("Showers Late"));
    g_hash_table_insert(hash, "Showers", _("Showers"));
    g_hash_table_insert(hash, "Showers / Wind", _("Showers / Wind"));
    g_hash_table_insert(hash, "Smoke", _("Smoke"));
    g_hash_table_insert(hash, "Snow Showers Late", _("Snow Showers Late"));
    g_hash_table_insert(hash, "Snow Shower", _("Snow Shower"));
    g_hash_table_insert(hash, "Snow", _("Snow"));
    g_hash_table_insert(hash, "Snow to Rain", _("Snow to Rain"));
    g_hash_table_insert(hash, "Snow to Wintry Mix", _("Snow to Wintry Mix"));
    g_hash_table_insert(hash, "Snow to Wintry Mix / Wind", _("Snow to Wintry Mix / Wind"));
    g_hash_table_insert(hash, "S", _("S"));
    g_hash_table_insert(hash, "SSE", _("SSE"));
    g_hash_table_insert(hash, "SSW", _("SSW"));
    g_hash_table_insert(hash, "steady", _("steady"));
    g_hash_table_insert(hash, "Storm", _("Storm"));
    g_hash_table_insert(hash, "Sunday", _("Sunday"));
    g_hash_table_insert(hash, "Su", _("Su"));
    g_hash_table_insert(hash, "Sunny", _("Sunny"));
    g_hash_table_insert(hash, "Sunny / Wind", _("Sunny / Wind"));
    g_hash_table_insert(hash, "SW", _("SW"));
    g_hash_table_insert(hash, "Thursday", _("Thursday"));
    g_hash_table_insert(hash, "Th", _("Th"));
    g_hash_table_insert(hash, "Tuesday", _("Tuesday"));
    g_hash_table_insert(hash, "Tu", _("Tu"));
    g_hash_table_insert(hash, "Unlimited", _("Unlimited"));
    g_hash_table_insert(hash, "Waning Crescent", _("Waning Crescent"));
    g_hash_table_insert(hash, "Waning Gibbous", _("Waning Gibbous"));
    g_hash_table_insert(hash, "Waxing Crescent", _("Waxing Crescent"));
    g_hash_table_insert(hash, "Wednesday", _("Wednesday"));
    g_hash_table_insert(hash, "We", _("We"));
    g_hash_table_insert(hash, "Wind", _("Wind"));
    g_hash_table_insert(hash, "Wintry Mix to Snow", _("Wintry Mix to Snow"));
    g_hash_table_insert(hash, "Wintry Mix / Wind Early", _("Wintry Mix / Wind Early"));
    g_hash_table_insert(hash, "Wintry Mix / Wind", _("Wintry Mix / Wind"));
    g_hash_table_insert(hash, "Wintry Mix", _("Wintry Mix"));
    g_hash_table_insert(hash, "WNW", _("WNW"));
    g_hash_table_insert(hash, "WSW", _("WSW"));
    g_hash_table_insert(hash, "W", _("W"));
    g_hash_table_insert(hash, "Jan", _("Jan"));
    g_hash_table_insert(hash, "Feb", _("Feb"));
    g_hash_table_insert(hash, "Mar", _("Mar"));
    g_hash_table_insert(hash, "Apr", _("Apr"));
    g_hash_table_insert(hash, "May", _("May"));
    g_hash_table_insert(hash, "Jun", _("Jun"));
    g_hash_table_insert(hash, "Jul", _("Jul"));
    g_hash_table_insert(hash, "Aug", _("Aug"));
    g_hash_table_insert(hash, "Sep", _("Sep"));
    g_hash_table_insert(hash, "Oct", _("Oct"));
    g_hash_table_insert(hash, "Nov", _("Nov"));
    g_hash_table_insert(hash, "Dec", _("Dec"));
    return  hash;
}

gpointer hash_table_find(gpointer key){
    gpointer	orig_key,
		value,
		result;
    
    if(g_hash_table_lookup_extended(app->hash,
				    key,
				    &orig_key,
				    &value))
	result = value;
    else
	result = key;
    return result;
}
