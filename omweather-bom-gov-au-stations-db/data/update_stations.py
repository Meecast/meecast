#!/usr/bin/python
# -*- coding: UTF-8 -*-
# vim: expandtab sw=4 ts=4 sts=4:
import sqlite3 as db
import libxml2, sys
import os
import re
import string


def search_and_add_stations (filename, cursor):
    xmldoc = libxml2.parseDoc(open("data/"+filename, 'r').read())
    nodes = xmldoc.xpathEval('product/forecast/area[@type="location"]')
    i = 0
    code = re.split(".xml", filename)[0]
    for node in nodes:
        anchors = xmldoc.xpathEval('/product/forecast/area[@type="location"][%i]/forecast-period[@index="2"]'%(i))
        if (anchors):
            cur = cursor.execute("select id from stations where region_id='13' and name = '%s'" %(xmldoc.xpathEval('/product/forecast/area[@type="location"][%i]/@description'%(i))[0].content))
            station_id= None
            for row in cur:
                station_id = row[0]
            if (station_id == None):
                cur = cursor.execute('insert into stations (name, region_id, code) values  ("%s", "%s", "%s")' % (xmldoc.xpathEval('/product/forecast/area[@type="location"][%i]/@description'%(i))[0].content, "13", code))
        i = i + 1
    xmldoc.freeDoc()
    return 

def main():
    #connect to database
    c = db.connect(database=r"./bom.gov.au.db")
    cu = c.cursor()

    for filename in os.listdir("./data"):
        search_and_add_stations(filename, cu)

    c.commit()
    c.close()  
    exit (0);


if __name__ == '__main__':
    main()
