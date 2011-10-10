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

#Country name and code
country = "Belarus"
country_code = "BY"
replacing_dict = { "Minsk":"Minsk fylke" } 


baseurl = "http://download.geonames.org/export/dump/"
yrnourl = 'http://yr.no'

def normalizing2 (source):
    result = source
    result = result.replace(" ","_")
    return result


def normalizing (source):
    result = source.replace("'","")
    result = result.replace(" ","_")
    return result


#connect to database
c = db.connect(database=r"./yr.no.db")
cu = c.cursor()

#checking country name
cur = cu.execute("select id from countries where name='%s'" %(country))
if (cur.fetchone() == None ):
    print "Country " + country + " is absent in the database yr.no.db"
    exit (-1);

cur = cu.execute("select id from countries where name='%s'" %(country))
for row in cur:
    country_id = row[0]

#zip filename
myzipfile = country_code + ".zip"
#downloading the dump file
url = baseurl + myzipfile
#urllib.urlretrieve (url, myzipfile)

#unzip file
fh = open(myzipfile, 'rb')
z = zipfile.ZipFile(fh)
outfile = open(country_code + ".txt", 'wb')
outfile.write(z.read(country_code + ".txt"))
outfile.close()
fh.close()

#fill regions
regions = {}
fh = open(country_code + ".txt")
for line in fh.readlines():
    pattern = re.split('(\t)', line)
    if (pattern[14] == "ADM1"):
        regions[pattern[20]] = pattern[4]
fh.close

regions_name = {}
regions_name_second = {}
fh = open(country_code + ".txt")
for line in fh.readlines():
    pattern = re.split('(\t)', line)
    if (pattern[14] == "PPLA" or pattern[14] == "PPLC"):
        for key in regions.keys():
            if (key == pattern[20]):
                regions_name[key] = normalizing(pattern[4]) 
                regions_name_second[key] = normalizing(pattern[6])
fh.close

for key in regions.keys():
    if (regions_name.get(key) == None): 
        regions_name[key] = regions[key] 

regions_name["00"] = "Other/" + country

#checking regions name
for key in regions_name.keys():
    cur = cu.execute("select name, id from regions where country_id='%i' and name = '%s'" %(country_id,regions_name[key]))
    if (cur.fetchone() == None ):
        pattern = re.split ('(,)',regions_name_second[key])
        flag = 0 
        for variant in pattern:
            cur = cu.execute("select name, id from regions where country_id='%i' and name = '%s'" %(country_id,variant))
            if (cur.fetchone()):
                flag = 1
                regions_name[key] = variant
        if (flag == 0):
            if (replacing_dict[regions_name[key]]):
                regions_name[key] = replacing_dict[regions_name[key]]
            else:
                print "Error in " + key + " " + regions_name[key]
    print regions_name[key]  + ' '  + key

#filling stations
fh = open(country_code + ".txt")
for line in fh.readlines():
    pattern = re.split('(\t)', line)
    if (pattern[14] == "PPLA" or pattern[14] == "PPLC" or pattern[14] == "PPL"):
        if (pattern[20] != "" and pattern[28] != "0"):
            result = country + "#" + re.sub("Other/" + country, "Other", normalizing(regions_name[pattern[20]])) + "#" + normalizing2(pattern[4].encode('utf8'))
            #check station
            country = country.encode('utf8')
            country_name_url = yrnourl + "/place/" + "/"+result.replace("#","/")
            print country_name_url
            req = urllib2.Request(country_name_url, None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
            page = urllib2.urlopen(req)
            for line2 in page.readlines():
                print line2
            #fileToSave = page.read()
            #oFile = open(r"./%s.html"%(country+result),'wb')
            #oFile.write(fileToSave)
            #oFile.close

            cur = cu.execute("select id from regions where country_id='%i' and name = '%s'" %(country_id,regions_name[pattern[20]]))
            region_id = None
            for row in cur:
                region_id = row[0]
            cur = cu.execute("select id from stations where region_id='%i' and name = '%s'" %(region_id,normalizing(pattern[4])))
            station_id= None
            for row in cur:
                station_id = row[0]
            if (station_id == None):
                cur = cu.execute("insert into stations (region_id, name, longititude, latitude, code) values ( '%i', '%s', '%s', '%s', '%s')" %(region_id, normalizing(pattern[4]), pattern[10], pattern[8], result.replace("'","%27")))
                c.commit()
            else:
                cur = cu.execute("update stations set region_id ='%i', name='%s', longititude='%s', latitude='%s', code='%s' where id ='%i'" %(region_id, normalizing(pattern[4]), pattern[10], pattern[8], result.replace("'","%27"), station_id))
                c.commit()
fh.close

