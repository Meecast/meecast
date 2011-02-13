#ifndef QMLLAYOUTITEM_H
#define QMLLAYOUTITEM_H


#include <QGraphicsLinearLayout>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#include "dataqml.h"
#include "configqml.h"

#ifdef LOCALDEBUG
    #define LAYOUTQML ":weatherlayoutitem.qml"
#else
    #define LAYOUTQML ":/usr/share/omweather/qml/weatherlayoutitem.qml"
#endif


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
