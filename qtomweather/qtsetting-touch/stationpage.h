#ifndef STATIONPAGE_H
#define STATIONPAGE_H

#include <MApplicationPage>
#include <MLabel>
#include <MComboBox>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "core.h"
#include "databasesqlite.h"



class StationPage : public MApplicationPage
{
    Q_OBJECT
public:
    StationPage();
    ~StationPage();
    Core::Station *station;
    Core::SourceList *sourcelist;

protected:
    virtual void createContent();
private:
    Core::DatabaseSqlite *db;
    MComboBox *sourceCombo, *countryCombo, *regionCombo, *cityCombo;
public slots:
    void saveClicked();
    void sourceChanged(int val);
    void countryChanged(int val);
    void regionChanged(int val);
signals:
    void stationSaved();
};

#endif // STATIONPAGE_H
