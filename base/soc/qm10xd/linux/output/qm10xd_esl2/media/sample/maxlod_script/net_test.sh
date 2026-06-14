#!/bin/sh
 
ethn=$1

serverip=10.0.38.1

#mkdir /nfs/net_test

#ip_gateway=`ifconfig | grep "inet" | awk '{print $2}' | awk -F ":" '{print $2}' | tail -1 | awk -F "." '{print $3}'`
#if [ "$ip_gateway" = 1 ];then
#	serverip=192.168.1.1
#fi

while true
do
	ping -I eth1 "$serverip"
	sleep 2
	#ping -I eth1 "$serverip" -s 65500 > /nfs/net_test/eth1_ping.log 2>&1
	#echo "net test :"`cat /nfs/net_test/eth1_ping.log | awk '{print $5}'`"
done
