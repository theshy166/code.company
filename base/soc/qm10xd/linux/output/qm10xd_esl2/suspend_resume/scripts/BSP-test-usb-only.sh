#!/bin/sh
check_process() {
    count=$(ps aux | grep -v "grep" | grep "$1" | wc -l)

    if [ $count -gt 0 ]; then
        echo " $1 active"
        return 1
    else
        return 0
    fi
}

while true
do
        val=$(devmem 0x10000544)
        if [ "$val" = "0x00000000" ] ; then
                check_process "BSP-test-usb.sh"
                if [ $? -eq 0 ] ; then
                        sh BSP-test-usb.sh &
                fi
		sleep 1s
        else
		while true
		do
			sleep 1s
			check_process "BSP-test-usb.sh"
                	if [ $? -ne 0 ] ; then
                        	continue
                	fi
                	devmem 0x10000544 32 0x2
			break 1
		done
	fi
        sleep 2s
done
