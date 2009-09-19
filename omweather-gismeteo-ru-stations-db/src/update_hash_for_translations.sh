#!/bin/sh
awk -F '\"' '{ print $2 }' ../../omweather/src/hash.data >/tmp/hash1.txt
awk -F '\"' '{ print $4 }' ./hash_description.data >/tmp/hash2.txt
while read -r string1; do
	flag=0
	while read -r string2; do
		if [ "$string1" = "$string2" ]; then
			flag=1
			break
		fi
	done < /tmp/hash1.txt
	if [ "$flag" != 1 ]; then
		echo "    g_hash_table_insert(hash, \"$string1\", _(\"$string1\"));">>../../omweather/src/hash.data
	fi
done < /tmp/hash2.txt 
