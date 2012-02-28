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

#Country name and code
country = "Europe/Finland"
#
baseurl = "http://foreca.com/Europe/%s/browse"
url = "http://foreca.com/%s/browse" %(country) 
print url


#connect to database
c = db.connect(database=r"./foreca.com.db")
cu = c.cursor()

#urllib.urlretrieve (url, "./temp.html")
#/html/body/div/div[2]/div[4]/div/div[2]/div[4]/p/a

# /html/body/div/div[2]/div[4]/div/div[2]/div[@class='col3']//a/@href
#parse xml file
doc = libxml2.htmlReadFile(r"./temp.html", "UTF-8", libxml2.HTML_PARSE_RECOVER)
ctxt = doc.xpathNewContext()
anchors = ctxt.xpathEval("/html/body/div/div[2]/div[4]/div/div[2]/div[@class='col3']//a/@href")
for anchor in anchors:
    print anchor.content
    name = re.split("/", anchor.content)[-1];
    #urllib.urlretrieve ("http://foreca.com/%s/%si" %(country, anchor), "./station.html")
    doc1 = libxml2.htmlReadFile(r"./station.html", "UTF-8", libxml2.HTML_PARSE_RECOVER)
    ctxt1 = doc1.xpathNewContext()
    anchors1 = ctxt1.xpathEval("/html/body/div/div/div[4]/div/div[2]/div/div/div[2]/a")
    for anchor1 in anchors1:
        print anchor1.prop("href")
        code = re.split("=", anchor1.prop("href"))[1]
        print name, "-", code;
#        os.unlink("./station.html");
    break
#os.unlink("./temp.html");
#os.unlink("./temp.html");
c.close()
