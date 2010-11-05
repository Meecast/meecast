#include "dataqml.h"
DataQml::DataQml():QObject(),Core::Data(){

}

QString
DataQml::temperature(){
    QString c;
    return c.number((DataQml::Data::temperature().value()  ),'f',0);
}

