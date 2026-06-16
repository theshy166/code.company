#!/bin/sh
while true
do
	val=$(devmem 0x1000051c 32)
	if [ "$val" = "0x00000000" ] || [ "$val" = "0x00006F6B" ]; then
		echo ">>> memory checksum pass!!!<<<<"
		devmem 0x10000518 32 0x0
		devmem 0x10000514 32 0x0
	else
		echo ">>> memory checksum failed!!!<<<<"
		exit 1
	fi
	sleep 5s
done

