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
id_region = "66" 

def main():
    #
    url = "http://foreca.com/%s/browse" %(country) 
    print url

    urllib.urlretrieve (url, "./temp.html")
    #/html/body/div/div[2]/div[4]/div/div[2]/div[4]/p/a
    stations_parser()
    doc = libxml2.htmlReadFile(r"./temp.html", "UTF-8", libxml2.HTML_PARSE_RECOVER)
    ctxt = doc.xpathNewContext()
    anchors = ctxt.xpathEval("/html/body/div/div[2]/div[4]/div/div[2]/div[4]/p/a/@href")
    for anchor in anchors:
        letter = re.split("=", anchor.content)[-1]
        new_url = url + "?bl=%s" %(letter)
        print new_url
        urllib.urlretrieve (new_url, "./temp.html")
        stations_parser()
    #os.unlink("./temp.html");
    return 0

def stations_parser():

    #connect to database
    c = db.connect(database=r"./foreca.com.db")
    cu = c.cursor()

    doc = libxml2.htmlReadFile(r"./temp.html", "UTF-8", libxml2.HTML_PARSE_RECOVER)
    ctxt = doc.xpathNewContext()
    anchors = ctxt.xpathEval("/html/body/div/div[2]/div[4]/div/div[2]/div[@class='col3']//a/@href")
    for anchor in anchors:
        #print anchor.content
        name = re.split("/", anchor.content)[-1];
        cityurl = "http://foreca.com/%s" %(anchor.content)
        urllib.urlretrieve (cityurl, "./station%s.html" %(name))
        doc1 = libxml2.htmlReadFile(r"./station%s.html" %(name), "UTF-8", libxml2.HTML_PARSE_RECOVER)
        ctxt1 = doc1.xpathNewContext()
        anchors1 = ctxt1.xpathEval("/html/body/div/div/div[4]/div/div[2]/div/div/div[2]/a")
        for anchor1 in anchors1:
            print anchor1.prop("href")
            code = re.split("=", anchor1.prop("href"))[1]
            print name, "-", code;
        cur = cu.execute("select id from stations where region_id='%s' and name = '%s'" %(id_region, name))
        station_id= None
        for row in cur:
            station_id = row[0]
        if (station_id == None):
            cur = cu.execute('insert into stations (name, region_id, code) values  ("%s", "%s", "%s")' % (name, id_region, code))
        code = None
        c.commit()
        os.unlink("./station%s.html"%(name));

    c.close()  


if __name__ == '__main__':
    main()
