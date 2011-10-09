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



baseurl = "http://download.geonames.org/export/dump/"

#connect to database
c = db.connect(database=r"./yr.no.db")
cu = c.cursor()

#cheking country name
cur = cu.execute("select name, id from countries where name='%s'" %(country))
if (cur.fetchone() == None ):
   print "Country " + country + " is absent in the database yr.no.db"
   exit (-1);

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

fh = open(country_code + ".txt")
#pattern = re.compile("/t*ADM1*")
for line in fh.readlines():
    pattern = re.split('(\t+)', line)
    if (pattern[14] == "ADM1"):
        print pattern[4]
#   if  (pattern.match(line)):
#        print line
