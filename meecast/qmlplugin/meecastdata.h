#ifndef MEECASTDATA_H
#define MEECASTDATA_H

#include <QObject>
#include <QQmlParserStatus>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>


class Meecastdata : public QObject, public QQmlParserStatus{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

    Q_PROPERTY(QString service READ service WRITE setService NOTIFY serviceChanged)

    Meecastdata(QObject *parent = 0);
    ~Meecastdata();

    void classBegin();
    void componentComplete();

    Q_INVOKABLE void initialize();

    bool active() const;
    void setActive(bool newActive);

    QString service() const;
    void setService(const QString &newService);

    Q_PROPERTY(QString nameString READ nameString NOTIFY nameStringChanged)

    QString nameString() const;
signals:
    void activeChanged();

    void serviceChanged();

    void nameStringChanged();

private:
    void setDBusProperty(const QString &interface, const QString &name, const QVariant &value);
    QVariantMap getAllDBusProperties(const QString &interface);
    QVariantMap convertMetadata(const QVariant &dbusArgumentMetadata);

    void getAllProperties();
    void getAllPropertiesPlayer();

    void emitProperties();
    void emitPropertiesPlayer();

    void connectSignals();
    void disconnectSignals();


    bool m_active;

    QString m_service;

    QVariantMap m_properties;
    QVariantMap m_properties_player;

private slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &propertiesChanged, const QStringList &propertiesInvalidated);
};

#endif // MEECASTDATA_H

