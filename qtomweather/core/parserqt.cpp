#include "parserqt.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
    ParserQt::ParserQt(){}

    void
    ParserQt::valid(QString filename, QUrl schema_filename)
    {
        /*
        if(filename.empty())
            throw("Invalid source file.");
        int r = access(filename.c_str(), R_OK);
        if(r)
            throw("File: " + filename + " - ");
        if(schema_filename.empty())
            throw("Invalid source schema file.");
        r = access(schema_filename.c_str(), R_OK);
        if(r)
            throw("File: " + filename + " - ");
            */
    #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
    #endif //LIBXMLCPP_EXCEPTIONS_ENABLED

            QXmlSchema schema;
            schema.load(schema_filename);

            if (schema.isValid()){
                QFile file(filename);
                file.open(QIODevice::ReadOnly);

                QXmlSchemaValidator validator(schema);
                if (validator.validate(&file, QUrl::fromLocalFile(file.fileName()))){
                    qDebug() << "file " << filename << "  is valid";
                }else {
                    qDebug() << "file " << filename << " is invalid";
                    throw("Document is not valid.");
                }
            } else {
                qDebug() << "schema is invalid";
            }


    #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            throw(ex.what());
        }
    #endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }

