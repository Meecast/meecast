/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2012 Vlad Vasilyeu
 * Copyright (C) 2006-2011 Pavel Fialko
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/


#include "parser.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
namespace Core {
////////////////////////////////////////////////////////////////////////////////
    Parser::Parser(){}
    Parser::Parser(const std::string& filename, const std::string& schema_filename)
                        : AbstractConfig(){
        _filename = filename;
        /* std::cerr<<"Parse file "<<filename<<std::endl; */

    AppLogDebug("Parser");
    #ifdef LIBXML
        if(filename.empty())
            throw("Invalid source file.");
        int r = access(filename.c_str(), R_OK);
        if(r)
            throw("File: " + filename + " - " + std::string(strerror(errno)));
        if(schema_filename.empty())
            throw("Invalid source schema file.");
        r = access(schema_filename.c_str(), R_OK);
        if(r)
            throw("File: " + filename + " - " + std::string(strerror(errno)));
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            parser = new xmlpp::DomParser;
            validator = new xmlpp::SchemaValidator;
            parser->set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.

            validator->parse_file(schema_filename);
            if(validator->validate(filename))
                throw("Document is not valid.");
            parser->parse_file(filename);
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            throw(ex.what());
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    #else //LIBXML
    #ifdef QT
    QFile file(QString::fromStdString(filename));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                std::cerr<<"error file open"<<std::endl;
                throw("Invalid source file");
                return;
        }
        if (false){
            QXmlSchema schema;
            if (!schema.load(QUrl(":" + QString::fromStdString(schema_filename)))){
                throw("Invalid schema file");
                return;
            }
            if (!schema.isValid()){
                throw("Schema is invalid");
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
        }
        if (!_doc.setContent((&file))){
            file.close();
            throw("Error set content");
            return;
        }
        file.close();
    #else
        ByteBuffer* pBuf = null;
//        String filepath = App::GetInstance()->GetAppDataPath() + filename.c_str();
        String filepath =  filename.c_str();
        //String  filepath = "/opt/apps/ctLjIIgCCj/data/config.xml";
        pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(filepath);

        /*AppLogDebug("Parser for libxml %s", pBuf->GetPointer()); */
        _doc = xmlReadFile((const char*)pBuf->GetPointer(), "UTF-8", XML_PARSE_SAX1);

       /* AppLogDebug("Parser for libxml %p ", _doc); */
    #endif
    #endif //LIBXML
    }
////////////////////////////////////////////////////////////////////////////////
    void
    Parser::Reloadfile(){
    
    #ifdef QT
        /* std::cerr<<"_Filename "<<_filename<<std::endl; */
        QFile file(QString::fromStdString(_filename));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                std::cerr<<"error file open"<<std::endl;
                throw("Invalid source file");
                return;
        }
        if (!_doc.setContent((&file))){
                file.close();
                throw("Error set content");
                return;
        }
        file.close();
    #else
        ByteBuffer* pBuf = null;
        String filepath = App::GetInstance()->GetAppDataPath() + _filename.c_str();
        pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(filepath);
        AppLogDebug("Reload file Parser for libxml %s", pBuf->GetPointer());
        _doc = xmlReadFile((const char*)pBuf->GetPointer(), "UTF-8", XML_PARSE_SAX1);
        AppLogDebug("Reload file Parser for libxml DOC %p ", _doc);
    #endif
    }
////////////////////////////////////////////////////////////////////////////////
    Parser::~Parser(){
    #ifdef LIBXML
        delete parser;
        delete validator;
    #endif
    #ifdef QT
    #else
        AppLog("Delete parser");
        xmlFreeDoc(_doc);
    #endif
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

