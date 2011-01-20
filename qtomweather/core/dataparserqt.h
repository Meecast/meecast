#ifndef DATAPARSERQT_H
#define DATAPARSERQT_H
////////////////////////////////////////////////////////////////////////////////
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

#include <QFile>
#include <QDebug>

#include "data.h"
#include "datalist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class DataParserQt
    {
        DataList *_list;
        int _timezone;
        //void processNode(const QDomNode node);
    public:
        DataParserQt(const QString filename);
        DataList& data();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATAPARSERQT_H
