#!/bin/bash
cmd="python ./manage.py"

path=$(readlink -f $1)
root=/stor4/media
course=$(nawk '/id/ {print $NF;}' $path/INFO)
term=$(nawk '/term/ {print $NF;}' $path/INFO)
pres=$(sed 's/[, ] //g' $path/INFO | nawk '/start/ {print $NF;}' | sed 's/[\ ,]*//g')

# move to correct path
mkdir -p $root/$term/$course
mv $path $root/$term/$course/$pres
path=$root/$term/$course/$pres

cd $HOME/webmanic

presid=$( $cmd addpres $path/INFO | nawk '{print $NF;}' )

# add all slideshows
for slides in `find $path -name slides.txt`; do
    $cmd addsrc $presid slide $slides "|"
done

# add audio
for audio in `find $path -name *.mp3`; do
    $cmd addsrc $presid audio $audio
done

# add video
for video in `find $path/presenter -name *.mp4`; do
    $cmd addsrc $presid video $video
done

# create thumbnails
$HOME/bin/thumbs.sh $path

echo "flush_all" | nc 127.0.0.1 11211
/home/webmanic/bin/cachePres.sh
