#include "settingstations.h"
#include "ui_settingstations.h"

SettingStations::SettingStations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingStations)
{
    _config = new Core::Config("config.xml", "config.xsd");
    _stationlist = new Core::StationsList;
    *_stationlist = _config->stationsList();
    qDebug() << "station count = " << _stationlist->size();

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
        qDebug() << "ok dialog " << station->name().c_str();
    }
}
void
SettingStations::remove()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    for (int i=0; i<_stationlist->size(); i++){
        if (item->toolTip().toStdString().compare(_stationlist->at(i)->id()) == 0 &&
            item->text().toStdString().compare(_stationlist->at(i)->name()) == 0){
            qDebug() << "erase i = " << i << " id = " << _stationlist->at(i)->id().c_str();
            _stationlist->erase(_stationlist->begin()+i);
        }
    }
    qDebug() << "remove " << item->toolTip();
    delete item;
    if (ui->listWidget->count() == 0)
        ui->removeButton->setEnabled(false);
}
void
SettingStations::setRemove(QListWidgetItem *item)
{
    qDebug() << "list activated " << item->text();
    ui->removeButton->setEnabled(true);
}

void
SettingStations::okClicked()
{
    /*
    std::vector<Core::Station*>::iterator cur;
    for (cur=_stationlist->begin(); cur<_stationlist->end(); cur++){
        qDebug() << "aaa " << (*cur)->name().c_str();
    }
    */

    _config->stationsList(*_stationlist);
    _config->saveConfig("newconfig.xml");
}
