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

#proxy
class ProxyPasswordMgr:
    def __init__(self):
        self.user = self.passwd = None
    def add_password(self, realm, uri, user, passwd):
        self.user = user
        self.passwd = passwd
    def find_user_password(self, realm, authuri):
        return self.user, self.passwd


proxy = ""
user = ""
password = ""

proxy = urllib2.ProxyHandler({"http" : proxy})
proxy_auth_handler = urllib2.ProxyBasicAuthHandler(ProxyPasswordMgr())
proxy_auth_handler.add_password(None, None, user, password)
opener = urllib2.build_opener(proxy, proxy_auth_handler)
urllib2.install_opener(opener)

#url_europe = 'http://gismeteo.by/city/catalog/countries/366/'
#url_uir = 'http://gismeteo.by/city/catalog/countries/367/'
#url_south_america = 'http://gismeteo.by/city/catalog/countries/368/'
#url_asia = 'http://gismeteo.by/city/catalog/countries/370/'
#url_asia_near_east = 'http://gismeteo.by/city/catalog/countries/371/'
#url_africa = 'http://gismeteo.by/city/catalog/countries/373/'
#url_north_america = 'http://gismeteo.by/city/catalog/countries/369/'
url_australia = 'http://gismeteo.by/city/catalog/countries/372/'
req = urllib2.Request(url_australia, None, {'User-agent': 'Mozilla/5.0'})
page = urllib2.urlopen(req)

fileToSave = page.read()
oFile = open(r"./australia.html",'wb')
oFile.write(fileToSave)
oFile.close

#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()

#Add id_gismeteo column to the regions table
#cur = cu.execute("alter table regions add gismeteo_id numeric after country_id")

#parse xml file
doc = libxml2.htmlReadFile(r"./australia.html" , "UTF-8", libxml2.HTML_PARSE_RECOVER)
ctxt = doc.xpathNewContext()
anchors = ctxt.xpathEval("//li")
count_li = 0
ind1 = 0
for anchor in anchors:
    if anchor.name == "li":
        count_li = count_li + 1
   # if count_li>14 and count_li<60: for europe
   # if count_li>14 and count_li<28: for UIR and Russia
   # if count_li>14 and count_li<39: for Asia
   # if count_li>14 and count_li<32: for Asia(Near East)
   # if count_li>14 and count_li<71: for Africa
   # if count_li>14 and count_li<52: for North and Central America
    if count_li>14 and count_li<33:
        text = anchor.content
        text_a = anchor.children
        prop = text_a.get_properties() 
        res = prop.content
        ind1 = res.find("id=")
        country_id = res[ind1+3:]
        print country_id
        sql_string = 'update regions set gismeteo_id="%d" where regions.russian_name="%s"' % (int(country_id), text)
        cu.execute(sql_string)

c.commit()
c.close()
