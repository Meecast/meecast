#include <QString>
#include <MWidget>

//#include <duiapplicationextensioninterface.h>
#include <QObject>
#include <MApplicationExtensionInterface>



class MeecastExtensionInterface : public MApplicationExtensionInterface
{
    Q_INTERFACES(MApplicationExtensionInterface)

public:
    virtual void MeecastExtensionSpecificOperation() = 0;
};

Q_DECLARE_INTERFACE(MeecastExtensionInterface, "com.omweather.meecast.DemoExtensionInterface/1.0")


class MButton;

class MeecastApplicationExtension : public QObject, public MeecastExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(MeecastExtensionInterface MApplicationExtensionInterface)

public:
    MeecastApplicationExtension();
    virtual ~MeecastApplicationExtension();

    virtual void MeecastExtensionSpecificOperation();

    virtual bool initialize(const QString &interface);
    virtual MWidget *widget();

private:
    MButton *button;
};

