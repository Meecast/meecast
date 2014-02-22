/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of MeeCast (Other Maemo Weather(omweather))
 *
 * Copyright (C) 2006-2012 Vlad Vasilyeu
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
#include <MContainer>
#include "meegotouchplugin.h"
#include <MLibrary>
#include "dbusadaptor.h"
#include "eventfeedif.h"
#include "weatherdataif.h"
#include <QThread>

// Debug
#include <QFile>
#include <QTextStream>
#include <QDate>

#if 0
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#endif

M_LIBRARY
Q_EXPORT_PLUGIN2(weatherextension, WeatherApplicationExtension)


void drawstandby(QHash <QString, QString> hash){

    QString temperature_hi = hash["temperature_hi"];
    QString temperature = hash["temperature"];
    QString temperature_low = hash["temperature_low"];
    QString lastupdate = hash["lastupdate"]; 
    QString stationname = hash["stationname"];
    QString iconpath = hash["iconpath"];
    
    QSettings *standby_settings;
    QColor color_font_station_name_standby;
    QColor color_font_temperature_standby;
    QColor color_font_current_temperature_standby;

    /* Init qsettings */
    standby_settings = new QSettings("/home/user/.config/com.meecast.omweather/standby.conf",QSettings::NativeFormat); 
    QVariant v = standby_settings->value("color_font_stationname", QColor(Qt::white));
    color_font_station_name_standby = v.value<QColor>();
    v = standby_settings->value("color_font_temperature", QColor(Qt::white));
    color_font_temperature_standby = v.value<QColor>();
    v = standby_settings->value("color_font_current_temperature", QColor(Qt::white));
    color_font_current_temperature_standby = v.value<QColor>();

    QImage *image;
    image = new QImage (QSize(120, 120), QImage::Format_RGB32);

    /* Left corner */
    int x = 0;
    int y = 0;

    QPainter paint;
    image->fill(Qt::black);
    paint.begin(image);
    QPen pen;

    /* set  color for station name */
    QColor myPenColor = QColor(color_font_station_name_standby);    pen.setColor(myPenColor);
    paint.setPen(pen);

    MGConfItem standbyItem("/desktop/meego/screen_lock/low_power_mode/operator_logo"); 
    standbyItem.set("/home/user/.cache/com.meecast.omweather/nologo.png");
    /* Station */
    paint.setFont(QFont("Nokia Pure", 12));
    // paint.setFont(QFont("Nokia Pure Light", 14));
    paint.drawText( x , y, 127, 21, Qt::AlignHCenter, stationname.mid(0,18));

    /* Icon */
    QPoint point(x + 50, y + 19);
    QImage icon;
    iconpath = "/opt/com.meecast.omweather/share/images/smallcontour/" +
                                 iconpath.right(iconpath.length() - iconpath.lastIndexOf("/") - 1);
    icon.load(iconpath);
    icon = icon.scaled(72, 72);
    paint.drawImage(point, icon); 
    
    /* Temperature */
    myPenColor = QColor(color_font_temperature_standby);    pen.setColor(myPenColor);
    paint.setPen(pen);
    paint.setFont(QFont("Nokia Pure", 20));
    if (temperature == "N/A" || temperature == ""){
               QString temp_string = temperature_hi + QString::fromUtf8("°");
               paint.drawText(x, y + 20, 60, 50, Qt::AlignHCenter, temp_string); 
               temp_string = temperature_low + QString::fromUtf8("°");
               paint.drawText(x, y + 55, 60, 50, Qt::AlignHCenter, temp_string); 
     }else{
   	    if (hash["current"] == "TRUE"){
   		    paint.setFont(QFont("Nokia Pure Bold", 21));
            myPenColor = QColor(color_font_current_temperature_standby);// set default color
            pen.setColor(myPenColor);
            paint.setPen(pen);
        }
        QString temp_string = temperature + QString::fromUtf8("°");
        paint.drawText(x, y + 35, 60, 48, Qt::AlignHCenter, temp_string); 
     }

     paint.end();

     image->save("/home/user/.cache/com.meecast.omweather/logo.png");
     delete image;
     standbyItem.set("/home/user/.cache/com.meecast.omweather/logo.png");

}
void drawwallpaper(QImage image, QHash <QString, QString> hash){

    QString temperature_hi = hash["temperature_hi"];
    QString temperature = hash["temperature"];
    QString temperature_low = hash["temperature_low"];
    QString lastupdate = hash["lastupdate"]; 
    QString stationname = hash["stationname"];
    QString iconpath = hash["iconpath"];
    
    /* Left corner */
    int x = 275;
    int y = 240;

    QSettings *lockscreen_settings;
    lockscreen_settings = new QSettings("/home/user/.config/com.meecast.omweather/lockscreen.conf",QSettings::NativeFormat); 
    QVariant v = lockscreen_settings->value("x_position", int(275));
    x = v.value<int>();
    v = lockscreen_settings->value("y_position", int(240));
    y = v.value<int>();

    
    QPainter paint;
    paint.begin(&image);
    QPen pen;
    QColor myPenColor = QColor(255, 255, 255, 128);// set default color
    pen.setColor(myPenColor);
    paint.setPen(pen);
    paint.setBrush(QColor(118, 118, 118, 50));
    
    /* Rect */
    paint.drawRoundedRect(x, y, 198, 160, 15.0, 15.0);

    myPenColor = QColor(255, 255, 255, 255);// set default color
    pen.setColor(myPenColor);
    paint.setPen(pen);

    /* Icon */
    QPoint point(x + 70, y + 21);
    QImage icon;
    icon.load(iconpath);
    paint.drawImage(point, icon.scaledToHeight(128, Qt::SmoothTransformation)); 
        
    /* Station */
    if (stationname.length() > 21)
        paint.setFont(QFont("Nokia Pure Light", 13));
    else
        if (stationname.length() > 18)
            paint.setFont(QFont("Nokia Pure Light", 15));
        else
            paint.setFont(QFont("Nokia Pure Light", 18));
    paint.drawText( x + 1, y, 196, 28, Qt::AlignHCenter, stationname.mid(0, 23));

    /* Temperature */
    paint.setFont(QFont("Nokia Pure", 22));
    if (temperature == "N/A" || temperature == ""){
        QString temp_string = temperature_hi + QString::fromUtf8("°");
        paint.drawText(x + 10, y + 40, 60, 40, Qt::AlignHCenter, temp_string); 
        temp_string = temperature_low + QString::fromUtf8("°");
        paint.drawText(x + 10, y + 80, 60, 40, Qt::AlignHCenter, temp_string); 
    }else{
        if (hash["current"] == "TRUE")
            paint.setFont(QFont("Nokia Pure Bold", 24));
        QString temp_string = temperature + QString::fromUtf8("°");
        paint.drawText(x + 10, y + 55, 60, 40, Qt::AlignHCenter, temp_string); 
    }

    /* Last update */
    paint.setFont(QFont("Nokia Pure", 13));
    paint.drawText(x + 10, y + 138, 170, 35, Qt::AlignHCenter, lastupdate); 

    paint.end();
#if 0
    // Debug begin
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out <<  "Refreshwallpaper paint has been finished\n";
        file.close();
    }
