#!/bin/bash
#fldr is the first arg passed to script
fldr=$1

#dest config
user=webmanic
host=prussian.cs.umass.edu
sshid=/home/paol/.ssh/id_rsa

#upload files
rmt_fldr=/stor4/cap140-incoming/cap140.pres$RANDOM/
ssh -i $sshid $user@$host touch /var/lock/manic.lck
ssh -i $sshid $user@$host mkdir -p $rmt_fldr
scp -r -i $sshid $user@$host $fldr/* $user@$host:$rmt_fldr
STATUS=$?
ssh -i $sshid $user@$host rm /var/lock/manic.lck

exit $STATUS