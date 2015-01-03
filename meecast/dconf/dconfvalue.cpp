/*
 * Copyright (C) 2012 Jolla Ltd. <robin.burchell@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include <QDirIterator>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QUrl>

#include "dconfvalue.h"

DConfValue::DConfValue(QObject *parent)
    : QObject(parent)
    , mItem(0)
{
}

QString DConfValue::key() const
{
    if (!mItem)
        return QString();

    return mItem->key();
}

void DConfValue::setKey(const QString &key)
{
    QVariant oldValue;

    // don't emit valueChanged unless absolutely necessary
    if (mItem)
        oldValue = mItem->value(mDefaultValue);

    delete mItem;
    mItem = new MDConfItem(key, this);
    connect(mItem, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));

    emit keyChanged();

    // we deleted the old item, so we must emit ourselves in this case
    if (oldValue != mItem->value(mDefaultValue))
        emit valueChanged();
}

QVariant DConfValue::value() const
{
    if (!mItem)
        return QVariant();

    return mItem->value(mDefaultValue);
}

void DConfValue::setValue(const QVariant &value)
{
    if (mItem)
        mItem->set(value); // TODO: setValue once we change MDConfItem API
    // MDConfItem will emit valueChanged for us
}

QVariant DConfValue::defaultValue() const
{
    return mDefaultValue;
}

void DConfValue::setDefaultValue(const QVariant &value)
{
    QVariant oldValue = this->value();
    mDefaultValue = value;
    emit defaultValueChanged();

    // if changing the default changed the value, emit valueChanged
    if (value != oldValue)
        emit valueChanged();
}

void DConfValue::sync()
{
    if (mItem) {
        mItem->sync();
    }
}

QStringList DConfValue::listDirs(const QString &key)
{
    MDConfItem *test = new MDConfItem("/apps/harbour-mitakuuluu2", 0);
    QStringList values = test->listDirs(key);
    delete test;
    return values;
}

QVariantList DConfValue::listValues(const QString &key)
{
    MDConfItem *test = new MDConfItem("/apps/harbour-mitakuuluu2", 0);
    QVariantList values = test->listItems(key);
    delete test;
    return values;
}

void DConfValue::unset()
{
    if (mItem) {
        mItem->unset();
    }
}