#endif

    image.save("/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast.png");
#if 0
    // Debug begin
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out <<  "Refreshwallpaper /home/user/.cache/com.meecast.omweather/wallpaper_MeeCast.png  saved \n";
        file.close();
    }
#endif

    MGConfItem  wallpaperItem("/desktop/meego/background/portrait/picture_filename"); 
    wallpaperItem.set("/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast_original.png");
    wallpaperItem.set("/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast.png");
#if 0
    // Debug begin
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out <<  "Stop refreshwallpaper"<< " \n";
        file.close();
    }
#endif
}

WeatherApplicationExtension::WeatherApplicationExtension() : box(0){
}

WeatherApplicationExtension::~WeatherApplicationExtension(){
    delete box;
}

void 
WeatherApplicationExtension::weatherExtensionSpecificOperation(){
    // do something specific to the demo extension interface
}
bool 
WeatherApplicationExtension::initialize(const QString &){
   QGraphicsObject* mWidget;

   /* ContextKit */
   QCoreApplication::setOrganizationDomain("meecast.omweather.com");
   QCoreApplication::setApplicationName("MeeCast");
   QValueSpace::initValueSpaceServer();


   box = new MyMWidget();

   /* D-BUS */
   new MeecastIf(box);
   new WeatherDataIf(box);
   QDBusConnection connection = QDBusConnection::sessionBus();
   bool ret = connection.registerService("com.meecast.applet");
   ret = connection.registerObject("/com/meecast/applet", box);
   //  signal sender=:1.9 -> dest=(null destination) serial=5824 path=/eventfeed; interface=com.nokia.home.EventFeed; member=refreshRequested
   EventFeedIf* client =  new EventFeedIf("com.nokia.home.EventFeed", "/eventfeed",
                                           QDBusConnection::sessionBus(), 0); 
   QObject::connect(client, SIGNAL(refreshRequested()), box, SLOT(refreshRequested()));  

   QTimer::singleShot(1000, box, SLOT(refreshRequested()));

   ret = connection.registerService("com.meecast.data");
   ret = connection.registerObject("/com/meecast/data", box);
   //WeatherDataIf* data_client =  new WeatherDataIf("com.meecast.data", "/",
   //                                        QDBusConnection::sessionBus(), 0); 
   //QObject::connect(data_client, SIGNAL(GetCurrentWeather()), box, SLOT(refreshRequested()));  

   /* Copy wallpaper */
   if (!(QFile::exists("/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast_original.png"))){
       QDir dir("/home/user/.cache/com.meecast.omweather");
       if (!dir.exists())
           dir.mkpath("/home/user/.cache/com.meecast.omweather");

       MGConfItem *wallpaperItem;
       wallpaperItem = new MGConfItem ("/desktop/meego/background/portrait/picture_filename"); 

       if (QFile::exists(wallpaperItem->value().toString()))
           QFile::copy(wallpaperItem->value().toString(),
                   "/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast_original.png"); 
   }
//   box->startpredeamon();
   return true;
}

