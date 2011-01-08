#ifndef DATAITEM_H
#define DATAITEM_H
#include "core.h"
#include <QObject>
#include <QVariant>
#include "configqml.h"

class DataItem : public QObject, public Core::Data
{
    Q_OBJECT
    ConfigQml* _config;
    public:
    enum Roles {
        NameRole = Qt::UserRole+1,
        Temp_hiRole,
        Temp_loRole,
        IconRole
    };
public:
    DataItem();
    DataItem(const Core::Data *data);
    virtual ~DataItem(){};
    QVariant data(int role);
    QHash<int, QByteArray> roleNames() const;
    inline QString temperature_high();
    inline QString temperature_low();
    inline QString icon();
    void setConfig(ConfigQml * config);

};

#endif // DATAITEM_H

