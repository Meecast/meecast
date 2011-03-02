#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MLabel>
#include <MComboBox>
#include <MButton>
#include <QGraphicsLinearLayout>

int main(int argc, char *argv[])
{
    MApplication app(argc, argv);
    MApplicationWindow window;
    MApplicationPage page;

    page.setTitle("Settings");

    MComboBox *combo = new MComboBox();
    combo->setTitle("Stations");
    QStringList list;
    list << "weather.com" << "gismeteo.by";
    combo->addItems(list);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    //page.setCentralWidget(combo);
    page.centralWidget()->setLayout(layout);

    layout->addItem(combo);

    MButton *delbutton = new MButton("Delete");

    layout->addItem(delbutton);

    page.appear(&window);

    window.show();

    return app.exec();
}
