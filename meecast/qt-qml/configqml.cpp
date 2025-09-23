/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2025 Vlad Vasilyeu
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


#include "configqml.h"

ConfigQml* ConfigQml::_self;
int ConfigQml::_refcount_;


//ConfigQml::ConfigQml():QObject(),Core::Config("config.xml", "../core/data/config.xsd"){}
ConfigQml::ConfigQml(const std::string& filename, const std::string& schema_filename, QGuiApplication *app):QObject(),Core::Config(filename, schema_filename){
    std::cerr<<"CONFIG CREATEQML11111!!!!!!!!!!!!!!"<<std::endl;
    _refcount_ = 0;
    _app = app;
    init();
}

ConfigQml::ConfigQml():QObject(),Core::Config(){
    std::cerr<<"CONFIG CREATEQML22222!!!!!!!!!!!!!!"<<std::endl;
    _refcount_ = 0;
    init();
}

ConfigQml::~ConfigQml(){
#if 0
    if (standby_settings)
        delete standby_settings;
    if (lockscreen_settings)
        delete lockscreen_settings;
#endif
    if (db)
        delete db;
    if (thread)
        delete thread;
    if (networkingcontrol){
        networkingcontrol->disconnectSession();
        delete networkingcontrol;
    }
}

ConfigQml* 
ConfigQml::Instance(){
    if (!_self)
        _self = new ConfigQml();
    _refcount_++;
    std::cerr<<"Refcount ConfigQML1: "<<_refcount_<<std::endl;
    return _self;
}

ConfigQml* 
ConfigQml::Instance(const std::string& filename, const std::string& schema_filename, QGuiApplication *app){
    if (!_self)
        _self = new ConfigQml(filename, schema_filename, app);
    _refcount_++;
    std::cerr<<"Refcount ConfigQML2: "<<_refcount_<<std::endl;
    return _self;
}

////////////////////////////////////////////////////////////////////////////////
bool 
ConfigQml::DeleteInstance(){
    std::cerr<<"ConfigQml::DeleteInstance() Refcount ConfigQML: "<<_refcount_<<std::endl;
    if (_refcount_ >1){
        _refcount_ --;
        return false;
    }
    if (_refcount_ == 1  && _self){
        delete _self;
        _self = 0;
        return true;
    }
    return false;
}

void
ConfigQml::init(){
    int index;
    _time_for_updating = 0;
    _updating = false;
    need_updating = false;
    db = new Core::DatabaseSqlite("");

#if 0
    /* setting for stndbyscreen */
    standby_settings = new QSettings("/home/user/.config/com.meecast.omweather/standby.conf",QSettings::NativeFormat); 
    QVariant v = standby_settings->value("color_font_stationname", QColor(Qt::white));
    _standby_color_font_stationname = v.value<QColor>();
     v = standby_settings->value("color_font_temperature", QColor(Qt::white));
    _standby_color_font_temperature = v.value<QColor>();
     v = standby_settings->value("color_font_current_temperature", QColor(Qt::green));
    _standby_color_font_current_temperature = v.value<QColor>();
    /* setting for lockscreen */
   lockscreen_settings = new QSettings("/home/user/.config/com.meecast.omweather/lockscreen.conf",QSettings::NativeFormat); 
    v = lockscreen_settings->value("x_position", int(275));
    _lockscreen_x_position = v.value<int>();
    v = lockscreen_settings->value("y_position", int(240));
    _lockscreen_y_position = v.value<int>();
#endif
    thread = new UpdateThread();
    connect(thread, SIGNAL(finished()), this, SLOT(downloadFinishedSlot()));


    wind_list << "m/s" << "km/h" << "mph" << "kn" << "Beaufort scale";
    press_list << "mbar" << "kPa" << "mmHg";
    vis_list << "m" << "km" << "mi";
    _gps = NULL;
    if (gps()){
         
         std::cerr<<"ConfigQml::init() GPS!!!!"<<std::endl;
        _gps = new GpsPosition();
        _gps->startGps(); 
        _gps->startTimer();
        connect(_gps, SIGNAL(findCoord(double, double)), this, SLOT(addGpsStation(double, double)));
        // if gps station exist, find it coordinates
        index = getGpsStation();
        if (index > -1){
            // get index station coordinates
            Core::DatabaseSqlite *db_w = new Core::DatabaseSqlite("");
            double lat, lon;
            std::string path(Core::AbstractConfig::prefix);
            path += Core::AbstractConfig::sharePath;
            path += "db/";
            QString filename = "weather.com";
            filename.append(".db");
            filename.prepend(path.c_str());
            qDebug() << "file name = " << filename;
            db_w->set_databasename(filename.toStdString());
            if (!db_w->open_database()){
                qDebug() << "error open database";
                return;
            }
            qDebug() << "gps station name = " << QString::fromStdString(stationsList().at(index)->id());
            db_w->get_station_coordinate(stationsList().at(index)->id(), lat, lon);
            _gps->setLastCoordinates(lat, lon);
            delete db_w;
        }
    }
    setLanguage();
#if 0 
    if (QFile::exists("/home/user/.cache/com.meecast.omweather/splash.png")){
        /* Check file size */
        QFile f1("/home/user/.cache/com.meecast.omweather/splash.png");
        qint64 size1 = f1.size();
        QFile f2("/opt/com.meecast.omweather/share/images/splash.png");
        qint64 size2 = f2.size();

        if (f1.size() != f2.size()){
            QFile::remove("/home/user/.cache/com.meecast.omweather/splash.png"); 
            /* Copy splash to cache directory */
            QFile::copy("/opt/com.meecast.omweather/share/images/splash.png",
                    "/home/user/.cache/com.meecast.omweather/splash.png");
        }
    }
#endif
}

