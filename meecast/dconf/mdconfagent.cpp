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
extern "C" {
#include <dconf/dconf.h>
};

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVariant>
#include <QDebug>

#include "mdconfagent.h"

#include "mdconf_p.h"

struct MDConfAgentPrivate {
    MDConfAgentPrivate() :
        client(dconf_client_new()),
	handler(0)
    {}

    QString prefix;
    QVariantMap values;
    DConfClient *client;
    gulong handler;
    static void changed(DConfClient *, gchar *prefix, GStrv, gchar *, gpointer data);
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

void MDConfAgentPrivate::changed(DConfClient *, gchar *prefix, GStrv, gchar *, gpointer data)
{
    MDConfAgent *item = (MDConfAgent *)data;
    QString key = QString::fromUtf8(prefix);
    item->update_value(key);
}

QVariant MDConfAgent::value(const QString &key) const
{
    if (priv->values.contains(key)) {
        return priv->values[key];
    }
    else {
        return MDConf::read(priv->client, convertKey(priv->prefix + key));
    }
}

QVariant MDConfAgent::value(const QString &key, const QVariant &def) const
{
    QVariant val = value(key);
    if (val.isNull())
        return def;
    else
        return val;
}

void MDConfAgent::setValue(const QString &key, const QVariant &val)
{
    if (priv->values.contains(key)) {
        priv->values[key] = val;
    }
    MDConf::write(priv->client, convertKey(priv->prefix + key), val);
}

void MDConfAgent::unsetValue(const QString &key)
{
    if (priv->values.contains(key)) {
        priv->values[key] = QVariant();
    }
    MDConf::clear(priv->client, convertKey(priv->prefix + key));
}

void MDConfAgent::watchKey(const QString &key, const QVariant &def)
{
    if (!priv->values.contains(key)) {
        if (!key.endsWith("/")) {
            priv->values[key] = MDConf::read(priv->client, convertKey(priv->prefix + key));
            if (priv->values[key].isNull() && !def.isNull()) {
                priv->values[key] = def;
            }
            Q_EMIT valueChanged(key);
        }
        MDConf::watch(priv->client, convertKey(priv->prefix + key));
    }
}

void MDConfAgent::unwatchKey(const QString &key)
{
    if (priv->values.contains(key)) {
        priv->values.remove(key);
        MDConf::unwatch(priv->client, convertKey(priv->prefix + key));
    }
}

QStringList MDConfAgent::listDirs(const QString &key) const
{
    QStringList children;
    gint length = 0;
    QByteArray k = convertKey(priv->prefix + key);
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

QVariantMap MDConfAgent::listItems(const QString &key) const
{

    QVariantMap children;
    gint length = 0;
    QByteArray k = convertKey(priv->prefix + key);
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
            children[child] = val;

            //qDebug() << "have item:" << child;

            g_variant_unref(v);
        }
        g_free ((gpointer)item);
      }
    }

    g_strfreev(items);

    return children;
}

bool MDConfAgent::sync()
{
    MDConf::sync(priv->client);
    return true;
}

void MDConfAgent::update_value(const QString &key)
{
    QString privKey = key;
    privKey = privKey.remove(0, priv->prefix.length());
    priv->values[privKey] = MDConf::read(priv->client, convertKey(key));
    Q_EMIT valueChanged(privKey);
}

MDConfAgent::MDConfAgent(const QString &prefix, QObject *parent)
    : QObject(parent)
{
    priv = new MDConfAgentPrivate;
    priv->prefix = prefix;
    priv->handler =
      g_signal_connect(priv->client, "changed", G_CALLBACK(MDConfAgentPrivate::changed), this);
}

MDConfAgent::~MDConfAgent()
{
    g_signal_handler_disconnect(priv->client, priv->handler);
    foreach (const QString &key, priv->values.keys()) {
        QByteArray k = convertKey(priv->prefix + key);
        dconf_client_unwatch_fast(priv->client, k.data());
    }
    g_object_unref(priv->client);
    delete priv;
}
