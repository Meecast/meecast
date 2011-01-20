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

        QDomElement docElem = doc.documentElement();

        QDomNode n = docElem.firstChild();
        while (!n.isNull()){
            QDomElement e = n.toElement();
            if (!e.isNull()){
                qDebug() << qPrintable(e.tagName());
            }
            n = n.nextSibling();
        }

    }


    DataList&
    DataParserQt::data()
    {
        return *_list;
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
