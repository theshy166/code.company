#!/bin/sh
while true
do
	while true 
	do
			echo ">>>goto sleep<<<"
			devmem 0x24000404 32 0xffffffff
			#echo N > /sys/module/printk/parameters/console_suspend
			#echo 8 > /proc/sys/kernel/printk
			#echo 1 > /sys/power/pm_print_times
			echo 3000 > /sys/devices/platform/9700000.systimer/alarm_event
			echo mem > /sys/power/state
			break 1
			sleep 2s
	done
	sleep 1s
done
