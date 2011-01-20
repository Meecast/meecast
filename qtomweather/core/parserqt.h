#ifndef PARSERQT_H
#define PARSERQT_H
////////////////////////////////////////////////////////////////////////////////
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>
#include <QFile>
#include <QDebug>
#include <string>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////

namespace Core {
    class ParserQt{
        public:
            ParserQt();
            bool valid(QString filename, QUrl schema_filename);

    };

} // namespace Core
#endif // PARSERQT_H
