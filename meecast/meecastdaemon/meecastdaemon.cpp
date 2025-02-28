/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of MeeCast (Other Maemo Weather(omweather)) - MeeCast
 *
 * Copyright (C) 2006-2025 Vlad Vasilyeu
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
#include "meecastdaemon.h"
#include "dbusadaptor.h"
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
#include <signal.h>


// Debug
#include <QFile>
#include <QTextStream>
#include <QDate>

using namespace QtConcurrent;

QTemporaryFile *tempfile; /* file for new Wallpaper */
MyMWidget *box;
MeecastIf *meecastIf;

void 
drawwallpaper(QImage image, QHash <QString, QString> hash){

//    std::cerr<<" drawwallpaper"<<std::endl; 
    
#if 0
    QFile file("/tmp/1.log");
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "Begin drawwallpaper"<<".\n";
	    file.close();
	}
#endif


    QString temperature_hi = hash["temperature_hi"];
    QString temperature = hash["temperature"];
    QString temperature_low = hash["temperature_low"];
    QString lastupdate = hash["lastupdate"]; 
    QString stationname = hash["stationname"];
    QString iconpath = hash["iconpath"];
    
    /* Left corner */
    int x = 335;
    int y = 195;

    QSettings *lockscreen_settings;
    lockscreen_settings = new QSettings(QDir::homePath()+"/.config/org.meecast/MeeCast/lockscreen.conf",QSettings::NativeFormat);
    QVariant v = lockscreen_settings->value("x_position", int(x));
    x = v.value<int>();
    v = lockscreen_settings->value("y_position", int(y));
    y = v.value<int>();
    delete lockscreen_settings;

    
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

    QString path = QDir::homePath()+"/.cache/org.meecast/MeeCast/meecast." + QString::number(QDateTime::currentMSecsSinceEpoch()) + ".XXXXXXX.png";
    tempfile = new QTemporaryFile(path);
    if (tempfile->open()){ 
        image.save(tempfile->fileName());
//        std::cerr<<"Set new wallpaper "<<tempfile->fileName().toStdString().c_str()<<std::endl;
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
    _iconpath = "/usr/share/org.meecast/MeeCast/iconsets/Meecast/49.png";
    _current = false;
    _lockscreen = false;
    _standbyscreen = false;
    _lazyrenderingtimer = new QTimer(this);
    _lazyrenderingtimer->setSingleShot(true);
    _lazyupdatedatatimer = new QTimer(this);
    _lazyupdatedatatimer->setSingleShot(true);
    _down = false;
    _force_draw = true;
    _need_update = true;
    _image = NULL;
    
    /* preparing for wallpaper widget */
    _wallpaperItem = new MDConfItem ("/desktop/jolla/background/portrait/home_picture_filename"); 
    connect(_wallpaperItem, SIGNAL(valueChanged()), this, SLOT(updateWallpaperPath()));
    if (!_wallpaperItem || _wallpaperItem->value() == QVariant::Invalid)
        _wallpaper_path = QDir::homePath() + "/.cache/org.meecast/MeeCast/wallpaper_MeeCast_original.png";
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
        if (_wallpaper_path.indexOf("meecast",0) != -1){
#if 0
      // Debug begin
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
    QTextStream out(&file);
    out <<  "Set wallpaper path to " + QDir::homePath() + "/.cache/org.meecast/MeeCast/wallpaper_MeeCast_original.png ."<<".\n";
    file.close();
    }
#endif

            _wallpaper_path = QDir::homePath()+"/.cache/org.meecast/MeeCast/wallpaper_MeeCast_original.png";
        }
    }
    _image = new QImage;
    _image->load(_wallpaper_path);
    if (_image->dotsPerMeterX() != 3780 || _image->dotsPerMeterY() != 3780 ){
        _image->setDotsPerMeterX(3780);
        _image->setDotsPerMeterY(3780);
    }
    if (_wallpaper_path.indexOf("meecast",0) == -1){
#if 0
      // Debug begin
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
    QTextStream out(&file);
    out <<  "Save Image."<<".\n";
    file.close();
    }
