#ifndef DATAQML_H
#define DATAQML_H
#include "core.h"
#include <QObject>

class DataQml : public QObject, public Core::Data

{
    Q_OBJECT
    Q_PROPERTY(QString temperature_low READ temperature_low NOTIFY temperature_lowChanged)
    Q_PROPERTY(QString temperature_high READ temperature_high NOTIFY temperature_highChanged)
    Q_PROPERTY(int icon READ icon NOTIFY iconChanged)
    public:
    DataQml();
    QString temperature_high();
    QString temperature_low();
    int icon();
    void refreshview();
    virtual ~DataQml(){};
    signals:
    void temperature_highChanged();
    void temperature_lowChanged();
    void iconChanged();
};

#endif // DATAQML_H
