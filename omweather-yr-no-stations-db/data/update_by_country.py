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
import zipfile

#Country name and code
country = "Italy"
country_code = "IT"
replacing_dict = {"Regione_Autonoma_Friuli_Venezia_Giulia":"Friuli-Venezia Giulia",
                  "Trentino-Alto_Adige":"Trentino and South-Tirol"} 
replacing_dict_after_region_filling = {  } 

#country = "Finland"
#country_code = "FI"
#replacing_dict = { "Eastern_Finland_Province":"Eastern Finland", "Lapin_Laeaeni":"Laponia",
#                           "Province_of_Western_Finland":"Western Finland", 
#                           "Province_of_Southern_Finland": "Southern Finland", "Itae-Suomen_Laeaeni": "Eastern Finland",
#                           "Southern_Finland_Province": "Southern Finland"} 
#country = "Sweden"
#country_code = "SE"
#replacing_dict = { "Vaestra_Goetalands_Laen":"Västra Götaland"} 
#replacing_dict_after_region_filling = {  } 
#country = "Germany"
#country_code = "DE"
#replacing_dict = { "Land_Niedersachsen": "Lower Saxony", "Land_Nordrhein-Westfalen":"North Rhine-Westphalia" } 
#replacing_dict_after_region_filling = {  } 
#country = "Greece"
#country_code = "GR"
#replacing_dict = { "North_Aegean":"North_Aegean", "South_Aegean": "South Aegean", "Ionian_Islands":"Ionian Islands",
#		"West_Greece":"West Greece", "Central_Greece":"Central Greece", 
#		"Central_Macedonia":"Central Macedonia", "East_Macedonia_and_Thrace":"East Macedonia and Thrace",
#		"West_Macedonia":"West Macedonia"} 
#replacing_dict_after_region_filling = {  } 
#country = "Macedonia"
#country_code = "MK"
#replacing_dict = {"Opstina Gjorce Petrov":"Ðorce Petrov", "Opstina Mavrovo i Rostusa":"Rostuša",
#		"Opstina Caska":"Caška", "Opstina Cesinovo":"Cešinovo", "Opstina Vrapciste":"Vraneštica"} 
#replacing_dict_after_region_filling = { } 
#country = "France"
#country_code = "FR"
#replacing_dict = {"Region_Bourgogne":"Burgundy", "Region_Centre":"Centre", "Region_Haute-Normandie":"Upper Normandy",
#		"Region_Basse-Normandie":"Lower Normandy", 
#		"Region_Provence-Alpes-Cote_dAzur":"Provence-Alpes-Côte d’Azur",
#		"Region_Pays_de_la_Loire":"Loire", "Region_Picardie":"Picardy"} 
#replacing_dict_after_region_filling = { } 
#country = "United kingdom"
#country_code = "gb"
#replacing_dict = {"northern_ireland": "northern ireland"} 
#replacing_dict_after_region_filling = { } 
#country = "Switzerland"
#country_code = "CH"
#replacing_dict = {"Kanton_Basel-Stadt":"Basel-Stadt", "Kanton_Appenzell_Innerrhoden": "Appenzell Innerrhoden",
#		"Kanton_St._Gallen":"Sankt Gallen", "Kanton_Appenzell_Ausserrhoden":"Appenzell Ausserrhoden"} 
#replacing_dict_after_region_filling = { } 
#country = "Poland"
#country_code = "PL"
#replacing_dict = {"Wojewodztwo_Wielkopolskie":"Greater Poland","Wojewodztwo_Malopolskie":"Lesser Poland",
#        "Wojewodztwo_Kujawsko-Pomorskie":"Kuyavian-Pomerania","Wojewodztwo_Dolnoslaskie":"Lower Silesia",
#        "Wojewodztwo_Podkarpackie":"Subcarpathia","Wojewodztwo_Podlaskie":"Podlachia",
#        "Wojewodztwo_Warminsko-Mazurskie":"Warmia-Masuria","Wojewodztwo_Zachodniopomorskie":"West Pomerania",
#        "Wojewodztwo_Swietokrzyskie":"Swiety Krzyz"
#        }
#country = "Australia"
#country_code = "AU"
#replacing_dict = {"State_of_New_South_Wales":"New South Wales", "Northern_Territory": "Northern Territory",
#                  "State_of_South_Australia":"South Australia", "Australian_Capital_Territory":"Australian Capital Territory",
#                  "State_of_Western_Australia":"Western Australia"} 

#replacing_dict_after_region_filling = { } 
#country = "China"
#country_code = "CN"
#replacing_dict = {"Inner_Mongolia_Autonomous_Region":"Nei Mongol"} 