#endif
        _image->save(QDir::homePath()+"/.cache/org.meecast/MeeCast/wallpaper_MeeCast_original.png");
    }

    connect(_lazyrenderingtimer, SIGNAL(timeout()), this, SLOT(refreshview()));
    connect(_lazyupdatedatatimer, SIGNAL(timeout()), this, SLOT(update_data()));


    // Network Manager Configutarion
    manager = new QNetworkConfigurationManager(this);
    connect(manager,SIGNAL(onlineStateChanged(bool)),
            this,SLOT(networkStatusChanged(bool)));

    if (manager->isOnline()) {
        this->_isOnline = true;
#if 0
    QFile file("/tmp/1.log");
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "Have connection!"<<".\n";
	    file.close();
	}
#endif

    }
    else {
        this->_isOnline = false;
#if 0
    QFile file("/tmp/1.log");
    if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  "Have no connection!"<<".\n";
	    file.close();
	}
#endif
    }


    keepalive = new BackgroundActivity(this);
    connect(keepalive, SIGNAL(running()), this, SLOT(checkActivity()));
    connect(keepalive, SIGNAL(stopped()), this, SLOT(wakeupStopped()));
    keepalive->setWakeupFrequency(BackgroundActivity::Range);
    _watcher = new QFileSystemWatcher();

    QFile watcher_file(QDir::homePath()+"/.cache/org.meecast/MeeCast/current.xml");
    if(!watcher_file.exists()){
        QDir dir(QDir::homePath()+"/.cache/org.meecast/MeeCast");
        if (!dir.exists())
            dir.mkpath(QDir::homePath()+"/.cache/org.meecast/MeeCast");
        //std::cerr<<"Create watcher file"<<std::endl;
        if (watcher_file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            watcher_file.close();
        }
    }
    _watcher->addPath(QDir::homePath()+"/.cache/org.meecast/MeeCast/current.xml");
    connect(_watcher,SIGNAL(fileChanged(QString)),this,SLOT(currentfileChanged(QString)));
    updateIntervalChanged(15*60);

    _watcher_config = new QFileSystemWatcher();

    QFile watcher_config_file(QDir::homePath()+"/.config/org.meecast/MeeCast/config.xml");
    if(!watcher_file.exists()){
        QDir dir(QDir::homePath()+"/.config/org.meecast/MeeCast");
        if (!dir.exists())
            dir.mkpath(QDir::homePath()+"/.config/org.meecast/MeeCast");
        //std::cerr<<"Create watcher file"<<std::endl;
        if (watcher_config_file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            watcher_config_file.close();
        }
    }
    _watcher_config->addPath(QDir::homePath()+"/.config/org.meecast/MeeCast/config.xml");
    connect(_watcher_config,SIGNAL(fileChanged(QString)),this,SLOT(configfileChanged(QString)));


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
//    std::cerr<<"MyMWidget::~MyMWidget() stop"<<std::endl;
    delete keepalive;
    delete _watcher;
    delete _watcher_config;
    delete manager;
    delete _lazyrenderingtimer;
    delete _lazyupdatedatatimer;
    delete _wallpaperItem; 
    if (_image)
        delete _image;
//    std::cerr<<"MyMWidget::~MyMWidget() end"<<std::endl;
}

void 
MyMWidget::SetCurrentData(const QString &station, const QString &temperature,
                          const QString &temperature_high, const QString &temperature_low,  
                          const QString &icon, const QString &description, const uint until_valid_time, bool current, bool lockscreen_param, bool standbyscreen_param, const QString &last_update){

   std::cerr<<"MyMWidget::SetCurrentData"<<std::endl;
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out << "MyMWidget::SetCurrentData "<<  "until_valid_time "<< until_valid_time <<"\n";
	    file.close();
	}
	// Debug end 
#endif


   if (lockscreen() && !lockscreen_param){
        setOriginalWallpaper();
   }
   /* Check similar data */
   if ((QString::compare(this->temperature(), temperature) == 0) &&
       (QString::compare(this->temperature_high(), temperature_high) == 0) &&
       (QString::compare(this->temperature_low(), temperature_low) == 0) &&
       (QString::compare(this->station(), station) == 0) && 
       (QString::compare(this->icon(), icon) == 0) &&
       (this->current() == current) &&
       (this->lockscreen() == lockscreen_param) &&
       (QString::compare(this->lastupdate(), last_update) == 0) &&
       (QString::compare(this->description(), description) == 0))
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

   //std::cerr<<"MyMWidget::SetCurrentData pre refreshview"<<std::endl; 
   //_lazyrenderingtimer->start(3000);
  // this->refreshview();
   _lazyupdatedatatimer->start(2000);

   if ((until_valid_time - utc_time.toTime_t()) > 0 && 
       (until_valid_time - utc_time.toTime_t()) < 12* 3600){
#if 0
	// Debug begin
//	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out << station << " "<<  QCoreApplication::organizationDomain() <<" "<<QCoreApplication::applicationName() <<" "<< QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "SetCurrentData next call "<< (until_valid_time - utc_time.toTime_t())<<"s "<<((until_valid_time - utc_time.toTime_t()+60)*1000)<<"ms\n";
	    file.close();
	}
	// Debug end 
