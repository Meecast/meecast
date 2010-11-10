#include "qmllayoutitem.h"

QmlLayoutItem::QmlLayoutItem(ConfigQml *configqml, DataQml *dataqml)
{
    _engine = new QDeclarativeEngine;
    _declarative_component = new QDeclarativeComponent(_engine, QUrl(":weatherlayoutitem.qml"));
    _engine->rootContext()->setContextProperty("Forecast", dataqml);
    _engine->rootContext()->setContextProperty("Config", configqml);
    _obj = qobject_cast<QGraphicsLayoutItem*>(_declarative_component->create());
}

////////////////////////////////////////////////////////////////////////////////
QDeclarativeEngine&
QmlLayoutItem::engine() const{
        return *_engine;
    }
////////////////////////////////////////////////////////////////////////////////
QGraphicsLayoutItem&
QmlLayoutItem::obj() const{
        return *_obj;
    }
////////////////////////////////////////////////////////////////////////////////
QmlLayoutItem::~QmlLayoutItem(){
   delete _engine;
   delete _declarative_component;
}