void
ConfigQml::setLanguage(){
    std::cout<<"setLanguage"<<std::endl;
    /* Locale */
    // Set up the translator.
    //QTranslator translator;
    QString locale_string = QLocale().system().name();
    QString filename = QString("omweather_%1").arg(locale_string);
    //std::cerr<<"Filename0: "<<filename.toStdString().c_str()<<std::endl; 

    QString localepath = QString::fromStdString(Core::AbstractConfig::prefix + "/share/harbour-meecast/locale");
    if (!_translator.isEmpty()){
          QCoreApplication::removeTranslator(&_translator);
    }
    if (_translator.load(filename, localepath)) {
        /* std::cerr<<"Success TR"<<std::endl; */
        _app->installTranslator(&_translator);
    }

//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    for (unsigned int i=1; i<languagesList().size(); i++){
        if (languagesList().at(i).first == Language()){
            QLocale::setDefault(QLocale(languagesList().at(i).second.c_str()));
            filename = QString("omweather_%1").arg(languagesList().at(i).second.c_str());
            std::cerr<<filename.toStdString().c_str()<<std::endl;
            QString localepath = QString::fromStdString(Core::AbstractConfig::prefix + "/share/harbour-meecast/locale");
            //std::cerr<<"Filename1: "<<localepath.toStdString().c_str()<<std::endl;
            if (_translator.load(filename, localepath)) {
                    std::cerr<<"Success TR"<<std::endl;
                    _app->installTranslator(&_translator);
            }
        }
    }
}

void 
ConfigQml::onNetworkSessionError(){

    std::cerr<<"ConfigQml::onNetworkSessionError()"<<std::endl;
    if(networkingcontrol){
        // Disconnect all slots connected to the network manager
       disconnect(networkingcontrol, SIGNAL(connectionSuccess()),
                   this, SLOT(onNetworkSessionOpened()));
       disconnect(networkingcontrol, SIGNAL(connectionError()),
                    this, SLOT(onNetworkSessionError()));
        networkingcontrol->disconnectSession();
    }
}
void 
ConfigQml::onNetworkSessionOpened(){
    std::cerr<<"ConfigQml::onNetworkSessionOpened()"<<std::endl;
    this->_updating = true;
    thread->start();
    /*
    if(networkingcontrol){
        // Disconnect all slots connected to the network manager
        disconnect(networkingcontrol, SIGNAL(connectionSuccess()),
                    this, SLOT(onNetworkSessionOpened()));
        disconnect(networkingcontrol, SIGNAL(connectionError()),
                    this, SLOT(onNetworkSessionError()));
    }
    */
}



bool
ConfigQml::isOnline(){
    if (networkingcontrol){
        return networkingcontrol->isOnline();
    }else
        return true;
}

void
ConfigQml::connectSession(bool background){
    if (networkingcontrol){
        networkingcontrol->connectSession(background);
    }
}


void
ConfigQml::saveConfig()
{
    networkingcontrol = new NetworkingControl(this);
    connect(networkingcontrol, SIGNAL(connectionSuccess()),
                    SLOT(onNetworkSessionOpened()), Qt::QueuedConnection);
    connect(networkingcontrol, SIGNAL(connectionError()),
                   SLOT(onNetworkSessionError()), Qt::QueuedConnection);
    connect(networkingcontrol, SIGNAL(connectionError()),
                   SLOT(onNetworkSessionError()), Qt::QueuedConnection);
//    connect(networkongcontrol, SIGNAL(valueChanged(bool)),
//                        this, SLOT(onInternetStateChanged(bool)) /*, Qt::QueuedConnection*/);


#if 0
    standby_settings->setValue("color_font_stationname", _standby_color_font_stationname);
    standby_settings->setValue("color_font_temperature", _standby_color_font_temperature);
    standby_settings->setValue("color_font_current_temperature", _standby_color_font_current_temperature);
    standby_settings->sync();
    lockscreen_settings->setValue("x_position", _lockscreen_x_position);
    lockscreen_settings->setValue("y_position", _lockscreen_y_position);
    lockscreen_settings->sync();
#endif
    try{
        Core::Config::saveConfig();
    }catch (std::exception &ex){
        std::cerr<<"Error in Save Config: "<< ex.what() <<std::endl;
        emit errorInConfig("Can't save config file");
    }
}

