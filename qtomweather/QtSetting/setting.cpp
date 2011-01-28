#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    ui->sourceCombo->addItem("gismeteo.ru", "gismeteo.ru.db");
    ui->sourceCombo->addItem("weather.com", "weather.com.db");
}

void
Setting::sourceChanged(int val)
{
    qDebug() << "source = " << val << " - " << ui->sourceCombo->itemData(val);
    this->ui->countryCombo->clear();

    getCountry(ui->sourceCombo->itemData(val).toString());
}
void
Setting::countryChanged(int val)
{
    qDebug() << "region = " << val << " - " << ui->countryCombo->itemData(val);
    this->ui->regionCombo->clear();

    this->ui->regionCombo->addItem("Belarus");

}

Setting::~Setting()
{
    delete ui;
}

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
