#!/bin/sh
for arg in $@
do
	delay=$1;
done
if [ "$delay" ]
then
	echo "sleep $delay s"
else
	delay=5
	echo "sleep $delay s"
fi

while true;
do
cat /sys/devices/platform_bus/mc_chip_temerature | xargs echo "sensor" &
sleep $delay;
done