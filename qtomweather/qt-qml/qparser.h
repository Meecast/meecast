#ifndef QPARSER_H
#define QPARSER_H
////////////////////////////////////////////////////////////////////////////////
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>
#include <QFile>
#include <QDebug>
#include <string>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////

    class QParser{
        public:
            QParser();
            void valid(QString filename, QUrl schema_filename);

    };


#endif // QPARSER_H
