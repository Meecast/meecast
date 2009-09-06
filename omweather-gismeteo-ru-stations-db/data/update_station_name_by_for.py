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


url = 'http://foreca.com/Europe/%s/browse?bl=%s'

#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()

country_name = "Armenia"
#Search  bad stations
cur = cu.execute("select distinct substr(name,1,1) from stations where region_id = (select id from countries where name= '%s') and name == russian_name order by name" % country_name)

myrow = []
for row in cur:
    myrow += [row[0]]
c.commit()

letter = ""
# Main cicle
for row in myrow:
    letter = row.encode('utf8')[0]
    letter = letter + row.encode('utf8')[1]
    req = urllib2.Request(url % (country_name, letter), None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
    page = urllib2.urlopen(req)

    fileToSave = page.read()
    oFile = open(r"./%s%s.html"%(country_name, letter),'wb')
    oFile.write(fileToSave)
    oFile.close

    #parse xml file
    doc = libxml2.htmlReadFile(r"./%s%s.html" % (country_name,letter), "UTF-8", libxml2.HTML_PARSE_RECOVER)
    ctxt = doc.xpathNewContext()
    anchors = ctxt.xpathEval("//div/dl/dd/a")
    for anchor in anchors:
        href = anchor.prop("href")
        name_href = href.split('/')
        name = name_href[2].replace("'","")
        russian_name = anchor.content.replace("'","")
        print name ,"-", russian_name
        cur = cu.execute('update  stations set name="%s" where russian_name="%s" and name = "%s" and region_id = (select id from countries where name= "%s")' % (name, russian_name, russian_name, country_name))
        c.commit()



    doc.freeDoc()
    os.remove(r"./%s%s.html" % (country_name, letter))

    letter = ""


c.close()