#ifndef QMLLAYOUTITEM_H
#define QMLLAYOUTITEM_H


#include <QGraphicsLinearLayout>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#include "dataqml.h"
#include "configqml.h"
#include "abstractconfig.h"

class QmlLayoutItem
{
    QDeclarativeEngine *_engine;
    QDeclarativeComponent *_declarative_component;
    QGraphicsLayoutItem* _obj;
public:
    QmlLayoutItem(ConfigQml *configqml, DataQml *dataqml);
    virtual ~QmlLayoutItem();
    QDeclarativeEngine& engine() const;
    QGraphicsLayoutItem& obj() const;
};

#endif // QMLLAYOUTITEM_H
