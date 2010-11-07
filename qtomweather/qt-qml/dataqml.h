#ifndef DATAQML_H
#define DATAQML_H
#include "core.h"
#include <QObject>

class DataQml : public QObject, public Core::Data

{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(int icon READ icon NOTIFY iconChanged)
    public:
    DataQml();
    QString temperature();
    int icon();
    void refreshview();
    virtual ~DataQml(){};
    signals:
    void temperatureChanged();
    void iconChanged();
};

#endif // DATAQML_H