#endif

        _next_time_for_check = until_valid_time;
        updateIntervalChanged((until_valid_time - utc_time.toTime_t() + 60));
   }else{
#if 0
      // Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "SetCurrentData Until Value: "<<  (until_valid_time - utc_time.toTime_t())<<"\n";
	    file.close();
	}
	// Debug end 
#endif
        _next_time_for_check = utc_time.toTime_t() + 3600 - 60;
        updateIntervalChanged(3600);
   }

}
void 
MyMWidget::refreshRequested(){
    
   std::cerr<<"MyMWidget::refreshRequested"<<std::endl;
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
    
   std::cerr<<"MyMWidget::update_data"<<std::endl;
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
    this->startpredeamon();
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
         //std::cerr<<"Update Wallpaper Path "<<new_wallpaper_path.toStdString().c_str()<<std::endl;

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
            _force_draw = true;
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

void 
MyMWidget::refreshwallpaper(bool new_wallpaper){

     std::cerr<<"refreshwallpaper"<<std::endl;
#if 0	    
	    // Debug begin
        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  "Start refreshwallpaper "<< " \n";
            file.close();
        }
#endif
        QDir dir(QDir::homePath()+"/.cache/org.meecast/MeeCast");
        
        if (!dir.exists())
            dir.mkpath(QDir::homePath()+"/.cache/org.meecast/MeeCast");

        if (new_wallpaper){
            //std::cerr<<"Save original file"<<std::endl;
            if (_image)
                delete _image;
            _image = new QImage;
            _image->load(_wallpaper_path);
            if (_image->dotsPerMeterX() != 3780 || _image->dotsPerMeterY() != 3780 ){
                _image->setDotsPerMeterX(3780);
                _image->setDotsPerMeterY(3780);
            }
            _image->save(QDir::homePath()+"/.cache/org.meecast/MeeCast/wallpaper_MeeCast_original.png");
        }
#if 0	    
	    // Debug begin
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<  "refreshwallpaper before check "<< " \n";
            file.close();
        }
#endif

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
MyMWidget::refreshview(){
//    std::cerr<<"MyMWidget::refreshview()"<<std::endl;
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
          refreshwallpaper();
};

void 
MyMWidget::startpredeamon(){

     std::cerr<<"startpredeamon()"<<std::endl; 
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "startpredeamon "<< _stationname<< "\n";
	    file.close();
	}
	// Debug end 
#endif
    QString executable("/usr/bin/meecast_predaemon"); 
    process.startDetached(executable);
}

void 
MyMWidget::intervalupdate(){
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "interval update "<< "\n";
	    file.close();
	}
	// Debug end 
#endif
}

void 
MyMWidget::checkActivity(){
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "MyMWidget::checkActivity "<< "\n";
	    file.close();
	}
	// Debug end 
#endif
    if (!this->_isOnline) 
        _need_update = true;
    else
        _need_update = false;
    update_data();
    keepalive->wait();
}

void 
MyMWidget::updateIntervalChanged(int interval){

//    std::cerr<<"MyMWidget::updateIntervalChanged "<<interval<<std::endl;
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "MyMWidget::updateIntervalChanged "<<interval<< "\n";
	    file.close();
	}
	// Debug end 
#endif

    keepalive->setWakeupRange(interval, interval);
    if (keepalive->state() == BackgroundActivity::Stopped) {
        keepalive->run();
    }
}

void 
MyMWidget::wakeupStopped(){
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "MyMWidget::wakeupStopped() "<< "\n";
	    file.close();
	}
	// Debug end 
#endif
}

void 
MyMWidget::setOriginalWallpaper(){
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << " setOriginalWallpaper()"<< "\n";
	    file.close();
	}
	// Debug end 
