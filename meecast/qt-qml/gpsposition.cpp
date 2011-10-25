#include "gpsposition.h"

GpsPosition::GpsPosition(QObject *parent) :
    QObject(parent)
{
    QGeoPositionInfoSource * source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source){
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        source->startUpdates();
    }else {
        qDebug() << "error geo source";
    }
}

void GpsPosition::positionUpdated(QGeoPositionInfo info)
{
    qDebug() << "gps info " << info;
    QGeoCoordinate coord = info.coordinate();
    if (coord.isValid()){
        QString longitude;
        longitude.setNum(coord.longitude());
        QString latitude;
        latitude.setNum(coord.latitude());
        qDebug() << "lon = " << longitude << ", lat = " << latitude;
    }
}
