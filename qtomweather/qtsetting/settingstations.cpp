#include "settingstations.h"
#include "ui_settingstations.h"

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
}

SettingStations::~SettingStations()
{
    delete ui;
}

void
SettingStations::add()
{
    Setting st(this);
    if (st.exec()){
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
    _config->saveConfig("newconfig.xml");
}
