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


url = 'http://informer.gismeteo.ru/getcode/mtown20.php?id=%i'

#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()


#Search  id of country
cur = cu.execute("select distinct id_gismeteo_old from stations")

myrow = []
for row in cur:
    myrow += [row[0]]
    c.commit()
# Main cicle
for row in myrow:

    old_code = row
    print old_code 
    req = urllib2.Request(url % old_code, None, {'User-agent': 'Mozilla/5.0'})
    page = urllib2.urlopen(req)

    fileToSave = page.read()
    oFile = open(r"./temp.html",'wb')
    oFile.write(fileToSave)
    oFile.close
    oFile = open(r"./temp.html",'r')
    text = oFile.readlines()[20]
    massiv = string.split(text, " ")
    new_code = massiv[3][0:-1]
    oFile.close

    sql_string = 'update stations set id_gismeteo_new=%i where id_gismeteo_old=%i'  % (int(new_code), old_code)
    cu.execute(sql_string)
    c.commit()


c.close()
