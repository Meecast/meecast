#include <QtGui/QApplication>
#include "mainwindow.h"

void test(void);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    test();
    return a.exec();

}
