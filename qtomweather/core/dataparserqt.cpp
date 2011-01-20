#include "dataparserqt.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    DataParserQt::DataParserQt(const QString filename)
    {
        _timezone = 0;
        _list = new DataList;
        QDomDocument doc("data");
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)){
            qDebug() << "error open file";
            return;
        }
        if (!doc.setContent((&file))){
            qDebug() << "error set content";
            file.close();
            return;
        }
        file.close();

        QDomElement root = doc.documentElement();
        Data* forecast_data;


        QDomNodeList nodelist = root.elementsByTagName("timezone");
        if (nodelist.count() == 1) {
            _timezone = nodelist.at(0).toElement().text().toInt();
            qDebug() << _timezone;
        }
        nodelist = root.elementsByTagName("period");
        for (int i=0; i<nodelist.count(); i++){
            forecast_data = new Data();
            QDomElement e = nodelist.at(i).toElement();
            forecast_data->StartTime(e.attribute("start").toInt());
            forecast_data->EndTime(e.attribute("end").toInt());
            if (e.hasAttribute("current") && e.attribute("current") == "true")
                forecast_data->Current(1);
            QDomNode n = e.firstChild();
            while (!n.isNull()){
                QDomElement el = n.toElement();
                QString tag = el.tagName();

                if (tag == "temperature" || tag == "temperature_hi"){
                    forecast_data->temperature_hi().value(el.text().toFloat());
                }else if (tag == "temperature_low"){
                    forecast_data->temperature_low().value(el.text().toFloat());
                }else if (tag == "icon"){
                    forecast_data->Icon(el.text().toInt());
                }else if (tag == "description"){
                    forecast_data->Text(el.text().toStdString());
                }else if (tag == "humidity"){
                    forecast_data->Humidity(el.text().toInt());
                }else if (tag == "wind_speed"){
                    forecast_data->WindSpeed(el.text().toFloat());
                }else if (tag == "wind_direction"){
                    forecast_data->WindDirection(el.text().toStdString());
                }else if (tag == "flike"){
                    forecast_data->Flike().value(el.text().toFloat());
                }else if (tag == "pressure"){
                    forecast_data->Pressure(el.text().toInt());
                }else if (tag == "ppcp")
                    forecast_data->Ppcp(el.text().toFloat());

                n = n.nextSibling();
            }
            _list->push_back(forecast_data);
        }

    }


    DataList&
    DataParserQt::data()
    {
        return *_list;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
