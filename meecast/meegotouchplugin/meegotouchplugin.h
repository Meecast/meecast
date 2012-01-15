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
    QTimer  *_timer;
public:

    MyMWidget(){
      _stationname = "Unknown";
      _temperature = "";
      _temperature_low = "";
      _temperature_high = "";
	  _iconpath = "/opt/com.meecast.omweather/share/icons/Meecast/49.png";
      _current = false;
      _timer = new QTimer(this);
      _timer->setSingleShot(true);
      connect(_timer, SIGNAL(timeout()), this, SLOT(update_data()));
    };

    ~MyMWidget(){
      delete _timer;
    };

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
           
	   
	    /* Left corner */
	    int x = 275;
	    int y = 230;

	    QString wallpaper_path;
            MGConfItem *wallpaperItem = new MGConfItem("/desktop/meego/background/portrait/picture_filename", this);
	    if (!wallpaperItem || wallpaperItem->value() == QVariant::Invalid)
	        wallpaper_path = "";
            else
		wallpaper_path = wallpaperItem->value().toString();
            
	    if (wallpaper_path.indexOf("MeeCast",0) != -1)
                wallpaper_path = "/home/user/.wallpapers/wallpaper.png";

	    QPainter paint;
	    QImage image;

	    image.load(wallpaper_path);

	    //image.load("/home/user/.wallpapers/wallpaper.png");
	    paint.begin(&image);
	    QPen pen;
	    QColor myPenColor = QColor(255, 255, 255, 128);// set default color
	    pen.setColor(myPenColor);
	    paint.setPen(pen);
	    paint.setBrush(QColor(118, 118, 118, 50));
	    
	    /* Rect */
	    //paint.drawRect(300,380, 178, 140);
	    paint.drawRoundedRect(x, y, 198, 140, 15.0, 15.0);

	    myPenColor = QColor(255, 255, 255, 255);// set default color
	    pen.setColor(myPenColor);
	    paint.setPen(pen);

	    /* Icon */
	    QPoint point(x + 60, y + 21);
	    QImage icon;
	    icon.load(_iconpath);
	    paint.drawImage(point, icon); 
		    
	    /* Station */
	    paint.setFont(QFont("Arial", 18));
	    paint.drawText( x + 1, y, 196, 28, Qt::AlignHCenter, _stationname);

	    /* Temperature */
	    paint.setFont(QFont("Arial", 22));
	    if (_temperature == "N/A" || _temperature == ""){
		paint.drawText(x + 10, y + 40, 60, 30, Qt::AlignHCenter, _temperature_high + '°'); 
		paint.drawText(x + 10, y + 80, 60, 30, Qt::AlignHCenter, _temperature_low + '°'); 
	    }else{
		if (_current)
			paint.setFont(QFont("Arial Bold", 24));
		paint.drawText(x + 10, y + 50, 40, 30, Qt::AlignHCenter, _temperature + '°'); 
	    }
	    paint.end();
	    image.save("/home/user/.wallpapers/wallpaper_MeeCast.png");
	    MGConfItem *newwallpaperItem = new MGConfItem("/desktop/meego/background/portrait/picture_filename", this);
	    newwallpaperItem->set("/home/user/.wallpapers/wallpaper.png");
	    newwallpaperItem->set("/home/user/.wallpapers/wallpaper_MeeCast.png");
	    //delete newwallpaperItem;
	    //delete wallpaperItem;

    };

public Q_SLOTS:
    void SetCurrentData(const QString &station, const QString &temperature, const QString &temperature_high, const QString &temperature_low,  const QString &icon, const uint until_valid_time, bool current);
    void update_data();
    void refreshRequested();

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

