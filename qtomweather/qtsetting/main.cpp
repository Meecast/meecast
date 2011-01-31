#include <QtGui/QApplication>
#include "settingstations.h"
#include "setting.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingStations w;
    w.show();

    return a.exec();
}
