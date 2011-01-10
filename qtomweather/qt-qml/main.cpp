#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QTranslator>
#include <QGraphicsGridLayout>
#include <exception>
#include <iostream>

#include "core.h"
#include "dataqml.h"
#include "configqml.h"
#include "qmllayoutitem.h"
#include "dataitem.h"
#include "datamodel.h"

//////////////////////////////////////////////////////////////////////////////

ConfigQml *
create_and_fill_config(){
    ConfigQml *config;
    try{
        config = new ConfigQml("config.xml", "../core/data/config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in ConfigQML class: "<< str <<std::endl;
    }

    return config;
}
//////////////////////////////////////////////////////////////////////////////
bool
update_weather_forecast(std::vector<Core::Station*> StationsList){
    int i;
    Core::Station* station;
    for (i=0; i<StationsList.size();i++){
        station = StationsList.at(i);
     //   std::cerr<<"yyyyyy    "<< station->forecastURL()<<std::endl;
    }
}

int main(int argc, char* argv[])
{
    QApplication::setGraphicsSystem("native");
    QApplication app(argc, argv);



/*
    //Set up a graphics scene with a QGraphicsWidget and Layout
    QGraphicsView view;
    QGraphicsScene scene;
    QGraphicsWidget *widget = new QGraphicsWidget();
    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    layout->setSpacing(0);
    widget->setLayout(layout);
    scene.addItem(widget);
    view.setScene(&scene);
*/

    //Add the QML snippet into the layout


    ConfigQml *config;
    DataItem *forecast_data = NULL;

    Core::DataList data_list;
    QmlLayoutItem* qml_layout_item;
    Core::DataParser* dp;
    Core::Data *temp_data = NULL;
    int i, id;

    std::vector<Core::Station*> StationsList;

    config = create_and_fill_config();
    StationsList = config->StationsList();
    std::cerr<<"size "<<StationsList.size()<<std::endl;
    update_weather_forecast(StationsList);

    try{
        dp = new Core::DataParser("data.xml", "../core/data/data.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return -1;
    }
    catch(const char *str){
        std::cerr<<"Error in DataParser class: "<< str <<std::endl;
        return -1;
    }

    DataModel *model = new DataModel(new DataItem, qApp);
    i = 0;
    while  (temp_data = dp->data().GetDataForTime(time(NULL) + i)) {
        i = i + 3600*24;
        forecast_data = new DataItem(temp_data);
        id = model->appendRow(forecast_data);
        forecast_data->setId(id);

    }

    QTranslator translator;
    translator.load("ru.qml", "i18n");
    app.installTranslator(&translator);

    QDeclarativeView qview;
    /*
    if (forecast_data){
        qview.rootContext()->setContextProperty("Forecast", forecast_data);
    }*/
    //std::cout << "temp = " << model->getRow(2) << std::endl;
    qview.rootContext()->setContextProperty("Forecast_model", model);
    qview.rootContext()->setContextProperty("Config", config);
    qview.setSource(QUrl(":weatherlayoutitem.qml"));
    qview.show();

/*
    qml_layout_item = new QmlLayoutItem(config, forecast_data);
    layout->addItem(&qml_layout_item->obj(),0,0);

    forecast_data =  create_and_fill_class_data_for_day_forecast();
    qml_layout_item = new QmlLayoutItem(config, forecast_data);
    layout->addItem(&qml_layout_item->obj(),1,0);



    widget->setGeometry(QRectF(0,0, 800,400));



    view.show();
*/

    return app.exec();
}
