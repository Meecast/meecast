/***************************************************************************
** Copyright (C) 2014 Jolla Mobile <andrew.den.exter@jollamobile.com>
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "mdconf_p.h"

#include <QDebug>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QStringList>
#include <QVector>

template <typename T> static T tupleValue(GVariant *tuple, int index)
{
    GVariant *child = g_variant_get_child_value(tuple, index);
    QVariant value = MDConf::convertValue(child);
    g_variant_unref(child);

    return value.value<T>();
}

QVariant MDConf::convertValue(GVariant *value, int typeHint)
{
    if (!value)
        return QVariant();

    switch (g_variant_classify(value)) {
    case G_VARIANT_CLASS_BOOLEAN:
        return QVariant::fromValue(bool(g_variant_get_boolean(value)));
    case G_VARIANT_CLASS_BYTE:
        return QVariant::fromValue(char(g_variant_get_byte(value)));
    case G_VARIANT_CLASS_INT16:
        return QVariant::fromValue(g_variant_get_int16(value));
    case G_VARIANT_CLASS_UINT16:
        return QVariant::fromValue(g_variant_get_uint16(value));
    case G_VARIANT_CLASS_INT32:
        return QVariant::fromValue(g_variant_get_int32(value));
    case G_VARIANT_CLASS_UINT32:
        return QVariant::fromValue(g_variant_get_uint32(value));
    case G_VARIANT_CLASS_INT64:
        return QVariant::fromValue(g_variant_get_int64(value));
    case G_VARIANT_CLASS_UINT64:
        return QVariant::fromValue(g_variant_get_uint64(value));
    case G_VARIANT_CLASS_DOUBLE:
        return QVariant::fromValue(g_variant_get_double(value));
    case G_VARIANT_CLASS_STRING: {
        QVariant variant = QVariant::fromValue(QString::fromUtf8(g_variant_get_string(value, 0)));
        if (typeHint != QMetaType::UnknownType && typeHint != QMetaType::QString)
            variant.convert(typeHint);
        return variant;
    }
    case G_VARIANT_CLASS_VARIANT: {
        GVariant *gvariant = g_variant_get_variant(value);
        QVariant qvariant = convertValue(gvariant);
        g_variant_unref(gvariant);
        return qvariant;
    }
    case G_VARIANT_CLASS_ARRAY: {
        const GVariantType *type = g_variant_get_type(value);
        if (g_variant_type_equal(type, G_VARIANT_TYPE_BYTESTRING)) {
            return QVariant::fromValue(QByteArray(g_variant_get_bytestring(value)));
        } else if (g_variant_type_equal(type, G_VARIANT_TYPE_STRING_ARRAY)) {
            QStringList stringList;

            gsize length = 0;
            const gchar **strings = g_variant_get_strv(value, &length);
            for (gsize i = 0; i < length ; ++i)
                stringList.append(QString::fromUtf8(strings[i]));

            return stringList;
        } else if (g_variant_type_equal(type, G_VARIANT_TYPE_BYTESTRING_ARRAY)) {
            QList<QByteArray> stringList;

            gsize length = 0;
            const gchar **strings = g_variant_get_bytestring_array (value, &length);
            for (gsize i = 0; i < length ; ++i)
                stringList.append(strings[i]);

            return QVariant::fromValue(stringList);
        } else if (g_variant_type_equal(type, G_VARIANT_TYPE_VARDICT)) {
            QVariantMap variantMap;

            for (gsize i = 0, count = g_variant_n_children(value); i < count; ++i) {
                GVariant *child = g_variant_get_child_value(value, i);

                variantMap.insert(tupleValue<QString>(child, 0), tupleValue<QVariant>(child, 1));

                g_variant_unref(child);
            }

            return variantMap;
        } else {
            QVariantList variantList;

            for (gsize i = 0, count = g_variant_n_children(value); i < count; ++i) {
                GVariant *child = g_variant_get_child_value(value, i);
                variantList.append(convertValue(child));
                g_variant_unref(child);
            }

            return variantList;
        }
    }
    case G_VARIANT_CLASS_TUPLE:
        switch (typeHint) {
        case QMetaType::QPoint:
            if (g_variant_n_children(value) == 2)
                return QPoint(tupleValue<int>(value, 0), tupleValue<int>(value, 1));
            break;
        case QMetaType::QPointF:
            if (g_variant_n_children(value) == 2)
                return QPoint(tupleValue<qreal>(value, 0), tupleValue<qreal>(value, 1));
            break;
        case QMetaType::QSize:
            if (g_variant_n_children(value) == 2)
                return QSize(tupleValue<int>(value, 0), tupleValue<int>(value, 1));
            break;
        case QMetaType::QSizeF:
            if (g_variant_n_children(value) == 2)
                return QSizeF(tupleValue<qreal>(value, 0), tupleValue<qreal>(value, 1));
            break;
        case QMetaType::QRect:
            if (g_variant_n_children(value) == 4) {
                return QRect(
                            tupleValue<qreal>(value, 0),
                            tupleValue<qreal>(value, 1),
                            tupleValue<qreal>(value, 2),
                            tupleValue<qreal>(value, 3));
            }
            break;
        case QMetaType::QRectF:
            if (g_variant_n_children(value) == 4) {
                return QRectF(
                            tupleValue<qreal>(value, 0),
                            tupleValue<qreal>(value, 1),
                            tupleValue<qreal>(value, 2),
                            tupleValue<qreal>(value, 3));
            }
            break;
        default:
            break;
        }
        break;
    default: {
        const GVariantType *type = g_variant_get_type(value);
        if (g_variant_type_equal(type, G_VARIANT_TYPE_BYTESTRING)) {
            return QVariant::fromValue(QByteArray(g_variant_get_bytestring(value)));
        }
    }
    }

    return QVariant();
}

bool MDConf::convertValue(const QVariant &variant, GVariant **valp)
{
    switch (int(variant.type())) {
    case QMetaType::UnknownType:
        return true;
    case QMetaType::Bool:
        *valp = g_variant_new_boolean(variant.toBool());
        return true;
    case QMetaType::Char:
        *valp = g_variant_new_byte(variant.value<char>());
        return true;
    case QMetaType::Short:
        *valp = g_variant_new_int16(variant.value<qint16>());
        return true;
    case QMetaType::UShort:
        *valp = g_variant_new_uint16(variant.value<quint16>());
        return true;
    case QMetaType::Int:
        *valp = g_variant_new_int32(variant.value<qint32>());
        return true;
    case QMetaType::UInt:
        *valp = g_variant_new_uint32(variant.value<quint32>());
        return true;
    case QMetaType::LongLong:
        *valp = g_variant_new_int64(variant.value<qint64>());
        return true;
    case QMetaType::ULongLong:
        *valp = g_variant_new_uint64(variant.value<quint64>());
        return true;
    case QMetaType::Double:
    case QMetaType::Float:
        *valp = g_variant_new_double(variant.value<double>());
        return true;
    case QMetaType::QString:
        *valp = g_variant_new_string(variant.toString().toUtf8().constData());
        return true;
    case QMetaType::QByteArray:
        *valp = g_variant_new_bytestring(variant.toByteArray().constData());
        return true;
    case QMetaType::QStringList: {
        QList<QByteArray> utf8List;
        QVector<const char *> pointers;
        foreach (const QString &string, variant.toStringList()) {
            const QByteArray utf8 = string.toUtf8();
            utf8List.append(utf8);
            pointers.append(utf8.constData());
        }
        *valp = g_variant_new_strv(pointers.data(), pointers.count());
        return true;
    }
    case QMetaType::QVariantList: {
        QVector<GVariant *> variants;
        foreach (const QVariant &qvariant, variant.value<QVariantList>()) {
            GVariant *value = 0;
            if (convertValue(qvariant, &value))
                variants.append(g_variant_new_variant(value));
        }
        *valp = g_variant_new_array(G_VARIANT_TYPE_VARIANT, variants.data(), variants.count());
        return true;
    }
    case QMetaType::QVariantMap: {
        const QVariantMap map = variant.value<QVariantMap>();
        GVariantBuilder builder;
        g_variant_builder_init(&builder, G_VARIANT_TYPE_VARDICT);

        for (QVariantMap::const_iterator it = map.begin(); it != map.end(); ++it) {
            GVariant *value = 0;
            if (convertValue(it.value(), &value))
                g_variant_builder_add(&builder, "{sv}", it.key().toUtf8().constData(), value);
        }
        *valp = g_variant_builder_end(&builder);
        return true;
    }
    case QMetaType::QPoint: {
        const QPoint point = variant.toPoint();
        GVariant *variants[] = { g_variant_new_int32(point.x()), g_variant_new_int32(point.y()) };
        *valp = g_variant_new_tuple(variants, 2);
        return true;
    }
    case QMetaType::QPointF: {
        const QPointF point = variant.toPointF();
        GVariant *variants[] = { g_variant_new_double(point.x()), g_variant_new_double(point.y()) };
        *valp = g_variant_new_tuple(variants, 2);
        return true;
    }
    case QMetaType::QSize: {
        const QSize size = variant.toSize();
        GVariant *variants[] = {
            g_variant_new_int32(size.width()),
            g_variant_new_int32(size.height())
        };
        *valp = g_variant_new_tuple(variants, 2);
        return true;
    }
    case QMetaType::QSizeF: {
        const QSizeF size = variant.toSizeF();
        GVariant *variants[] = {
            g_variant_new_double(size.width()),
            g_variant_new_double(size.height())
        };
        *valp = g_variant_new_tuple(variants, 2);
        return true;
    }
    case QMetaType::QRect: {
        const QRect rect = variant.toRect();
        GVariant *variants[] = {
            g_variant_new_int32(rect.x()),
            g_variant_new_int32(rect.y()),
            g_variant_new_int32(rect.width()),
            g_variant_new_int32(rect.height())
        };
        *valp = g_variant_new_tuple(variants, 4);
        return true;
    }
    case QMetaType::QRectF: {
        const QRectF rect = variant.toRectF();
        GVariant *variants[] = {
            g_variant_new_double(rect.x()),
            g_variant_new_double(rect.y()),
            g_variant_new_double(rect.width()),
            g_variant_new_double(rect.height())
        };
        *valp = g_variant_new_tuple(variants, 4);
        return true;
    }
    default:
        if (variant.userType() == qMetaTypeId<QList<QByteArray> >()) {
            QVector<const char *> pointers;
            foreach (const QByteArray &utf8, variant.value<QList<QByteArray> >())
                pointers.append(utf8.constData());
            *valp = g_variant_new_bytestring_array(pointers.data(), pointers.count());
            return true;
        } else if (variant.canConvert(QMetaType::QString)) {
            *valp = g_variant_new_string(variant.toString().toUtf8().constData());
            return true;
        }
        return false;
    }
}

QVariant MDConf::read(DConfClient *client, const QByteArray &key, int typeHint)
{
    QVariant value;

    GVariant *gvariant = dconf_client_read(client, key.constData());
    if (gvariant) {
        value = convertValue(gvariant, typeHint);

        g_variant_unref(gvariant);
    }

    return value;
}

void MDConf::write(DConfClient *client, const QByteArray &key, const QVariant &value, bool synchronous)
{
    GError *error = 0;
    GVariant *gvariant = 0;
    if (convertValue(value, &gvariant)) {
        if (synchronous)
            dconf_client_write_sync(client, key, gvariant, 0, 0, &error);
        else
            dconf_client_write_fast(client, key, gvariant, &error);

        if (error) {
            qWarning() << "MDConf: Failed to write value for " << key << value;
            qWarning() << error->message;
            g_error_free(error);
        }
    } else {
        qWarning() << "MDConf: no conversion for" << key << value;
    }
}

void MDConf::clear(DConfClient *client, const QByteArray &key, bool synchronous)
{
    if (synchronous)
        dconf_client_write_sync(client, key.constData(), 0, 0, 0, 0);
    else
        dconf_client_write_fast(client, key.constData(), 0, 0);
}

void MDConf::watch(DConfClient *client, const QByteArray &key, bool synchronous)
{
    if (synchronous)
        dconf_client_watch_sync(client, key.constData());
    else
        dconf_client_watch_fast(client, key.constData());
}

void MDConf::unwatch(DConfClient *client, const QByteArray &key, bool synchronous)
{
    if (synchronous)
        dconf_client_unwatch_sync(client, key.constData());
    else
        dconf_client_unwatch_fast(client, key.constData());
}

void MDConf::sync(DConfClient *client)
{
    dconf_client_sync(client);
}

DConfClient *MDConf::client()
{
    return dconf_client_new();
}
