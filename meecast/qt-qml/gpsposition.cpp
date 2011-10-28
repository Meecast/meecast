#include "gpsposition.h"

GpsPosition::GpsPosition(QObject *parent) :
    QObject(parent)
{
    _location = QGeoPositionInfoSource::createDefaultSource(this);
    if (_location){
        connect(_location, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        _location->startUpdates();
        //_location->startUpdates(1000);
    }else {
        qDebug() << "default gps source not exist";

    }
}

GpsPosition::~GpsPosition()
{
    _location->stopUpdates();
}

void GpsPosition::positionUpdated(QGeoPositionInfo info)
{
    //qDebug() << "gps info " << info;
    QGeoCoordinate coord = info.coordinate();
    if (coord.isValid()){
        longitude = coord.longitude();
        latitude = coord.latitude();
        qDebug() << "lon = " << longitude << ", lat = " << latitude;
        _location->stopUpdates();
    }
}
