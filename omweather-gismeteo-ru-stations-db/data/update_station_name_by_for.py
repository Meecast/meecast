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

country_name = "Finland"
#Search  bad stations
cur = cu.execute("select name from stations where region_id = (select id from countries where name= '%s') and name == russian_name" % country_name)

myrow = []
for row in cur:
    myrow += [row[0]]
    print row[0]
c.commit()

letter = ""
# Main cicle
for row in myrow:
    letter = row.encode('utf8')[0]
    letter = letter + row.encode('utf8')[1]
    req = urllib2.Request(url % (country_name, letter), None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
    page = urllib2.urlopen(req)

    fileToSave = page.read()
    oFile = open(r"./finland%s.html"%letter,'wb')
    oFile.write(fileToSave)
    oFile.close
    letter = ""



c.close()