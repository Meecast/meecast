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



#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()
cur = cu.execute("select code,name from stations ")
all_rows = cu.fetchall()

# Main cicle
for row in all_rows:
    print row[0],row[1] 

c.close()
