#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "core.h"
#include "databasesqlite.h"

namespace Ui {
    class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();
    QString station_code;
    QString station_name;
public slots:
    void sourceChanged(int val);
    void countryChanged(int val);
    void regionChanged(int val);
    void okClicked();
private:
    Ui::Setting *ui;
    Core::DatabaseSqlite *db;
    //bool open_database(const QString filename);
    //void getCountry(QString filename);
};

#endif // SETTING_H
