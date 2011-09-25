#include <QString>
#include <MWidget>
#include <QObject>
#include <MApplicationExtensionInterface>
#include <QProcess>


#include "core.h"

class MyMWidget : public MWidget
{
   Q_OBJECT                                                                                                                                                                       
   Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)  
   Q_PROPERTY(QString station READ station NOTIFY stationChanged)  
   Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)  
public:

    MyMWidget(){
        _stationname = "Unknown";
    };

    ~MyMWidget(){};

    Q_INVOKABLE void startapplication(){
	QString executable("/usr/bin/invoker");    
	QStringList arguments;
	arguments << "--single-instance";
	arguments << "--type=e";
	arguments <<"/opt/com.meecast.omweather/bin/omweather-qml";	
        process.start(executable, arguments);
    }

    QString icon(){
        QString c;
	c="/opt/com.meecast.omweather/share/icons/Meecast/28.png";
	return c;
    } 
    void station(const std::string& stationname){
	_stationname = stationname.c_str();
    }
    QString station(){
        QString c;
	c="+15";
	return c;
	//return _stationname;
    } 
    QString temperature(){
        QString c;
	c="+15";
	return c;
    }

    void refreshview(){
	    emit iconChanged();
	    emit stationChanged();
	    emit temperatureChanged();
    };

private:
    QProcess process;
    QString  _stationname;
signals:
    void iconChanged();
    void stationChanged();
    void temperatureChanged();
};

class WeatherExtensionInterface : public MApplicationExtensionInterface
{
    Q_INTERFACES(MApplicationExtensionInterface)

public:
    virtual void weatherExtensionSpecificOperation() = 0;
};

Q_DECLARE_INTERFACE(WeatherExtensionInterface, "com.nokia.home.EventsExtensionInterface/1.0")


//class MContainer;
//class MButton;
class MyMWidget;

class WeatherApplicationExtension : public QObject, public WeatherExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(WeatherExtensionInterface MApplicationExtensionInterface)

public:
    WeatherApplicationExtension();
    virtual ~WeatherApplicationExtension();

    virtual void weatherExtensionSpecificOperation();

    virtual bool initialize(const QString &interface);
    virtual MWidget *widget();

private:
    MyMWidget *box;
    //MContainer *box;
    //MButton *box;
    //MButton *button;
};

