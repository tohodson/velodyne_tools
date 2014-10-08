#!/bin/bash

# SETUP
# Create a file named 81.velo-logger.rules in /etc/udev/rules.d
# and write the rule:
# KERNEL=="sd?1", SUBSYSTEMS=="usb", ATTRS{product}=="USB DISK", SYMLINK+="logger", RUN+="/tmp/velog.sh"

# consider a trap for SIGPIPE on disk full
tcpdump -n port 45 -s 0 -i eth0 /dev/logger/$(date '+%Y%j-%H:%M').pcap
