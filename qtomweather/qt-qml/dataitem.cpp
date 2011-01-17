#include "dataitem.h"

DataItem::DataItem()
{
}

DataItem::DataItem(const Core::Data* data):QObject(),Core::Data(data){

}

QHash<int, QByteArray> DataItem::roleNames() const
{
    QHash<int,QByteArray> names;
    names[NameRole] = "name";
    names[Temp_hiRole] = "temp_high";
    names[Temp_loRole] = "temp_low";
    names[IconRole] = "pict";
    names[Wind_directionRole] = "wind_direction";
    names[Wind_speedRole] = "wind_speed";
    names[HumidityRole] = "humidity";
    names[DescriptionRole] = "description";
    names[CurrentRole] = "current";
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
    case Wind_directionRole:
        return wind_direction();
    case Wind_speedRole:
        return wind_speed();
    case HumidityRole:
        return humidity();
    case CurrentRole:
        return current();
    case DescriptionRole:
        return description();
    default:
        return QVariant();
    }
}

QVariant DataItem::getData(QByteArray name)
{
    return data(roleNames().key(name));

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
DataItem::humidity() {
    QString c;
    if (DataItem::Data::Humidity() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataItem::Data::Humidity()),'i',0);
}
QString
DataItem::icon() {
    QString c;
    return c.number((DataItem::Data::Icon()), 'i', 0) + ".png";
}

QString
DataItem::wind_direction() {
    QString c;
    c = DataItem::Data::WindDirection().c_str();
    return c;
}

QString
DataItem::wind_speed() {
    QString c;
    if (DataItem::Data::WindSpeed() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataItem::Data::WindSpeed()), 'f', 0);
}
bool
DataItem::current()
{
    return DataItem::Data::Current();
}
QString
DataItem::description()
{
    return DataItem::Data::Text().c_str();
}

