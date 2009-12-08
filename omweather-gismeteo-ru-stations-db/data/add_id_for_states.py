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

url_states = 'http://gismeteo.by/city/catalog/cities/?country=US&id=181'
req = urllib2.Request(url_states, None, {'User-agent': 'Mozilla/5.0'})
page = urllib2.urlopen(req)

fileToSave = page.read()
oFile = open(r"./states.html",'wb')
oFile.write(fileToSave)
oFile.close

#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()

#parse xml file
doc = libxml2.htmlReadFile(r"./states.html" , "UTF-8", libxml2.HTML_PARSE_RECOVER)
ctxt = doc.xpathNewContext()
anchors = ctxt.xpathEval("//li")
count_li = 0
ind1 = 0
text = ""
for anchor in anchors:
    if anchor.name == "li":
      count_li = count_li + 1
    #if count_li == 22:
    #  count_li = count_li + 1
    if count_li>15 and count_li<85:
        text = anchor.content
        if len(text) > 2:
            text_a = anchor.children
            prop = text_a.get_properties()
            res = prop.content
            ind1 = res.find("district=")
            country_id = res[ind1+9:]
            print country_id
            sql_string = 'update regions set gismeteo_id="%d" where regions.russian_name="%s" and regions.country_id = 202' % (int(country_id), text)
            cu.execute(sql_string)
 
c.commit()
c.close()
