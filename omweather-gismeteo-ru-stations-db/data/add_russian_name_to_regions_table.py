#!/usr/bin/python
# -*- coding: UTF-8 -*-
# vim: expandtab sw=4 ts=4 sts=4:
#from urllib import urlopen
import urllib2
import sqlite3 as db
#import libxml2, sys
import os
import re
import string


#connect to database
c = db.connect(database=r"./gismeteo.ru.db")
cu = c.cursor()

#Add russian_name column to the regions table
#cur = cu.execute("alter table regions add russian_name text")
#c.commit()
#cu = c.cursor()
cur = cu.execute("update regions set russian_name = (select countries.russian_name from countries where countries.id = regions.country_id) where exists (select countries.russian_name from countries where countries.id = regions.country_id)");
c.commit()
c.close()
