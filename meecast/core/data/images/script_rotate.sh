#!/bin/sh
for i in 0 45 90 135 180 225 270 315
do
    convert -rotate $i wind_direction_arrow.png  wind_direction_arrow_$i.png
    echo $i
done