QString
ConfigQml::iconset(){
    QString c;
    c = ConfigQml::Config::iconSet().c_str();
    /* std::cerr<<"ConfigQml::iconset()"<< c.toStdString().c_str()<<std::endl; */
    return c;
}
QStringList
ConfigQml::icon_list(){
    QStringList icon_list;
    Dirent *dp = 0;
    DIR *dir_fd = opendir(iconspath().toStdString().c_str());
    if(dir_fd){
        while((dp = readdir(dir_fd))){
            std::string name = dp->d_name;
            if(name == "." || name == "..")
                continue;
            if(dp->d_type == DT_DIR && name[0] != '.'){
                try{
                    icon_list << QString::fromStdString(name);
                }
                catch(std::string& err){
                    std::cerr << "error " << err << std::endl;
                    continue;
                }
                catch(const char *err){
                    std::cerr << "error " << err << std::endl;
                    continue;
                }
            }
        }
        closedir(dir_fd);
    }
    return icon_list;
}

void
ConfigQml::set_iconset(QString c){
    ConfigQml::Config::iconSet(c.toStdString());
    saveConfig();
    refreshconfig();
}
void
ConfigQml::set_language(QString c){
    std::cerr<<"ConfigQml::set_language"<<std::endl;
    ConfigQml::Config::Language(c.toStdString());
    /* Set new locale for application */
    for (unsigned int i=1; i<languagesList().size(); i++){
        if (languagesList().at(i).first == Language()){
            std::cerr<<"ConfigQml::set_language - "<<languagesList().at(i).second.c_str() <<std::endl;
            setlocale (LC_ALL, languagesList().at(i).second.c_str());
            setlocale (LC_MESSAGES, languagesList().at(i).second.c_str());
            QLocale::setDefault(QLocale(languagesList().at(i).second.c_str()));
        }
    }

    saveConfig();
    setLanguage();
    refreshconfig();
}

QString
ConfigQml::iconspath(){
    return ConfigQml::Config::iconspath().c_str();
}

QString
ConfigQml::imagespath(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("images");
    return c;
}

QString
ConfigQml::screenorientation(){
    return ConfigQml::Config::screenOrientation().c_str();
}

QString
ConfigQml::iconsbutton(){
    QString c;
    c = ConfigQml::Config::prefix_path().c_str();
    c.append("buttons_icons");
    return c;
}

QString
ConfigQml::temperatureunit(){
    QString c;
    c = ConfigQml::Config::TemperatureUnit().c_str();
    return c;
}

QStringList ConfigQml::temperature_list(){
    QStringList l;
    l << "C" << "F";
    return l;
}

void ConfigQml::temperature_unit(QString c){
    ConfigQml::Config::TemperatureUnit(c.toStdString());
    saveConfig();
    refreshconfig();
}

QString
ConfigQml::windspeedunit(){
    QString c;
    //c = QString(QString::fromUtf8(_(ConfigQml::Config::WindSpeedUnit().c_str())));
    c = ConfigQml::Config::WindSpeedUnit().c_str();
    return c;
}

QStringList 
ConfigQml::windspeed_list(){
    QStringList l;
    for (int i=0; i < wind_list.size(); i++){
        l.append(QString(QString::fromUtf8(_(wind_list.at(i).toStdString().c_str()))));
    }
    return l;
}

QStringList 
ConfigQml::languages_list(){
    QStringList l;
    for (unsigned int i=0; i<languagesList().size(); i++){
        l << QString::fromUtf8(languagesList().at(i).first.c_str());
    }
    return l;
}

int 
ConfigQml::index_of_current_language(){
    unsigned int i;
    unsigned result = 0;
    for (i=0; i<languagesList().size(); i++){
        if (languagesList().at(i).first.c_str() == Language() ){
            result = i;    
            break;
        }
    }
    return result;
}

void 
ConfigQml::windspeed_unit(int index){
    ConfigQml::Config::WindSpeedUnit(wind_list.at(index).toStdString());
    saveConfig();
    refreshconfig();
}

