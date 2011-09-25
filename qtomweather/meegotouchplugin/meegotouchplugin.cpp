#include <MButton>
#include <MContainer>
#include "meegotouchplugin.h"
#include <MLibrary>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeView>


M_LIBRARY
Q_EXPORT_PLUGIN2(weatherextension, WeatherApplicationExtension)

WeatherApplicationExtension::WeatherApplicationExtension() : box(0)
{
}

WeatherApplicationExtension::~WeatherApplicationExtension()
{
    delete box;
}

void WeatherApplicationExtension::weatherExtensionSpecificOperation()
{
    // do something specific to the demo extension interface
}

Core::Config *
create_and_fill_config(){
    Core::Config *config;
  //  try{
    {
        config = new Core::Config(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
    }
    /*
    catch(const std::string &str){
        config = new Core::Config();
    }
    catch(const char *str){
        config = new Core::Config();
    }
    config->saveConfig();
*/
    return config;
}


bool WeatherApplicationExtension::initialize(const QString &)
{
   QGraphicsObject* mWidget;
 //  Core::Config *config;
   
   QDeclarativeView* view = new QDeclarativeView();
   view->setSource(QUrl::fromLocalFile("/opt/com.meecast.omweather/share/omweather/qml/applet.qml"));
   box = new MyMWidget();
   mWidget = qobject_cast<QGraphicsObject*>(view->rootObject());
   mWidget->setParentItem(box);
   view->rootContext()->setContextProperty("Applet", box);

 //  config = create_and_fill_config();
/*
    if (config->current_station_id() == INT_MAX && config->stationsList().size() > 0){
        config->current_station_id(0);
    }
    if (config->current_station_id() != INT_MAX && config->stationsList().size() > 0
        &&  config->stationsList().at(config->current_station_id()))
        box->station(config->stationsList().at(config->current_station_id())->name().c_str());
    else
   	box->station("Unknown");
*/

   return true;
}

MWidget *WeatherApplicationExtension::widget()
{
    return box;
}

