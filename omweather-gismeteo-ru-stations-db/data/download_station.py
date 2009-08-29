#!/usr/bin/python
# -*- coding: UTF-8 -*-
# vim: expandtab sw=4 ts=4 sts=4:
#from urllib import urlopen
import urllib
import sqlite3 as db
import libxml2, sys
import os


base_url = "http://wap.gismeteo.ru/gm/normal/node/select_country_city/6/?field__assoc_4_country=%s"

#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()

#Search  id of country
cur = cu.execute("select distinct id_gismeteo_old from countries")
# Main cicle
for row in cur:

    #download list of stations
    country_id = row[0]
    print country_id
    mysock = urllib.urlopen(base_url % country_id)
    fileToSave = mysock.read()
    oFile = open(r"./%s" % country_id,'wb')
    oFile.write(fileToSave)
    oFile.close

    #normalize file
    oFile = open(r"./%s" % country_id,'r')
    iFile = open(r"./%s.xml" % country_id,'w')
    iFile.write(oFile.read().replace("&", "&amp;"))
    oFile.close
    iFile.close
    os.remove(r"./%s" % country_id)

    #parse xml file
    doc = libxml2.htmlReadFile(r"./%s.xml" % country_id, "UTF-8", libxml2.HTML_PARSE_RECOVER)
    ctxt = doc.xpathNewContext()
    anchors = ctxt.xpathEval("//body/div/a")
    for anchor in anchors:
        href = anchor.prop("href")
        if '/gm/normal/node/prognoz_type/6/?field_wmo' in href:
            print href, " ", anchor.content

    doc.freeDoc()

c.commit()
c.close()

