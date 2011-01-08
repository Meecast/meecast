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
        IdRole,
        Temp_hiRole,
        Temp_loRole,
        IconRole,
        Wind_directionRole,
        Wind_speedRole
    };
public:
    DataItem();
    DataItem(const Core::Data *data);
    virtual ~DataItem(){};
    QVariant data(int role);
    QVariant getData(QString name);
    QHash<int, QByteArray> roleNames() const;
    inline QString temperature_high();
    inline QString temperature_low();
    inline QString wind_direction();
    inline QString wind_speed();
    inline QString icon();
    void setId(int id);
    int getId();

private:
    int _id;
};

#endif // DATAITEM_H

