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
Setting::connect(const QString filename)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if (db.isValid()){
        qDebug() << "db is valid";
        db.setDatabaseName(filename);
    }
    if (!db.isValid() || !db.open()){
        qDebug() << "error open";
        return false;
    }
    return true;
}

QStringList
Setting::getCountry()
{
    if (connect("weather.com.db"))
        qDebug() << "ok";
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select id, name from countries");
    qDebug() << model->rowCount();
}
