#!/usr/bin/python
# -*- coding: UTF-8 -*-
# vim: expandtab sw=4 ts=4 sts=4:
#from urllib import urlopen
import urllib
import sqlite3 as db

base_url = "http://wap.gismeteo.ru/gm/normal/node/select_country_city/6/?field__assoc_4_country=%s"

#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()

#Search  id of country
cur = cu.execute("select distinct id_gismeteo_old from countries")
# Main cicle
for row in cur:
    #download list
    country_id = row[0]
    print country_id
    mysock = urllib.urlopen(base_url % country_id)
    fileToSave = mysock.read()
    oFile = open(r"./%s" % country_id,'wb')
    oFile.write(fileToSave)
    oFile.close

c.commit()
c.close()

