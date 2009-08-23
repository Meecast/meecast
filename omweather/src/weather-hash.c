/* vim: set sw=4 ts=4 et: */
/* This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
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
 *
*/
#include <glib.h>
#include "weather-hash.h"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
GHashTable *hash_table_create(void) {
    GHashTable *hash;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    hash = g_hash_table_new(g_str_hash, g_str_equal);
/*
 * WARNING!
 * Do not insert new lines to this file, use file hash.data
 * To add new reserved word from data (xml) file you can use
 * script get_reserved_word.pl like this:
 * cat BOXX0014.xml USCA0001.xml > |./get_reserved_word.pl
*/
#include "hash.data"
    return hash;
}

/*******************************************************************************/
gpointer hash_table_find(gpointer key, gboolean search_short_name) {
    gpointer orig_key, search_text, value, result;
    gchar buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (search_short_name) {
        buffer[0] = 0;
        snprintf(buffer, sizeof(buffer) - 1, "%s_short", (gchar *) key);
        search_text = buffer;
    } else
        search_text = key;
    if (g_hash_table_lookup_extended(app->hash,
                                     search_text, &orig_key, &value))
        result = value;
    else
        result = key;
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return result;
}

/*******************************************************************************/
