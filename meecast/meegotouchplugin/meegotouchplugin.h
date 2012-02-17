/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include <QString>
#include <MWidget>
#include <MLabel>
#include <MImageWidget>
#include <QObject>
#include <MApplicationExtensionInterface>
#include <MGConfItem>
#include <QProcess>
#include <QDir>
#include <QGraphicsGridLayout>
#include <MButton>
// Debug
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTimer>



class MyMWidget : public MWidget
{
   Q_OBJECT                                                                                                                                                                       
   Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)  
   Q_PROPERTY(QString station READ station NOTIFY stationChanged)  
   Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)  
   Q_PROPERTY(QString temperature_high READ temperature_high NOTIFY temperature_highChanged)  
   Q_PROPERTY(QString temperature_low READ temperature_low NOTIFY temperature_lowChanged)  
   Q_PROPERTY(bool current READ current NOTIFY currentChanged)  

private:
    QProcess process;
    QString  _stationname;
    QString  _temperature;
    QString  _temperature_high;
    QString  _temperature_low;
    QString  _iconpath;
    QString  _lastupdate;
    bool    _current;
    bool    _lockscreen;
    QTimer  *_timer;
    MGConfItem *_wallpaperItem;
    MGConfItem *_original_wallpaperItem;
    QString _wallpaper_path;
    QImage *_image;
    MLabel *_station_label;
    MLabel *_temperature_label;
    MLabel *_temperature_hi_label;
    MLabel *_temperature_low_label;
    MImageWidget *_icon;
public:

    MyMWidget(){

        
//#if 0
    QFile file("/tmp/1.log");
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "Begin PreInit MyWidget ."<<".\n";
	    file.close();
	}

//#endif

      _stationname = "Unknown";
      _temperature = "";
      _temperature_low = "";
      _temperature_high = "";
      _iconpath = "/opt/com.meecast.omweather/share/icons/Meecast/49.png";
      _current = false;
      _lockscreen = false;
      _timer = new QTimer(this);
      _timer->setSingleShot(true);

      QGraphicsGridLayout  *layout = new QGraphicsGridLayout();
      _station_label = new MLabel();
      _station_label->setColor("white");
      _station_label->setFont(QFont("Arial", 12));
      layout->addItem(_station_label, 0, 0, 1, 5, Qt::AlignHCenter);
      _temperature_label = new MLabel();
      _temperature_label->setFont(QFont("Arial", 12));
      _temperature_label->setColor("white");
      layout->addItem(_temperature_label, 2, 0, 2, 1);
      _temperature_hi_label = new MLabel();
      _temperature_hi_label->setColor("white");
      layout->addItem(_temperature_hi_label, 0, 2, 1, 1);
      _temperature_low_label = new MLabel();
      _temperature_low_label->setColor("white");
      layout->addItem(_temperature_low_label, 0, 3, 1, 1);
      _icon = new MImageWidget();
      layout->addItem(_icon, 1, 1, 5, 5);
      layout->setSpacing(0.0);
//      layout->setColumnFixedWidth(0,10);
      setLayout(layout);

      connect(_timer, SIGNAL(timeout()), this, SLOT(update_data()));
      _wallpaperItem = new MGConfItem ("/desktop/meego/background/portrait/picture_filename"); 
      connect(_wallpaperItem, SIGNAL(valueChanged()), this, SLOT(updateWallpaperPath()));
      if (!_wallpaperItem || _wallpaperItem->value() == QVariant::Invalid)
        _wallpaper_path = "/home/user/.wallpapers/wallpaper.png";
      else{
//#if 0
          // Debug begin
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "PreInit MyWidget ."<<_wallpaperItem->value().toString()<<".\n";
	    file.close();
	}
