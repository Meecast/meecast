#include <MApplication>
#include <MApplicationWindow>
#include "mainpage.h"

int main(int argc, char *argv[])
{
    MApplication app(argc, argv);
    MApplicationWindow window;

    MainPage *mainPage = new MainPage();

    mainPage->appear(&window);

    window.show();

    return app.exec();
}
