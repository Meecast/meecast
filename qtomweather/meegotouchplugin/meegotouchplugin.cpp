#include <MButton>
#include "meegotouchplugin.h"
#include <MLibrary>

M_LIBRARY
Q_EXPORT_PLUGIN2(weatherextension, WeatherApplicationExtension)

WeatherApplicationExtension::WeatherApplicationExtension() : button(0)
{
}

WeatherApplicationExtension::~WeatherApplicationExtension()
{
    delete button;
}

void WeatherApplicationExtension::weatherExtensionSpecificOperation()
{
    // do something specific to the demo extension interface
}

bool WeatherApplicationExtension::initialize(const QString &)
{
    button = new MButton("Hello World");

    return true;
}

MWidget *WeatherApplicationExtension::widget()
{
    return button;
}

