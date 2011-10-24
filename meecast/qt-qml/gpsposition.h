#ifndef GPSPOSITION_H
#define GPSPOSITION_H

#include <QObject>
#include <QDebug>
#include <QtMobility/qmobilityglobal.h>
#include <QtLocation/QGeoPositionInfo>
#include <QtLocation/QGeoPositionInfoSource>
#include <QtLocation/QGeoCoordinate>

class GpsPosition : public QObject
{
    Q_OBJECT
public:
    explicit GpsPosition(QObject *parent = 0);

signals:

private slots:
    void positionUpdated(QGeoPositionInfo info);

};

#endif // GPSPOSITION_H
