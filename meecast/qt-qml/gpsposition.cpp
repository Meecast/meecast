#include "gpsposition.h"

GpsPosition::GpsPosition(QObject *parent) :
    QObject(parent)
{
    _location = QGeoPositionInfoSource::createDefaultSource(this);
    _isUpdated = false;
    _timer = NULL;
    if (_location){
        connect(_location, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        _location->startUpdates();
        _isUpdated = true;

    }else {
        qDebug() << "default gps source not exist";

    }
}

GpsPosition::~GpsPosition()
{
    if (_isUpdated)
        _location->stopUpdates();
    /*if (_timer->isActive())
        _timer->stop();
        */
    if (_timer)
        delete _timer;
}

void GpsPosition::positionUpdated(QGeoPositionInfo info)
{
    //qDebug() << "gps info " << info;
    QGeoCoordinate coord = info.coordinate();
    if (coord.isValid()){
        longitude = coord.longitude();
        latitude = coord.latitude();
        qDebug() << "lon = " << longitude << ", lat = " << latitude;
        if (_isUpdated){
            _location->stopUpdates();
            _isUpdated = false;
        }
        emit findCoord(latitude, longitude);

        /* set timer */
        _timer = new QTimer(this);
        connect(_timer, SIGNAL(timeout()), this, SLOT(timeout()));
        _timer->start(1 * 1000 * 60); /* 1 min */
    }
}
void GpsPosition::timeout()
{
    qDebug() << "\ntimeout\n";
    QGeoCoordinate coord = _location->lastKnownPosition(true).coordinate();
    qDebug() << "LAST POSITION lat = " << coord.latitude() << " lon = " << coord.longitude();
    if (coord.isValid()){
        /* TODO: check distance between the last and found coordinates */
        emit findCoord(coord.latitude(), coord.longitude());
    }
}
