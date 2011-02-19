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
    names[Wind_gustRole] = "wind_gust";
    names[HumidityRole] = "humidity";
    names[DescriptionRole] = "description";
    names[CurrentRole] = "current";
    names[DateRole] = "date";
    names[ShortDateRole] = "shortdate";
    names[FullDateRole] = "fulldate";
    names[StartRole] = "start";
    names[EndRole] = "end";
    names[PressureRole] = "pressure";
    names[FlikeRole] = "flike";
    names[PpcpRole] = "ppcp";
    names[TemperatureLabelRole] = "temperature_label";
    names[HumidityLabelRole] = "humidity_label";
    names[WindLabelRole] = "wind_label";
    names[WindSpeedLabelRole] = "wind_speed_label";
    names[PressureLabelRole] = "pressure_label";
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
    case Wind_gustRole:
        return wind_gust();
    case HumidityRole:
        return humidity();
    case CurrentRole:
        return current();
    case DescriptionRole:
        return description();
    case DateRole:
        return date();
    case ShortDateRole:
        return shortdate();
    case FullDateRole:
        return fulldate();
    case StartRole:
        return start();
    case EndRole:
        return end();
    case PressureRole:
        return pressure();
    case FlikeRole:
        return flike();
    case PpcpRole:
        return ppcp();
    case TemperatureLabelRole:
        return QString(QString::fromUtf8( _("Temperature:")));
    case HumidityLabelRole:
        return QString(QString::fromUtf8( _("Humidity:")));
    case WindLabelRole:
        return QString(QString::fromUtf8( _("Wind:")));
    case WindSpeedLabelRole:
        return QString(QString::fromUtf8( _("Wind Speed:")));
    case PressureLabelRole:
        return QString(QString::fromUtf8( _("Pressure:")));
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
DataItem::flike() {
    QString c;
    if (DataItem::Data::Flike().value() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataItem::Data::Flike().value()),'f',0);
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
    c = QString(QString::fromUtf8(_(DataItem::Data::WindDirection().c_str())));
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
QString
DataItem::wind_gust() {
    QString c;
    if (DataItem::Data::WindGust() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataItem::Data::WindGust()), 'f', 0);
}
bool
DataItem::current()
{
    return DataItem::Data::Current();
}
QString
DataItem::description()
{
    return QString(QString::fromUtf8(_(DataItem::Data::Text().c_str())));
}
QString
DataItem::date()
{
    QDateTime t;
    t.setTime_t(DataItem::Data::StartTime());
    return t.toString("dddd");
    //return t.toString(Qt::SystemLocaleLongDate);
}
QString
DataItem::shortdate()
{
    return QString::fromUtf8(DataItem::ShortDayName().c_str());

}
QString
DataItem::fulldate()
{
    return QString::fromUtf8((DataItem::FullDayName()+" "+DataItem::DayOfMonthName()+", "+DataItem::FullMonthName()).c_str());

}
QString
DataItem::start()
{
    QDateTime t;
    t.setTime_t(DataItem::Data::StartTime());
    //return t.toString("dddd");
    return t.toString(Qt::SystemLocaleLongDate);
}
QString
DataItem::end()
{
    QDateTime t;
    t.setTime_t(DataItem::Data::EndTime());
    //return t.toString("dddd");
    return t.toString(Qt::SystemLocaleLongDate);
}
QString
DataItem::pressure() {
    QString c;
    if (DataItem::Data::Pressure() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataItem::Data::Pressure()), 'i', 0);
}
QString
DataItem::ppcp() {
    QString c;
    if (DataItem::Data::Ppcp() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataItem::Data::Ppcp()), 'f', 0);
}
