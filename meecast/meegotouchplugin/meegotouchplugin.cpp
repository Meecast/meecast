/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of MeeCast (Other Maemo Weather(omweather)) - MeeCast
 *
 * Copyright (C) 2006-2015 Vlad Vasilyeu
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
#include "meegotouchplugin.h"
#include "dbusadaptor.h"
#include "weatherdataif.h"
#include <QThread>
#include <QColor>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QtCore>
#include <QtConcurrent>
#include <QTemporaryFile>

#include <iostream>


// Debug
#include <QFile>
#include <QTextStream>
#include <QDate>
#if 0
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#endif
using namespace QtConcurrent;

QTemporaryFile *tempfile;
void 
drawwallpaper(QImage image, QHash <QString, QString> hash){
    std::cerr<<" drawwallpaper"<<std::endl;

    QString temperature_hi = hash["temperature_hi"];
    QString temperature = hash["temperature"];
    QString temperature_low = hash["temperature_low"];
    QString lastupdate = hash["lastupdate"]; 
    QString stationname = hash["stationname"];
    QString iconpath = hash["iconpath"];
    
    /* Left corner */
    int x = 275;
    int y = 180;

    QSettings *lockscreen_settings;
    lockscreen_settings = new QSettings("/home/nemo/.config/harbour-meecast/lockscreen.conf",QSettings::NativeFormat); 
    QVariant v = lockscreen_settings->value("x_position", int(275));
    x = v.value<int>();
    v = lockscreen_settings->value("y_position", int(180));
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
        paint.setFont(QFont("Sail Sans Pro Light", 13));
    else
        if (stationname.length() > 18)
            paint.setFont(QFont("Sail Sans Pro Light", 15));
        else
            paint.setFont(QFont("Sail Sans Pro Light", 18));
    paint.drawText( x + 1, y, 196, 28, Qt::AlignHCenter, stationname.mid(0, 23));

    /* Temperature */
    paint.setFont(QFont("Droid Serif", 22));
    if (temperature == "N/A" || temperature == ""){
        QString temp_string = temperature_hi + QString::fromUtf8("°");
        paint.drawText(x + 10, y + 40, 60, 40, Qt::AlignHCenter, temp_string); 
        temp_string = temperature_low + QString::fromUtf8("°");
        paint.drawText(x + 10, y + 80, 60, 40, Qt::AlignHCenter, temp_string); 
    }else{
        if (hash["current"] == "TRUE")
            paint.setFont(QFont("Droid Serif Bold", 24));
        QString temp_string = temperature + QString::fromUtf8("°");
        paint.drawText(x + 10, y + 55, 60, 40, Qt::AlignHCenter, temp_string); 
    }


    /* Last update */
    paint.setFont(QFont("Droid Sans", 13));
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
    if (tempfile)
        delete tempfile;
    tempfile = new QTemporaryFile("/home/nemo/.cache/harbour-meecast/meecast.XXXXXXX.png");
    if (tempfile->open()){ 
        image.save(tempfile->fileName());
        std::cerr<<"Set new wallpaper "<<tempfile->fileName().toStdString().c_str()<<std::endl;
        MDConfItem  wallpaperItem("/desktop/jolla/background/portrait/home_picture_filename"); 
        wallpaperItem.set(tempfile->fileName());
    }
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

      _stationname = "Unknown";
      _temperature = "";
      _temperature_low = "";
      _temperature_high = "";
      _iconpath = "/usr/share/harbour-meecast/iconsets/Meecast/49.png";
      _current = false;
      _lockscreen = false;
      _standbyscreen = false;
      _timer = new QTimer(this);
      _timer->setSingleShot(true);
      _down = false;
      
    
      /* preparing for wallpaper widget */
      _wallpaperItem = new MDConfItem ("/desktop/jolla/background/portrait/home_picture_filename"); 
      connect(_wallpaperItem, SIGNAL(valueChanged()), this, SLOT(updateWallpaperPath()));
      if (!_wallpaperItem || _wallpaperItem->value() == QVariant::Invalid)
        _wallpaper_path = "/home/nemo/.wallpapers/wallpaper.png";
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
            _wallpaper_path = "/home/nemo/.cache/harbour-meecastr/wallpaper_MeeCast_original.png";
        }
      }
      _image = new QImage;
      _image->load(_wallpaper_path);
      if (_image->dotsPerMeterX() != 3780 || _image->dotsPerMeterY() != 3780 ){
        _image->setDotsPerMeterX(3780);
        _image->setDotsPerMeterY(3780);
      }
      if (_wallpaper_path.indexOf("MeeCast",0) == -1){
        _image->save("/home/nemo/.cache/harbour-meecast/wallpaper_MeeCast_original.png");
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

    std::cerr<<"MyMWidget::SetCurrentData"<<std::endl;
   if (lockscreen() && !lockscreen_param){
        this->current(current);
	    _wallpaperItem->set("/home/nemo/.cache/harbour-meecast/wallpaper_MeeCast_original.png");
   }
   if ((this->temperature() == temperature) &&
       (this->temperature_high() == temperature_high) &&
       (this->temperature_low() == temperature_low) &&
       (this->station() == station) && 
       (this->icon() == icon) &&
       (this->current() == current) &&
       (this->lockscreen() == lockscreen_param) &&
       (this->lastupdate() == last_update) &&
       (this->description() == description))
       return;

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
         std::cerr<<"Update Wallpaper Path "<<new_wallpaper_path.toStdString().c_str()<<std::endl;

        if (!(QFile::exists(new_wallpaper_path)))
            return;  
        if ( new_wallpaper_path.indexOf("meecast",0) == -1 && new_wallpaper_path.indexOf("MeeCast",0) == -1 && new_wallpaper_path != ""){
        
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

        std::cerr<<"refreshwallpaper"<<std::endl;
#if 0	    
	    // Debug begin
        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  "Start refreshwallpaper"<< " \n";
            file.close();
        }
#endif
        QDir dir("/home/nemo/.cache/harbour-meecast");
        
        if (!dir.exists())
            dir.mkpath("/home/nemo/.cache/harbour-meecast");

#if 0	    
	    // Debug begin
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  "Refreshwallpaper "<<_wallpaper_path<< " \n";
            file.close();
        }
