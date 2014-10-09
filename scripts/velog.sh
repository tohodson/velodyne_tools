#!/bin/bash
##########################################################################################
# FILE: velog.sh
# VERSION: 0.1
# AUTHOR: Timothy Hodson (tohodson at gmail.com)
# DATE: 10/8/2014
#
# DESCRIPTION
# Upon mounting a usb drive, this script will log packets sent from a VeloDyne LIDAR to said drive. 
#
# SETUP INSTRUCTIONS
# Create a file named 99.velo-logger.rules in /etc/udev/rules.d
# and write the following rule:
# KERNEL=="sd?1", SUBSYSTEMS=="usb", ATTRS{bInterfaceClass}=="08", SYMLINK+="logger", RUN+="/home/pi/velog.sh"
#########################################################################################

interval="2"
timestamp=$(date '+%Y%m%d-%H%M') 
logdir=/home/pi/log
vlog_file=${logdir}/${timestamp}_vlog.pcap

#set -e
#trap 'kill -TERM -$$; wait; umount /dev/logger' SIGINT SIGTERM EXIT

umount /dev/logger || echo "`date` umount failed" >> /home/pi/velog.log

mount /dev/logger/ /home/pi/log 
echo `date`" mounted logger" >> /home/pi/velog.log

tcpdump -n port 8308 or port 2368 -s 0 -i eth0 -w ${vlog_file} &

echo `date`" starting loop" >> /home/pi/velog.log

while true; do
    echo `date`" in loop" >> /home/pi/velog.log
    sleep $interval
    newsize=$(wc -c < "${vlog_file}")
    if [ $size -eq $newsize ]; then
    	echo `date`" exiting" >> /home/pi/velog.log
	kill -TERM -$$
	wait
	umount /dev/logger 2>>  /home/pi/velog.log
        exit 0 
    fi
        size="$newsize"
done
