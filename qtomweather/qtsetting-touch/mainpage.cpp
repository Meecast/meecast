#include <QtDBus>
#include "mainpage.h"

MainPage::MainPage() :
    MApplicationPage(0)
{
    setTitle("Settings");
    stationPage = new StationPage();
    connect(stationPage, SIGNAL(stationSaved()),
            this, SLOT(stationSave()));
}
MainPage::~MainPage()
{
}
void MainPage::createContent()
{

    combo = new MComboBox();
    combo->setTitle("Stations");

    _stationlist = new Core::StationsList;
    std::string configpath = Core::AbstractConfig::getConfigPath();
    configpath += "config.xml";
    std::string schemapath = Core::AbstractConfig::prefix;
    schemapath += Core::AbstractConfig::schemaPath;
    schemapath += "config.xsd";
    //qDebug() << configpath.c_str() << " " << schemapath.c_str();
    //Core::Config *_config;
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

    for(int i=0; i<_stationlist->size(); i++){
        combo->addItem(QString::fromStdString(_stationlist->at(i)->name()));
        combo->setData(i, QString::fromStdString(_stationlist->at(i)->id()));
    }

    //combo->setCurrentIndex(0);
    connect(combo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stationChanged(int)));

    //QStringList list;
    //list << "weather.com" << "gismeteo.by";
    //combo->addItems(list);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    QGraphicsLinearLayout *layout_station = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout *layout_btn = new QGraphicsLinearLayout(Qt::Vertical);
    //page.setCentralWidget(combo);
    centralWidget()->setLayout(layout);
    layout_station->addItem(combo);

    addbutton = new MButton("Add");
    delbutton = new MButton("Delete");
    delbutton->setEnabled(false);
    layout_btn->addItem(addbutton);
    layout_btn->addItem(delbutton);

    layout_station->addItem(layout_btn);
    layout->addItem(layout_station);

    temperature_combo = new MComboBox();
    temperature_combo->setTitle("Temperature unit");
    temperature_combo->addItem("C");
    temperature_combo->addItem("F");
    if (_config->TemperatureUnit().compare("F") == 0)
        temperature_combo->setCurrentIndex(1);
    else
        temperature_combo->setCurrentIndex(0);

    layout->addItem(temperature_combo);

    iconset_combo = new MComboBox();
    iconset_combo->setTitle("IconSet");
    iconset_combo->addItem("Glance");
    iconset_combo->addItem("Mecast");
    if (_config->iconSet().compare("Mecast") == 0)
        iconset_combo->setCurrentIndex(1);
    else
        iconset_combo->setCurrentIndex(0);

    layout->addItem(iconset_combo);

    MButton *savebutton = new MButton("Save");
    layout->addItem(savebutton);

    connect(addbutton, SIGNAL(clicked()),
            this, SLOT(addClicked()));
    connect(delbutton, SIGNAL(clicked()),
            this, SLOT(delClicked()));
    connect(savebutton, SIGNAL(clicked()),
            this, SLOT(saveClicked()));
}

void MainPage::addClicked()
{

    //stationPage->appear((MWindow *)applicationWindow(), MSceneWindow::DestroyWhenDismissed);
    stationPage->appear((MWindow *)applicationWindow());
}
void MainPage::delClicked()
{
    std::cerr << "size = " << _stationlist->size() << std::endl;
    int index = combo->currentIndex();
    for (int i=0; i<_stationlist->size(); i++){
        if (QString::fromStdString(_stationlist->at(i)->id()) ==
            combo->data(index).toString() &&
            QString::fromStdString(_stationlist->at(i)->name()) ==
            combo->currentText()){
            _stationlist->erase(_stationlist->begin()+i);
        }
    }
    std::cerr << "size2 = " << _stationlist->size() << std::endl;
    combo->removeItem(combo->currentIndex());

}
void MainPage::stationChanged(int val)
{
    if (val > -1)
        delbutton->setEnabled(true);
    else
        delbutton->setEnabled(false);
}
void MainPage::stationSave()
{
    std::cerr << "signal station saved" << std::endl;

    Core::Station *station = stationPage->station;
    _stationlist->push_back(station);
    combo->addItem(station->name().c_str());
    combo->setData(combo->count(), station->id().c_str());
    std::cerr << "size2 = " << _stationlist->size() << std::endl;
}
void MainPage::saveClicked()
{
    _config->stationsList(*_stationlist);
    _config->TemperatureUnit(temperature_combo->currentText().toStdString());
    _config->iconSet(iconset_combo->currentText().toStdString());
    _config->saveConfig();
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/org/meego/omweather",
                               "org.meego.omweather", "reload_config");
    bus.send(message);
    message = QDBusMessage::createMethodCall("org.meego.omweather","/org/meego/omweather",
                               "org.meego.omweather", "reload_config");
    bus.send(message);
    //close();
    //emit closeButtonClicked();
    emit exit(0);
}
