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
                        sh BSP-test-usb.sh 
                fi
		sleep 1s
                check_process "BSP-test-sdio.sh"
                if [ $? -eq 0 ] ; then
                        sh BSP-test-sdio.sh 
                fi
		sleep 1s
                check_process "BSP-test-sfc.sh"
                if [ $? -eq 0 ] ; then
                        sh BSP-test-sfc.sh 
                fi
		sleep 1s
                check_process "BSP_demo_other.sh"
                if [ $? -eq 0 ] ; then
                        sh BSP_demo_other.sh 
                fi
		sleep 1s
		echo -e "\033111111!\033[0m"
        else
		while true
		do
			sleep 1s
			check_process "BSP-test-usb.sh"
                	if [ $? -ne 0 ] ; then
                        	continue
                	fi
			check_process "BSP-test-sdio.sh"
                	if [ $? -ne 0 ] ; then
                        	continue
                	fi
			check_process "BSP-test-sfc.sh"
                	if [ $? -ne 0 ] ; then
                        	continue
                	fi
			check_process "BSP_demo_other.sh"
                	if [ $? -ne 0 ] ; then
                        	continue
                	fi
                	devmem 0x10000544 32 0x2
			
			echo -e "\03322222!\033[0m"
			break 1
		done
	fi
        sleep 2s
done
