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

    class ParserQt{
        public:
            ParserQt();
            void valid(QString filename, QUrl schema_filename);

    };


#endif // PARSERQT_H
