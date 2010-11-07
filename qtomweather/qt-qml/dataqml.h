#ifndef DATAQML_H
#define DATAQML_H
#include "core.h"
#include <QObject>

class DataQml : public QObject, public Core::Data

{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)
    public:
    DataQml();
    QString temperature();
    void refreshview();
    virtual ~DataQml(){};
    signals:
    void temperatureChanged();

};

#endif // DATAQML_H
