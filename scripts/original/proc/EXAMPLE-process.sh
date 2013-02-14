#!/bin/bash

lck="/var/lock/manic.lck";

if [ -f $lck ]; then
    echo "Locked"
    exit 0
fi

if [ "$1" == "" ]; then
    next=( $(find /recordings/raw/S12 -iname "20*" -type d) )
    next=${next[0]}
else
    next=$1;
fi

if [ "$next" == "" ]; then
    echo "Nothing to do"
    exit 0
fi

touch $lck

echo "running process"

outDir="/forUpload/$(basename $next)" 
presName="$(basename $next)"

echo "Will output to: $outDir"

echo "Making dirs"
mkdir $outDir
mkdir $outDir/logs
mkdir $outDir/computerVideo
mkdir $outDir/computerVideoFrames
mkdir $outDir/presenter
mkdir $outDir/screen
mkdir $outDir/whiteboard

echo "Copying logs"
cp $next/*.log $outDir/logs/
echo "Copying video"
if [ "$(ls -A $next/video.mpeg)" ]; then
    ffmpeg -i $next/video.mpeg -an -pass 1 -vcodec libx264 -vpre slowfirstpass -b 200k -threads 8 -f rawvideo -y /dev/null 
    ffmpeg -i $next/video.mpeg -an -pass 2 -vcodec libx264 -vpre normal -b 200k -threads 8 -y $outDir/presenter/video.mp4
    ffmpeg -i $next/video.mpeg -an -vcodec libtheora -threads 8 -y $outDir/presenter/video.ogv
fi
echo "Copying INFO"
cp $next/INFO $outDir/INFO

exe="/home/paol/paol-threads/PaolThreads2"
args=" -O $outDir/ -I $outDir/INFO"

if [ "$(ls -A $next/screen)" ]; then
    screenDir="$next/screen/" 
    screenFirst="$(ls $next/screen/screenscreen000001*)"
    screenFirst="$(basename $screenFirst)"
    echo "Will process screen: $screenDir $screenFirst"
    args="$args --Cdir $screenDir --Cfirst $screenFirst"
else
    echo "No screen to process"
fi

if [ "$(ls -A $next/wboard)" ]; then
    wboardDir="$next/wboard/" 
    wboardFirst="$(ls $next/wboard/wboardwboard000001*)"
    wboardFirst="$(basename $wboardFirst)"
    echo "Will process WB: $wboardDir $wboardFirst"
    args="$args --WBdir $wboardDir --WBfirst $wboardFirst"
else
    echo "No screen to process"
fi

echo "Launching PaolThreads2, this will take a while"
$exe $args > $outDir/proc.log
echo "PaolThreads2 ended"

if [ "$(ls -A $outDir/computerVideoFrames)" ]; then
    screenDir="$outDir/computerVideoFrames/" 
    screenFirst="$(ls $screenDir/ForCompMovie_000001*)"
    screenFirst="$(basename $screenFirst)"
    echo "Will make screen movie with: $screenDir $screenFirst"
    cd $outDir/computerVideoFrames/
    ffmpeg -r 15 -i ForCompMovie_%06d.png -an -pass 1 -vcodec libx264 -vpre slowfirstpass -b 200k -threads 8 -f rawvideo -y /dev/null 
    ffmpeg -r 15 -i ForCompMovie_%06d.png -an -pass 2 -vcodec libx264 -vpre normal -b 200k -threads 8 -y $outDir/computerVideo/video.mp4
    ffmpeg -r 15 -i ForCompMovie_%06d.png -an -vcodec libtheora -threads 8 -y $outDir/computerVideo/video.ogv
    #ffmpeg -r 15 -i ForCompMovie_%06d.png $outDir/computerVideo/uncompressed.avi
    cp $outDir/computerVideoFrames/screenVideo.txt $outDir/computerVideo/screenVideo.txt
    rm -rf $outDir/computerVideoFrames/
else
    echo "No screen video to make"
fi

echo "Uploading"

/home/paol/proc/upload.sh $outDir


upload="/recordings/uploaded/$presName/"
mkdir -p `dirname $upload`
mv $next $upload
echo "Uploaded and moved to /recordings/uploaded"

rm -rf $lck