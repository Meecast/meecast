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
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)


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
signals:
    void activeChanged();
    void refreshWidget();


    void nameStringChanged();
    void forecastdataChanged();

private:

    void getWeatherdata();
    QTimer  *_filemonitoring; /* For check exsisting of file */
    QVariantMap _weatherdata;
    QFileSystemWatcher *_watcher;
    void parsePeriod(QXmlStreamReader& xml, int itemnumber);

private slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &propertiesChanged, const QStringList &propertiesInvalidated);
    void updatefilemonitoring();
};

#endif // MEECASTDATA_H

