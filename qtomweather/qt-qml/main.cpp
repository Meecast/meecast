#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
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
DataQml *
create_and_fill_class_data_for_day_forecast()
{
    time_t      current_time, begin_current_day, end_current_day;
    struct tm   *tm = NULL;
    int         year, current_month;

    DataQml *wdata = new DataQml;
    /* Create current day */
    current_time = time(NULL);
    begin_current_day = current_time;
    tm = localtime(&begin_current_day);
    year = 1900 + tm->tm_year;
    current_month = tm->tm_mon;
    tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
    tm->tm_isdst = 1;
    begin_current_day = mktime(tm);
    tm->tm_sec = 0; tm->tm_min = 59; tm->tm_hour = 23;
    end_current_day = mktime(tm);
    wdata->StartTime(begin_current_day);
    wdata->EndTime(end_current_day);
    wdata->Data::temperature_low().value(20.0);
    wdata->Data::temperature_hi().value(30.0);
    wdata->Flike().value(17.0);
    wdata->WindGust(2.8);
    wdata->WindSpeed(2.4);
    wdata->Humidity(845);
    wdata->Icon(4);
    wdata->Text(std::string("Cloudy"));
    return wdata;
}
//////////////////////////////////////////////////////////////////////////////

DataItem *
create_and_fill_class_data_for_hours_forecast()
{
    DataItem *wdata = new DataItem;
    wdata->StartTime(time(NULL) - 3600);
    wdata->EndTime(time(NULL) + 3600);
    wdata->Data::temperature_hi().value(23.0);
    wdata->Flike().value(18.0);
    wdata->WindSpeed(3.0);
    wdata->WindGust(4.0);
    wdata->WindDirection(std::string("NNW"));
    wdata->Icon(0);
    wdata->Text(std::string("Clear"));
    return wdata;
}
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
    int i;

    config = create_and_fill_config();

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

    forecast_data = create_and_fill_class_data_for_hours_forecast();
    DataModel *model = new DataModel(new DataItem, qApp);
    i = 0;
    while  (temp_data = dp->data().GetDataForTime(time(NULL) + i)) {
        i = i + 3600*24;
        forecast_data = new DataItem(temp_data);
        model->appendRow(forecast_data);
    }


    QDeclarativeView qview;
    /*
    if (forecast_data){
        qview.rootContext()->setContextProperty("Forecast", forecast_data);
    }*/
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
