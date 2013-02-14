#!/bin/bash

preview=300x300
thumb=128x128

for slides in `find $1 -name slides.txt`; do
    dir=`dirname $slides`

    mkdir $dir/preview
    mkdir $dir/thumbs

    for slide in $dir/*.png; do 
        name=`basename $slide`
        echo "`basename $dir`:  $name"
        convert -resize $preview $slide $dir/preview/$name
        convert -resize $thumb $slide $dir/thumbs/$name
    done
done
