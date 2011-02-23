#include "settingstations.h"
#include "ui_settingstations.h"
#include <QtDBus>

SettingStations::SettingStations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingStations)
{
    _stationlist = new Core::StationsList;
    std::string configpath = Core::AbstractConfig::getConfigPath();
    configpath += "config.xml";
    std::string schemapath = Core::AbstractConfig::prefix;
    schemapath += Core::AbstractConfig::schemaPath;
    schemapath += "config.xsd";
    //qDebug() << configpath.c_str() << " " << schemapath.c_str();
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
    ui->setupUi(this);
    ui->removeButton->setEnabled(false);

    Core::StationsList::iterator cur;
    for (cur=_stationlist->begin(); cur<_stationlist->end(); cur++){
        QListWidgetItem *item = new QListWidgetItem((*cur)->name().c_str(), ui->listWidget);
        item->setToolTip((*cur)->id().c_str());
    }

    ui->temperatureCombo->addItem("C");
    ui->temperatureCombo->addItem("F");
    //std::cerr << _config->TemperatureUnit() << std::endl;
    if (_config->TemperatureUnit().compare("F") == 0)
        ui->temperatureCombo->setCurrentIndex(1);
    else
        ui->temperatureCombo->setCurrentIndex(0);

}

SettingStations::~SettingStations()
{
    delete ui;
}

void
SettingStations::add()
{
    Setting st(this);
    int res = st.exec();
    //std::cerr << "dialog result " << res << std::endl;
    if (res){
        Core::Station *station = st.station;
        _stationlist->push_back(station);
        QListWidgetItem *item = new QListWidgetItem(station->name().c_str(), ui->listWidget);
        item->setToolTip(station->id().c_str());
    }
}
void
SettingStations::remove()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    for (int i=0; i<_stationlist->size(); i++){
        if (item->toolTip().toStdString().compare(_stationlist->at(i)->id()) == 0 &&
            item->text().toStdString().compare(_stationlist->at(i)->name()) == 0){
            _stationlist->erase(_stationlist->begin()+i);
        }
    }
    delete item;
    if (ui->listWidget->count() == 0)
        ui->removeButton->setEnabled(false);
}
void
SettingStations::setRemove(QListWidgetItem *item)
{
    ui->removeButton->setEnabled(true);
}

void
SettingStations::okClicked()
{
    _config->stationsList(*_stationlist);
    _config->TemperatureUnit(ui->temperatureCombo->currentText().toStdString());
    _config->saveConfig();
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/org/meego/omweather",
                               "org.meego.omweather", "reload_config");
    bus.send(message);
    message = QDBusMessage::createMethodCall("org.meego.omweather","/org/meego/omweather",
                               "org.meego.omweather", "reload_config");
    bus.send(message);

}
