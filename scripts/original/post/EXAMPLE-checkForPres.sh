#!/bin/bash
lck="/var/lock/addPres.lck"
if [ -f /var/lock/manic.lck ]; then
    echo "There is an upload"
    exit 0
fi

if [ -f $lck ]; then
    echo "Already inserting"
    exit 0
fi

pres=( $(find /stor4/media/tmp -maxdepth 1 -iname "*.pres*" -type d) )
pres=${pres[0]-}

if [ "$pres" == "" ]; then
    echo "No new presentations"
    exit 0
fi


touch $lck
echo "uploading $pres"
/home/webmanic/bin/addPres.sh $pres

rm -rf $lck