#replacing_dict_after_region_filling = { } 
#country = "Mexico"
#country_code = "MX"
#replacing_dict = {"Estado_de_Coahuila_de_Zaragoza":"Coahuila de Zaragoza",
#		  "Estado_de_San_Luis_Potosi":"San Luis Potosí",
#		  "Estado_de_Michoacan_de_Ocampo":"Michoacán de Ocampo",
#		  "Estado_de_Queretaro_de_Arteaga":"Querétaro de Arteaga",
#		  "Estado_de_Baja_California_Sur":"Baja California Sur",
#		  "Estado_de_Baja_California":"Baja California",
#		  "Estado_de_Nuevo_Leon":"Nuevo León",
#		  "Estado_de_Quintana_Roo":"Quintana Roo",
#		  "Distrito_Federal":"Distrito Federal"}
#
#replacing_dict_after_region_filling = { } 
#

#country = "Netherlands"
#country_code = "NL"
#replacing_dict = {"Provincie_Zuid-Holland":"South Holland","Provincie_Zeeland":"Zealand",
#                  "Provincie_Noord-Holland":"North Holland", "Provincie_Noord-Brabant": "North Brabant"}

#replacing_dict_after_region_filling = { } 

#country = "Argentina"
#country_code = "AR"
#replacing_dict = {"Provincia_de_Buenos_Aires":"Buenos Aires", "Santa_Fe_Province": "Santa Fe",
#                  "Provincia_de_Santiago_del_Estero":"Santiago del Estero",
#                  "Provincia_de_Tierra_del_Fuego,_Antartida_e_Islas_del_Atlantico_Sur":"Tierra del Fuego",
#                  "Provincia_de_La_Rioja":"La Rioja", "Provincia_de_Entre_Rios":"Entre Ríos",
#                  "Provincia_de_Santa_Cruz":"Santa Cruz", "Ciudad_Autonoma_de_Buenos_Aires":"Buenos Aires",
#                  "Provincia_de_Rio_Negro":"Río Negro", "Provincia_de_La_Pampa":"La Pampa",
#                  "Provincia_de_San_Luis":"San Luis","Provincia_de_San_Juan":"San Juan"}

#country = "Hong Kong"
#country_code = "HK"
#replacing_dict = {}
#country = "Serbia"
#country_code = "RS"
#replacing_dict = {"Central_Serbia": "Central Serbia"}
                

replacing_dict_after_region_filling = { } 





baseurl = "http://download.geonames.org/export/dump/"
yrnourl = 'http://yr.no'

def normalizing3 (source):
    result = source.replace("'","%92")
    return result

def normalizing2 (source):
    result = source.replace("'","%92")
    result = result.replace(" ","_")
    return result


def normalizing (source):
    result = source.replace("'","")
    result = result.replace(" ","_")
    result = result.replace("%20","_")
    return result

def normalizing4 (source):
    result = source.replace("'","")
    result = result.replace("%20"," ")
    return result



#connect to database
c = db.connect(database=r"./yr.no.db")
cu = c.cursor()

#checking country name
cur = cu.execute("select id from countries where name='%s'" %(country))
if (cur.fetchone() == None ):
    print "Country " + country + " is absent in the database yr.no.db"
    exit (-1);

cur = cu.execute("select id from countries where name='%s'" %(country))
for row in cur:
    country_id = row[0]

#zip filename
myzipfile = country_code + ".zip"
#downloading the dump file
url = baseurl + myzipfile
urllib.urlretrieve (url, myzipfile)


#unzip file
fh = open(myzipfile, 'rb')
z = zipfile.ZipFile(fh)
outfile = open(country_code + ".txt", 'wb')
outfile.write(z.read(country_code + ".txt"))
outfile.close()
fh.close()

#fill regions
regions = {}
regions_name = {}
regions_name_second = {}
fh = open(country_code + ".txt")
for line in fh.readlines():
    pattern = re.split('(\t)', line)
    if (pattern[14] == "ADM1"):
        regions_name[pattern[20]] = normalizing(pattern[4])
        regions_name_second[pattern[20]] = normalizing(pattern[6])
#        regions_name[pattern[20]] = pattern[4]
#        regions_name_second[pattern[20]] = pattern[6]
fh.close


regions_name["00"] = "Other/" + country
print country_id
#checking regions name
for key in regions_name.keys():
    cur = cu.execute("select name, id from regions where country_id='%i' and name = '%s'" %(country_id,normalizing3(regions_name[key])))
    if (cur.fetchone() == None ):
        print regions_name_second[key]
        pattern = re.split ('(,)',regions_name_second[key])
        flag = 0 
        for variant in pattern:
        #    print variant
            cur = cu.execute("select name, id from regions where country_id='%i' and name = '%s'" %(country_id,normalizing3(variant)))
            if (cur.fetchone()):
                flag = 1
                regions_name[key] = variant
        if (replacing_dict.get(regions_name[key])):
            regions_name[key] = replacing_dict[regions_name[key]]
            flag = 1
        if (flag == 0):
            print "Error in " + key + " " + regions_name[key]
    print regions_name[key]  + ' '  + key

