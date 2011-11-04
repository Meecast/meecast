#ifndef GPSPOSITION_H
#define GPSPOSITION_H

#include <QObject>
#include <QDebug>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include <QTimer>
#include "core.h"

QTM_USE_NAMESPACE

class GpsPosition : public QObject
{
    Q_OBJECT
public:
    explicit GpsPosition(QObject *parent = 0);
    ~GpsPosition();
    void setLastCoordinates(double latitude, double longitude);
private:
    QGeoPositionInfoSource * _location;
    bool _isUpdated;
    QTimer *_timer;
    double _latitude;
    double _longitude;
signals:
    void findCoord(double latitude, double longitude);

private slots:
    void positionUpdated(QGeoPositionInfo info);
    void timeout();
};

#endif // GPSPOSITION_H
