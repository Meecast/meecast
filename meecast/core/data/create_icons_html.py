#!/usr/bin/python
# -*- coding: UTF-8 -*-
# vim: expandtab sw=4 ts=4 sts=4:
import os

print "<html>"

dirList=os.listdir("./icons")
#for fname in dirList:
#    print fname

print " <table>"
text_file = open("icons_description.txt", "r")
lines = text_file.readlines()
for line in lines:
    words = line.split('-')
    print "  <tr bgcolor='#999999'>"
    print "<td>" + words[0] + "</td>" + "<td> " + words[1] + "</td>"
    for fname in dirList:
        print "<td><img src='icons/" + fname + "/" +  words[0].strip() + ".png' width='128' height='128' ></td>"

    print "  </tr>"
text_file.close()


print " </table>"
print "</html>"
