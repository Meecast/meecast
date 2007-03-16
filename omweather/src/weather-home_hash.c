#include "weather-home_hash.h"
#include <glib.h>

static GHashTable *hash;

void hash_table_create(){
    hash = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(hash, "AM Clouds / PM Sun", _("AM Clouds / PM Sun"));
    g_hash_table_insert(hash, "AM Rain / Snow Showers", _("AM Rain / Snow Showers"));
    g_hash_table_insert(hash, "AM Showers", _("AM Showers"));
    g_hash_table_insert(hash, "AM Snow Showers", _("AM Snow Showers"));
    g_hash_table_insert(hash, "CALM", _("CALM"));
    g_hash_table_insert(hash, "Clear", _("Clear"));
    g_hash_table_insert(hash, "Clear / Wind", _("Clear / Wind"));
    g_hash_table_insert(hash, "Clouds Early / Clearing Late", _("Clouds Early / Clearing Late"));
    g_hash_table_insert(hash, "Cloudy", _("Cloudy"));
    g_hash_table_insert(hash, "Drizzle", _("Drizzle"));
    g_hash_table_insert(hash, "E", _("E"));
    g_hash_table_insert(hash, "ENE", _("ENE"));
    g_hash_table_insert(hash, "ESE", _("ESE"));
    g_hash_table_insert(hash, "failing", _("falling"));
    g_hash_table_insert(hash, "Fair", _("Fair"));
    g_hash_table_insert(hash, "Few Showers", _("Few Showers"));
    g_hash_table_insert(hash, "Few Snow", _("Few Snow"));
    g_hash_table_insert(hash, "Few Snow Showers", _("Few Snow Showers"));
    g_hash_table_insert(hash, "Fog", _("Fog"));
    g_hash_table_insert(hash, "Friday", _("Friday"));
    g_hash_table_insert(hash, "Full", _("Full"));
    g_hash_table_insert(hash, "Haze", _("Haze"));
    g_hash_table_insert(hash, "High", _("High"));
    g_hash_table_insert(hash, "Isolated T-Storms", _("Isolated T-Storms"));
    g_hash_table_insert(hash, "Light Drizzle", _("Light Drizzle"));
    g_hash_table_insert(hash, "Light Rain", _("Light Rain"));
    g_hash_table_insert(hash, "Light Snow", _("Light Snow"));
    g_hash_table_insert(hash, "Low", _("Low"));
    g_hash_table_insert(hash, "Mist", _("Mist"));
    g_hash_table_insert(hash, "Monday", _("Monday"));
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
    g_hash_table_insert(hash, "PM Showers", _("PM Showers"));
    g_hash_table_insert(hash, "PM Snow Showers", _("PM Snow Showers"));
    g_hash_table_insert(hash, "Rain", _("Rain"));
    g_hash_table_insert(hash, "Rain / Snow", _("Rain / Snow"));
    g_hash_table_insert(hash, "Rain / Snow Showers Late", _("Rain / Snow Showers Late"));
    g_hash_table_insert(hash, "Rain / Snow Showers", _("Rain / Snow Showers"));
    g_hash_table_insert(hash, "rising", _("rising"));
    g_hash_table_insert(hash, "Saturday", _("Saturday"));
    g_hash_table_insert(hash, "Scattered Showers", _("Scattered Showers"));
    g_hash_table_insert(hash, "Scattered Snow", _("Scattered Snow"));
    g_hash_table_insert(hash, "Scattered Snow Showers", _("Scattered Snow Showers"));
    g_hash_table_insert(hash, "Scattered T-Storms", _("Scattered T-Storms"));
    g_hash_table_insert(hash, "SE", _("SE"));
    g_hash_table_insert(hash, "Showers Late", _("Showers Late"));
    g_hash_table_insert(hash, "Showers", _("Showers"));
    g_hash_table_insert(hash, "Smoke", _("Smoke"));
    g_hash_table_insert(hash, "Snow Showers Late", _("Snow Showers Late"));
    g_hash_table_insert(hash, "Snow Shower", _("Snow Shower"));
    g_hash_table_insert(hash, "Snow", _("Snow"));
    g_hash_table_insert(hash, "Snow to Rain", _("Snow to Rain"));
    g_hash_table_insert(hash, "S", _("S"));
    g_hash_table_insert(hash, "SSE", _("SSE"));
    g_hash_table_insert(hash, "SSW", _("SSW"));
    g_hash_table_insert(hash, "steady", _("steady"));
    g_hash_table_insert(hash, "Storm", _("Storm"));
    g_hash_table_insert(hash, "Sunday", _("Sunday"));
    g_hash_table_insert(hash, "Sunny", _("Sunny"));
    g_hash_table_insert(hash, "SW", _("SW"));
    g_hash_table_insert(hash, "Thursday", _("Thursday"));
    g_hash_table_insert(hash, "Tuesday", _("Tuesday"));
    g_hash_table_insert(hash, "Unlimited", _("Unlimited"));
    g_hash_table_insert(hash, "Waning Gibbous", _("Waning Gibbous"));
    g_hash_table_insert(hash, "Wednesday", _("Wednesday"));
    g_hash_table_insert(hash, "Wind", _("Wind"));
    g_hash_table_insert(hash, "WNW", _("WNW"));
    g_hash_table_insert(hash, "WSW", _("WSW"));
    g_hash_table_insert(hash, "W", _("W"));
    g_hash_table_insert(hash, "Heavy Snow", _("Heavy Snow"));
    g_hash_table_insert(hash, "Snow to Wintry Mix / Wind", _("Snow to Wintry Mix / Wind"));
    g_hash_table_insert(hash, "Rain / Snow Showers / Wind", _("Rain / Snow Showers / Wind"));
}

gpointer hash_table_find(gpointer key){
    gpointer	orig_key,
		value,
		result;
    
    if(g_hash_table_lookup_extended(hash,
				    key,
				    &orig_key,
				    &value))
	result = value;
    else
	result = key;
    return result;
}
