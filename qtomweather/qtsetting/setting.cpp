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
    if (ui->cityCombo->currentIndex() == -1)
        return;
    qDebug() << "ok " << ui->cityCombo->itemData(ui->cityCombo->currentIndex()) << "index = " << ui->cityCombo->currentIndex();

    std::string code = ui->cityCombo->itemData(ui->cityCombo->currentIndex()).toString().toStdString();

    /* temporary */
    std::string url_template;
    if (ui->sourceCombo->currentText() == "weather.com")
        url_template = "http://xml.weather.com/weather/local/%s?cm_ven=1CW&amp;site=wx.com-bar&amp;cm_ite=wx-cc&amp;par=1CWFFv1.1.9&amp;cm_pla=wx.com-bar&amp;cm_cat=FFv1.1.9&amp;unit=m&amp;dayf=10&amp;cc=*";
    else
        url_template = "http://www.gismeteo.by/city/weekly/%s/";
    /* ******* */
    char forecast_url[4096];
    snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), code.c_str());
    qDebug() << "url = " << forecast_url;
    //std::cerr << "url = " << forecast_url << std::endl;
    station = new Core::Station(
                ui->sourceCombo->currentText().toStdString(),
                code,
                ui->cityCombo->currentText().toStdString(),
                ui->countryCombo->currentText().toStdString(),
                ui->regionCombo->currentText().toStdString(),
                forecast_url);
    qDebug() << "sourse name - " << station->country().c_str();
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
