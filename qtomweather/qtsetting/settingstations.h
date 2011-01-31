#ifndef SETTINGSTATIONS_H
#define SETTINGSTATIONS_H

#include <QDialog>
#include "setting.h"
#include <QListWidget>

namespace Ui {
    class SettingStations;
}

class SettingStations : public QDialog
{
    Q_OBJECT

public:
    explicit SettingStations(QWidget *parent = 0);
    ~SettingStations();
public slots:
    void add();
    void remove();
    void setRemove(QListWidgetItem *item);

private:
    Ui::SettingStations *ui;
};

#endif // SETTINGSTATIONS_H
