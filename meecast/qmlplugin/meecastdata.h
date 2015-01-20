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

    Q_INVOKABLE void initialize();

    bool active() const;
    void setActive(bool newActive);

    void setService(const QString &newService);

    Q_PROPERTY(QString nameString READ nameString NOTIFY nameStringChanged)

    QString nameString() const;

    Q_INVOKABLE void activated();

public Q_SLOTS:
    void currentfileChanged(QString path);
signals:
    void activeChanged();
    void refreshWidget();


    void nameStringChanged();

private:
    void setDBusProperty(const QString &interface, const QString &name, const QVariant &value);
    QVariantMap getAllDBusProperties(const QString &interface);
    QVariantMap convertMetadata(const QVariant &dbusArgumentMetadata);

    void getAllProperties();
    void getWeatherdata();
    void getAllPropertiesPlayer();

    void emitProperties();
    void emitPropertiesPlayer();

    void connectSignals();
    void disconnectSignals();


    bool m_active;

    QTimer  *_filemonitoring; /* For check exsisting of file */
    QVariantMap _weatherdata;
    QFileSystemWatcher *_watcher;

private slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &propertiesChanged, const QStringList &propertiesInvalidated);
    void updatefilemonitoring();
};

#endif // MEECASTDATA_H

