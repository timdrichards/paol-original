#!/bin/bash

#plug in using following order
#1) vga2usb into usb 2.0 slot
#2) camera into usb 3.0 slot (video)
#3) camera into usb 3.0 slot (whiteboard)
sudo chmod 777 /dev/vga2usb*
~/paol-scripts/rec-Ithaca/IthacaCap.sh Spring13 207 5 2
#~/paol-scripts/rec-Ithaca/camCapture/camCap ~/recordings/raw/Spring13/207/ ~/recordings/raw/Spring13/207/ 5 2 &> wCam.log
