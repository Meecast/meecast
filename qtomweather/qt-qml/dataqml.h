#ifndef DATAQML_H
#define DATAQML_H
#include "core.h"
#include <QObject>

class DataQml : public QObject, public Core::Data

{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature)
    public:
    DataQml();
    QString temperature();
    virtual ~DataQml(){};
};

#endif // DATAQML_H
