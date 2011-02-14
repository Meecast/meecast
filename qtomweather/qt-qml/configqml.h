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
    Q_PROPERTY(QString iconsbutton READ iconsbutton NOTIFY iconsbuttonChanged)
    Q_PROPERTY(QString temperatureunit READ temperatureunit NOTIFY temperatureunitChanged)
    Q_PROPERTY(QColor fontcolor READ fontcolor NOTIFY fontcolorChanged)
    Q_PROPERTY(QString stationname READ stationname NOTIFY stationnameChanged)
    public:
    ConfigQml();
    ConfigQml(const std::string& filename, const std::string& schema_filename = "/usr/" + schemaPath + "config.xsd");
    QString iconset();
    QString iconspath();
    QString iconsbutton();
    QString temperatureunit();
    QColor fontcolor();
    QString stationname();
    void refreshconfig();
    virtual ~ConfigQml(){};
    signals:
    void iconsetChanged();
    void iconspathChanged();
    void iconsbuttonChanged();
    void temperatureunitChanged();
    void fontcolorChanged();
    void stationnameChanged();
};

#endif // CONFIGQML_H
