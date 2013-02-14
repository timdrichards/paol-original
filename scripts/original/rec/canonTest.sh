#!/bin/bash

room=$1
foo=$((gphoto2 --capture-image-and-download) 2>&1)

if [[ "$foo" == *Error* ]]; then
    echo "Camera in $room needs restarting"
fi