#endif

    _wallpaperItem->set(QDir::homePath()+"/.cache/org.meecast/MeeCast/wallpaper_MeeCast_original.png");
}

void
MyMWidget::configfileChanged(QString path){

    std::cerr<<"MyMWidget::configfileChanged"<<std::endl;
    update_data();
    keepalive->wait();
}

void 
MyMWidget::currentfileChanged(QString path){

    QString station;
    QString temperature;
    QString temperature_high; 
    QString temperature_low; 
    QString icon;
    QString description;
    uint until_valid_time; 
    bool current;
    bool lockscreen;
    QString last_update;
    int itemnumber = 0;

    temperature = this->temperature();
    temperature_high = this->temperature_high();
    temperature_low = this->temperature_low();
    station = this->station();
    icon = this->icon();
    current = this->current();
    last_update = this->lastupdate();
    description = this->description();
    lockscreen = this->lockscreen();


//    std::cerr<<"Watcher !!!!"<<std::endl;
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << " MyMWidget::currentfileChanged(QString path) "<< "\n";
	    file.close();
	}
	// Debug end 
#endif

    QFile current_file(QDir::homePath()+"/.cache/org.meecast/MeeCast/current.xml");

    if (current_file.size()<=0)
        return;
    if (current_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QXmlStreamReader xml(&current_file);
        this->current(false);
        while(!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            /* If token is just StartDocument, we'll go to next.*/
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if(token == QXmlStreamReader::StartElement) {
                if(xml.name() == "lockscreen") {
                    QString lockscreen_config = xml.readElementText();
                    if (lockscreen_config.contains("true", Qt::CaseInsensitive)){
                        this->lockscreen(true);
                    }else
                        this->lockscreen(false);
                    continue;
                }
                
                if(xml.name() == "station") {
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("name")){
                        this->station(attributes.value("name").toString());
                    } 
                    continue;
                }

                if(xml.name() == "period") {
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("current")){
                        if (attributes.value("current").toString() == "true") {
                            this->current(true);
                        }
                    } 
                    if(attributes.hasAttribute("itemnumber")){
                        itemnumber = attributes.value("itemnumber").toInt();  
                    } 

                    if(attributes.hasAttribute("end") && itemnumber == 1){
                        until_valid_time = attributes.value("end").toInt();  
                    } 
                    if (itemnumber == 1){
                        parsePeriod(xml, itemnumber); 
                    }
                    continue;
                }
                if(xml.name() == "last_update") {
                    this->lastupdate(xml.readElementText());
                }
            }
        }
        if(xml.hasError()) {
            std::cerr<<xml.errorString().toStdString().c_str()<<std::endl;
        }
        xml.clear();

    #if 0
        // Debug begin
        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<"Xml ended" <<"\n";
            file.close();
        }
        // Debug end 
    #endif

        /* Check for disabling weather on lockscreen */
        if (!this->lockscreen() && lockscreen){
            setOriginalWallpaper();
            return;
        }
    //    std::cerr<<"Xml ended"<< std::endl; 
        /* Check similar data */
        if ((this->temperature() == temperature) &&
            (this->temperature_high() == temperature_high) &&
            (this->temperature_low() == temperature_low) &&
            (this->station() == station) && 
            (this->icon() == icon) &&
            (this->current() == current) &&
            (this->lastupdate() == last_update) &&
            (this->description() == description)&&
            !_force_draw){
    #if 0
        // Debug begin
        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out <<"Simaliar data return!!!!!!" <<"\n";
            file.close();
        }
        // Debug end 
    #endif
            std::cerr<<"Simaliar data return!!!!!!"<< std::endl; 
            /* Simaliar data !!!!!! */
            return;
        }
        if (_force_draw)
            _force_draw = false;
        QDateTime utc_time;
        utc_time = QDateTime::currentDateTimeUtc();


        if ((until_valid_time - utc_time.toTime_t()) > 0 && 
           (until_valid_time - utc_time.toTime_t()) < 12* 3600){
    #if 0
        // Debug begin
//        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << station << " "<<  QCoreApplication::organizationDomain() <<" "<<QCoreApplication::applicationName() <<" "<< QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "SetCurrentData next call "<< (until_valid_time - utc_time.toTime_t())<<"s "<<((until_valid_time - utc_time.toTime_t()+60)*1000)<<"ms\n";
            out << station << " "<<  QCoreApplication::organizationDomain() <<" "<<QCoreApplication::applicationName() <<" "<< QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "utc_time "<<  utc_time.toTime_t()<<"s Until valid time "<<until_valid_time<<"\n";
            file.close();
        }
        // Debug end 
    #endif

            _next_time_for_check = until_valid_time;
            updateIntervalChanged((until_valid_time - utc_time.toTime_t() + 60));
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
            _next_time_for_check = utc_time.toTime_t() + 3600 - 60;
            updateIntervalChanged(3600);
        }

        _lazyrenderingtimer->start(2000);

    }else{
        std::cerr<<"Problem with current.xml file\n"<< std::endl;
    #if 0
        // Debug begin
        QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << "Problem with current file\n";
            file.close();
        }
        // Debug end 
    #endif
    }
    #if 0
        // Debug begin
        //QFile file("/tmp/1.log");
        if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << "Watcher End!!!\n";
            file.close();
        }
        // Debug end 
    #endif

