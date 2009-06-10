#!/usr/bin/python -tt

import csv

def parse_csv(line):
    reader = csv.reader([line], escapechar='\\' ,delimiter=';')
    return reader.next()

from sqlite3 import dbapi2 as sqlite

connection = sqlite.connect('weather.com.db')
cursor = connection.cursor()
for line in open("regions.list"):
    fields = parse_csv(line)
    stroka ="UPDATE regions  SET latitudemin='%s', longtitudemin='%s', latitudemax='%s', longtitudemax='%s' WHERE name='%s'" % (fields[3], fields[4], fields[5], fields[6], fields[0])
    cursor.execute(stroka)
connection.commit()


