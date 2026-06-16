#!/bin/sh
echo 7 4 1 7 > /proc/sys/kernel/printk
while true
do
    top -b -n 1
    cat /proc/meminfo
    free
    sleep 10
done


