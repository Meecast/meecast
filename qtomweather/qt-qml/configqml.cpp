#include "configqml.h"

//ConfigQml::ConfigQml():QObject(),Core::Config("config.xml", "../core/data/config.xsd"){}
ConfigQml::ConfigQml(const std::string& filename, const std::string& schema_filename):QObject(),Core::Config(filename, schema_filename)
{

}

ConfigQml::ConfigQml():QObject(),Core::Config(){}

QString
ConfigQml::iconset(){
    QString c;
    c = ConfigQml::Config::iconSet().c_str();
    return c;
}

QString
ConfigQml::iconspath(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("/icons");
    return c;
}

QString
ConfigQml::iconsbutton(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("/buttons_icons");
    return c;
}

QString
ConfigQml::temperatureunit(){
    QString c;
    c = ConfigQml::Config::TemperatureUnit().c_str();
    return c;
}
QColor
ConfigQml::fontcolor(){
    QColor c;
    c.setNamedColor(ConfigQml::Config::FontColor().c_str());
    return c;
}
QString
ConfigQml::stationname()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->name().c_str();
    else
        return "Unknown";
}
QString
ConfigQml::filename()
{
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->fileName().c_str();
    else
        return QString();
}
void ConfigQml::changestation()
{
    if (this->current_station_id() + 1 < this->stationsList().size()){
       this->current_station_id(this->current_station_id() + 1);

   }else {
       this->current_station_id(0);
   }

}
void
ConfigQml::refreshconfig(){
    emit ConfigQml::iconsetChanged();
    emit ConfigQml::iconspathChanged();
    emit ConfigQml::temperatureunitChanged();
    emit ConfigQml::fontcolorChanged();
    emit ConfigQml::stationnameChanged();
}

void
ConfigQml::updatestations()
{
    int i;
    int success = 0;
    Core::Station* station;

    for (i=0; i < this->stationsList().size();i++){
        station = this->stationsList().at(i);
        if (station->updateData(true)){
            success ++;

        }
    }
}