#endif
        if (new_wallpaper){
            std::cerr<<"Save original file"<<std::endl;
            delete _image;
            _image = new QImage;
            _image->load(_wallpaper_path);
            if (_image->dotsPerMeterX() != 3780 || _image->dotsPerMeterY() != 3780 ){
                _image->setDotsPerMeterX(3780);
                _image->setDotsPerMeterY(3780);
            }
            _image->save("/home/nemo/.cache/harbour-meecast/wallpaper_MeeCast_original.png");
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

void
MyMWidget::ambiencedChanged(){
    std::cerr<<"ambiencedChanged()"<<std::endl;
}



int main (int argc, char *argv[]) {
    tempfile = NULL;

    QGuiApplication app(argc, argv);

    MDConfItem dconf(QString("/desktop/jolla/background/portrait/home_picture_filename"));

    std::cerr<<"22222 "<<std::endl;
    QString Value = dconf.value().toString();;
    std::cerr<<Value.toStdString().c_str()<<std::endl;

    std::cerr<<"4444 "<<std::endl;

    MyMWidget *box;
    box = new MyMWidget();

   /* D-BUS */
   new MeecastIf(box);
   new WeatherDataIf(box);
   QDBusConnection connection = QDBusConnection::sessionBus();
   bool ret = connection.registerService("com.meecast.applet");
   ret = connection.registerObject("/com/meecast/applet", box);

   QTimer::singleShot(1000, box, SLOT(refreshRequested()));

   ret = connection.registerService("com.meecast.data");
   ret = connection.registerObject("/com/meecast/data", box);
   //WeatherDataIf* data_client =  new WeatherDataIf("com.meecast.data", "/",
   //                                        QDBusConnection::sessionBus(), 0); 
   //QObject::connect(data_client, SIGNAL(GetCurrentWeather()), box, SLOT(refreshRequested()));  

   /* Copy wallpaper */
   if (!(QFile::exists("/home/nemo/.cache/harbour-meecast/wallpaper_MeeCast_original.png"))){
       QDir dir("/home/nemo/.cache/harbour-meecast");
       if (!dir.exists())
           dir.mkpath("/home/nemo/.cache/harbour-meecast");

       MGConfItem *wallpaperItem;
       wallpaperItem = new MGConfItem ("/desktop/jolla/background/portrait/home_picture_filename"); 

       if (QFile::exists(wallpaperItem->value().toString()))
           QFile::copy(wallpaperItem->value().toString(),
                   "/home/nemo/.cache/harbour-meecast/wallpaper_MeeCast_original.png"); 
   }



//    MApplication app(argc, argv);
//    MApplicationWindow window;
//    window.showFullScreen();
//    MApplicationPage page;
//    page.setPannable(false);
//    page.appear(MApplication::instance()->activeWindow());

//    WeatherApplicationExtension *WAE;
//    WAE = new WeatherApplicationExtension();
//    WAE->initialize("dddd");

//    page.setCentralWidget(WAE->widget());
    int result = app.exec();
    return result;
}