//    std::cerr<<"Watcher End!!!!"<<std::endl;
}

void
MyMWidget::parsePeriod(QXmlStreamReader& xml, int itemnumber){

    std::cerr<<"ParsePeriod\n"<< std::endl;

    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "period") {
        return;
    }

    QXmlStreamAttributes attributes = xml.attributes();
    xml.readNext();
    QString temporary = xml.name().toString();
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "period")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            if(xml.name() == "temperature") {
                this->temperature(xml.readElementText());
            }
            if(xml.name() == "temperature_hi") {
                this->temperature_high(xml.readElementText());
            }
            if(xml.name() == "temperature_low") {
                this->temperature_low(xml.readElementText());
            }
            if(xml.name() == "icon") {
                this->icon(xml.readElementText());
            }
            if(xml.name() == "description") {
                this->description(xml.readElementText());
            }
        }
        xml.readNext();
    }

    //std::cerr<<"ParsePeriod end"<< std::endl;
}

void 
MyMWidget::networkStatusChanged(bool isOnline) {
    QString status = (isOnline) ? "Online" : "Offline";
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "Network connection changed:" << status<< "\n";
	    file.close();
	}
	// Debug end 
#endif

    this->_isOnline = isOnline;

    if (isOnline){
        if (_need_update){
            update_data();
        }
    }else{
    }
}

void 
MyMWidget::connectionActivated(){
    networkStatusChanged(true);
}

void 
MyMWidget::connectionDeactivated(){
    networkStatusChanged(false);
}

bool 
MyMWidget::isNetworkAvailable(){
    return _isOnline;
}




void 
signalhandler(int sig) {
    box->setOriginalWallpaper();
    delete meecastIf; 
    delete box;
    //std::cerr<<"signalhandler"<<std::endl;
    if (sig == SIGINT) {
        qApp->quit();
    }
    else if (sig == SIGTERM) {
        qApp->quit();
    }
}

int
main (int argc, char *argv[]) {
    tempfile = NULL;

    QGuiApplication app(argc, argv);
    box = new MyMWidget();

    /* D-BUS */
    meecastIf = new MeecastIf(box);
    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService("com.meecast.applet");
    ret = connection.registerObject("/com/meecast/applet", box);
 
    QTimer::singleShot(1000, box, SLOT(refreshRequested()));
 
    ret = connection.registerService("com.meecast.data");
    ret = connection.registerObject("/com/meecast/data", box);
 
    /* Copy wallpaper */
    if (!(QFile::exists(QDir::homePath()+"/.cache/org.meecast/MeeCast/wallpaper_MeeCast_original.png"))){
        QDir dir(QDir::homePath()+"/.cache/org.meecast/MeeCast");
        if (!dir.exists())
            dir.mkpath(QDir::homePath()+"/.cache/org.meecast/MeeCast");
 
        MGConfItem *wallpaperItem;
        wallpaperItem = new MGConfItem ("/desktop/jolla/background/portrait/home_picture_filename"); 
 
        if (QFile::exists(wallpaperItem->value().toString()))
            QFile::copy(wallpaperItem->value().toString(),
                    QDir::homePath()+"/.cache/org.meecast/MeeCast/wallpaper_MeeCast_original.png");
    }
    signal(SIGINT, signalhandler);
    signal(SIGTERM, signalhandler);

    int result = app.exec();
    return result;
}
