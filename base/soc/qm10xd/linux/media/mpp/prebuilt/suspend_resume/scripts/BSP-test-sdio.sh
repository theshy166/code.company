#!/bin/sh

sdio_test() {
        sd_list=" "
        for i in `ls /dev/mmc* | grep blk0`
        do
                sd_list="$sd_list ${i}"
                echo "sd-list${sd_list}"
        done

        for i in ${sd_list}
        do
        sd_dev=${i}
        done

        if [ ! -e $sd_dev ]; then
           echo "fail:  $sd_dev does not exist! "
           return 1
        fi

        touch /tmp/sd_read.bin
        touch /tmp/sd_write.bin

        dd if=/dev/urandom of=/tmp/sd_write.bin bs=64k count=4 conv=fsync >/dev/null 2>&1
        dd if=/tmp/sd_write.bin of=$sd_dev bs=64k count=4 conv=fsync >/dev/null 2>&1
        if [ "$?" -ne 0 ];then
                echo "error:SDIO: $sd_dev dd write failed!"
        fi
        sync
        sleep 1s
        sh -c "sync && echo 3 > /proc/sys/vm/drop_caches"
        dd if=$sd_dev of=/tmp/sd_read.bin bs=64k count=4 conv=fsync >/dev/null 2>&1
        if [ "$?" -ne 0 ];then
                echo "error:SDIO: $sd_dev dd read failed"
        fi
	sync
        sleep 1s

        sd_pass=`diff /tmp/sd_write.bin /tmp/sd_read.bin`
        if [ -z "$sd_pass" ];then
                echo "sdio_test pass" 
                return 0
        rm /tmp/sd_read.bin
        rm /tmp/sd_write.bin
        else
                echo "\033[31merror:sdio_test fail!\033[0m"
                return 1
        rm /tmp/sd_read.bin
        rm /tmp/sd_write.bin
        fi

}
	sdio_test
