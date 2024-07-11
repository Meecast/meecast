/***************************************************************************
** This file was derived from the MDesktopEntry implementation in the
** libmeegotouch library.
**
** Original Copyright:
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Copyright on new work:
** Copyright 2011 Intel Corp.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

// This has to be the first include otherwise gdbusintrospection.h causes an error.
#include <glib.h>
#if GLIB_MINOR_VERSION > 66
#include <dconf/dconf.h>
#else
extern "C" {
    #include <dconf/dconf.h>
};
#endif

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVariant>
#include <QDebug>

#include "mdconfitem.h"

#include "mdconf_p.h"

struct MDConfItemPrivate {
    MDConfItemPrivate() :
        client(dconf_client_new()),
	handler(0)
    {}

    QString key;
    QVariant value;
    DConfClient *client;
    gulong handler;
    static void notify_trampoline(DConfClient *, gchar *, GStrv, gchar *, gpointer);
};

static QByteArray convertKey(const QString &key)
{
    if (key.startsWith('/'))
        return key.toUtf8();
    else {
        QString replaced = key;
        replaced.replace('.', '/');
        qWarning() << "Using dot-separated key names with MGConfItem is deprecated.";
        qWarning() << "Please use" << '/' + replaced << "instead of" << key;
        return '/' + replaced.toUtf8();
    }
}

static QString convertKey(const char *key)
{
    return QString::fromUtf8(key);
}

void MDConfItemPrivate::notify_trampoline(DConfClient *, gchar *, GStrv, gchar *, gpointer data)
{
    MDConfItem *item = (MDConfItem *)data;
    item->update_value(true);
}

void MDConfItem::update_value(bool emit_signal)
{
    QVariant new_value = MDConf::read(priv->client, convertKey(priv->key));

    if (new_value != priv->value || new_value.userType() != priv->value.userType()) {
        priv->value = new_value;
        if (emit_signal)
            emit valueChanged();
    }
}

QString MDConfItem::key() const
{
    return priv->key;
}

QVariant MDConfItem::value() const
{
    return priv->value;
}

QVariant MDConfItem::value(const QVariant &def) const
{
    if (priv->value.isNull())
        return def;
    else
        return priv->value;
}

void MDConfItem::set(const QVariant &val)
{

    MDConf::write(priv->client, convertKey(priv->key), val);
}

void MDConfItem::unset()
{
    MDConf::clear(priv->client, convertKey(priv->key));
}

QStringList MDConfItem::listDirs(const QString &key) const
{
    QStringList children;
    gint length = 0;
    QByteArray k = convertKey(key);
    if (!k.endsWith("/")) {
        k = k.left(k.lastIndexOf("/") + 1);
    }
    //qDebug() << "MDConfItem::listDirs" << k;

    gchar **dirs = dconf_client_list(priv->client, k.data(), &length);
    GError *error = NULL;

    for (gint x = 0; x < length; x++) {
      const gchar *dir = g_strdup_printf ("%s%s", k.data(), dirs[x]);
      if (dconf_is_dir(dir, &error)) {
        // We have to mimic how gconf was behaving.
        // so we need to chop off trailing slashes.
        // dconf will also barf if it gets a "path" with 2 slashes.
        QString d = convertKey(dir);
        g_free ((gpointer)dir);
        //qDebug() << "have dir:" << d;
        if (d.endsWith("/")) {
          d.chop(1);
        }
        children.append(d);
      }
    }

    g_strfreev(dirs);

    return children;
}

QVariantList MDConfItem::listItems(const QString &key) const
{

    QVariantList children;
    gint length = 0;
    QByteArray k = convertKey(key);
    if (!k.endsWith("/")) {
        k = k.left(k.lastIndexOf("/") + 1);
    }
    //qDebug() << "MDConfItem::listItems" << k;

    gchar **items = dconf_client_list(priv->client, k.data(), &length);
    GError *error = NULL;

    for (gint x = 0; x < length; x++) {
      const gchar *item = g_strdup_printf ("%s%s", k.data(), items[x]);
      if (dconf_is_key(item, &error)) {
        // We have to mimic how gconf was behaving.
        // so we need to chop off trailing slashes.
        // dconf will also barf if it gets a "path" with 2 slashes.
        QString child = convertKey(item);
        QVariant val;
        GVariant *v = dconf_client_read(priv->client, item);
        if (!v) {
            qWarning() << "MGConfItem Failed to read" << child;
        }
        else {
            val = MDConf::convertValue(v);

            QVariantMap value;
            value["key"] = child;
            value["value"] = val;
            children.append(value);
        }

        if (v)
            g_variant_unref(v);
        g_free ((gpointer)item);
      }
    }

    g_strfreev(items);

    return children;
}

bool MDConfItem::sync()
{
    MDConf::sync(priv->client);
    return true;
}

MDConfItem::MDConfItem(const QString &key, QObject *parent)
    : QObject(parent)
{
    priv = new MDConfItemPrivate;
    priv->key = key;
    priv->handler =
      g_signal_connect(priv->client, "changed", G_CALLBACK(MDConfItemPrivate::notify_trampoline), this);

    MDConf::watch(priv->client, convertKey(priv->key));
    update_value(false);
}

MDConfItem::~MDConfItem()
{
    g_signal_handler_disconnect(priv->client, priv->handler);
    MDConf::unwatch(priv->client, convertKey(priv->key));
    g_object_unref(priv->client);
    delete priv;
}
