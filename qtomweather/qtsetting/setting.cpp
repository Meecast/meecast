#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    db = new Core::DatabaseSqlite("");

    //sourcelist = new Core::SourceList("../test/sources/");
    //QString path = QString::fromStdString(Core::AbstractConfig::prefix);
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sourcesPath;

    sourcelist = new Core::SourceList(path);

    ui->setupUi(this);
    for (int i=0; i<sourcelist->size(); i++){
        ui->sourceCombo->addItem(QString::fromStdString(sourcelist->at(i)->name()), i);
    }

}

void
Setting::sourceChanged(int val)
{
    QString filename = ui->sourceCombo->currentText();
    filename.append(".db");
    if (!this->db) {
        this->db->set_databasename(filename.toStdString());
    }else {
        this->db->set_databasename(filename.toStdString());

    }
    this->db->open_database();

    Core::listdata * list = db->create_countries_list();

    Core::listdata::iterator cur;
    this->ui->countryCombo->clear();
    this->ui->countryCombo->addItem("", "0");
    for (cur=list->begin(); cur<list->end(); cur++)
        ui->countryCombo->addItem(
                QString::fromStdString((*cur).second),
                QString::fromStdString((*cur).first)
        );


}
void
Setting::countryChanged(int val)
{
    this->ui->regionCombo->clear();

    Core::listdata * list = db->create_region_list(ui->countryCombo->itemData(val).toInt());

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
    this->ui->cityCombo->clear();

    Core::listdata * list = db->create_stations_list(ui->regionCombo->itemData(val).toInt());

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
    if (ui->cityCombo->currentIndex() == -1){
        this->reject();
    }

    std::string code = ui->cityCombo->itemData(ui->cityCombo->currentIndex()).toString().toStdString();
    int index = ui->sourceCombo->itemData(ui->sourceCombo->currentIndex()).toInt();
    std::string url_template = sourcelist->at(index)->url_template();

    char forecast_url[4096];
    snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), code.c_str());
    station = new Core::Station(
                ui->sourceCombo->currentText().toStdString(),
                code,
                ui->cityCombo->currentText().toStdString(),
                ui->countryCombo->currentText().toStdString(),
                ui->regionCombo->currentText().toStdString(),
                forecast_url);
    std::string filename(Core::AbstractConfig::getConfigPath());
    filename += ui->sourceCombo->currentText().toStdString();
    filename += "_";
    filename += code;
    station->fileName(filename);
    station->converter(sourcelist->at(index)->binary());
    return;
}

Setting::~Setting()
{
    delete ui;
    //if (station)
      //  delete station;
    delete sourcelist;
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
