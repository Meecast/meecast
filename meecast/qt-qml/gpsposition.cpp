#include "gpsposition.h"

#define UPDATE_PERIOD 20
#define UPDATE_DISTANCE 20.0

GpsPosition::GpsPosition(QObject *parent) :
    QObject(parent)
{
    _location = QGeoPositionInfoSource::createDefaultSource(this);
    _isUpdated = false;
    _timer = NULL;
    _latitude = 0;
    _longitude = 0;
    if (_location){
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
    double latitude, longitude;
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
        startTimer();
    }
}
void GpsPosition::timeout()
{
    double distance = 0;
    qDebug() << "\ntimeout\n\n";
    QGeoCoordinate coord = _location->lastKnownPosition(true).coordinate();
    qDebug() << "SAVED POSITION lat = " << _latitude << " lon = " << _longitude;
    qDebug() << "LAST POSITION lat = " << coord.latitude() << " lon = " << coord.longitude();
    if (coord.isValid()){
        distance = Core::DatabaseSqlite::calculate_distance(_latitude, _longitude,
                                                            coord.latitude(), coord.longitude());
        qDebug() << "distancd = " << distance;
        /* check distance between the last and found coordinates */
        if (distance > UPDATE_DISTANCE){
            /* distance more then UPDATE_DISTANCE km */
            emit findCoord(coord.latitude(), coord.longitude());
        }
    }
}
void GpsPosition::setLastCoordinates(double latitude, double longitude)
{
    _latitude = latitude;
    _longitude = longitude;
    qDebug() << "SET LAST COORDINATE lat = " << latitude << " lon = " << longitude;
}
void GpsPosition::startGps()
{
    if (_location){
        connect(_location, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        _location->startUpdates();
    }
}
void GpsPosition::startTimer()
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    _timer->start(UPDATE_PERIOD * 1000 * 60); /* UPDATE_PERIOD min */
}
