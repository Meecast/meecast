/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
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
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!_doc.setContent((&file))){
            file.close();
            throw("Error set content");
            return;
        }
        file.close();
    #endif //LIBXML
    }
////////////////////////////////////////////////////////////////////////////////
    Parser::~Parser(){
    #ifdef LIBXML
        delete parser;
        delete validator;
    #endif
    }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core

