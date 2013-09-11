#!/usr/bin/python
# -*- coding: UTF-8 -*-
# vim: expandtab sw=4 ts=4 sts=4:
#from urllib import urlopen
import urllib2
import sqlite3 as db
import libxml2, sys
import os
import re
import string


url = 'http://www.gismeteo.com/city/daily/%s'

#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()


#Search  id of country
cur = cu.execute("select distinct id_gismeteo_new from stations where (name == russian_name)")

myrow = []
for row in cur:
    myrow += [row[0]]
    c.commit()
# Main cicle
for row in myrow:
    code = row
    print code 

    req = urllib2.Request(url % code, None, {'User-agent': 'Mozilla/5.0'})
    page = urllib2.urlopen(req)

    fileToSave = page.read()
    oFile = open(r"./temp.html",'wb')
    oFile.write(fileToSave)
    oFile.close
    #parse xml file
    doc = libxml2.htmlReadFile(r"./temp.html" , "UTF-8", libxml2.HTML_PARSE_RECOVER + libxml2.HTML_PARSE_NOERROR + libxml2.HTML_PARSE_NOWARNING)
    ctxt = doc.xpathNewContext()
    anchors = ctxt.xpathEval("/html//div/h3/text()")
    for anchor in anchors:
        print "test"
        print anchor
        sql_string = 'update stations set name="%s" where id_gismeteo_new="%i"'  % (anchor, code)
        print sql_string
        cu.execute(sql_string)
        c.commit()

    os.unlink("./temp.html"); 



c.close()