MWidget *WeatherApplicationExtension::widget(){
    return box;
}
 

MyMWidget::MyMWidget(){

        
#if 0
    QFile file("/tmp/1.log");
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "Begin PreInit MyWidget ."<<".\n";
	    file.close();
	}
#endif
      publisher = new QValueSpacePublisher("Weather");

      _stationname = "Unknown";
      _temperature = "";
      _temperature_low = "";
      _temperature_high = "";
      _iconpath = "/opt/com.meecast.omweather/share/iconsets/Meecast/49.png";
      _current = false;
      _lockscreen = false;
      _standbyscreen = false;
      _timer = new QTimer(this);
      _timer->setSingleShot(true);
      _down = false;
      
      /* preparing for events widget */ 
      QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
      _events_image = new QImage (QSize(127, 96), QImage::Format_ARGB32);
      _events_image->load("/opt/com.meecast.omweather/share/iconsets/Meecast/49.png");
      *_events_image = _events_image->scaled(127, 96);
      _icon = new MImageWidget(_events_image);
      grabMouse();

      layout->addAnchor(layout, Qt::AnchorHorizontalCenter, _icon, Qt::AnchorHorizontalCenter);
      layout->setContentsMargins(1, 1, 1, 1);
      layout->setSpacing(0);
      setLayout(layout);
    
      /* preparing for standby screen */
      _standbyItem = new MGConfItem ("/desktop/meego/screen_lock/low_power_mode/operator_logo"); 
      connect(_standbyItem, SIGNAL(valueChanged()), this, SLOT(updateStandbyPath()));
      /* preparing for wallpaper widget */
      _wallpaperItem = new MGConfItem ("/desktop/meego/background/portrait/picture_filename"); 
      connect(_wallpaperItem, SIGNAL(valueChanged()), this, SLOT(updateWallpaperPath()));
      if (!_wallpaperItem || _wallpaperItem->value() == QVariant::Invalid)
        _wallpaper_path = "/home/user/.wallpapers/wallpaper.png";
      else{
#if 0
          // Debug begin
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "PreInit MyWidget ."<<_wallpaperItem->value().toString()<<".\n";
	    file.close();
	}
#endif
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
      }

      connect(_timer, SIGNAL(timeout()), this, SLOT(update_data()));
#if 0
    // Debug begin
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "Finish Init MyWidget ."<<_wallpaper_path<<".\n";
	    file.close();
	}
#endif
}
 
MyMWidget::~MyMWidget(){
    delete _timer;
    delete publisher;
}

QString 
MyMWidget::GetCurrentWeather(QString &temperature, QString &temperature_hi, QString &temperature_low, QString &icon, QString &description, bool &current, QString &last_update)
{
    temperature = this->temperature();
    temperature_hi = this->temperature_high();
    temperature_low = this->temperature_low();
    icon = this->icon();
    description = this->description();
    last_update = this->lastupdate();
    current = this->current();
    return  this->station();
}

