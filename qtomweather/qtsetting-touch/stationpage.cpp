#include <MSceneManager>
#include "stationpage.h"

StationPage::StationPage() :
    MApplicationPage(0)
{
    station = NULL;
    setTitle("Select station");
    db = new Core::DatabaseSqlite("");

    //sourcelist = new Core::SourceList("../test/sources/");
    //QString path = QString::fromStdString(Core::AbstractConfig::prefix);
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sourcesPath;

    sourcelist = new Core::SourceList(path);
}
StationPage::~StationPage()
{
}
void StationPage::createContent()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    centralWidget()->setLayout(layout);
    QGraphicsLinearLayout *layout_btn = new QGraphicsLinearLayout(Qt::Vertical);
    layout_btn->setSpacing(0);
    layout->addItem(layout_btn);

    sourceCombo = new MComboBox();
    sourceCombo->setTitle("Select source");
    for (int i=0; i<sourcelist->size(); i++){
        sourceCombo->addItem(QString::fromStdString(sourcelist->at(i)->name()));
    }
    connect(sourceCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(sourceChanged(int)));
    layout_btn->addItem(sourceCombo);

    countryCombo = new MComboBox();
    countryCombo->setTitle("Select country");
    connect(countryCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(countryChanged(int)));
    layout_btn->addItem(countryCombo);

    regionCombo = new MComboBox();
    regionCombo->setTitle("Select region");
    connect(regionCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(regionChanged(int)));
    layout_btn->addItem(regionCombo);

    cityCombo = new MComboBox();
    cityCombo->setTitle("Select city");
    layout_btn->addItem(cityCombo);

    MButton *savebutton = new MButton("Save");
    connect(savebutton, SIGNAL(clicked()),
            this, SLOT(saveClicked()));
    layout->addItem(savebutton);
}
void StationPage::sourceChanged(int val)
{
    std::cerr << "source changed val = " << val << std::endl;
    std::string path(Core::AbstractConfig::prefix);
    path += Core::AbstractConfig::sharePath;
    path += "db/";
    QString filename = sourceCombo->currentText();
    filename.append(".db");
    filename.prepend(path.c_str());
    if (!this->db) {
        this->db->set_databasename(filename.toStdString());
    }else {
        this->db->set_databasename(filename.toStdString());

    }
    this->db->open_database();

    Core::listdata * list = db->create_countries_list();

    countryCombo->clear();

    //countryCombo->addItem("");
    if (!list)
        return;
    for (int i=0; i<list->size(); i++){
        countryCombo->addItem(QString::fromStdString(list->at(i).second));
        countryCombo->setData(i, QString::fromStdString(list->at(i).first));
    }
}
void StationPage::countryChanged(int val)
{
    QString str = countryCombo->data(val).toString();
    std::cerr << "country changed val = " << str.toStdString() << std::endl;
    regionCombo->clear();

    Core::listdata * list = db->create_region_list(countryCombo->data(val).toInt());

    if (list->size() == 0) return;
    for (int i=0; i<list->size(); i++){
        regionCombo->addItem(QString::fromStdString(list->at(i).second));
        regionCombo->setData(i, QString::fromStdString(list->at(i).first));
    }

}
void
StationPage::regionChanged(int val)
{
    cityCombo->clear();

    Core::listdata * list = db->create_stations_list(regionCombo->data(val).toInt());

    if (list->size() == 0) return;
    for (int i=0; i<list->size(); i++){
        cityCombo->addItem(QString::fromStdString(list->at(i).second));
        cityCombo->setData(i, QString::fromStdString(list->at(i).first));
    }

}
void
StationPage::saveClicked()
{
    if (cityCombo->currentIndex() == -1){
        //this->reject();
        //return;
        close();
        return;
    }

    std::string code = cityCombo->data(cityCombo->currentIndex()).toString().toStdString();
    int index = sourceCombo->data(sourceCombo->currentIndex()).toInt();
    std::string url_template = sourcelist->at(index)->url_template();

    char forecast_url[4096];
    snprintf(forecast_url, sizeof(forecast_url)-1, url_template.c_str(), code.c_str());
    station = new Core::Station(
                sourceCombo->currentText().toStdString(),
                code,
                cityCombo->currentText().toStdString(),
                countryCombo->currentText().toStdString(),
                regionCombo->currentText().toStdString(),
                forecast_url);
    std::string filename(Core::AbstractConfig::getConfigPath());
    filename += sourceCombo->currentText().toStdString();
    filename += "_";
    filename += code;
    station->fileName(filename);
    station->converter(sourcelist->at(index)->binary());
    //return;
    close();
    //emit
    emit stationSaved();
}
