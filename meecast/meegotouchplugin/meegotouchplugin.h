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
#include <QObject>
#include <MApplicationExtensionInterface>
#include <MGConfItem>
#include <QProcess>
#include <QDir>
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
    bool    _current;
    bool    _lockscreen;
    QTimer  *_timer;
    MGConfItem *_wallpaperItem;
    MGConfItem *_original_wallpaperItem;
    QString _wallpaper_path;

public:

    MyMWidget(){
      _stationname = "Unknown";
      _temperature = "";
      _temperature_low = "";
      _temperature_high = "";
	  _iconpath = "/opt/com.meecast.omweather/share/icons/Meecast/49.png";
      _current = false;
      _lockscreen = false;
      _timer = new QTimer(this);
      _timer->setSingleShot(true);
      connect(_timer, SIGNAL(timeout()), this, SLOT(update_data()));
      _wallpaperItem = new MGConfItem ("/desktop/meego/background/portrait/picture_filename"); 
      connect(_wallpaperItem, SIGNAL(valueChanged()), this, SLOT(updateWallpaperPath()));
      _original_wallpaperItem = new MGConfItem("/desktop/meego/background/portrait/picture_filename_original", this);
      if (!_wallpaperItem || _wallpaperItem->value() == QVariant::Invalid)
        _wallpaper_path = "/home/user/.wallpapers/wallpaper.png";
      else{
#if 0
          // Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "PreInit MyWidget ."<<_wallpaperItem->value().toString()<<".\n";
	    file.close();
	}
#endif
        if (!_original_wallpaperItem || _original_wallpaperItem->value() == QVariant::Invalid)
            _wallpaper_path = _wallpaperItem->value().toString();
        else{
            _wallpaper_path = _original_wallpaperItem->value().toString();
#if 0
            if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "PreInit2 MyWidget ."<<_wallpaper_path<<".\n";
	    file.close();
	        }
#endif
        }
        if (_wallpaper_path.indexOf("MeeCast",0) == -1){
            if (!_original_wallpaperItem || 
                _original_wallpaperItem->value() == QVariant::Invalid)
                _original_wallpaperItem->set(_wallpaper_path);
        }
      }
#if 0
    // Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "Init MyWidget ."<<_wallpaper_path<<".\n";
	    file.close();
	}
#endif
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
#if 0

	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "startpredeamon"<< _stationname<< "\n";
	    file.close();
	}
	// Debug end 
#endif
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
#if 0
        // Debug begin
        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "refreshview"<< " \n";
            file.close();
        }
        // Debug end 
#endif
       emit iconChanged();
       emit stationChanged();
       emit temperatureChanged();
       emit temperature_highChanged();
       emit temperature_lowChanged();
       emit currentChanged();
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

