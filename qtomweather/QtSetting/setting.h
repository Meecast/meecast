#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
    class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();
public slots:
    void sourceChanged(int val);

private:
    Ui::Setting *ui;
    bool connect(const QString filename);
    QStringList getCountry();
};

#endif // SETTING_H
