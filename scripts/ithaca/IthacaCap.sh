#!/bin/bash

#############
## Config ###
#############
term=$1
crs=$2
dur=$3
numCam=$4

lck="/var/lock/rmanic.lck"

while [ -f $lck ]; do
    echo "locked...waiting $dur"
    dur=$((dur -1))
    if [ $dur -le 0 ]; then
	echo "waited to long"
	exit 0
    fi
    sleep 1
done

touch /var/lock/manic.lck
touch $lck

#location for the recordings to be stored
#record=/recordings/raw
record=~/recordings/raw

pth=$record/$term/$crs/`date +%Y%m%d%H%M%S`
pthCam=$record/$term/$crs/`date +%Y%m%d%H%M%S`/video.ogv
#log="sudo tee $pth/main.log"
log="tee $pth/main.log"

#location of capture scripts (no longer using v2u wCam gets both whiteboard and computer
cam="/home/paol/paol-scripts/rec-Ithaca/IthacaffmpegStreamCap /dev/video2 hw:2"
#cam="/home/paol/paol-scripts/rec-Ithaca/IthacaffmpegStreamCap /dev/video0 hw:0"
wCam="/home/paol/paol-scripts/rec-Ithaca/camCapture/camCap"
#v2u="/home/paol/paol-scripts/rec-Ithaca/v2u"

###################
## PRE RECORDING ##
###################

## create path
mkdir -p $pth
mkdir -p $pth/wboard
mkdir -p $pth/screen
cd $pth

####################
## Record ##########
####################

$wCam $pth/wboard/ $pth/screen/ $dur $numCam &> $pth/wCam.log &
wCamPID=$! # sets up camera
$cam $dur $pth/video.mpeg &> $pth/cam.log &
camPID=$!
#$v2u $dur screen $pth/screen &> $pth/v2u.log &
#v2uPID=$!

#record information
echo "[course]" >> $pth/INFO
echo "id: $crs" >> $pth/INFO
echo "term: $term" >> $pth/INFO
echo "" >> $pth/INFO
echo "[pres]" >> $pth/INFO
echo "start: `date +%Y,%m,%d,%k,%M,%S`" >> $pth/INFO
echo "duration: $dur" >>$pth/INFO

echo "waiting for processes to finish"
wait $wCamPID
wait $capPID
#wait $v2uPID

echo "Finished" | $log

rm -rf /var/lock/manic.lck
rm -rf $lck
