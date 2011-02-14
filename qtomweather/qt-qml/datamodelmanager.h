#ifndef DATAMODELMANAGER_H
#define DATAMODELMANAGER_H

#include <QObject>
#include "configqml.h"
#include "dataitem.h"
#include "datamodel.h"
#include "core.h"

class DataModelManager: public QObject
{
    Q_OBJECT
    ConfigQml *config;
    DataModel *model;
public:
    DataModelManager(ConfigQml *c);
};

#endif // DATAMODELMANAGER_H
