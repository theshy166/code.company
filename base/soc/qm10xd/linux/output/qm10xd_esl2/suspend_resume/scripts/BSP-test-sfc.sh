#!/bin/sh

sfc_test_nor(){
	echo ">>>sfc test start<<<"
        sfc_test /dev/mtd3 0x10000 > ./sfc_log.txt
        result_sfc=`cat ./sfc_log.txt | awk '{print $3}' | tail -1`
        #rm ./sfc_log.txt

        if [ "$result_sfc" = "pass" ];then
                echo "sfc_test pass"
                return 0
        else
                echo -e "\033[31merror:sfc_test fail!\033[0m"
                return 1
        fi
}

	sfc_test_nor
