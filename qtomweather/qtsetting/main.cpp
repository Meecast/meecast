#include <QtGui/QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "settingstations.h"
#include "setting.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator myaTranslator;
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
			               QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    myaTranslator.load("omweather-settings_" + QLocale::system().name());
    a.installTranslator(&myaTranslator);

    SettingStations w;
    w.show();

    return a.exec();
}
