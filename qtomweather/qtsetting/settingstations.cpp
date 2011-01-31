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
        qDebug() << "ok dialog " << st.station_code;
        QListWidgetItem *item = new QListWidgetItem(st.station_name, ui->listWidget);
        item->setToolTip(st.station_code);
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