void 
MyMWidget::SetCurrentData(const QString &station, const QString &temperature,
                          const QString &temperature_high, const QString &temperature_low,  
                          const QString &icon, const QString &description, const uint until_valid_time, bool current, bool lockscreen_param, bool standbyscreen_param, const QString &last_update){

   if (lockscreen() && !lockscreen_param){
        this->current(current);
	    _wallpaperItem->set("/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast_original.png");
   }
/* To do check standbyscreen */
   QDateTime utc_time;
   utc_time = QDateTime::currentDateTimeUtc();

   this->temperature(temperature);
   this->temperature_high(temperature_high);
   this->temperature_low(temperature_low);
   this->station(station);
   this->icon(icon);
   this->current(current);
   this->lockscreen(lockscreen_param);
   this->standbyscreen(standbyscreen_param);
   this->lastupdate(last_update);
   this->description(description);
   this->refreshview();

   /* ContexKit */

   publisher->setValue("Description", QString::fromUtf8(description.toUtf8())); 
   publisher->setValue("Station", QString::fromUtf8(station.toUtf8())); 
   publisher->setValue("Temperature", QVariant(temperature)); 
   publisher->setValue("HighTemperature", QString(temperature_high)); 
   publisher->setValue("LowTemperature", QString(temperature_low)); 
   publisher->setValue("CurrentWeather", QVariant(current)); 
   publisher->setValue("TimeUpdatingForecast", QString(last_update)); 
   publisher->setValue("IconPath", QString(icon)); 

   publisher->sync();

   if ((until_valid_time - utc_time.toTime_t()) > 0 && 
       (until_valid_time - utc_time.toTime_t()) < 12* 3600){
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out << station << " "<< publisher->path() <<" "<< QCoreApplication::organizationDomain() <<" "<<QCoreApplication::applicationName() <<" "<< QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "SetCurrentData next call "<< (until_valid_time - utc_time.toTime_t())<<"s "<<((until_valid_time - utc_time.toTime_t()+60)*1000)<<"ms\n";
	    file.close();
	}
	// Debug end 
#endif
        _timer->start(((until_valid_time - utc_time.toTime_t() + 60)*1000));
   }else{
#if 0
      // Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "SetCurrentData 36000000 Value: "<<  (until_valid_time - utc_time.toTime_t())<<"\n";
	    file.close();
	}
	// Debug end 
#endif
        _timer->start(3600000);
   }

}
void 
MyMWidget::refreshRequested(){
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "refreshRequested"<< "\n";
	    file.close();
	}
	// Debug end 
#endif
    this->startpredeamon();
}
void MyMWidget::update_data(){
#if 0
    // Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "update_data"<< "\n";
	    file.close();
	}
	// Debug end 
#endif
    QNetworkConfigurationManager m_network;
    if  (m_network.isOnline()){
        fprintf(stderr," Connection!!!!!!!!!!!!!!!\n");
        this->startpredeamon();
    }
}
void MyMWidget::updateStandbyPath(){
/* To Do make changed to path for MeeCast */
} 

void MyMWidget::updateWallpaperPath(){ 
#if 0
    // Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "updateWallpaperPath start"<< "\n";
	    file.close();
	}
	// Debug end 
#endif

   if (_wallpaperItem && _wallpaperItem->value() != QVariant::Invalid){
         QString new_wallpaper_path = _wallpaperItem->value().toString();

        if (!(QFile::exists(new_wallpaper_path)))
            return;  
        if (new_wallpaper_path.indexOf("MeeCast",0) == -1 && new_wallpaper_path != ""){
        
#if 0
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "New wallpaper path ."<<new_wallpaper_path<< ".\n";
	    file.close();
	    }
#endif
            _wallpaper_path = new_wallpaper_path;
            this->refreshwallpaper(true);
        }
    }

#if 0
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "updateWallpaperPath stop"<< "\n";
	    file.close();
	}
	// Debug end 
#endif
}

void MyMWidget::refreshwallpaper(bool new_wallpaper){

#if 0	    
	    // Debug begin
        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  "Start refreshwallpaper"<< " \n";
            file.close();
        }
