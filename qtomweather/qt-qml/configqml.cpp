#include "configqml.h"

//ConfigQml::ConfigQml():QObject(),Core::Config("config.xml", "../core/data/config.xsd"){}
ConfigQml::ConfigQml(const std::string& filename, const std::string& schema_filename):QObject(),Core::Config(filename, schema_filename){}

//ConfigQml::ConfigQml():QObject(),Core::Config(){}

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
void
ConfigQml::refreshconfig(){
    emit ConfigQml::iconsetChanged();
    emit ConfigQml::iconspathChanged();
    emit ConfigQml::temperatureunitChanged();
    emit ConfigQml::fontcolorChanged();
}