QString
ConfigQml::pressureunit(){
    QString c;
    //c = QString(QString::fromUtf8(_(ConfigQml::Config::WindSpeedUnit().c_str())));
    c = ConfigQml::Config::PressureUnit().c_str();
    return c;
}

QString
ConfigQml::visibleunit(){
    QString c;
    //c = QString(QString::fromUtf8(_(ConfigQml::Config::WindSpeedUnit().c_str())));
    c = ConfigQml::Config::VisibleUnit().c_str();
    return c;
}

QStringList
ConfigQml::pressure_list(){
    QStringList l;
    for (int i=0; i < press_list.size(); i++){
        l.append(QString(QString::fromUtf8(_(press_list.at(i).toStdString().c_str()))));
    }
    return l;
}

QStringList
ConfigQml::visible_list(){
    QStringList l;
    for (int i=0; i < vis_list.size(); i++){
        l.append(QString(QString::fromUtf8(_(vis_list.at(i).toStdString().c_str()))));
    }
    return l;
}

void
ConfigQml::pressure_unit(int index){
    ConfigQml::Config::PressureUnit(press_list.at(index).toStdString());
    saveConfig();
    refreshconfig();
}

void
ConfigQml::visible_unit(int index){
    ConfigQml::Config::VisibleUnit(vis_list.at(index).toStdString());
    saveConfig();
    refreshconfig();
}

bool
ConfigQml::fullscreen(){
    return ConfigQml::Config::Fullscreen();
}

void
ConfigQml::setfullscreen(bool c){
    ConfigQml::Config::Fullscreen(c);
    saveConfig();
    refreshconfig();
}

bool
ConfigQml::lockscreen(){
    return ConfigQml::Config::Lockscreen();
}

bool
ConfigQml::setlockscreen(bool c){

    if (!c){
        ConfigQml::Config::Lockscreen(c);
        saveConfig();
        if (QFile::exists("/usr/bin/meecastd")){
            refreshconfig();
        }
        return true;
    }else{
        if (QFile::exists("/usr/bin/meecastd")){
            ConfigQml::Config::Lockscreen(c);
            saveConfig();
            refreshconfig();
            return true;
        }else{
            ConfigQml::Config::Lockscreen(false);
            saveConfig();
            return false;
        }
    }
}

bool
ConfigQml::standbyscreen(){
    return ConfigQml::Config::Standbyscreen();
}

void
ConfigQml::setstandbyscreen(bool c){
#if 0
    ConfigQml::Config::Standbyscreen(c);
    if (!c && (QFile::exists("/home/user/.cache/com.meecast.omweather/logo.png")))
       QFile::remove("/home/user/.cache/com.meecast.omweather/logo.png"); 
#ifdef MEEGO_EDITION_HARMATTAN
    MGConfItem standbyItem("/desktop/meego/screen_lock/low_power_mode/operator_logo");
    standbyItem.set("/home/user/.cache/com.meecast.omweather/nologo.png");
#endif
    saveConfig();
    refreshconfig();
#endif
}

bool
ConfigQml::eventwidget(){
    bool result = false;
#if 0
    QFile file("/usr/share/meegotouch/applicationextensions/events-weather.desktop");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        QString  line = in.readAll();
        file.close();
	    if (line.indexOf("meecast") == -1)
	        result = false;
        else
	        result = true;
    }else
        result = false;
    if (result == false){
        setlockscreen(false);
        setstandbyscreen(false);
     }
#endif
    return result;

}
void
ConfigQml::seteventwidget(bool c)
{    
#if 0
    QFile file("/usr/share/meegotouch/applicationextensions/events-weather.desktop");
    if (!c){
    	QDesktopServices::openUrl(QUrl("file:///usr/bin/installui"));
        setlockscreen(false);
        setstandbyscreen(false);
    }
    QFile file("/usr/share/meegotouch/applicationextensions/events-weather.desktop");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        QString  line = in.readAll();
        file.close();
        if (line.indexOf("meecast") != -1)
	        return;
    }

    if (c)
    	QDesktopServices::openUrl(QUrl("file:///opt/com.meecast.omweather/share/packages/meecast-applet-enable_0.2_all.deb"));
    
#endif
}

bool
ConfigQml::splash(){
#if 0
    if (QFile::exists("/home/user/.cache/com.meecast.omweather/splash.png"))
        return true;
    else
        return false;
#endif
    return false;
}

