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

class Weather : public QObject, public QQmlParserStatus{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:


    Weather(QObject *parent = 0);
    ~Weather();
    void classBegin();
    void componentComplete();



public Q_SLOTS:
signals:

private:


private slots:
};

#endif // MEECASTDATA_H

