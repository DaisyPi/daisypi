#!/bin/bash
#===============================================================================
#
#          FILE:  epir.sh
# 
#         USAGE:  ./epir.sh 
# 
#   DESCRIPTION:  Reads the ePIR values to detect motion near Daisy.
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR:  Vaduva Jan Alexandru (), 
#       COMPANY:  Daisy Pi
#       VERSION:  1.0
#       CREATED:  08/12/2013 12:10:46 PM EEST
#      REVISION:  ---
#===============================================================================

#/bin/bash

#For debug purposes remove the '> /dev/null 2>&1' near the commands.
stty -F /dev/ttyAMA0 9600 cs8 -cstopb -parenb -icanon min 1 time 1 > /dev/null 2>&1

STATUS_CMD="1"
DELAY=0.5

while true
    do
        echo $STATUS_CMD > /dev/ttyAMA0
        read LINE < /dev/ttyAMA0
        echo $LINE
        sleep $DELAY
    done
