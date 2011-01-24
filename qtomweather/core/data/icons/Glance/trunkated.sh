#!/bin/sh
sed -i s"/^ *//"g animation.xml
sed -ni 'H;${x;s/\n//g;p}' animation.xml


