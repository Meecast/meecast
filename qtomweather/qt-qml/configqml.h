#ifndef CONFIGQML_H
#define CONFIGQML_H

#include "core.h"
#include <QObject>

class ConfigQml : public QObject, public Core::Config

{
    Q_OBJECT
    Q_PROPERTY(QString iconset READ iconset NOTIFY iconsetChanged)
    Q_PROPERTY(QString iconspath READ iconspath NOTIFY iconspathChanged)
    Q_PROPERTY(QString temperatureunit READ temperatureunit NOTIFY temperatureunitChanged)
    public:
    ConfigQml();
    QString iconset();
    QString iconspath();
    QString temperatureunit();
    void refreshconfig();
    virtual ~ConfigQml(){};
    signals:
    void iconsetChanged();
    void iconspathChanged();
    void temperatureunitChanged();
};

#endif // CONFIGQML_H
