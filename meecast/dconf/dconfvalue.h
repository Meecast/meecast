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

#ifndef DCONFVALUE_H
#define DCONFVALUE_H

#include <QAbstractListModel>
#include <QFileInfo>
#include <QVector>
#include <QStringList>

#include "mdconfitem.h"

class DConfValue : public QObject
{
    Q_OBJECT
public:
    DConfValue(QObject *parent = 0);

    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
    QString key() const;
    void setKey(const QString &key);

    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    QVariant value() const;
    void setValue(const QVariant &value);

    Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)
    QVariant defaultValue() const;
    void setDefaultValue(const QVariant &defaultValue);

public slots:
    void sync();
    QStringList listDirs(const QString &key);
    QVariantList listValues(const QString &key);
    void unset();

signals:
    void keyChanged();
    void valueChanged();
    void defaultValueChanged();

private:
    MDConfItem *mItem;
    QVariant mDefaultValue;
};

#endif // DCONFVALUE_H
