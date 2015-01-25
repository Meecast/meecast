#include "meecastdata.h"

#include <QTimer>

Meecastdata::Meecastdata(QObject *parent):
    QObject(parent),
    m_active(false){
    _filemonitoring = NULL;
    _watcher = new QFileSystemWatcher();

    QFile watcher_file("/home/nemo/.cache/harbour-meecast/current.xml");
    if(!watcher_file.exists()){
       std::cerr<<"Create watcher file"<<std::endl;
        QDir dir("/home/nemo/.cache/harbour-meecast");
        if (!dir.exists())
            dir.mkpath("/home/nemo/.cache/harbour-meecast");
        if (watcher_file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
            watcher_file.close();
        }
    }
    _watcher->addPath("/home/nemo/.cache/harbour-meecast/current.xml"); 
    connect(_watcher,SIGNAL(fileChanged(QString)),this,SLOT(currentfileChanged(QString)));

    getWeatherdata();

//    Q_EMIT activeChanged();
}
 
Meecastdata::~Meecastdata(){
    if (_filemonitoring)
        delete _filemonitoring;
    if(_watcher)
        delete _watcher;
}

void 
Meecastdata::updatefilemonitoring(){

    std::cerr<<"updatefilemonitoring()"<<std::endl;
    QFile watcher_file("/home/nemo/.cache/harbour-meecast/current.xml");
    if(watcher_file.exists()){
        _watcher->addPath("/home/nemo/.cache/harbour-meecast/current.xml"); 
        getWeatherdata();
        Q_EMIT refreshWidget();
    }else{
        _filemonitoring->start(60*1000);
    }
}

void 
Meecastdata::currentfileChanged(QString path){

    std::cerr<<"currentfileChanged"<<std::endl;
#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << "currentfileChanged "<< "\n";
	    file.close();
	}
	// Debug end 
#endif

    getWeatherdata();
    Q_EMIT refreshWidget();
}


void Meecastdata::classBegin(){

}

void Meecastdata::componentComplete(){
    if (m_active) {
        initialize();
    }
}

void Meecastdata::initialize(){
    getAllProperties();
    emitProperties();
    getAllPropertiesPlayer();
    emitPropertiesPlayer();
}

void Meecastdata::setActive(bool newActive){
    if (m_active != newActive) {
        m_active = newActive;

        if (!m_active) {
            disconnectSignals();
        }
        Q_EMIT activeChanged();
    }
}

void Meecastdata::activated(){

#if 0
	// Debug begin
	QFile file("/tmp/1.log");
	if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
	    QTextStream out(&file);
	    out <<  QLocale::system().toString(QDateTime::currentDateTime(), QLocale::LongFormat) << " Meecastdata::activated() "<< "\n";
	    file.close();
	}
	// Debug end 
#endif

}



void Meecastdata::emitProperties(){
}

void Meecastdata::emitPropertiesPlayer() {
}

void Meecastdata::connectSignals() {
}

void Meecastdata::disconnectSignals() {
}

void 
Meecastdata::getWeatherdata(){

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

   std::cerr<<" getWeatherData"<<std::endl;
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

    //std::cerr<<" getWeatherData1"<<std::endl;
	QFile current_file("/home/nemo/.cache/harbour-meecast/current.xml");

    /* Check file */
    if (current_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        current_file.close();
    }else{
        if (_filemonitoring)
            delete _filemonitoring;
        _filemonitoring = new QTimer(this);
        _filemonitoring->setSingleShot(true);
        connect(_filemonitoring, SIGNAL(timeout()), this, SLOT(updatefilemonitoring()));
        _filemonitoring->start(1000);
      //  std::cerr<<" Problem"<<std::endl;
        return;
    }
    if (current_file.size()<=0)
        return;
    if (current_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        _weatherdata.clear();
        QXmlStreamReader xml(&current_file);
        while(!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            /* If token is just StartDocument, we'll go to next.*/
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if(token == QXmlStreamReader::StartElement) {
                if(xml.name() == "temperatureunit") {
                     std::cerr<<"temperatureunit "<<std::endl;
                    _weatherdata.insert("temperatureunit", xml.readElementText());
                    continue;
                }
                if(xml.name() == "station") {
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("name")){
                        std::cerr<<" station_name "<<attributes.value("name").toString().toStdString().c_str()<<std::endl;
                        _weatherdata.insert("station_name", attributes.value("name").toString());
                    } 
                    continue;
                }
                if(xml.name() == "period") {
                    QXmlStreamAttributes attributes = xml.attributes();

                    _weatherdata.insert("current", false);
                    if(attributes.hasAttribute("current")){
                        if (attributes.value("current").toString() == "true") {
                            _weatherdata.insert("current", false);
                        }
                    } 
                    if(attributes.hasAttribute("end") && itemnumber == 1){
                        until_valid_time = attributes.value("end").toInt();  
                    } 
                    if(attributes.hasAttribute("itemnumber")){
                        itemnumber = attributes.value("itemnumber").toInt();  
                    } 
                    parsePeriod(xml, itemnumber); 
                    continue;
                }
                if(xml.name() == "last_update") {
                    _weatherdata.insert("last_update", xml.readElementText());
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
Meecastdata::parsePeriod(QXmlStreamReader& xml, int itemnumber){

    //std::cerr<<"ParsePeriod\n"<< std::endl;

    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "period") {
        return;
    }

    QXmlStreamAttributes attributes = xml.attributes();
    xml.readNext();
    QString temporary = xml.name().toString();
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "period")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            if(xml.name() == "temperature") {
                _weatherdata.insert("item" + QString::number(itemnumber) + "_temperature", xml.readElementText());
            }
            if(xml.name() == "temperature_hi") {
                _weatherdata.insert("item" + QString::number(itemnumber) + "_temperature_high", xml.readElementText());
            }
            if(xml.name() == "temperature_low") {
                _weatherdata.insert("item" + QString::number(itemnumber) + "_temperature_low", xml.readElementText());
            }
            if(xml.name() == "icon") {
                _weatherdata.insert("item" + QString::number(itemnumber) + "_icon", xml.readElementText());
            }
            if(xml.name() == "description") {
                _weatherdata.insert("item" + QString::number(itemnumber) + "_description", xml.readElementText());
            }
            if(xml.name() == "short_day_name") {
                _weatherdata.insert("item" + QString::number(itemnumber) + "_short_day_name", xml.readElementText());
            }
        }
        xml.readNext();
    }

    //std::cerr<<"ParsePeriod end"<< std::endl;
}

void Meecastdata::setDBusProperty(const QString &interface, const QString &name, const QVariant &value) {
    //if (propertiesIface)
    //    propertiesIface->call("Set", interface, name, value);
}

QVariantMap Meecastdata::getAllDBusProperties(const QString &interface){
    /*
    if (propertiesIface) {
        QDBusReply<QVariantMap> reply = propertiesIface->call("GetAll", interface);
        if (reply.isValid()) {
            return reply.value();
        }
        else {
            return QVariantMap();
        }
    }
    return QVariantMap();
    */
}

void Meecastdata::onPropertiesChanged(const QString &interface, const QVariantMap &propertiesChanged, const QStringList &propertiesInvalidated) {
}

QString 
Meecastdata::nameString() const{
    return _weatherdata["station_name"].toString();
}

QVariantMap 
Meecastdata::forecastdata() const{
    return _weatherdata;
}

