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


url = 'http://ru.wikipedia.org/w/index.php?title=%D0%A1%D0%BF%D0%B8%D1%81%D0%BE%D0%BA_%D0%B3%D0%BE%D1%80%D0%BE%D0%B4%D0%BE%D0%B2_%D0%A4%D0%B8%D0%BD%D0%BB%D1%8F%D0%BD%D0%B4%D0%B8%D0%B8&printable=yes'


req = urllib2.Request(url, None, {'User-agent': 'Mozilla/5.0'})
page = urllib2.urlopen(req)

fileToSave = page.read()
oFile = open(r"./finland.html",'wb')
oFile.write(fileToSave)
oFile.close
#parse xml file
doc = libxml2.htmlReadFile(r"./finland.html" , "UTF-8", libxml2.HTML_PARSE_RECOVER)
ctxt = doc.xpathNewContext()
anchors = ctxt.xpathEval("//table")
table = False
count_of_table = 0
for anchor in anchors:
    if anchor.name == "table":
        count_of_table = count_of_table +1
    if count_of_table == 3:
        text = anchor.content

a = re.compile('\d+')
count_of_row = 1
russian_name = ""
for stroka in string.split(text, "\n"):
  
  if count_of_row == 1:
      russian_name = stroka
  if count_of_row == 2:
      name = stroka
  if count_of_row == 4:
      if stroka != "Год основания":
          if not a.match(stroka):
              russian_name = stroka
              count_of_row = 1
  count_of_row = count_of_row + 1
  if (count_of_row >4):
      count_of_row = 1
      print russian_name,"-", name

#            print a.search(href).group()

