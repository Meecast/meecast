#include <glib.h>
#include "weather-hash.h"
/*******************************************************************************/
GHashTable* hash_table_create(void){
    GHashTable *hash;
    
    hash = g_hash_table_new(g_str_hash, g_str_equal);
/*
 * WARNING!
 * Do not insert new lines to this file, use file hash.data
 * To add new reserved word from data (xml) file you can use
 * script get_reserved_word.pl like this:
 * cat BOXX0014.xml USCA0001.xml >./get_reserved_word.pl
*/
#include "hash.data"
    return  hash;
}
/*******************************************************************************/
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
/*******************************************************************************/
