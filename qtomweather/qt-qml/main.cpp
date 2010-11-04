#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>



#include "core.h"
#include "dataqml.h"

//////////////////////////////////////////////////////////////////////////////

DataQml *
create_and_fill_class_data_for_hours_forecast()
{
    DataQml *wdata = new DataQml;
    wdata->StartTime(time(NULL) - 3600);
    wdata->EndTime(time(NULL) + 3600);
    wdata->Data::temperature().value(23.0);
    wdata->Flike(18.0);
    wdata->WindSpeed(3.0);
    wdata->WindGust(4.0);
    wdata->WindDirection(std::string("NNW"));
    wdata->Icon(3);
    wdata->Text(std::string("Clear"));
    return wdata;
}

int main(int argc, char* argv[])
{
    QApplication::setGraphicsSystem("native");
    QApplication app(argc, argv);

    DataQml *wdata = create_and_fill_class_data_for_hours_forecast();
    //Set up a graphics scene with a QGraphicsWidget and Layout
    QGraphicsView view;
    QGraphicsScene scene;
    QGraphicsWidget *widget = new QGraphicsWidget();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
    widget->setLayout(layout);
    scene.addItem(widget);
    view.setScene(&scene);


    //Add the QML snippet into the layout
//    QDeclarativeEngine engine;
    QDeclarativeEngine *engine = new QDeclarativeEngine;




    QDeclarativeComponent c(engine, QUrl(":layoutitem.qml"));

    QGraphicsLayoutItem* obj = qobject_cast<QGraphicsLayoutItem*>(c.create());
    layout->addItem(obj);

    widget->setGeometry(QRectF(0,0, 400,400));

    engine->rootContext()->setContextProperty("MyObject", wdata);

    view.show();


    return app.exec();
}
