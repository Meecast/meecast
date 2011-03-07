#ifndef STATIONPAGE_H
#define STATIONPAGE_H

#include <MApplicationPage>
#include <MLabel>
#include <MComboBox>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <MSceneManager>
#include "core.h"
#include "mainpage.h"

class StationPage : public MApplicationPage
{
    Q_OBJECT
public:
    StationPage(QGraphicsItem *parent = 0);

protected:
    virtual void createContent();
signals:

public slots:

};

#endif // STATIONPAGE_H
