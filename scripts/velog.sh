#!/bin/bash

# SETUP
# Create a file named 81.velo-logger.rules in /etc/udev/rules.d
# and write the rule:
# KERNEL=="sd?1", SUBSYSTEMS=="usb", ATTRS{product}=="USB DISK", SYMLINK+="logger", RUN+="/tmp/velog.sh"

interval="5"
timestamp=$(date '+%Y%j-%H:%M') 
firing_file=${timestamp}_laser.pcap 
pos_file=${timestamp}_pos.pcap

# consider a trap for SIGPIPE on disk full
tcpdump -n port 45 -s 0 -i eth0 /dev/logger/${firing_file}
tcpdump -n port 45 -s 0 -i eth0 /dev/logger/${pos_file}

while true; do
    sleep $interval
    newsize=$(wc -c < "$firing_file")
    if [ $size -eq $newsize ]; then
        #kill tcpdump 
    fi
        size="$newsize"
done
