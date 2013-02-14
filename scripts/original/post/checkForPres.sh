#!/bin/bash
lck="/var/lock/addPres.lck"
UPlck="/var/lock/manic.lck"

inDir="/presentations/incoming"
addPres="/home/present/dev/post/addPres.sh"


if [ -f $UPlck ];then
    echo "Upload from processing server in progress"
    exit 0
fi

if [ -f $lck ]; then
    echo "Already inseting"
    exit 0
fi

pres=( $(find $inDir -maxdepth 1 -iname "*.pres" -type d) )
pres=${pres[0]-}

if [ "$pres" == "" ]; then
    echo "No new presentations"
    exit 0
fi

touch $lck
echo "adding $pres"
$addPres $pres

rm -rf $lck
