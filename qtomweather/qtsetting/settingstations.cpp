#include "settingstations.h"
#include "ui_settingstations.h"
#include <QtDBus>

SettingStations::SettingStations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingStations)
{
    _stationlist = new Core::StationsList;
    std::string configpath = Core::AbstractConfig::getConfigPath();
    configpath += "config.xml";
    std::string schemapath = Core::AbstractConfig::prefix;
    schemapath += Core::AbstractConfig::schemaPath;
    schemapath += "config.xsd";
    //qDebug() << configpath.c_str() << " " << schemapath.c_str();
    try {
        _config = new Core::Config(configpath, schemapath);
        *_stationlist = _config->stationsList();
    }catch(const std::string &str){
        std::cerr<<"Error in ConfigQML class: "<< str <<std::endl;
        _config = new Core::Config();
    }
    catch(const char *str){
        std::cerr<<"Error in ConfigQML class: "<< str <<std::endl;
        _config = new Core::Config();
    }
    ui->setupUi(this);
    ui->removeButton->setEnabled(false);

    Core::StationsList::iterator cur;
    for (cur=_stationlist->begin(); cur<_stationlist->end(); cur++){
        QListWidgetItem *item = new QListWidgetItem((*cur)->name().c_str(), ui->listWidget);
        item->setSizeHint(QSize(ui->listWidget->width(), 40));
        item->setToolTip((*cur)->id().c_str());
    }

    ui->temperatureCombo->addItem("C");
    ui->temperatureCombo->addItem("F");
    //std::cerr << _config->TemperatureUnit() << std::endl;
    if (_config->TemperatureUnit().compare("F") == 0)
        ui->temperatureCombo->setCurrentIndex(1);
    else
        ui->temperatureCombo->setCurrentIndex(0);

    ui->windCombo->addItem("m/s");
    ui->windCombo->addItem("km/h");
    ui->windCombo->addItem("mi/h");
    //std::cerr << _config->TemperatureUnit() << std::endl;
    if (_config->WindSpeedUnit().compare("m/s") == 0)
        ui->windCombo->setCurrentIndex(0);
    else if (_config->WindSpeedUnit().compare("km/h") == 0)
             ui->windCombo->setCurrentIndex(1);
         else if (_config->WindSpeedUnit().compare("mi/h") == 0)
             ui->windCombo->setCurrentIndex(2);

    if (_config->UpdateConnect())
        ui->updateCheck->setChecked(true);
    else
        ui->updateCheck->setChecked(false);


    period_hash["2 hour"] = 2*60*60;
    period_hash["1 hour"] = 60*60;
    period_hash["30 min"] = 30*60;
    period_hash["10 min"] = 10*60;
    period_hash["never"] = INT_MAX;

    QHashIterator<QString, int> iter(period_hash);
    int ii = 0;
    while (iter.hasNext()) {
         iter.next();
         //cout << i.key() << ": " << i.value() << endl;
         ui->updateCombo->addItem(iter.key());
         if (_config->UpdatePeriod() == iter.value())
             ui->updateCombo->setCurrentIndex(ii);
         ii++;
     }

    Dirent *dp = 0;
    DIR *dir_fd = opendir((Core::AbstractConfig::prefix+Core::AbstractConfig::iconsPath).c_str());
    //std::cerr << (Core::AbstractConfig::prefix+Core::AbstractConfig::iconsPath) << std::endl;
    int i = 0;
    if(dir_fd){
        while((dp = readdir(dir_fd))){
            std::string name = dp->d_name;
            if(name == "." || name == "..")
                continue;
            if(dp->d_type == DT_DIR && name[0] != '.'){
                try{
                    ui->iconsetCombo->addItem(QString::fromStdString(name));
                    if (_config->iconSet().compare(name) == 0)
                        ui->iconsetCombo->setCurrentIndex(i);
                    i++;
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
    connect(ui->listWidget, SIGNAL(itemEntered(QListWidgetItem*)), this, SLOT(setRemove(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setRemove(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(setRemove(QListWidgetItem*)));

}

SettingStations::~SettingStations()
{
    delete ui;
}

void
SettingStations::add()
{
    Setting st(this);
    int res = st.exec();
    //std::cerr << "dialog result " << res << std::endl;
    if (res){
        Core::Station *station = st.station;
        _stationlist->push_back(station);
        QListWidgetItem *item = new QListWidgetItem(station->name().c_str(), ui->listWidget);
        item->setSizeHint(QSize(ui->listWidget->width(), 40));
        item->setToolTip(station->id().c_str());
    }
}
void
SettingStations::remove()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    for (int i=0; i<_stationlist->size(); i++){
        if (item->toolTip().toStdString().compare(_stationlist->at(i)->id()) == 0 &&
            item->text().toStdString().compare(_stationlist->at(i)->name()) == 0){
            _stationlist->erase(_stationlist->begin()+i);
        }
    }
    delete item;
    if (ui->listWidget->count() == 0)
        ui->removeButton->setEnabled(false);
}
void
SettingStations::setRemove(QListWidgetItem *item)
{
    ui->removeButton->setEnabled(true);
}

void
SettingStations::okClicked()
{
    _config->stationsList(*_stationlist);
    _config->TemperatureUnit(ui->temperatureCombo->currentText().toStdString());
    _config->WindSpeedUnit(ui->windCombo->currentText().toStdString());
    _config->iconSet(ui->iconsetCombo->currentText().toStdString());
    _config->UpdateConnect(ui->updateCheck->isChecked());
    _config->UpdatePeriod(period_hash[ui->updateCombo->currentText()]);
    _config->saveConfig();

    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/org/meego/omweather",
                               "org.meego.omweather", "reload_config");
    bus.send(message);
    message = QDBusMessage::createMethodCall("org.meego.omweather","/org/meego/omweather",
                               "org.meego.omweather", "reload_config");
    bus.send(message);

}
