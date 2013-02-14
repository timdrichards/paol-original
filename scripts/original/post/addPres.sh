#!/bin/bash

host="seeshare.net"
path=$(readlink -f $1)
root=/presentations
course=$(nawk '/id/ {print $NF;}' $path/INFO)
term=$(nawk '/term/ {print $NF;}' $path/INFO)
pres=$(sed 's/[, ] //g' $path/INFO | nawk '/start/ {print $NF;}' | sed 's/[\ ,]*//g')

mkdir -p $root/$term/$course
mv $path $root/$term/$course/$pres
path=$root/$term/$course/$pres

json="$path/pres.json"

touch $json

echo '{"host" : "$host",' >> $json
echo ' "class" : "/$term/$course/$pres",' >> $json
echo ' "base"  : "/$root/$term/$course/$pres",' >> $json
echo ' "video" : {"ogv" : "presenter/video.ogv",' >> $json
echo '            "mp4" " "presenter/video.mp4"},' >> $json
