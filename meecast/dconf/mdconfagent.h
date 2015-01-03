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

#ifndef MDCONAGENT_H
#define MDCONAGENT_H

#include <QVariant>
#include <QStringList>
#include <QObject>

#include <mlite5/mlite-global.h>

/*!

  \brief MDConfAgent is a simple C++ wrapper for dcconf.

  Creating a MDConfAgent instance gives you access to a multiple dconf
  keys.  You can get and set its value, and connect to its
  valueChanged(key) signal to be notified about changes.

  The value of a dconf key is returned to you as a QVariant, and you
  pass in a QVariant when setting the value.  MDConfAgent converts
  between a QVariant and dconf values as needed, and according to the
  following rules:

  - A QVariant of type QVariant::Invalid denotes an unset dconf key.

  - QVariant::Int, QVariant::Double, QVariant::Bool are converted to
    and from the obvious equivalents.

  - QVariant::String is converted to/from a dconf string and always
    uses the UTF-8 encoding.  No other encoding is supported.

  - QVariant::StringList is converted to a list of UTF-8 strings.

  - QVariant::List (which denotes a QList<QVariant>) is converted
    to/from a dconf list.  All elements of such a list must have the
    same type, and that type must be one of QVariant::Int,
    QVariant::Double, QVariant::Bool, or QVariant::String.  (A list of
    strings is returned as a QVariant::StringList, however, when you
    get it back.)

  - Any other QVariant or dconf value is essentially ignored.

  \warning MDConfAgent is as thread-safe as dconf.

*/

class MLITESHARED_EXPORT MDConfAgent : public QObject
{
    Q_OBJECT

public:
    /*! Initializes a MDConfAgent to access the dconf key denoted by
        \a key.  Key names should follow the normal dconf conventions
        like "/myapp/settings/first".

        \param key    The name of the key.
        \param parent Parent object
    */
    explicit MDConfAgent(const QString &prefix = QString(), QObject *parent = 0);

    /*! Finalizes a MDConfAgent.
     */
    virtual ~MDConfAgent();

    /*! Returns the current value of this item, as a QVariant.
     */
    QVariant value(const QString &key) const;

    /*! Returns the current value of this item, as a QVariant.  If
     *  there is no value for this item, return \a def instead.
     */
    QVariant value(const QString &key, const QVariant &def) const;

    /*! Set the value of this item to \a val.  If \a val can not be
        represented in dconf or dconf refuses to accept it for other
        reasons, the current value is not changed and nothing happens.

        When the new value is different from the old value, the
        changedValue() signal is emitted on this MDConfAgent as part
        of calling set(), but other MDConfAgent:s for the same key do
        only receive a notification once the main loop runs.

        \param val  The new value.
    */
    void setValue(const QString &key, const QVariant &val);

    /*! Unset this item.  This is equivalent to

        \code
        item.set(QVariant(QVariant::Invalid));
        \endcode
     */
    void unsetValue(const QString &key);

    void watchKey(const QString &key, const QVariant &def = QVariant());

    void unwatchKey(const QString &key);

    /*! Return a list of the directories below this item.  The
        returned strings are absolute key names like
        "/myapp/settings".

        A directory is a key that has children.  The same key might
        also have a value, but that is confusing and best avoided.
    */
    QStringList listDirs(const QString &key) const;

    /*! Return a map of the items inside this directory.  The
        returned map are key names like "settings" and key values.

        A directory is a key that has children.  The same key might
        also have a value, but that is confusing and best avoided.
    */

    QVariantMap listItems(const QString &key) const;

    /*! Request dconf to sync value(s) which are not yet synced to the cache.
        Sometimes values may not be synced because eventually this just hints
        dconf to start sync.

        Returns true if there's no errors and false on error.
    */
    bool sync();

Q_SIGNALS:
    /*! Emitted when the value of this item has changed.
     */
    void valueChanged(const QString &key);

private:
    friend struct MDConfAgentPrivate;
    struct MDConfAgentPrivate *priv;

    void update_value(const QString &key);
};

#endif // MDCONAGENT_H
