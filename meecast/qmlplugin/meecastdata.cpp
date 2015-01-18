#include "meecastdata.h"

#include <QTimer>

Meecastdata::Meecastdata(QObject *parent):
    QObject(parent),
    m_active(false),
    m_service(){

}

Meecastdata::~Meecastdata(){
}

void Meecastdata::classBegin(){

}

void Meecastdata::componentComplete(){
    if (m_active && m_service != "") {
        initialize();
    }
}

void Meecastdata::initialize(){
    getAllProperties();
    emitProperties();
    getAllPropertiesPlayer();
    emitPropertiesPlayer();
}

void Meecastdata::setActive(bool newActive){
    if (m_active != newActive) {
        m_active = newActive;

        if (!m_active) {
            disconnectSignals();
        }
        else if (m_service != "") {
            connectSignals();
            initialize();
        }

        Q_EMIT activeChanged();
    }
}

QString Meecastdata::service() const {
    return m_service;
}

void Meecastdata::setService(const QString &newService) {
    if (m_service != newService) {
        m_service = newService;

        if (m_service != "") {
//            mprisIface.reset(new QDBusInterface(m_service, MPRIS2_PATH, MPRIS2_MEDIAPLAYER_IF, QDBusConnection::sessionBus(), this));
//            playerIface.reset(new QDBusInterface(m_service, MPRIS2_PATH, MPRIS2_MEDIAPLAYER_PLAYER_IF, QDBusConnection::sessionBus(), this));
//            propertiesIface.reset(new QDBusInterface(m_service, MPRIS2_PATH, FREEDESKTOP_PROPERTIES_IF, QDBusConnection::sessionBus(), this));
 //           peerIface.reset(new QDBusInterface(m_service, MPRIS2_PATH, FREEDESKTOP_PEER_IF, QDBusConnection::sessionBus(), this));

            if (m_active) {
                connectSignals();
                initialize();
            }
        }
        else if (m_active) {
            disconnectSignals();
        }

        Q_EMIT serviceChanged();
    }
}

void Meecastdata::emitProperties(){
}

void Meecastdata::emitPropertiesPlayer() {
}

void Meecastdata::connectSignals() {
}

void Meecastdata::disconnectSignals() {
}

void Meecastdata::setDBusProperty(const QString &interface, const QString &name, const QVariant &value) {
    //if (propertiesIface)
    //    propertiesIface->call("Set", interface, name, value);
}

QVariantMap Meecastdata::getAllDBusProperties(const QString &interface){
    /*
    if (propertiesIface) {
        QDBusReply<QVariantMap> reply = propertiesIface->call("GetAll", interface);
        if (reply.isValid()) {
            return reply.value();
        }
        else {
            return QVariantMap();
        }
    }
    return QVariantMap();
    */
}

void Meecastdata::onPropertiesChanged(const QString &interface, const QVariantMap &propertiesChanged, const QStringList &propertiesInvalidated) {
}

QString Meecastdata::nameString() const
{
    return QString("MeeCast");
}