//#endif
        _wallpaper_path = _wallpaperItem->value().toString();
        if (_wallpaper_path.indexOf("MeeCast",0) != -1){
            _wallpaper_path = "/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast_original.png";
        }
      }
      _image = new QImage;
      _image->load(_wallpaper_path);
      if (_image->dotsPerMeterX() != 3780 || _image->dotsPerMeterY() != 3780 ){
        _image->setDotsPerMeterX(3780);
        _image->setDotsPerMeterY(3780);
      }
      if (_wallpaper_path.indexOf("MeeCast",0) == -1){
        _image->save("/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast_original.png");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  "PreInit3 MyWidget ."<<_wallpaper_path<<".\n";
            file.close();
	    }

      }

//#if 0
    // Debug begin
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "Finish Init MyWidget ."<<_wallpaper_path<<".\n";
	    file.close();
	}
//#endif
    };

    ~MyMWidget(){
      delete _timer;
    };

    void refreshwallpaper(bool new_wallpaper = false);

    Q_INVOKABLE void startapplication(){
        QString executable("/usr/bin/invoker");    
        QStringList arguments;
        arguments << "--single-instance";
        arguments << "--type=d";
        arguments <<"/opt/com.meecast.omweather/bin/omweather-qml";	
        process.startDetached(executable, arguments);
    }

    Q_INVOKABLE void startpredeamon(){
//#if 0

	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "startpredeamon"<< _stationname<< "\n";
	    file.close();
	}
	// Debug end 
//#endif
        QString executable("/opt/com.meecast.omweather/bin/predaemon");    
        process.startDetached(executable);
    }


    void icon(const QString &iconpath){
	    _iconpath = iconpath;
    }

    QString icon(){
	    return _iconpath; 
    } 

    void station(const QString &stationname){
	    _stationname = stationname;
    }

    QString station(){
	    return _stationname;
    } 

    void temperature(const QString &temperature){
	    _temperature = temperature;
    }

    QString temperature(){
	    return _temperature;
    }

    void temperature_high(const QString &temperature){
	    _temperature_high = temperature;
    }

    QString temperature_high(){
	    return _temperature_high;
    }

    void temperature_low(const QString &temperature){
	    _temperature_low = temperature;
    }

    QString temperature_low(){
	    return _temperature_low;
    }

    void lastupdate(const QString &lastupdate){
	    _lastupdate = lastupdate;
    }

    QString lastupdate(){
	    return _lastupdate;
    } 


    void current(bool cur){
        _current = cur;
    }
    bool current(){
        return _current;
    }

    void lockscreen(bool cur){
        _lockscreen = cur;
    }
    bool lockscreen(){
        return _lockscreen;
    }

    void refreshview(){
//#if 0
        // Debug begin
        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "refreshview"<< " \n";
            file.close();
        }
        // Debug end 
//#endif
       emit iconChanged();
       emit stationChanged();
       emit temperatureChanged();
       emit temperature_highChanged();
       emit temperature_lowChanged();
       emit currentChanged();
       _station_label->setText(station());
       _temperature_label->setText(temperature());
       QPixmap pixmap = QPixmap(icon());
//       pixmap=pixmap.scaled(QSize(128, 128), Qt::KeepAspectRatio);
       _icon->setPixmap(pixmap);
//       _icon->setPixmap(QPixmap(icon()));
       refreshwallpaper();           
	   
    };

public Q_SLOTS:
    void SetCurrentData(const QString &station, const QString &temperature, const QString &temperature_high, const QString &temperature_low, const QString &icon, 
                        const uint until_valid_time, bool current, bool lockscreen_param, const QString &last_update);
    void update_data();
    void refreshRequested();
    void updateWallpaperPath();
signals:
    void iconChanged();
    void stationChanged();
    void temperatureChanged();
    void temperature_highChanged();
    void temperature_lowChanged();
    void currentChanged();
};

class WeatherExtensionInterface : public MApplicationExtensionInterface
{
    Q_INTERFACES(MApplicationExtensionInterface)

public:
    virtual void weatherExtensionSpecificOperation() = 0;
};

Q_DECLARE_INTERFACE(WeatherExtensionInterface, "com.nokia.home.EventsExtensionInterface/1.0")


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
};

