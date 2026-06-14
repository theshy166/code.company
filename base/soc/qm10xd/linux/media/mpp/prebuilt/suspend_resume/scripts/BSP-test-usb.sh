#!/bin/sh

udisk_id="/dev/sda"

usb_test(){
	echo -e "\033[47;30m>>>usb_test start<<<\033[0m"
	if [ -b "$udisk_id" ] ; then
	#	mkfs.vfat $udisk_id
                dd if=/dev/urandom of=usb_write.bin bs=64k count=4 conv=fsync >/dev/null 2>&1
		sync
                #md5sum /slt_file/usb_write.bin
                dd if=usb_write.bin of=$udisk_id bs=64k count=4 conv=fsync >/dev/null 2>&1
                if [ "$?" -ne 0 ];then
                        echo -e "\033[31merror:USB$udisk_id dd write failed\033[0m"
                        return 1
                fi
                sync
		sleep 1s
                sh -c "sync && echo 3 > /proc/sys/vm/drop_caches"
                time dd if=$udisk_id of=usb_read.bin bs=64k count=4 conv=fsync >/dev/null 2>&1
		sync
		sleep 1s
                #md5sum /slt_file/usb_read.bin
                if [ "$?" -ne 0 ];then
                        echo -e "\033[31merror:USB$udisk_id dd read failed\033[0m"
                        return 1
                fi
                let udisk_count=${udisk_count}+1
		sh -c "sync && echo 3 > /proc/sys/vm/drop_caches"
                usb_pass=`diff usb_write.bin usb_read.bin`
                if [ -z "$usb_pass" ];then
			echo -e "\033[31>>>musb_test pass<<<\033[0m"
                        rm usb_read.bin
                        rm usb_write.bin
                else
                        echo -e "\033[31merror:usb_test contrast failed\033[0m" 
			exit 1
                        rm usb_read.bin
                        rm usb_write.bin
                        return 1
                fi
		return 0

	else
		echo -e "\033[47;30m>>>not find usb devices<<<\033[0m"
	fi
}

	usb_test
