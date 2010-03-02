#!/bin/sh
#Reload omweather plugin
cat /home/user/.config/hildon-desktop/home.plugins|  grep -v "X-Desktop-File=/usr/share/applications/hildon-home/omweather-home.desktop"|grep -v "\[omweather-home.desktop-0\]" >/home/user/.config/hildon-desktop/home.plugins
sleep 2 
echo "[omweather-home.desktop-0]">>/home/user/.config/hildon-desktop/home.plugins
echo "X-Desktop-File=/usr/share/applications/hildon-home/omweather-home.desktop">>/home/user/.config/hildon-desktop/home.plugins
