#ifndef MEECASTDATA_H
#define MEECASTDATA_H

#include <QObject>
#include <QQmlParserStatus>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QXmlStreamReader>
#include <QDir>
#include <QFileSystemWatcher>
#include <iostream>

class Meecastdata : public QObject, public QQmlParserStatus{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:


    Meecastdata(QObject *parent = 0);
    ~Meecastdata();
    void classBegin();
    void componentComplete();

    Q_PROPERTY(QString nameString READ nameString NOTIFY nameStringChanged)
    Q_PROPERTY(QVariantMap forecastdata READ forecastdata NOTIFY forecastdataChanged)
    QString nameString() const;
    QVariantMap forecastdata() const;

public Q_SLOTS:
    void currentfileChanged(QString path);
    void SetCurrentData(const QString &station, const QString &temperature, const QString &temperature_high, const QString &temperature_low, const QString &icon, const QString &description, 
                        const uint until_valid_time, bool current, bool lockscreen_param, bool standbyscreen_param, const QString &last_update);
signals:
    void refreshWidget();
    void nameStringChanged();
    void forecastdataChanged();

private:

    void getWeatherdata();
    QTimer  *_filemonitoring; /* For check exsisting of file */
    QTimer  *_lazyrenderingtimer; /* Timer lazy rendering */
    QVariantMap _weatherdata;
    QFileSystemWatcher *_watcher;
    void parsePeriod(QXmlStreamReader& xml, int itemnumber);

private slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &propertiesChanged, const QStringList &propertiesInvalidated);
    void updatefilemonitoring();
    void refreshview();
};

#endif // MEECASTDATA_H

