#!/bin/sh
while true
do
	devmem 0x10000544 32 0x1
	while true 
	do
		val=$(devmem 0x10000544 32)
		if [ "$val" = "0x00000002" ] ; then
			echo ">>>goto sleep<<<"
			echo 10000 > /sys/devices/platform/9700000.systimer/alarm_event
			echo mem > sys/power/state
			devmem 0x10000544 32 0x0
			break 1
		fi
		sleep 2s
	done
	sleep 10s
done
