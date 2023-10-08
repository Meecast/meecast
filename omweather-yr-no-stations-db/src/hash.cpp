/* vim: set sw=4 ts=4 et: */
/* This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2011-2013 Vlad Vasilyeu
 * 	for the code
 *        
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
*/
#ifdef GLIB
    #include <glib.h>
#endif
#include <stdio.h>
#include "hash.h"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif

/*******************************************************************************/

xmlHashTablePtr 
hash_icons_yrno_table_create(){
    xmlHashTablePtr hash;
    hash = xmlHashCreate(49 + 21);
#include "hash_icons.data"
    return hash;
}

xmlHashTablePtr
hash_description_yrno_table_create(){
    xmlHashTablePtr hash;
    hash = xmlHashCreate(49 + 21);
#include "hash_description.data"
    return hash;
}

#ifdef QT
QHash<QString, QString> *
hash_icons_yrno_table_create(void) {
    QHash<QString, QString> *hash = new QHash <QString, QString>;
#include "hash_icons.data"
    return hash;
}

QHash<QString, QString> *
hash_description_yrno_table_create(void) {
    QHash<QString, QString> *hash = new QHash <QString, QString>;
#include "hash_description.data"
    return hash;
}

QString
hash_yrno_icon_table_find(QHash<QString, QString> *hash_for_icons, char* key) {
    fprintf(stderr, "qqqqqqqqqqqqqqqa\n");
    QString result;
    if (hash_for_icons->contains(QString(key)))
        return hash_for_icons->value(QString(key));
    else{
        fprintf(stderr,"Unknown strings %s\n", key);
                return QString("49");
   }
}

/*******************************************************************************/
QString
hash_yrno_description_table_find(QHash<QString, QString> *hash_for_description, char* key) {
    QString result;
    if (hash_for_description->contains(QString(key)))
        return hash_for_description->value(QString(key));
    else{
        fprintf(stderr,"Unknown strings %s\n", key);
        return QString(key);
   }
}

#endif
/*******************************************************************************/

#ifdef GLIB
GHashTable *hash_description_yrno_table_create(void) {
    GHashTable *hash = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    hash = g_hash_table_new(g_str_hash, g_str_equal);
#include "hash_description.data"
    return hash;
}
/*******************************************************************************/
GHashTable *hash_icons_yrno_table_create(void) {
    GHashTable *hash = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    hash = g_hash_table_new(g_str_hash, g_str_equal);
#include "hash_icons.data"
    return hash;
}

/*******************************************************************************/
gpointer 
hash_yrno_table_find(GHashTable *hash, gpointer key, gboolean search_short_name) {
    gpointer orig_key, search_text, 
             value = NULL, 
             result = NULL;
    gchar buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (search_short_name) {
        buffer[0] = 0;
        snprintf(buffer, sizeof(buffer) - 1, "%s_short", (gchar *)key);
        search_text = buffer;
    } else
        search_text = key;
    if (g_hash_table_lookup_extended(hash,
                                     search_text, &orig_key, &value))
        result = value;
    else{
        result = key;
        fprintf(stderr,"Not found %s\n",(gchar *)key);
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return result;
}
#endif
/*******************************************************************************/
