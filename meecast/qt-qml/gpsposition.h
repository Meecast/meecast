#ifndef GPSPOSITION_H
#define GPSPOSITION_H

#include <QObject>
#include <QDebug>
#include <QGeoPositionInfo>
//#include <qmobilityglobal.h>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>

QTM_USE_NAMESPACE

class GpsPosition : public QObject
{
    Q_OBJECT
public:
    explicit GpsPosition(QObject *parent = 0);
    ~GpsPosition();
    double latitude;
    double longitude;
private:
    QGeoPositionInfoSource * _location;
signals:
    void findCoord(double latitude, double longitude);

private slots:
    void positionUpdated(QGeoPositionInfo info);

};

#endif // GPSPOSITION_H
