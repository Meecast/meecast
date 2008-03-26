/* This file is part of Other Maemo Weather(omweather)
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
