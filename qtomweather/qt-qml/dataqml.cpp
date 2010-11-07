#include "dataqml.h"
DataQml::DataQml():QObject(),Core::Data(){

}

QString
DataQml::temperature_high(){
    QString c;
    if (DataQml::Data::temperature_hi().value() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataQml::Data::temperature_hi().value()),'f',0);
}

QString
DataQml::temperature_low(){
    QString c;
    if (DataQml::Data::temperature_low().value() == INT_MAX){
        c = "N/A";
        return c;
    }
    return c.number((DataQml::Data::temperature_low().value()),'f',0);
}

int
DataQml::icon(){
    QString c;
    return DataQml::Data::Icon();
}

void
DataQml::refreshview(){
    emit DataQml::temperature_highChanged();
    emit DataQml::temperature_lowChanged();
    emit DataQml::iconChanged();
}