void
ConfigQml::setsplash(bool c){
#if 0
    ConfigQml::Config::Splash(c);
    saveConfig();
    refreshconfig();
    if (!c && (QFile::exists("/home/user/.cache/com.meecast.omweather/splash.png")))
        QFile::remove("/home/user/.cache/com.meecast.omweather/splash.png"); 

    if (c){
       /* Check directory */
        QDir dir("/home/user/.cache/com.meecast.omweather");
        if (!dir.exists())
            dir.mkpath("/home/user/.cache/com.meecast.omweather");
        /* Copy splash to cache directory */
        QFile::copy("/opt/com.meecast.omweather/share/images/splash.png",
                    "/home/user/.cache/com.meecast.omweather/splash.png");
    }
#endif
}

bool
ConfigQml::logocoverpage(){
    return ConfigQml::Config::LogoOnCover();
}

void
ConfigQml::setlogocoverpage(bool c){
    ConfigQml::Config::LogoOnCover(c);
    saveConfig();
    refreshconfig();
}

void
ConfigQml::settransparency(bool c){
    ConfigQml::Config::Transparency(c);
    saveConfig();
    refreshconfig();
}

bool
ConfigQml::windcoverpage(){
    return ConfigQml::Config::WindOnCover();
}

bool
ConfigQml::transparency(){
    return ConfigQml::Config::Transparency();
}

void
ConfigQml::setwindcoverpage(bool c){
    ConfigQml::Config::WindOnCover(c);
    saveConfig();
    refreshconfig();
}

bool
ConfigQml::updating(){
    return _updating;
}

void
ConfigQml::updating(bool c){
     _updating = c;
}

bool
ConfigQml::lastupdatecoverpage(){
    return ConfigQml::Config::LastUpdateOnCover();
}

void
ConfigQml::setlastupdatecoverpage(bool c){
    ConfigQml::Config::LastUpdateOnCover(c);
    saveConfig();
    refreshconfig();
}

bool
ConfigQml::gps(){
    return ConfigQml::Config::Gps();
}

void
ConfigQml::setgps(bool c){
    /* if gps option changed */
    if (c == ConfigQml::Config::Gps())
        return;
    if (c)
        enableGps();
    else {
        if (_gps){
            delete _gps;
            _gps = NULL;
        }
        /* find exist gps station */
        int index = getGpsStation();
        if (index > -1){
            /* delete gps station */
            removeStation(index);
        }
    }
    ConfigQml::Config::Gps(c);
    saveConfig();
    refreshconfig2();
}

QString
ConfigQml::version(){
    QString c;
    c = QString(MEECAST_VERSION);
    return c;
}


QColor
ConfigQml::fontcolor(){
    QColor c;
    c.setNamedColor(ConfigQml::Config::FontColor().c_str());
    return c;
}

QColor
ConfigQml::standby_color_font_stationname(){
    return _standby_color_font_stationname;
}

int
ConfigQml::lock_screen_x_position(){
    return _lockscreen_x_position;
}

int
ConfigQml::lock_screen_y_position(){
    return _lockscreen_y_position;
}

void
ConfigQml::set_standby_color_font_stationname(QColor c)
{   
    _standby_color_font_stationname = c;
    saveConfig();
}
QColor
ConfigQml::standby_color_font_temperature(){
    return _standby_color_font_temperature;
}

void
ConfigQml::set_standby_color_font_temperature(QColor c){   
    _standby_color_font_temperature = c;
    saveConfig();
}

QColor
ConfigQml::standby_color_font_current_temperature(){
    return _standby_color_font_current_temperature;
}

void
ConfigQml::set_standby_color_font_current_temperature(QColor c){   
    _standby_color_font_current_temperature = c;
    saveConfig();
}

void
ConfigQml::set_lock_screen_x_position(int x){   
    _lockscreen_x_position = x;
    saveConfig();
}

void
ConfigQml::set_lock_screen_y_position(int y){   
    _lockscreen_y_position = y;
    saveConfig();
    refreshconfig();
}

QStringList
ConfigQml::stations(){
    QStringList l;
    for (unsigned int i=0; i<stationsList().size(); i++){
        l << QString::fromUtf8(stationsList().at(i)->name().c_str());
    }
    return l;
}

void
ConfigQml::removeStation(int index){
    stationsList().erase(stationsList().begin() + index);
    //stationlist->erase(stationlist->begin()+index);
    //ConfigQml::Config::stationsList(*stationlist);
    if (this->stationsList().size() > 0){
        this->current_station_id(0);
    }
    saveConfig();
    refreshconfig2();
}

QStringList
ConfigQml::Sources(){
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sourcesPath;
    Core::SourceList *sourcelist = new Core::SourceList(path);
    QStringList l;
    for (unsigned int i=0; i<sourcelist->size(); i++){
        l << QString::fromStdString(sourcelist->at(i)->name());
    }
    delete sourcelist;
    return l;
}

