#!/bin/bash

#############
## Config ###
#############
term=$1
crs=$2
dur=$3

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


record=/recordings/raw

pth=$record/$term/$crs/`date +%Y%m%d%H%M%S`
pthCam=$record/$term/$crs/`date +%Y%m%d%H%M%S`/video.ogv
log="sudo tee $pth/main.log"

cam="/home/paol/rec/ffmpegStreamCap /dev/video0 hw:1"
canon="/home/paol/rec/gphotoCanonCap"
v2u="/home/paol/rec/v2u"

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

$cam $dur $pth/video.mpeg &> $pth/cam.log &
camPID=$!
$canon $pth/wboard/ usb:002,025 $dur wboard &> $pth/canon.log &
canonPID=$!
$v2u V2U15139 $dur screen $pth/screen &> $pth/v2u.log &
v2uPID=$!

#record information
echo "[course]" >> $pth/INFO
echo "id: $crs" >> $pth/INFO
echo "term: $term" >> $pth/INFO
echo "" >> $pth/INFO
echo "[pres]" >> $pth/INFO
echo "start: `date +%Y,%m,%d,%k,%M,%S`" >> $pth/INFO
echo "duration: $dur" >>$pth/INFO

echo "waiting for processes to finish"
wait $capPID
wait $canonPID
wait $v2uPID

echo "Finished" | $log

rm -rf /var/lock/manic.lck
rm -rf $lck
