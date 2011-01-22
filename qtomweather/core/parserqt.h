#ifndef PARSERQT_H
#define PARSERQT_H
////////////////////////////////////////////////////////////////////////////////
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QDebug>
#include <string>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////

namespace Core {
    class ParserQt{
        protected:
            QDomDocument _doc;
        public:
            ParserQt(const QString filename, const QUrl schema_filename);

    };

} // namespace Core
#endif // PARSERQT_H