QStringList
ConfigQml::Countries(QString source){
    QStringList l;
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    if (source == "") return l;
    QString filename(source);
    filename.append(".db");
    filename.prepend(path.c_str());
    if (!this->db) {
        this->db->set_databasename(filename.toStdString());
    }else {
        this->db->set_databasename(filename.toStdString());

    }
    if (!this->db->open_database()){
        l << "error open db";
        return l;
    }
    Core::listdata * list = db->create_countries_list();
    Core::listdata::iterator cur;

    if (!list)
        return l;
    for (cur=list->begin(); cur<list->end(); cur++){
        l << QString::fromStdString((*cur).second);
        /*if (isKeys) {
            l << QString::fromStdString((*cur).first);
        }else {
            l << QString::fromStdString((*cur).second);
        }*/
    }

    return l;
}

int
ConfigQml::getCountryId(int index){
    Core::listdata * list = db->create_countries_list();

    Core::listdata::iterator cur;

    if (!list)
        return 0;
    cur = list->begin()+index;
    return atoi((*cur).first.c_str());
}

int
ConfigQml::getRegionId(int country, int index){
    Core::listdata * list = db->create_region_list(getCountryId(country));

    Core::listdata::iterator cur;

    if (!list)
        return 0;
    cur = list->begin()+index;
    return atoi((*cur).first.c_str());
}

QString
ConfigQml::getCityId(int region_id, int index){
    Core::listdata * list = db->create_stations_list(region_id);
    Core::listdata::iterator cur;

    if (!list)
        return 0;
    cur = list->begin()+index;
    return QString::fromStdString((*cur).first);
}

QStringList
ConfigQml::Regions(int index){
    QStringList l;
    
    Core::listdata * list = db->create_region_list(getCountryId(index));

    if (list->size() == 0) return l;
    Core::listdata::iterator cur;
    for (cur=list->begin(); cur<list->end(); cur++)
        l << QString::fromStdString((*cur).second);

    return l;
}

QStringList
ConfigQml::Cities(int country_index, int index){
    QStringList l;
    Core::listdata * list = db->create_stations_list(getRegionId(country_index, index));

    if (list->size() == 0) return l;
    Core::listdata::iterator cur;
    for (cur=list->begin(); cur<list->end(); cur++)
        l << QString::fromStdString((*cur).second);

    return l;
}
void
ConfigQml::saveStation1(QString city_id, QString city_name, QString region, QString country,
                        QString source, int source_id, bool gps, 
                        double latitude, double longitude){
    Core::Station *station;
    

    (void)source_id;

    if (latitude == 0 && longitude == 0 && (source == "weather.com" || source == "openweathermap.org" || source == "yr.no")){
        Core::DatabaseSqlite *db_w = new Core::DatabaseSqlite("");
        std::string path(Core::AbstractConfig::prefix);
        path += Core::AbstractConfig::sharePath;
        path += "db/";
            QString filename;
        if (source == "weather.com")
            filename = "weather.com";
        if (source == "openweathermap.org")
            filename = "openweathermap.org";
        if (source == "yr.no")
            filename = "yr.no";
        filename.append(".db");
        filename.prepend(path.c_str());
        db_w->set_databasename(filename.toStdString());
        if (!db_w->open_database()){
            qDebug() << "error open database";
            return;
        }
        db_w->get_station_coordinate(city_id.toStdString(), latitude, longitude);
    }
    station = new Core::Station(
                source.toStdString(),
                city_id.toStdString(), 
                city_name.toUtf8().data(),
                country.toStdString(),
                region.toStdString(),
                gps, latitude, longitude);

    stationsList().push_back(station);
    //ConfigQml::Config::stationsList(*stationlist);
    saveConfig();
    refreshconfig();
}

void
ConfigQml::saveStation(int city_id, QString city,
                       int region_id, QString region,
                       int country_id, QString country,
                       int source_id, QString source){
    Core::Station *station;
    double latitude; 
    double longitude;
 
    (void)source_id;

    region_id = getRegionId(country_id, region_id);
    country_id = getCountryId(country_id);
    std::string code = getCityId(region_id, city_id).toStdString();
    db->get_station_coordinate(code, latitude, longitude); 
    station = new Core::Station(
                source.toStdString(),
                code,
                city.toUtf8().data(),
                country.toStdString(),
                region.toStdString(),
                false, latitude, longitude);

    stationsList().push_back(station);
    //ConfigQml::Config::stationsList(*stationlist);
    saveConfig();
    refreshconfig();
}

QString
ConfigQml::stationname_index(int i){
    QString qstr = "";
    if (i>=0 && i <this->stationsList().size()){
        /* std::cerr<<"ConfigQml::stationname_index "<<i<<std::endl; */
        return qstr.fromUtf8(ConfigQml::Config::stationname(i).c_str()); 
    }else
        return QString("Unknown");
}

