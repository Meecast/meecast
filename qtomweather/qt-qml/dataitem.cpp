#include "dataitem.h"

DataItem::DataItem()
{
}

DataItem::DataItem(const Core::Data* data):QObject(),Core::Data(data){
    _id = -1;
}

QHash<int, QByteArray> DataItem::roleNames() const
{
    QHash<int,QByteArray> names;
    names[IdRole] = "id";
    names[NameRole] = "name";
    names[Temp_hiRole] = "temp_high";
    names[Temp_loRole] = "temp_low";
    names[IconRole] = "pict";
    names[Wind_directionRole] = "wind_direction";
    names[Wind_speedRole] = "wind_speed";
    names[HumidityRole] = "humidity";
    return names;
}

QVariant DataItem::data(int role)
{
    switch (role){
    case NameRole:
        return "item";
    case IdRole:
        return getId();
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
    default:
        return QVariant();
    }
}

QVariant DataItem::getData(QString name)
{
    if (name == "temp_high")
        return temperature_high();
    if (name == "temp_low")
        return temperature_low();
    if (name == "pict")
        return icon();
    if (name == "wind_direction")
        return wind_direction();
    if (name == "wind_speed")
        return wind_speed();
    if (name == "humidity")
        return humidity();
    return QVariant();

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

void
DataItem::setId(int id)
{
    _id = id;
}

int
DataItem::getId()
{
    return _id;
}
