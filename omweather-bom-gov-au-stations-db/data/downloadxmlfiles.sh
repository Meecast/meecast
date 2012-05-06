#!/bin/sh
mkdir data
cd data
wget  ftp://ftp2.bom.gov.au/anon/gen/fwo/ID*[0-9].xml
cd ..
