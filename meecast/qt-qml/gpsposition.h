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

signals:

private slots:
    void positionUpdated(QGeoPositionInfo info);

};

#endif // GPSPOSITION_H
