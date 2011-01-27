#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
}

void
Setting::sourceChanged(int val)
{
    qDebug() << "source = " << val << " - " << this->ui->sourceCombo->currentText();
    this->ui->countryCombo->clear();

    this->ui->countryCombo->addItem("Belarus");
    getCountry();
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
Setting::getCountry()
{
    if (open_database("weather.com.db"))
        qDebug() << "ok";
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select name from countries");
    qDebug() << model->rowCount();
    this->ui->countryCombo->setModel(model);
}