#endif
        QDir dir("/home/user/.cache/com.meecast.omweather");
        
        if (!dir.exists())
            dir.mkpath("/home/user/.cache/com.meecast.omweather");


#if 0	    
	    // Debug begin
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  "Refreshwallpaper "<<_wallpaper_path<< " \n";
            file.close();
        }
#endif
        if (new_wallpaper){
            delete _image;
            _image = new QImage;
            _image->load(_wallpaper_path);
            if (_image->dotsPerMeterX() != 3780 || _image->dotsPerMeterY() != 3780 ){
                _image->setDotsPerMeterX(3780);
                _image->setDotsPerMeterY(3780);
            }
            _image->save("/home/user/.cache/com.meecast.omweather/wallpaper_MeeCast_original.png");
#if 0
            // Debug begin
            if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream out(&file);
                out <<  "Refreshwallpaper saved "<<_wallpaper_path<< " to original\n";
                file.close();
            }
#endif
        }

        if (!lockscreen())
            return;
#if 0
        // Debug begin
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  "Refreshwallpaper paint has been started\n";
            file.close();
        }
#endif
        QHash <QString, QString> hash;
        hash["temperature"] = _temperature;
        hash["temperature_low"] = _temperature_low;
        hash["temperature_hi"] = _temperature_high;
        hash["iconpath"] = _iconpath;
        hash["stationname"] = _stationname;
        hash["lastupdate"] = lastupdate();
        if (_current)
            hash["current"] = "TRUE";
        else
            hash["current"] = "FALSE";
        QFuture<void> f1 =  QtConcurrent::run(drawwallpaper, QImage(_image->copy()), QHash <QString, QString> (hash));
    }

void MyMWidget::refresheventswidget(){
    /* Left corner */
     int x = 0;
     int y = 0;

     QPainter paint;
     _events_image->fill(Qt::transparent);
     paint.begin(_events_image);
     QPen pen;

     QColor myPenColor = QColor(255, 255, 255, 255);// set default color
     pen.setColor(myPenColor);
     paint.setPen(pen);

	 /* Station */
	 paint.setFont(QFont("Nokia Pure", 12));
	 // paint.setFont(QFont("Nokia Pure Light", 14));
	 paint.drawText( x , y, 127, 21, Qt::AlignHCenter, _stationname.mid(0, 14));

     /* Icon */
     QPoint point(x + 50, y + 19);
     QImage icon;
     icon.load(_iconpath);
     icon = icon.scaled(72, 72);
     paint.drawImage(point, icon); 
        
	 /* Temperature */
	 paint.setFont(QFont("Nokia Pure", 20));
	 if (_temperature == "N/A" || _temperature == ""){
                QString temp_string = _temperature_high + QString::fromUtf8("°");
                paint.drawText(x, y + 20, 60, 50, Qt::AlignHCenter, temp_string); 
                temp_string = _temperature_low + QString::fromUtf8("°");
                paint.drawText(x, y + 55, 60, 50, Qt::AlignHCenter, temp_string); 
	  }else{
		 if (_current)
			paint.setFont(QFont("Nokia Pure Bold", 21));
         QString temp_string = _temperature + QString::fromUtf8("°");
	     paint.drawText(x, y + 35, 60, 48, Qt::AlignHCenter, temp_string); 
	  }

	  paint.end();
          _icon->setImage(*_events_image);

}

void MyMWidget::refreshstandby(){

    QHash <QString, QString> hash;
    hash["temperature"] = _temperature;
    hash["temperature_low"] = _temperature_low;
    hash["temperature_hi"] = _temperature_high;
    hash["iconpath"] = _iconpath;
    hash["stationname"] = _stationname;
    hash["lastupdate"] = lastupdate();
    if (_current)
        hash["current"] = "TRUE";
    else
        hash["current"] = "FALSE";
    QFuture<void> f1 =  QtConcurrent::run(drawstandby, QHash <QString, QString> (hash));

}

#if 0
int main (int argc, char *argv[]) {
    MApplication app(argc, argv);
    MApplicationWindow window;
    window.showFullScreen();
    MApplicationPage page;
    page.setPannable(false);
    page.appear(MApplication::instance()->activeWindow());

    WeatherApplicationExtension *WAE;
    WAE = new WeatherApplicationExtension();
    WAE->initialize("dddd");

    page.setCentralWidget(WAE->widget());
    int result = app.exec();
    return result;
}
#endif
