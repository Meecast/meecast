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


#ifndef PARSER_H
#define PARSER_H
////////////////////////////////////////////////////////////////////////////////
#ifdef LIBXML
    #include <libxml++/libxml++.h>
    #include <libxml++/validators/schemavalidator.h>
#else
    #ifdef QT   
        #include <QtXmlPatterns/QXmlSchema>
        #include <QtXmlPatterns/QXmlSchemaValidator>
        #include <QtXml/QDomDocument>
        #include <QFile>
    #else
        #include <libxml/parser.h>
    #endif
#endif
#include <string>
#include <iostream>
#include "abstractconfig.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Parser : public AbstractConfig{
        protected:
            #ifdef LIBXML
                xmlpp::DomParser *parser;
                xmlpp::SchemaValidator *validator;
            #else
                #ifdef QT
                    QDomDocument _doc;
                #else
                    xmlDocPtr _doc;
                #endif
            #endif
            Parser(const std::string& filename, const std::string& schema_filename);
            Parser();
        public:
            virtual ~Parser();
            #ifdef LIBXML
            virtual void processNode(const xmlpp::Node* node) = 0;
            #endif
    };

} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // PARSER_H
