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
            region_name = code[0] + code[1] + code[2]
            region_code = "13"
            if (region_name == "IDN"):
                region_code = "302" 
            if (region_name == "IDV"):
                region_code = "303"
            if (region_name == "IDQ"):
                region_code = "304"
            if (region_name == "IDW"):
                region_code = "305"
            if (region_name == "IDS"):
                region_code = "306"
            if (region_name == "IDT"):
                region_code = "307"
            if (region_name == "IDD"):
                region_code = "309"

            cur = cursor.execute("select id from stations where region_id='%s' and name = '%s'" %(region_code, xmldoc.xpathEval('/product/forecast/area[@type="location"][%i]/@description'%(i))[0].content))
            station_id= None
            for row in cur:
                station_id = row[0]
            if (station_id == None):
                cur = cursor.execute('insert into stations (name, region_id, code) values  ("%s", "%s", "%s")' % (xmldoc.xpathEval('/product/forecast/area[@type="location"][%i]/@description'%(i))[0].content, region_code, code))
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
