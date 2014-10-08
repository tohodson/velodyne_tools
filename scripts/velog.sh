#!/bin/bash

# FILE: velog.sh
# VERSION: 0.1
# AUTHOR: Timothy Hodson (tohodson at gmail.com)
# DATE: 10/8/2014

# DESCRIPTION
# Upon mounting a usb drive, this script will log packets sent from a VeloDyne LIDAR to said drive. 

# SETUP INSTRUCTIONS
# Create a file named 99.velo-logger.rules in /etc/udev/rules.d
# and write the rule:
# KERNEL=="sd?1", SUBSYSTEMS=="usb", ATTRS{product}=="USB DISK", SYMLINK+="logger", RUN+="/tmp/velog.sh"

interval="2"
timestamp=$(date '+%Y%j-%H:%M') 
firing_file=${timestamp}_laser.pcap 
pos_file=${timestamp}_pos.pcap

set -e
trap 'kill -TERM -$$' SIGINT SIGTERM EXIT

# consider a trap for SIGPIPE on disk full
tcpdump -n port 2368 -s 0 -i eth0 /dev/logger/${firing_file} &
tcpdump -n port 8308 -s 0 -i eth0 /dev/logger/${pos_file} &

while true; do
    sleep $interval
    newsize=$(wc -c < "$firing_file")
    if [ $size -eq $newsize ]; then
        exit 0 
    fi
        size="$newsize"
done
