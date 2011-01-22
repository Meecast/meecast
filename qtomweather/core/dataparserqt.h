#ifndef DATAPARSERQT_H
#define DATAPARSERQT_H
////////////////////////////////////////////////////////////////////////////////
#include <QtXml/QDomDocument>

#include <QFile>
#include <QDebug>

#include "data.h"
#include "datalist.h"
#include "parserqt.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class DataParserQt: public ParserQt
    {
        DataList *_list;
        int _timezone;
    public:
        DataParserQt(const QString filename, const QUrl schema_filename);
        DataList& data();
        virtual ~DataParserQt();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATAPARSERQT_H
