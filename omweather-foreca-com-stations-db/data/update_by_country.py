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
country = "Europe/Ukraine"
id_region = "198" 

def normalizing (source):
    result = source.replace("'","")
    return result

def main():
    #
    url = "http://foreca.com/%s/browse" %(country) 
    print url

    urllib.urlretrieve (url, "./temp.html")
    stations_parser()
    doc = libxml2.htmlReadFile(r"./temp.html", "UTF-8", libxml2.HTML_PARSE_RECOVER)
    ctxt = doc.xpathNewContext()
    anchors = ctxt.xpathEval("/html/body/div/div/div[4]/div/div[2]/div[4]/p/a/@href")
    for anchor in anchors:
        letter = re.split("=", anchor.content)[-1]
        #if (letter < 'K'):
        #    continue
        new_url = url + "?bl=%s" %(letter)
        print new_url
        urllib.urlretrieve (new_url, "./temp.html")
        stations_parser()
#    os.unlink("./temp.html");
    doc.freeDoc()
    return 0

def stations_parser():

    #connect to database
    c = db.connect(database=r"./foreca.com.db")
    cu = c.cursor()

    doc = libxml2.htmlReadFile(r"./temp.html", "UTF-8", libxml2.HTML_PARSE_RECOVER) 
    ctxt = doc.xpathNewContext()
    anchors = ctxt.xpathEval("/html/body/div/div/div[4]/div/div[2]/div[@class='col3']//a/@href")
    anchors2 = ctxt.xpathEval("/html/body/div/div/div[4]/div/div[2]/div[@class='col3']//a/text()")
    i = 0
    for anchor in anchors:
        #print anchor.content
        #print anchors2[i]
        name = normalizing(re.split("/", anchor.content)[-1])
        cityurl = "http://foreca.com/%s" %(anchor.content)
        urllib.urlretrieve (cityurl, "./station%s.html" %(name))
        doc1 = libxml2.htmlReadFile(r"./station%s.html" %(name), "UTF-8", libxml2.HTML_PARSE_RECOVER +
                                                                          libxml2.HTML_PARSE_NOERROR +
                                                                          libxml2.HTML_PARSE_NOWARNING)
        ctxt1 = doc1.xpathNewContext()
        anchors1 = ctxt1.xpathEval("/html/body/div/div/div[4]/div/div[2]/div/div/div[2]/a")
        for anchor1 in anchors1:
            if anchor1.prop("href"):
                break
        if (anchor1.prop("href").find("=") == -1):
            continue
        code = re.split("=", anchor1.prop("href"))[1]
        print name, "-", code;
        real_name = ""
        real_name = anchors2[i].content
        real_name = normalizing(real_name)
        cur = cu.execute("select id from stations where region_id='%s' and name = '%s'" %(id_region, real_name))
        station_id= None
        for row in cur:
            station_id = row[0]
        if (station_id == None):
            cur = cu.execute('insert into stations (name, region_id, code) values  ("%s", "%s", "%s")' % (real_name, id_region, code))
        code = None
        c.commit()
        i = i + 1
        os.unlink("./station%s.html"%(name));

    c.close()  
    doc.freeDoc()


if __name__ == '__main__':
    main()
