#ifndef STATIONPAGE_H
#define STATIONPAGE_H

#include <MApplicationPage>
#include <MLabel>
#include <MComboBox>
#include <MButton>
#include <QGraphicsLinearLayout>

//#include "core.h"
#include "mainpage.h"

class StationPage : public MApplicationPage
{
    Q_OBJECT
public:
    StationPage();

protected:
    virtual void createContent();
signals:

public slots:

};

#endif // STATIONPAGE_H
