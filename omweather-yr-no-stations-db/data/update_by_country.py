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
country = "Finland"
country_code = "FI"
replacing_dict = { "Lapin_Laeaeni":"Laponia", "Province_of_Western_Finland":"Western Finland", 
                   "Province_of_Southern_Finland": "Southern Finland", "Itae-Suomen_Laeaeni": "Eastern Finland" } 
replacing_dict_after_region_filling = {  } 


baseurl = "http://download.geonames.org/export/dump/"
yrnourl = 'http://yr.no'

def normalizing2 (source):
    result = source.replace("'","%92")
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
urllib.urlretrieve (url, myzipfile)

#unzip file
fh = open(myzipfile, 'rb')
z = zipfile.ZipFile(fh)
outfile = open(country_code + ".txt", 'wb')
outfile.write(z.read(country_code + ".txt"))
outfile.close()
fh.close()

#fill regions
regions = {}
regions_name = {}
regions_name_second = {}
fh = open(country_code + ".txt")
for line in fh.readlines():
    pattern = re.split('(\t)', line)
    if (pattern[14] == "ADM1"):
        regions_name[pattern[20]] = normalizing(pattern[4])
        regions_name_second[pattern[20]] = normalizing(pattern[6])
fh.close


regions_name["00"] = "Other/" + country
print country_id
#checking regions name
for key in regions_name.keys():
    cur = cu.execute("select name, id from regions where country_id='%i' and name = '%s'" %(country_id,regions_name[key]))
    if (cur.fetchone() == None ):
        pattern = re.split ('(,)',regions_name_second[key])
        flag = 0 
        for variant in pattern:
        #    print variant
            cur = cu.execute("select name, id from regions where country_id='%i' and name = '%s'" %(country_id,variant))
            if (cur.fetchone()):
                flag = 1
                regions_name[key] = variant
        if (replacing_dict.get(regions_name[key])):
            regions_name[key] = replacing_dict[regions_name[key]]
            flag = 1
        if (flag == 0):
            print "Error in " + key + " " + regions_name[key]
    print regions_name[key]  + ' '  + key

#filling stations
fh = open(country_code + ".txt")
for line in fh.readlines():
    pattern = re.split('(\t)', line)
    if (pattern[14] == "PPLA" or pattern[14] == "PPLC" or pattern[14] == "PPL"):
        if (pattern[20] != "" and pattern[28] != "0"):
            if (regions_name.get(pattern[20]) == None):
                continue
            if (replacing_dict_after_region_filling.get(regions_name[pattern[20]])):
                fixed_regions_name = replacing_dict_after_region_filling[regions_name[pattern[20]]]
            else:
                fixed_regions_name = regions_name[pattern[20]]

            #check station
            u1 = urllib.quote(pattern[2])
            result = country + "#" + re.sub("Other/" + country, "Other", normalizing(fixed_regions_name)) + "#" + u1.encode('utf-8') 
            country = country.encode('utf8')
            country_name_url = yrnourl + "/place/" + result.replace("#","/")
#            print country_name_url
            req = urllib2.Request(country_name_url, None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
            page = urllib2.urlopen(req)
            for line2 in page.readlines():
                if (line2.find("Det har oppstått en feil") != -1):
                    u1 = urllib.quote(pattern[2])
                    result = country + "#" + re.sub("Other/" + country, "Other", normalizing(fixed_regions_name)) + "#" + u1.encode('utf-8') 
                    country = country.encode('utf8')
                    country_name_url = yrnourl + "/place/" + result.replace("#","/")
                    req = urllib2.Request(country_name_url, None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
                    page2 = urllib2.urlopen(req)

                    for line3 in page2.readlines():
                        if (line3.find("Det har oppstått en feil") != -1):
                            print "problem in " + country_name_url
                            continue
 
            cur = cu.execute("select id from regions where country_id='%i' and name = '%s'" %(country_id, regions_name[pattern[20]]))
            region_id = None
            for row in cur:
                region_id = row[0]
            cur = cu.execute("select id from stations where region_id='%i' and name = '%s'" %(region_id, normalizing(pattern[4])))
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

#filling lat, lon for regions
cur = cu.execute("select id from regions where country_id='%i'" %(country_id))
cu1 = c.cursor()
for row in cur:
    region_id = row[0]
    cur1 = cu1.execute("select min(latitude) from stations where region_id='%i'" %(region_id))
    for row1 in cur1:
        minlat = row1[0]
    cur2 = cu1.execute("select max(latitude) from stations where region_id='%i'" %(region_id))
    for row2 in cur2:
        maxlat = row2[0]
    cur3 = cu1.execute("select min(longititude) from stations where region_id='%i'" %(region_id))
    for row3 in cur3:
        minlong = row3[0]
    cur4 = cu1.execute("select max(longititude) from stations where region_id='%i'" %(region_id))
    for row4 in cur4:
        maxlong = row4[0]
    # print "Id %i minlat %s maxlat %s minlong %s maxlong %s"%(region_id, minlat,  maxlat, minlong, maxlong) 
    if (minlat != None or maxlat != None or minlong != None or maxlong != None):
        cu1.execute("update regions set  latitudemax='%s', latitudemin='%s', longititudemax='%s', longititudemin='%s' where id ='%i'" %(maxlat, minlat, maxlong, minlong, region_id))
        c.commit()

