#include "mainpage.h"

MainPage::MainPage(QObject *parent) :
    MApplicationPage(parent)
{
    setTitle("Settings");

    MComboBox *combo = new MComboBox();
    combo->setTitle("Stations");

    Core::StationsList *_stationlist = new Core::StationsList;
    std::string configpath = Core::AbstractConfig::getConfigPath();
    configpath += "config.xml";
    std::string schemapath = Core::AbstractConfig::prefix;
    schemapath += Core::AbstractConfig::schemaPath;
    schemapath += "config.xsd";
    //qDebug() << configpath.c_str() << " " << schemapath.c_str();
    Core::Config *_config;
    try {
        _config = new Core::Config(configpath, schemapath);
        *_stationlist = _config->stationsList();
    }catch(const std::string &str){
        std::cerr<<"Error in ConfigQML class: "<< str <<std::endl;
        _config = new Core::Config();
    }
    catch(const char *str){
        std::cerr<<"Error in ConfigQML class: "<< str <<std::endl;
        _config = new Core::Config();
    }

    Core::StationsList::iterator cur;
    for (cur=_stationlist->begin(); cur<_stationlist->end(); cur++){
        combo->addItem(QString::fromStdString((*cur)->name()));

    }

    //QStringList list;
    //list << "weather.com" << "gismeteo.by";
    //combo->addItems(list);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    QGraphicsLinearLayout *layout_station = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout *layout_btn = new QGraphicsLinearLayout(Qt::Vertical);
    //page.setCentralWidget(combo);
    centralWidget()->setLayout(layout);

    layout_station->addItem(combo);

    MButton *addbutton = new MButton("Add");
    MButton *delbutton = new MButton("Delete");
    layout_btn->addItem(addbutton);
    layout_btn->addItem(delbutton);

    layout_station->addItem(layout_btn);
    layout->addItem(layout_station);

    MComboBox *temperature_combo = new MComboBox();
    temperature_combo->setTitle("Temperature unit");
    temperature_combo->addItem("C");
    temperature_combo->addItem("F");

    layout->addItem(temperature_combo);
}
