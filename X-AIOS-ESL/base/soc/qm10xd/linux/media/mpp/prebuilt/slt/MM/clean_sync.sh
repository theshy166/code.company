#!/bin/sh

while true
do
	sync
	echo 1 > /proc/sys/vm/drop_caches
	sleep 3
done
