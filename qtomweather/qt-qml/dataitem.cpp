#include "dataitem.h"

DataItem::DataItem()
{
}

QHash<int, QByteArray> DataItem::roleNames() const
{
    QHash<int,QByteArray> names;
    names[NameRole] = "name";
    names[Temp_hiRole] = "temp_high";
    names[Temp_loRole] = "temp_low";
    names[IconRole] = "pict";
    return names;
}

QVariant DataItem::data(int role)
{
    switch (role){
    case NameRole:
        return "item";
    case Temp_hiRole:
        return temperature_high();
    case Temp_loRole:
        return temperature_low();
    case IconRole:
        return icon();
    default:
        return QVariant();
    }
}

QString
DataItem::temperature_high() {
    QString c;
    if (DataItem::Data::temperature_hi().value() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataItem::Data::temperature_hi().value()),'f',0);
}

QString
DataItem::temperature_low() {
    QString c;
    if (DataItem::Data::temperature_low().value() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataItem::Data::temperature_low().value()),'f',0);
}

QString
DataItem::icon() {
    QString c;
    return c.number((DataItem::Data::Icon()), 'i', 0) + ".png";
}
void
DataItem::setConfig(ConfigQml *config)
{
    _config = config;
}
