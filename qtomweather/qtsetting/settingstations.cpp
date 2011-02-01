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
        qDebug() << "ok dialog " << station->id().c_str();
        QListWidgetItem *item = new QListWidgetItem(st.station->name().c_str(), ui->listWidget);
        item->setToolTip(st.station->id().c_str());
    }
}
void
SettingStations::remove()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
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
