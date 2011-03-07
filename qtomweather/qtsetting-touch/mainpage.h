#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <MLabel>
#include <MComboBox>
#include <MButton>
#include <QGraphicsLinearLayout>
#include "core.h"

class MainPage : public MApplicationPage
{
    Q_OBJECT
public:
    explicit MainPage(QObject *parent = 0);

signals:

public slots:

};

#endif // MAINPAGE_H
