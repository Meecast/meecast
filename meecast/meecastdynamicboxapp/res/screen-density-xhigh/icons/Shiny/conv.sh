#!/bin/bash


for i in `ls -C1 *.png`
do
convert $i -resize 128x128 $i
done