QString
ConfigQml::stationname(){
    /* std::cerr<<"ConfigQml::stationname() "<<ConfigQml::Config::stationname()<<std::endl; */
    QString qstr = "";
    return qstr.fromUtf8(ConfigQml::Config::stationname().c_str()); 
}

QString
ConfigQml::nextstationname(){
    QString qstr = "";
    return qstr.fromUtf8(ConfigQml::Config::nextstationname().c_str()); 
}

QString
ConfigQml::prevstationname(){
    QString qstr = "";
    return qstr.fromUtf8(ConfigQml::Config::prevstationname().c_str()); 
}

int
ConfigQml::updateinterval(){
    return ConfigQml::Config::UpdatePeriod(); 
}

void
ConfigQml::update_interval(int interval){
    ConfigQml::Config::UpdatePeriod(interval);
    saveConfig();
    refreshconfig();
}

QString
ConfigQml::data_filename()
{
    std::cerr<<"current_station_id is "<<this->current_station_id()<<" ."<<std::endl; 
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id())){
        std::cerr<<"this->stationsList().at(this->current_station_id())->fileName().c_str() "<<this->stationsList().at(this->current_station_id())->fileName()<<std::endl; 
        return this->stationsList().at(this->current_station_id())->fileName().c_str();
    }
    else
        return QString();
}

QString
ConfigQml::source_index(int i){
    QString qstr = "";
    if (i>=0 && i <this->stationsList().size())
        return  this->stationsList().at(i)->sourceName().c_str();
    else
        return QString();
}

QString
ConfigQml::source(){
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->sourceName().c_str();
    else
        return QString();
}

QString
ConfigQml::_source(){
    return source();
}

QString
ConfigQml::viewURL(){
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        return this->stationsList().at(this->current_station_id())->viewURL().c_str();
    else
        return QString();
}

void
ConfigQml::changestation(){
   if ((uint)(this->current_station_id() + 1) < this->stationsList().size()){
       std::cerr<<"+1 this->current_station_id() + 1 "<< this->current_station_id() + 1<<std::endl;
       this->current_station_id(this->current_station_id() + 1);
   }else {
       std::cerr<<"0 this->current_station_id() + 1 "<< this->current_station_id() + 1<<std::endl;
       this->current_station_id(0);
   }
   std::cerr<<"this->current_station_id()"<< this->current_station_id()<<std::endl;
   this->saveConfig();
   emit ConfigQml::configReloadCoverPage();
//   refreshconfig2();
}

void
ConfigQml::changestationback(){
   
   if ((int)(this->current_station_id() - 1) >= 0){
       this->current_station_id(this->current_station_id() - 1);
   }else {
       this->current_station_id(stationsList().size() - 1);
   }
   this->saveConfig();
   emit ConfigQml::configReloadCoverPage();
//   refreshconfig2();
}


void
ConfigQml::station_by_index(int i){
    if ((int)(i) < this->stationsList().size() && i > 0){
       this->current_station_id(i);

   }else {
       this->current_station_id(0);
   }
   this->saveConfig();
   refreshconfig();
}

void
ConfigQml::nextstation(){
    if ((int)(this->current_station_id() + 1) < this->stationsList().size()){
       this->current_station_id(this->current_station_id() + 1);

   }else {
       this->current_station_id(0);
   }
   this->saveConfig();
   refreshconfig();
}

void
ConfigQml::prevstation(){
    if ((int)(this->current_station_id() - 1) >= 0){
       this->current_station_id(this->current_station_id() - 1);
   }else {
       this->current_station_id(stationsList().size());
   }
   this->saveConfig();
   refreshconfig();
}

void
ConfigQml::refreshconfig2(){
    emit ConfigQml::configChanged();
}

void
ConfigQml::refreshconfig3(){
    this->ReLoadConfig();
    emit ConfigQml::configReloadCoverPage();
}

void
ConfigQml::refreshconfig(){
    emit ConfigQml::iconsetChanged();
    emit ConfigQml::iconspathChanged();
    emit ConfigQml::imagespathChanged();
    emit ConfigQml::temperatureunitChanged();
    emit ConfigQml::windspeedunitChanged();
    emit ConfigQml::visibleunitChanged();
    emit ConfigQml::fontcolorChanged();
    emit ConfigQml::stationnameChanged();
    emit ConfigQml::configChanged();
    emit ConfigQml::lock_screen_x_positionChanged();
    emit ConfigQml::lock_screen_y_positionChanged();
    emit ConfigQml::languageChanged();
}

void
ConfigQml::updatestations(){
    this->_updating = true;
    thread->start();
}

