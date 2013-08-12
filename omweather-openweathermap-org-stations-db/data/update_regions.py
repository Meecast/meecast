#!/usr/bin/python
# -*- coding: UTF-8 -*-
# vim: expandtab sw=4 ts=4 sts=4:
#from urllib import urlopen
import urllib2
import urllib
import sqlite3 as db
import libxml2, sys
import os
import re
import string
import zipfile
country = "Sweden"
country_code = "SE"
                

def normalizing4 (source):
    result = source.replace("'","")
    result = result.replace("%20"," ")
    return result

def normalizing (source):
    result = source.replace("'","")
#    result = result.replace(" ","_")
    result = result.replace("%20","_")
    return result

def normalizing3 (source):
    result = source.replace("'","%92")
    return result



#timezoneurl=" http://api.geonames.org/timezone?lat=47.01&lng=10.2&username=demo"

baseurl = "http://download.geonames.org/export/dump/"
openweathrmapurl = "http://api.openweathermap.org/data/2.5/weather?id=" 

#connect to database
c = db.connect(database=r"./openweathermap.org.db")
cu = c.cursor()
#filling lat, lon for regions
cur = cu.execute("select id from regions" )
cu1 = c.cursor()
for row in cur:
    region_id = row[0]
    cur1 = cu1.execute("select min(latitude) from stations where region_id='%i'" %(region_id))
    for row1 in cur1:
        print "RegionId %i "%(region_id)
        print "Minlat %f"%(row1[0])
        minlat = row1[0] - 1
    cur2 = cu1.execute("select max(latitude) from stations where region_id='%i'" %(region_id))
    for row2 in cur2:
        maxlat = row2[0] + 1
    cur3 = cu1.execute("select min(longititude) from stations where region_id='%i'" %(region_id))
    for row3 in cur3:
        minlong = row3[0] - 1
    cur4 = cu1.execute("select max(longititude) from stations where region_id='%i'" %(region_id))
    for row4 in cur4:
        maxlong = row4[0] + 1
    # print "Id %i minlat %s maxlat %s minlong %s maxlong %s"%(region_id, minlat,  maxlat, minlong, maxlong) 
    if (minlat != None or maxlat != None or minlong != None or maxlong != None):
        cu1.execute("update regions set  latitudemax='%s', latitudemin='%s', longititudemax='%s', longititudemin='%s' where id ='%i'" %(maxlat, minlat, maxlong, minlong, region_id))
        c.commit()

