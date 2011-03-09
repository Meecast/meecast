#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <MLabel>
#include <MComboBox>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <MSceneManager>
#include "core.h"

#include "stationpage.h"

class MainPage : public MApplicationPage
{
    Q_OBJECT
public:
    MainPage();
    virtual ~MainPage();
    Core::StationsList *_stationlist;
    Core::Config *_config;

protected:
    virtual void createContent();

private slots:
    void addClicked();
    void delClicked();
    void saveClicked();
    void stationChanged(int val);
    void stationSave();
private:
    MComboBox *combo, *temperature_combo;
    MButton *addbutton, *delbutton;
    StationPage *stationPage;
};

#endif // MAINPAGE_H
