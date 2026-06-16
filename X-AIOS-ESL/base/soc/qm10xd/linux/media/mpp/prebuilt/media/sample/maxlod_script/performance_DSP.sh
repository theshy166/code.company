#!/bin/sh


while :

do
	echo "***************************Cat performance_DSP***************************"
	date
	cat /proc/driver/vo
	sleep 3
	cat /proc/driver/vdec
	sleep 3
	cat /proc/driver/vppu
	sleep 1
	cat /proc/driver/jpegd
	sleep 1
	cat /proc/driver/g2d
	#top -b -n 1
	sleep 30
done

