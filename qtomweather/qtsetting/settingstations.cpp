#include "settingstations.h"
#include "ui_settingstations.h"

SettingStations::SettingStations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingStations)
{
    ui->setupUi(this);
    ui->removeButton->setEnabled(false);
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