void
ConfigQml::updategpsstation(){
    qDebug() << "Update GPS ";
    if (ConfigQml::Config::Gps()){
        if (_gps){
            delete _gps;
            _gps = NULL;
        }
        enableGps();
    }
}


void
ConfigQml::showweb(){
    if (this->current_station_id() != INT_MAX && this->stationsList().size() > 0
                                                &&  this->stationsList().at(this->current_station_id()))
        QDesktopServices::openUrl(QUrl(this->stationsList().at(this->current_station_id())->viewURL().c_str()));     
}

void
ConfigQml::showwebdonation(){
    QDesktopServices::openUrl(QUrl("file:///usr/share/harbour-meecast/html/donation.html"));     
}

void
ConfigQml::showwebsupport(){
    QDesktopServices::openUrl(QUrl("http://meecast.com"));     
}

void
ConfigQml::downloadFinishedSlot(){
    std::cerr<<"ConfigQml::downloadFinishedSlot()"<<std::endl;
    if (!need_updating)
        emit configChanged();
}


void
ConfigQml::closeapplication(){
   exit (0);
}


void
ConfigQml::reload_config(){
   this->ReLoadConfig();
   this->refreshconfig();
}

QString
ConfigQml::tr(QString str){
    return QString(QString::fromUtf8(_(str.toUtf8())));
}

void
ConfigQml::enableGps(){
    if (!_gps) {
        _gps = new GpsPosition();
        _gps->startGps();
        qDebug() << "create gps, add slot";
        connect(_gps, SIGNAL(findCoord(double, double)), this, SLOT(addGpsStation(double, double)));
    }
    //addGpsStation(55.1882, 30.2177);
}

void
ConfigQml::addGpsStation(double latitude, double longitude){
    Core::DatabaseSqlite *db_w = new Core::DatabaseSqlite("");
    std::string country, region, code, name;
    int index;
    int source_id = 0;
    double lat, lon;
    qDebug() << "ConfigQml::addGpsStation lat=" << latitude << " lon" << longitude; 
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    QString filename = "weather.com";
    filename.append(".db");
    filename.prepend(path.c_str());
    db_w->set_databasename(filename.toStdString());
    if (!db_w->open_database()){
        qDebug() << "error open database";
        return;
    }
    db_w->get_nearest_station(latitude, longitude, country, region, code, name, lat, lon);
    qDebug() << "find station " << name.c_str();

    index = getGpsStation();
    if (index > -1){ /* gps station already exist */
        if (code.compare(stationsList().at(index)->id()) != 0){ /* found another station */
            stationsList().erase(stationsList().begin() + index); /* delete previous gps station */
            qDebug() << "FOUND NEW STATION, DELETE PREV";
        }else {
            /* found the same station, do nothing */
            qDebug() << "FOUND THE SAME STATION";
            return;
        }
    }else {
        qDebug() << "FOUND FIRST GPS STATION";
    }

    /* save new gps station */
    path = Core::AbstractConfig::prefix;
    path += Core::AbstractConfig::sourcesPath;
    Core::SourceList *sourcelist = new Core::SourceList(path);

    for (uint i=0; i < sourcelist->size(); i++){
        if (sourcelist->at(i)->name().compare("weather.com") == 0)
            source_id = i;
    }

    qDebug() << "Name "<<QString::fromStdString(name);
    saveStation1(QString::fromStdString(code), QString::fromStdString(name)+" (GPS)", QString::fromStdString(region),
                 QString::fromStdString(country), "weather.com", source_id, true, latitude, longitude);
    qDebug() << "SAVE GPS STATION";

    /* save gps station's coordinates */
    _gps->setLastCoordinates(lat, lon);
}

int
ConfigQml::getGpsStation(){
    int index = -1;
    for (unsigned int i=0; i<stationsList().size(); i++){
        if (stationsList().at(i)->gps() == true)
            index = i;
    }
    return index;
}

int
ConfigQml::_current_station_id(){
    return current_station_id();
}

void
ConfigQml::_current_station_id(int i){
    current_station_id(i);
}

void
ConfigQml::check_and_update_station(){
    std::cerr<<"ConfigQml::check_and_update_station "<< _time_for_updating <<" "<< time(NULL)<< std::endl;
    if (_time_for_updating < time(NULL)){
        std::cerr<<"ConfigQml::check_and_update_stationi2"<<std::endl;
        Core::DataParser* dp = NULL;
        dp = Core::DataParser::Instance();
        if (dp && (time(NULL) - dp->LastUpdate()) > this->UpdatePeriod()){
            this->updatestations();
        }else{
            this->configChanged();
        }
        if (dp)
            dp->DeleteInstance();
    }
}

void ConfigQml::time_for_updating(uint _time){
    _time_for_updating = _time;
}
