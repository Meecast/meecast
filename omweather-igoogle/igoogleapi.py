# -*- coding: utf8 -*-
import re 
import time
import datetime
from datetime import date
from lxml import etree

icons = {
    'sunny':32,
    'mostly_sunny':30,
    'partly_cloudy':44,
    'mostly_cloudy':28,
    'chance_of_storm':39,
    'rain':40,
    'chance_of_rain':39,
    'chance_of_snow':13,
    'cloudy':26,
    'mist':20,
    'storm':12,
    'thunderstorm':17,
    'chance_of_tstorm':37,
    'sleet':16,
    'snow':14,
    'icy':25,
    'dust':19,
    'fog':20,
    'smoke':22,
    'haze':21,
    'flurries':13
    }

station = "san+francisco"

doc = etree.parse("http://www.google.com/ig/api?weather="+station+"&hl=en-gb")

name = doc.find("weather/forecast_information/city").get("data")

new = etree.Element("station", name=name, id=station, xmlns="http://omweather.garage.maemo.org/schemas")
units = etree.SubElement(new, "units")
etree.SubElement(units, "t").text = "C"
etree.SubElement(units, "ws").text = "m/s"
etree.SubElement(units, "wg").text = "m/s"
etree.SubElement(units, "d").text = "km"
etree.SubElement(units, "h").text = "%"
etree.SubElement(units, "p").text = "mmHg"

etree.SubElement(new, "timezone").text = "0"

today = doc.find("weather/forecast_information/forecast_date").get("data")
periodstart = int(time.mktime(time.strptime(today, "%Y-%m-%d")))
periodend = periodstart + 86400 

current = doc.find("weather/current_conditions")
curperiod = etree.SubElement(new, "period", start=str(periodstart), end=str(periodend-1), current="true")
for child in current:
    if child.tag == "temp_c":
        etree.SubElement(curperiod, "temperature").text = child.get("data")
    if child.tag == "wind_condition":
        match = re.search(ur"Wind: ([^ ]*) at (\d*) km/h", child.get("data"))
        etree.SubElement(curperiod, "wind_speed").text = match.groups()[1]
        etree.SubElement(curperiod, "wind_direction").text = match.groups()[0]
    if child.tag == "humidity":
        match = re.search(ur"Humidity: (\d*)%", child.get("data"))
        etree.SubElement(curperiod, "humidity").text = match.groups()[0]
    if child.tag == "condition":
        etree.SubElement(curperiod, "description").text = child.get("data")
    if child.tag == "icon":
        match = re.search(ur"/ig/images/weather/(.*).gif", child.get("data"))
        etree.SubElement(curperiod, "icon").text = str(icons[match.groups()[0]])

for element in doc.iter("forecast_conditions"):
    period = etree.SubElement(new, "period", start=str(periodstart), end=str(periodend-1)) 
    periodstart = periodend
    periodend = periodstart + 86400
    for child in element:
        if child.tag == "high":
            etree.SubElement(period, "temperature_hi").text = child.get("data")
        if child.tag == "low":
            etree.SubElement(period, "temperature_low").text = child.get("data")
        if child.tag == "condition":
            etree.SubElement(period, "description").text = child.get("data")
        if child.tag == "icon":
            match = re.search(ur"/ig/images/weather/(.*).gif", child.get("data"))
            etree.SubElement(period, "icon").text = str(icons[match.groups()[0]])

xml = etree.tostring(new, pretty_print=True, encoding='utf-8', xml_declaration=True)
fl = open("data.xml", "w")
fl.writelines(xml)
fl.close()

