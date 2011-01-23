#include "parserqt.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    ParserQt::ParserQt(const std::string& filename, const std::string& schema_filename)
    {
        QXmlSchema schema;
        if (!schema.load(QUrl(":" + QString::fromStdString(schema_filename)))){
            throw("Invalid schema file");
            return;
        }
        if (!schema.isValid()){
            throw("Schema is invalid");
            return;
        }
        QFile file(QString::fromStdString(filename));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            throw("Invalid source file");
            return;
        }
        QXmlSchemaValidator validator(schema);
        if (!validator.validate(&file, QUrl::fromLocalFile(file.fileName()))){
            //qDebug() << "File " << filename << " is invalid";
            file.close();
            throw("Xml file is invalid");
            return;
        }
        file.close();
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!_doc.setContent((&file))){
            file.close();
            throw("Error set content");
            return;
        }
        file.close();

    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
