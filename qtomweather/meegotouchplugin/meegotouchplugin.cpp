#include <MButton>
#include "meegotouchplugin.h"
#include <MLibrary>

M_LIBRARY
Q_EXPORT_PLUGIN2(meecastextension, MeecastApplicationExtension)

MeecastApplicationExtension::MeecastApplicationExtension() : button(0)
{
}

MeecastApplicationExtension::~MeecastApplicationExtension()
{
    delete button;
}

void MeecastApplicationExtension::MeecastExtensionSpecificOperation()
{
    // do something specific to the demo extension interface
}

bool MeecastApplicationExtension::initialize(const QString &)
{
    button = new MButton("Hello World");

    return true;
}

MWidget *MeecastApplicationExtension::widget()
{
    return button;
}

