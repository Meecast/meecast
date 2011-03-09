#include <MSceneManager>
#include "stationpage.h"

StationPage::StationPage() :
    MApplicationPage(0)
{
    setTitle("Select station");
}
void StationPage::createContent()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->addItem(new MLabel("fdssfdf"));
    centralWidget()->setLayout(layout);
}
