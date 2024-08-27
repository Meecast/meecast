#!/usr/bin/python
# -*- coding: UTF-8 -*-
# vim: expandtab sw=4 ts=4 sts=4:
import urllib.request, json
import sqlite3 as db
import sys
import os
import re
import string
import json
                

baseurl = "https://data.fmi.fi/fmi-apikey/fc0dbac7-64c8-41f1-a553-cf032e355e83/timeseries?format=json&param=name,region&lang=en&lonlat="

#connect to database
c = db.connect(database=r"./fmi.fi.db")
cu = c.cursor()

#filling lat, lon for regions
cur = cu.execute("select id,latitude,longititude from stations")
cu1 = c.cursor()
for row in cur:
    _id = row[0]
    _latitude = row[1]
    _longititude = row[2]
    url_string = baseurl + str(_longititude) + "," + str(_latitude) 
    print (url_string)
    with urllib.request.urlopen(url_string) as url:
        data = json.load(url)
        result_string = str(data[0]['region']) + '/' + data[0]['name']
        cu1.execute("update stations set code='%s' where id ='%i'" %(result_string, _id))
        c.commit()

#cur = cu.execute("select id,code from stations where code like 'Finland%'")
#cu1 = c.cursor()
#for row in cur:
#    _id = row[0]
#    new_code = row[1].split('%2F')[1]
#    print(new_code)
#    cu1.execute("update stations set code='%s' where id ='%i'" %(new_code, _id))
#    c.commit()
#
