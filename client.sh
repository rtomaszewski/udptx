#!/bin/bash
# Copyright (C) 2012 Radoslaw Tomaszewski
# License: GPL
#
# Description:
# Sent data over UDP connection by using the source UDP port for payload transmission
#
# Execution:
#  ./client.sh <ip> <msg>
#    <ip>  - represent the server that can receive the packets 
#    <msg> - the data we want to sent to server <ip>
#
# Example:
#  client.sh 127.0.0.1 'abc'
#

ip="$1"
msg="$2"

for i in  $(seq 0 ${#msg}) ; do 
  msg_elem=$(printf '%d' "'${msg:$i:1}")
  src_port=$(( 53 + 100 * $msg_elem ))
  hping2 -D -c 1 --udp -s $src_port -p 5555 $ip; 
done
