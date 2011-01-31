#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    db = new Core::DatabaseSqlite("");
    ui->setupUi(this);
    ui->sourceCombo->addItem("gismeteo.ru", "gismeteo.ru.db");
    ui->sourceCombo->addItem("weather.com", "weather.com.db");


}

void
Setting::sourceChanged(int val)
{
    qDebug() << "source = " << val << " - " << ui->sourceCombo->itemData(val);

    if (!this->db) {
        this->db->set_databasename(ui->sourceCombo->itemData(val).toString().toStdString());
    }else {
        this->db->set_databasename(ui->sourceCombo->itemData(val).toString().toStdString());

    }
    this->db->open_database();

    Core::listdata * list = db->create_countries_list();
    qDebug() << "list size = " << list->size();

    Core::listdata::iterator cur;
    this->ui->countryCombo->clear();
    this->ui->countryCombo->addItem("", "0");
    for (cur=list->begin(); cur<list->end(); cur++)
        //std::cerr << (*cur).first << " - " << (*cur).second << std::endl;
        ui->countryCombo->addItem(
                QString::fromStdString((*cur).second),
                QString::fromStdString((*cur).first)
        );


}
void
Setting::countryChanged(int val)
{

    qDebug() << "region = " << val << " - " << ui->countryCombo->itemData(val);
    this->ui->regionCombo->clear();

    Core::listdata * list = db->create_region_list(ui->countryCombo->itemData(val).toInt());
    qDebug() << "list size = " << list->size();

    if (list->size() == 0) return;
    Core::listdata::iterator cur;
    for (cur=list->begin(); cur<list->end(); cur++)
        ui->regionCombo->addItem(
                QString::fromStdString((*cur).second),
                QString::fromStdString((*cur).first)
        );

}
void
Setting::regionChanged(int val)
{
    qDebug() << "city = " << val << " - " << ui->regionCombo->itemData(val);

    this->ui->cityCombo->clear();

    Core::listdata * list = db->create_stations_list(ui->regionCombo->itemData(val).toInt());
    qDebug() << "list size = " << list->size();

    if (list->size() == 0) return;
    Core::listdata::iterator cur;
    this->ui->cityCombo->addItem("", "0");
    for (cur=list->begin(); cur<list->end(); cur++)
        ui->cityCombo->addItem(
                QString::fromStdString((*cur).second),
                QString::fromStdString((*cur).first)
        );

}

void
Setting::okClicked()
{
    qDebug() << "ok " << ui->cityCombo->itemData(ui->cityCombo->currentIndex());
    station_code = ui->cityCombo->itemData(ui->cityCombo->currentIndex()).toString();
    station_name = ui->cityCombo->currentText();
}

Setting::~Setting()
{
    delete ui;
}
/*
bool
Setting::open_database(const QString filename)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQueryModel *model = new QSqlQueryModel;
    if (db.isValid()){
        qDebug() << "db is valid";
        db.setDatabaseName(filename);
    }
    if (!db.isValid() || !db.open()){
        qDebug() << "error open";
        return false;
    }
    if (filename == "gismeteo.ru.db"){
        //model->setQuery("CREATE TEMP VIEW nstations AS SELECT russian_name as name, id, region_id, longititude, latitude, code, id_gismeteo_new, id_gesmeteo_old FROM stations");
        model->setQuery("CREATE TEMP VIEW nstations AS SELECT * FROM stations where russian_name != name");
    }else {
        model->setQuery("CREATE TEMP VIEW nstations AS SELECT * FROM stations");
    }
    return true;
}

void
Setting::getCountry(QString filename)
{
    if (open_database(filename))
        qDebug() << "ok";
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select id, name from countries where (select count(name) from nstations where nstations.region_id = (select distinct regions.id from regions where regions.country_id=countries.id)) >0 order by name");
    qDebug() << model->rowCount();
    this->ui->countryCombo->setModel(model);
    this->ui->countryCombo->setModelColumn(1);
}
*/
