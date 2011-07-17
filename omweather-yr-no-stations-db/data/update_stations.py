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


baseurl = 'http://www.yr.no'
#connect to database
c = db.connect(database=r"./yr.no.db")
cu = c.cursor()

cur = cu.execute("select name, id from countries")

for row in cur:
    # Search country region
    country_name = row[0]
    id = row[1]
    country_name = country_name.encode('utf8')
    country_name_url = baseurl + "/place/" + country_name.replace(" ","_")
    print country_name_url
    req = urllib2.Request(country_name_url, None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
    page = urllib2.urlopen(req)

    fileToSave = page.read()
    oFile = open(r"./%s.html"%(country_name),'wb')
    oFile.write(fileToSave)
    oFile.close
    #parse xml file
    doc = libxml2.htmlReadFile(r"./%s.html" % (country_name), "UTF-8", libxml2.HTML_PARSE_RECOVER)
    ctxt = doc.xpathNewContext()
    anchors = ctxt.xpathEval("//div[@class='yr-list-places clear clearfix']/dl/dd/a")
    for anchor in anchors:
        cu1 = c.cursor()
        cur1 = cu1.execute('insert into regions (name, country_id) values  ("%s", "%s")' % (anchor.content, id))
        c.commit()
        cur1 = cu1.execute('select id from regions where name="%s" and country_id="%s"' % (anchor.content, id))
        for row1 in cur1:
            id_region = row1[0]
        c.commit()
        region_name_url = baseurl + anchor.prop("href")
        req = urllib2.Request(region_name_url, None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
        page = urllib2.urlopen(req)

        fileToSave = page.read()
        oFile = open(r"./%s.html"%(anchor.content),'wb')
        oFile.write(fileToSave)
        oFile.close
        #parse xml file
        doc = libxml2.htmlReadFile(r"./%s.html" % (anchor.content), "UTF-8", libxml2.HTML_PARSE_RECOVER)
        ctxt = doc.xpathNewContext()
        anchors1 = ctxt.xpathEval("//table[@class='yr-table yr-table-places yr-popup-area']/tbody/tr/th/a")
        for anchor1 in anchors1:
            print anchor1.content 
            print anchor1.prop("href")
            code = anchor1.prop("href")
            code = code.replace("/place/", "")
            print code
            cu2 = c.cursor()
            cur2 = cu2.execute('insert into stations (name, region_id, code) values  ("%s", "%s", "%s")' % (anchor1.content, id_region, code))
            c.commit()

        #print region_name_url 
        #print anchor.content 
        #print id_region

c.commit()
#    #Search  bad stations
#    cur = cu.execute("select distinct substr(name,1,1) from stations where region_id = (select id from regions where name= \"%s\") and name == russian_name order by name" % country_name)
#
#    myrow = []
#    for row in cur:
#        myrow += [row[0]]
#    c.commit()
#
#    letter = ""
#    # Main cicle
#    for row in myrow:
#        letter = row.encode('utf8')[0]
#        letter = letter + row.encode('utf8')[1]
#        print letter
#        country_name_url = country_name.replace(" ","_")
#        print         country_name
#        req = urllib2.Request(url % (country_name_url, letter), None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
#        page = urllib2.urlopen(req)
#
#        fileToSave = page.read()
#        oFile = open(r"./%s%s.html"%(country_name_url, letter),'wb')
#        oFile.write(fileToSave)
#        oFile.close
#
#        #parse xml file
#        doc = libxml2.htmlReadFile(r"./%s%s.html" % (country_name_url,letter), "UTF-8", libxml2.HTML_PARSE_RECOVER)
#        ctxt = doc.xpathNewContext()
#        anchors = ctxt.xpathEval("//div/dl/dd/a")
#        for anchor in anchors:
#            href = anchor.prop("href")
#            name_href = href.split('/')
#            name = name_href[2].replace("'","")
#            russian_name = anchor.content.replace("'","")
#            print name ,"-", russian_name
#            cur = cu.execute('update  stations set name="%s" where russian_name="%s" and name = "%s" and region_id = (select id from regions where name= "%s")' % (name, russian_name, russian_name, country_name))
#            c.commit()
#
#
#
#        doc.freeDoc()
#        os.remove(r"./%s%s.html" % (country_name_url, letter))
#
#        letter = ""


c.close()
