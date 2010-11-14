#ifndef CONFIGQML_H
#define CONFIGQML_H

#include "core.h"
#include <QObject>
#include <QColor>

class ConfigQml : public QObject, public Core::Config

{
    Q_OBJECT
    Q_PROPERTY(QString iconset READ iconset NOTIFY iconsetChanged)
    Q_PROPERTY(QString iconspath READ iconspath NOTIFY iconspathChanged)
    Q_PROPERTY(QString temperatureunit READ temperatureunit NOTIFY temperatureunitChanged)
    Q_PROPERTY(QColor fontcolor READ fontcolor NOTIFY fontcolorChanged)
    public:
    ConfigQml();
    QString iconset();
    QString iconspath();
    QString temperatureunit();
    QColor fontcolor();
    void refreshconfig();
    virtual ~ConfigQml(){};
    signals:
    void iconsetChanged();
    void iconspathChanged();
    void temperatureunitChanged();
    void fontcolorChanged();
};

#endif // CONFIGQML_H
