#!/bin/bash

SUBJECT="Camera in room $1 needs restart"

EMAIL="diw08@hampshire.edu"

EMAILMESSAGE="/tmp/emailmessage.txt"
echo "This is an automatic message:"> $EMAILMESSAGE
echo "The camera in $1 appears to be offline, this was likely caused by a power disruption and will likely be fixed by restarting it.">$EMAILMESSAGE
echo "Thanks!" > $EMAILMESSAGE

/usr/bin/mail -s "$SUBJECT" "$EMAIL" < $EMAILMESSAGE
