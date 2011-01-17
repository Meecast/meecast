#ifndef DATAITEM_H
#define DATAITEM_H
#include "core.h"
#include <QObject>
#include <QVariant>
#include "configqml.h"

class DataItem : public QObject, public Core::Data
{
    Q_OBJECT
    public:
    enum Roles {
        NameRole = Qt::UserRole+1,
        Temp_hiRole,
        Temp_loRole,
        IconRole,
        Wind_directionRole,
        Wind_speedRole,
        HumidityRole,
        DescriptionRole,
        CurrentRole
    };
public:
    DataItem();
    DataItem(const Core::Data *data);
    virtual ~DataItem(){};
    QVariant data(int role);
    QVariant getData(QByteArray name);
    QHash<int, QByteArray> roleNames() const;
    inline QString temperature_high();
    inline QString temperature_low();
    inline QString wind_direction();
    inline QString wind_speed();
    inline QString humidity();
    inline QString icon();
    inline bool current();
    inline QString description();

};

#endif // DATAITEM_H