#filling stations
fh = open(country_code + ".txt")
for line in fh.readlines():
    pattern = re.split('(\t)', line)
    if (pattern[14] == "PPLA" or pattern[14] == "PPLC" or pattern[14] == "PPL" or pattern[14] == "STM" or pattern[14] == "ISL"  or pattern[14] == "PPLA3"  ):
        if (pattern[20] != "" and (int(pattern[28]) >= 0 or  pattern[14] == "ISL" )):
            if (regions_name.get(pattern[20]) == None):
                continue
            if (replacing_dict_after_region_filling.get(regions_name[pattern[20]])):
                fixed_regions_name = urllib.quote(replacing_dict_after_region_filling[regions_name[pattern[20]]])
            else:
                fixed_regions_name = urllib.quote(regions_name[pattern[20]])

            #check station
            u1 = urllib.quote(pattern[2])
            result = normalizing(country) + "#" + re.sub("Other/" + normalizing(country), "Other", normalizing(fixed_regions_name)) + "#" + u1.encode('utf-8') 
            country = country.encode('utf8')
            country_name_url = yrnourl + "/place/" + result.replace("#","/")
            print country_name_url
            req = urllib2.Request(country_name_url, None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
            page = urllib2.urlopen(req)
            for line2 in page.readlines():
                if (line2.find("Det har oppstått en feil") != -1):
                    u1 = urllib.quote(pattern[2])
                    result = normalizing(country) + "#" + re.sub("Other/" + normalizing(country), "Other", normalizing(fixed_regions_name)) + "#" + u1.encode('utf-8') 
                    country = country.encode('utf8')
                    country_name_url = yrnourl + "/place/" + result.replace("#","/")
                    req = urllib2.Request(country_name_url, None, {'User-agent': 'Mozilla/5.0', 'Accept-Language':'ru'})
                    page2 = urllib2.urlopen(req)

                    for line3 in page2.readlines():
                        if (line3.find("Det har oppstått en feil") != -1):
                            print "problem in " + country_name_url
                            continue
 
	    
           	cur = cu.execute("select id from regions where country_id='%i' and name = '%s'" %(country_id, regions_name[pattern[20]]))
            print "select id from regions where country_id='%i' and name = '%s'" %(country_id, regions_name[pattern[20]])
            region_id = None
            for row in cur:
                region_id = row[0]
# 	        print "Country id %i" %(country_id)
#            print pattern[20]
#            print "Regions name %s" %(regions_name[pattern[20]])
#            print region_id 
#            print  normalizing(pattern[4])
#            print "select id from stations where region_id='%i' and name = '%s'" %(region_id, normalizing(pattern[4]));
            cur = cu.execute("select id from stations where region_id='%i' and name = '%s'" %(region_id, normalizing4(pattern[4])))
            station_id= None
            for row in cur:
                station_id = row[0]
            if (station_id == None):
                cur = cu.execute("insert into stations (region_id, name, longititude, latitude, code) values ( '%i', '%s', '%s', '%s', '%s')" %(region_id, normalizing4(pattern[4]), pattern[10], pattern[8], result.replace("'","%27")))
                c.commit()
            else:
                cur = cu.execute("update stations set region_id ='%i', name='%s', longititude='%s', latitude='%s', code='%s' where id ='%i'" %(region_id, normalizing4(pattern[4]), pattern[10], pattern[8], result.replace("'","%27"), station_id))
                c.commit()
fh.close

#filling lat, lon for regions
cur = cu.execute("select id from regions where country_id='%i'" %(country_id))
cu1 = c.cursor()
for row in cur:
    region_id = row[0]
    cur1 = cu1.execute("select min(latitude) from stations where region_id='%i'" %(region_id))
    for row1 in cur1:
        minlat = row1[0]
    cur2 = cu1.execute("select max(latitude) from stations where region_id='%i'" %(region_id))
    for row2 in cur2:
        maxlat = row2[0]
    cur3 = cu1.execute("select min(longititude) from stations where region_id='%i'" %(region_id))
    for row3 in cur3:
        minlong = row3[0]
    cur4 = cu1.execute("select max(longititude) from stations where region_id='%i'" %(region_id))
    for row4 in cur4:
        maxlong = row4[0]
    # print "Id %i minlat %s maxlat %s minlong %s maxlong %s"%(region_id, minlat,  maxlat, minlong, maxlong) 
    if (minlat != None or maxlat != None or minlong != None or maxlong != None):
        cu1.execute("update regions set  latitudemax='%s', latitudemin='%s', longititudemax='%s', longititudemin='%s' where id ='%i'" %(maxlat, minlat, maxlong, minlong, region_id))
        c.commit()

