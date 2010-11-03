#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>

/* This example demonstrates using a LayoutItem to let QML snippets integrate
   nicely with existing QGraphicsView applications designed with GraphicsLayouts
*/
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    //Set up a graphics scene with a QGraphicsWidget and Layout
    QGraphicsView view;
    QGraphicsScene scene;
    QGraphicsWidget *widget = new QGraphicsWidget();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
    widget->setLayout(layout);
    scene.addItem(widget);
    view.setScene(&scene);


    //Add the QML snippet into the layout
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, QUrl(":layoutitem.qml"));
    QGraphicsLayoutItem* obj = qobject_cast<QGraphicsLayoutItem*>(c.create());
    layout->addItem(obj);

    widget->setGeometry(QRectF(0,0, 400,400));
    view.show();
    return app.exec();
}
