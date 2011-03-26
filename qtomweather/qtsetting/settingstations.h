#ifndef SETTINGSTATIONS_H
#define SETTINGSTATIONS_H

#include <QDialog>
#include "setting.h"
#include <QListWidget>
#if defined (BSD) && !_POSIX_SOURCE
    #include <sys/dir.h>
    typedef struct dirent Dirent;
#else
    #include <dirent.h>
    #include <linux/fs.h>
    typedef struct dirent Dirent;
#endif

namespace Ui {
    class SettingStations;
}

class SettingStations : public QDialog
{
    Q_OBJECT

public:
    explicit SettingStations(QWidget *parent = 0);
    ~SettingStations();
    //std::vector<Core::Station*> *_stationlist;
    Core::StationsList *_stationlist;
    Core::Config *_config;
    QHash<QString, int> period_hash;
public slots:
    void add();
    void remove();
    void setRemove(QListWidgetItem *item);
    void okClicked();

private:
    Ui::SettingStations *ui;
};

#endif // SETTINGSTATIONS_H
