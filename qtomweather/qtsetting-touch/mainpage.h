#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <MLabel>
#include <MComboBox>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <MSceneManager>
#include "core.h"
#if defined (BSD) && !_POSIX_SOURCE
    #include <sys/dir.h>
    typedef struct dirent Dirent;
#else
    #include <dirent.h>
    #include <linux/fs.h>
    typedef struct dirent Dirent;
#endif

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
    MComboBox *combo, *temperature_combo, *iconset_combo;
    MButton *addbutton, *delbutton;
    StationPage *stationPage;
};

#endif // MAINPAGE_H
