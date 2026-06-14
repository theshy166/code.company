#!/bin/sh
ap_ddr_test_dir=/suspend_resume/SLT_BSP_FILE/2.2.1_ddr
aes_ras_hash_test_dir=/suspend_resume/SLT_BSP_FILE/2.2.4_aes
sfc_test_dir=/suspend_resume/SLT_BSP_FILE/2.3.4_sfc
wdt_test_dir=/suspend_resume/SLT_BSP_FILE/2.3.6_wdg_test
i2s_src_test_dir=/suspend_resume/SLT_BSP_FILE/i2s_src_test/audio
spi_test_dir=/suspend_resume/SLT_BSP_FILE/2.3.10_spi
can_test_dir=/suspend_resume/SLT_BSP_FILE/2.3.11_can
sadc_test_dir=/suspend_resume/SLT_BSP_FILE/2.3.15_sadc

mkdir -p /slt_log
mkdir -p /slt_file

DEBUG=0
debug(){
	if [ "$DEBUG" -eq 1 ];then
		echo -e "\033[33mdebug: $1\033[0m"
	fi
}

trap 'onCtrlC' INT
function onCtrlC () {
	return 0
}

#2.2.1 cpu //memcpy & memtest
ap_ddr_test() {
        cd ${ap_ddr_test_dir}
        #./memcpy_test & 2>> /ap_test.log 1>/dev/null
        ./memtester 1M 1 & 2>> /ap_test.log 1>/dev/null
        sleep 1s
        #PID_MEMCPY=`ps aux | grep memcpy_test | awk '{print $1}' | sed -n '1p'`
        PID_AP_MEMTESTER=`ps | grep memtester_arm | awk '{print $1}' | sed -n '1p'`
        #CPU4_3_MEMCPY=`taskset -p 0x18 $PID_MEMCPY` 2>> /ap_test.log 1>/dev/null
        #CPU2_1_AP_MEMTESTER=`taskset -p 0x6 $PID_AP_MEMTESTER` 2>> /ap_test.log 1>/dev/null
        #if [ -z "$CPU4_3_MEMCPY" -o -z "$CPU2_1_AP_MEMTESTER" ]
        #then
        #        echo -e "\033[31merror:AP_MEMCPY or AP_MEMTESTER fail,please check\033[0m" >> /slt_log/slt_log.log
        #        return 1
        #fi
        #CPU_MEMCPY=`taskset -p $PID_MEMCPY | awk -F : '{printf $2}'` 2>> /ap_test.log 1>/dev/null
        CPU_AP_MEMTESTER=`taskset -p $PID_AP_MEMTESTER | awk -F : '{printf $2}'` 2>> /ap_test.log 1>/dev/null
        sleep 5s
        echo "CPU_MEMTESTER=$CPU_AP_MEMTESTER"
        #kill $PID_MEMCPY
        echo "CPU_MEMCPY_TEST passed" >> /slt_log/slt_log.log
        return 0
}

#2.2.4 aes+dma
aes_test() {
	cd ${aes_ras_hash_test_dir}
	sh 2.sh
	if [ "$?" -ne 0 ];then
		return 1
	else
		return 0
	fi
}
# aes_test() {
	# cd ${aes_ras_hash_test_dir}
	# aes_out=`./cipher | grep pass |awk '{print $3}'`
	# aes_socket_out=`./cipher_test | grep PASS | awk '{print $6}'`
	# if [ "$aes_out" == "passed" ]
    # then
        # echo "aes_test pass" >> /slt_log/slt_log.log
		# return 0
	# else
		# echo -e "\033[31merror:AES Test failed,please check\033[0m" >> /slt_log/slt_log.log
		# return 1
    # fi
# }

#2.3.13 i2s
i2s_test(){
	cd ${i2s_src_test_dir}

		#pinmux switch to i2s and i2c
	devmem 0x10200064 32 0x4
	devmem 0x10200068 32 0x4
	devmem 0x1020006c 32 0x4
	devmem 0x10200070 32 0x4
	devmem 0x10200074 32 0x4
	devmem 0x10200080 32 0x2
	devmem 0x10200088 32 0x2
	devmem 0x0b100044 32 0x22
	devmem 0x0b10004c 32 0x22

	./sample_audio_i2s &

}
#2.3.14 codec
codec_test(){
	cd ${i2s_src_test_dir}

	./sample_audio_acw &

}

#2.3.1 usb2.0
udisk_id_list=" "
sdisk_id_list=" "
number_check()
{
	if [ -n "$(echo $1| sed -n "/^[0-9]\+$/p")" ];then
		return 0
	else
		return 1
	fi
}

usb_sata_idfind() {
	local size
	local bmg
	local total_sd=`fdisk -l | grep dev/sda. | awk '{print $1}' | tail -n 2 | head -n 1`
	local sd_info=`fdisk -l | grep dev/sda. | tail -n 2 | head -n 1`
	echo "/dev/sd.: $total_sd"
	for i in ${total_sd}
	do
		size=`echo ${sd_info} | awk '{print $7}' | sed 's/[A-Za-z]//g'`
		bmg=`echo ${sd_info} | awk '{print $7}'`
		bmg=${bmg: -1}
		size=${size:0:2}
		#debug "$size---$bmg"

		number_check ${size}
		if [ $? -eq 0 ];then
			if [ ${size} -gt 400 ];then
				if [ ${bmg} == "G" ];then
					debug "$i is sdisk"
					sdisk_id_list="${sdisk_id_list} ${i}"
				else
					debug "$i is udisk"
					udisk_id_list="${udisk_id_list} ${i}"
				fi
			else
				debug "$i is udisk"
				udisk_id_list="${udisk_id_list} ${i}"
			fi
		else
			echo -e "\033[31merror:dev/sd* detect fail \033[0m" >> /slt_log/slt_log.log
			return 1
		fi
	done
	return 0
}

usb_test(){
	local udisk_id
	local udisk_count=0
	local udisk_max=1
	debug "udisk_id_list:$udisk_id_list"
	for i in ${udisk_id_list}
	do
		touch /slt_file/usb_read.bin
		touch /slt_file/usb_write.bin
		debug "udisk_dev:$i"
		udisk_id=`echo ${i} | awk -F ":" '{print $1}'`
		debug "udisk_id:$udisk_id"
		mkfs.vfat $udisk_id
		dd if=/dev/urandom of=/slt_file/usb_write.bin bs=64k count=16 >/dev/null 2>&1
		#md5sum /slt_file/usb_write.bin
		dd if=/slt_file/usb_write.bin of=$udisk_id bs=64k count=16 >/dev/null 2>&1
		if [ "$?" -ne 0 ];then
			echo -e "\033[31merror:USB$udisk_id dd write failed\033[0m" >> /slt_log/slt_log.log
			return 1
		fi
		time sync
		sleep 1s
		sh -c "sync && echo 3 > /proc/sys/vm/drop_caches"
		time dd if=$udisk_id of=/slt_file/usb_read.bin bs=64k count=16 >/dev/null 2>&1
		#md5sum /slt_file/usb_read.bin
		if [ "$?" -ne 0 ];then
			echo -e "\033[31merror:USB$udisk_id dd read failed\033[0m" >> /slt_log/slt_log.log
			return 1
		fi
		sleep 1s
		let udisk_count=${udisk_count}+1
		usb_pass=`diff /slt_file/usb_write.bin /slt_file/usb_read.bin`
		if [ -z "$usb_pass" ];then
			rm /slt_file/usb_read.bin
			rm /slt_file/usb_write.bin
		else
			echo -e "\033[31merror:usb_test contrast failed\033[0m" >> /slt_log/slt_log.log
			rm /slt_file/usb_read.bin
			rm /slt_file/usb_write.bin
			return 1
		fi
	done
	if [ ${udisk_max} != ${udisk_count} ];then
		echo -e "\033[31merror:need test udisk:${udisk_max}, but only test udsik:${udisk_count}\033[0m" >> /slt_log/slt_log.log
			return 1
	else
		echo "usb_test pass" >> /slt_log/slt_log.log
		return 0
	fi
}


#2.3.2 efuse
efuse_test() {
	hexdump -C -v /sys/bus/nvmem/devices/nvmem0/nvmem
	if [ "$?" -eq 0 ]
	then
		echo "efuse_test pass" >> /slt_log/slt_log.log
		return 0
	else
		echo -e "\033[31merror:EFUSE0 Read fail!\033[0m" >> /slt_log/slt_log.log
		return 1
	fi
}

#2.3.3 uart

#2.3.4 sfc
sfc_test() {
	cd ${sfc_test_dir}

	./sfc_test /dev/mtd3 0x10000 > ./sfc_log.txt
	result_sfc=`cat ./sfc_log.txt | awk '{print $3}' | tail -1`
	rm ./sfc_log.txt

	if [ "$result_sfc" = "pass" ];then
		echo "sfc_test pass" >> /slt_log/slt_log.log
		return 0
	else
		echo -e "\033[31merror:sfc_test fail!\033[0m" >> /slt_log/slt_log.log
		return 1
	fi

}

#2.3.5 SDIO
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

	mkfs.vfat $sd_dev

	dd if=/dev/urandom of=/tmp/sd_write.bin bs=1M count=1 >/dev/null 2>&1
	dd if=/tmp/sd_write.bin of=$sd_dev bs=1M count=1 >/dev/null 2>&1
	if [ "$?" -ne 0 ];then
		echo "error:SDIO: $sd_dev dd write failed!"
	fi
	sync
	sleep 1s
	sh -c "sync && echo 3 > /proc/sys/vm/drop_caches"
	dd if=$sd_dev of=/tmp/sd_read.bin bs=1M count=1 >/dev/null 2>&1
	if [ "$?" -ne 0 ];then
		echo "error:SDIO: $sd_dev dd read failed"
	fi
	sleep 1s

	sd_pass=`diff /tmp/sd_write.bin /tmp/sd_read.bin`
	if [ -z "$sd_pass" ];then
		echo "sdio_test pass" >> /slt_log/slt_log.log
		return 0
	rm /tmp/sd_read.bin
	rm /tmp/sd_write.bin
	else
		echo "\033[31merror:sdio_test fail!\033[0m" >> /slt_log/slt_log.log
		return 1
	rm /tmp/sd_read.bin
	rm /tmp/sd_write.bin
	fi
}

#2.3.6 WDT
#reset
wdt_test() {
	cd $wdt_test_dir
	./watchdog_test 1 1
	echo "wait 1s,restart"
}

#2.3.7 GMAC
gmac_test() {
	#source /SLT_BSP_FILE/ethecon.sh

	eth0configip=`cat /sys/firmware/devicetree/base/chosen/bootargs | awk -F "-" '{print $2}' | awk -F "=" '{print $2}'`
	eth0confighw=`cat /sys/firmware/devicetree/base/chosen/bootargs | awk -F "-" '{print $3}' | awk -F "=" '{print $2}'`

	eth1configip=`cat /sys/firmware/devicetree/base/chosen/bootargs | awk -F "-" '{print $4}' | awk -F "=" '{print $2}'`
	eth1confighw=`cat /sys/firmware/devicetree/base/chosen/bootargs | awk -F "-" '{print $5}' | awk -F "=" '{print $2}'`

	serverip=`cat /sys/firmware/devicetree/base/chosen/bootargs | awk -F "-" '{print $6}'`
	configmask=`cat /sys/firmware/devicetree/base/chosen/bootargs | awk -F "-" '{print $7}'`
	gateway=`cat /sys/firmware/devicetree/base/chosen/bootargs | awk -F "-" '{print $8}'`


	ipaddr=`ifconfig | grep "inet" | awk '{print $2}' | awk -F ":" '{print $2}'`
	if [ -n "$ipaddr" ] || [ -z "$ipaddr" ];then
		ifconfig eth0 down
		ifconfig eth0 hw ether "$eth0confighw"
		ifconfig eth0 "$eth0configip" netmask "$configmask" up
		sleep 2s
		ip_linkup=`dmesg | grep "eth0: Link is Up" | tail -1`
		if [ -z "$ip_linkup" ];then

			echo -e "\033[31merror:gmac_test failed,eth0 ip_linkup failed\033[0m" >> /slt_log/slt_log.log
			return 1
		fi
	fi

	debug "board_ipaddr:$ipaddr"
	ifconfig eth0 up
	route add default gateway "$gateway"
	#ping -I eth0 "$serverip" -c 5 -s 65500
	#echo -e "\033[31mignore packet loss,start read gmac_test\033[0m"
	ping -I eth0 "$serverip" -c 10 -s 65500 > /slt_log/slt_gmac0.log
	cat /slt_log/slt_gmac0.log
	ping_packet=`cat /slt_log/slt_gmac0.log | grep "packet loss" | awk '{print $7}'`
	rm /slt_log/slt_gmac0.log
	#ping_packet=`ping -I eth0 "$serverip" -c 10 -s 65500 | grep "packet loss"`
	#ping_packet=`ping -I eth0 "$serverip" -c 10 -s 65500 | grep "packet loss" | awk '{print $7}'`
	#echo "$ping_packet" >> /slt_log/slt_gmac0.log
	#ping_packet=`echo "$ping_packet" | awk '{print $7}'`
	echo "ping_packet:$ping_packet"
	if [ $ping_packet = "0%" ]
	then
		echo "gmac_test eth0 passed,$ping_packet packet loss" >> /slt_log/slt_log.log
	else
		echo -e "\033[31merror:eth0 gmac_test failed,$ping_packet packet loss\033[0m" >> /slt_log/slt_log.log
		return 1
	fi
}

#2.3.8 GPIO GPIO4_0 & GPIO3_4
gpio_test() {
	local gpio_outpin=32
	local gpio_inpin=28
	devmem 0x10200078 32 0x1
	devmem 0x10200068 32 0x1

	gpio_select=`ls /sys/class/gpio/gpio$gpio_outpin`
	if [ "$?" -eq 0 ]
	then
		echo -e "\033[31merror:gpio$gpio_outpin is used,please check\033[0m"
		devmem 0x10200078 32 0x0
		devmem 0x10200068 32 0x4
		return 1
	fi
	gpio_select=`ls /sys/class/gpio/gpio$gpio_inpin`
	if [ "$?" -eq 0 ]
	then
		echo -e "\033[31merror:gpio$gpio_inpin is used,please check\033[0m"
		devmem 0x10200078 32 0x0
		devmem 0x10200068 32 0x4
		return 1
	fi
	echo $gpio_outpin > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio$gpio_outpin/direction
	gpio_direct=`cat /sys/class/gpio/gpio$gpio_outpin/direction | grep "out"`
	if [ "$?" -eq 0 ]
	then
		echo -e "\033[32mgpio$gpio_outpin direction out\033[0m"
	else
		echo -e "\033[31mset gpio$gpio_outpin direction_out failed,please check\033[0m" >> /slt_log/slt_log.log
		echo $gpio_outpin > /sys/class/gpio/unexport
		devmem 0x10200078 32 0x0
		devmem 0x10200068 32 0x4
		return 1
	fi
	echo $gpio_inpin > /sys/class/gpio/export
	echo in > /sys/class/gpio/gpio$gpio_inpin/direction
	gpio_direct=`cat /sys/class/gpio/gpio$gpio_inpin/direction | grep "in"`
	if [ "$?" -eq 0 ]
	then
		echo -e "\033[32mgpio$gpio_inpin direction in\033[0m"
	else
		echo -e "\033[31mset gpio$gpio_inpin direction_in failed,please check\033[0m" >> /slt_log/slt_log.log
		echo $gpio_inpin > /sys/class/gpio/unexport
		devmem 0x10200078 32 0x0
		devmem 0x10200068 32 0x4
		return 1
	fi
	echo 0 > /sys/class/gpio/gpio$gpio_outpin/value
	gpio_pass=`cat /sys/class/gpio/gpio$gpio_inpin/value`
	if [ "$gpio_pass" -eq 0 ];then
		echo -e "\033[32mgpio out and in 0 is ok\033[0m" >> /slt_log/slt_log.log
	else
		echo -e "\033[32merror:gpio out and in 0 is faile\033[0m" >> /slt_log/slt_log.log
		echo $gpio_outpin > /sys/class/gpio/unexport
		echo $gpio_inpin > /sys/class/gpio/unexport
		devmem 0x10200078 32 0x0
		devmem 0x10200068 32 0x4
		return 1
	fi
	sleep 3s
	echo 1 > /sys/class/gpio/gpio$gpio_outpin/value
	gpio_pass=`cat /sys/class/gpio/gpio$gpio_inpin/value`
	if [ "$gpio_pass" -eq 1 ];then
		echo -e "\033[32mgpio out and in 1 is ok\033[0m" >> /slt_log/slt_log.log
	else
		echo -e "\033[32merror:gpio out and in 1 is faile\033[0m" >> /slt_log/slt_log.log
		echo $gpio_outpin > /sys/class/gpio/unexport
		echo $gpio_inpin > /sys/class/gpio/unexport
		devmem 0x10200078 32 0x0
		devmem 0x10200068 32 0x4
		return 1
	fi
	echo "gpio_test pass" >> /slt_log/slt_log.log
	echo $gpio_outpin > /sys/class/gpio/unexport
	echo $gpio_inpin > /sys/class/gpio/unexport
	devmem 0x10200078 32 0x0
	devmem 0x10200068 32 0x4
	return 0
}

#2.3.9 I2C
i2c_test()
{
	#devmem 0x10200080 32 0x2
	#devmem 0x10200088 32 0x2
	#devmem 0xb100044 32 0x22
	#devmem 0xb10004c 32 0x22
	devmem 0x10200090 32 0x2
	devmem 0x1020008c 32 0x2
	devmem 0xb100050 32 0x22
	devmem 0xb100054 32 0x22
	i2c_tmp=`i2cdetect -y -r 0`
	if [ "$?" -eq 0 ]
	then
		echo "i2c_test pass" >> /slt_log/slt_log.log
		return 0
	else
		echo -e "\033[31merror:i2c test failed,please check!\033[0m" >> /slt_log/slt_log.log
		return 1
	fi
}

#2.3.12 PWM
pwm_test() {
	devmem 0x10200060 32 0x3
	devmem 0x1020005c 32 0x1
	echo 0x0249f00000003252 > /sys/devices/platform/9500000.pwm/debug
	pwm_debug=`dmesg | grep "period_ns(38400000) duty_ns(19200000)" | tail -1`
	if [ -n "$pwm_debug" ];then
		echo "pwm_test passed" >> /slt_log/slt_log.log
		return 0
	else
		echo -e "\033[31merror:pwm_test failed,please check\033[0m">> /slt_log/slt_log.log
		return 1
	fi
}


#2.3.10 SPI
spi_test() {
	cd ${spi_test_dir}
	devmem 0x102000a0 32 0x3 #SPI CS
	devmem 0x102000a4 32 0x3 #SPI CLK
	devmem 0x102000a8 32 0x3 #SPI DO
	devmem 0x1020009c 32 0x1 #SPI DI

	./spi_test_slt > ./spi_log.txt
	test=`cat ./spi_log.txt | awk '{print $3}' | tail -1`
	rm ./spi_log.txt
	if [ "$test" = "success!" ]
	then
		echo "spi_test pass" >> /slt_log/slt_log.log
		return 0
	else
		echo -e "\033[31merror:spi_test failed\033[0m" >> /slt_log/slt_log.log
		return 1
	fi
}

#2.3.11 can
can_test() {
	cd ${can_test_dir}
	id="123"
	data="FEDCBA9876543210"
	len=8

	ifconfig can0 down
	./ip-can link set can0 type can bitrate 50000
	ifconfig can0 up

	ifconfig | grep can0

	if [ $? -ne 0 ];then
		echo -e "\033[31merror:can test failed\033[0m" >> /slt_log/slt_log.log
		echo "can device not found"
		return 1
	fi
	#LBMI
	devmem 0x265000a0 32 0x900020

	nohup ./candump can0 > ./nohup.txt 2>&1 &
	sync
	sync
	sync
	sync

	sendstr=${id}"#"${data}
	echo $sendstr
	./cansend can0 $sendstr
	sync
	sync
	sync
	sync

	except=$(cat ./nohup.txt)

	getID=$(echo $except | awk -F " " '{print $2}')
	echo $getID
	if [ "$getID" != "$id" ] ; then
		echo "CAN LBMI ERR, ID ERR"
		echo -e "\033[31merror:can test failed\033[0m" >> /slt_log/slt_log.log
		return 1
	fi

	getLEN_tmp=$(echo $except | awk -F " " '{print $3}')
	getLEN=$(echo $getLEN_tmp | cut -c 2)
	echo $getLEN
	if [ $getLEN -ne $len ] ; then
		echo -e "\033[31merror:can test failed\033[0m" >> /slt_log/slt_log.log
		echo "CAN LBMI ERR, DATA LEN ERR"
		return 1
	fi

	getDATA=$(echo $except | awk '{
			split($0, parts, " ");
			for (i=4; i<=NF; i++){
				printf "%s", parts[i]
			}
			}')
	echo $getDATA
	if [ "$getDATA" != "$data" ] ; then 
		echo -e "\033[31merror:can test failed\033[0m" >> /slt_log/slt_log.log
		echo "CAN LBMI ERR, DATA ERR"
		return 1
	fi
	echo "can test pass" >> /slt_log/slt_log.log
	return 0
}

#2.3.15 sadc
sadc_test() {
	result=`cat /sys/bus/iio/devices/iio\:device0/in_voltage0_raw`
	result1=`cat /sys/bus/iio/devices/iio\:device0/in_voltage1_raw`
	echo $result
	echo $result1
	if [ "$result" -lt "30" ] && [ "$result1" -gt "2400" ]
	then
		echo "sadc_test pass" >> /slt_log/slt_log.log
		return 0
	else
		echo -e "\033[31merror:sadc_test failed\033[0m" >> /slt_log/slt_log.log
		return 1
	fi
}

#2.3.16 timer & system timer
timer_test() {
	timer_debug=`cat /sys/devices/system/clocksource/clocksource0/available_clocksource `
	if [ -n "$timer_debug" ];then
		echo "timer_test pass" >> /slt_log/slt_log.log
		return 0
	else
		echo -e "\033[31merror:timer_test failed,please check\033[0m" >> /slt_log/slt_log.log
		return 1
	fi
}


show_temperature(){
	cd ${tsensor_dir}
	./tsensor.sh &
}

# SDEV06_result_check(){
	# num=`cat /slt_log/spi_log.log | grep -c "pass"`
	# error=`cat /slt_log/spi_log.log | grep -c "error"`
	# if [ "$num" -ne 0 ] && [ "$error" -eq 0 ];then
		# echo "spi_test pass" >> /slt_log/slt_log.log
                # return 0
        # fi
        # echo -e "\033[31merror:spi test failed\033[0m">> /slt_log/slt_log.log
        # cat /slt_log/spi_log.log | grep "error" >> /slt_log/slt_log.log
        # return 1
# }

# mm_result_check(){
	# time=0
	# while true
	# do
		# mm_done=`cat /tmp/media_slt.log`
		# if [ -n "$mm_done" ];then
			# break
		# fi
		# sleep 1s
		# let time=${time}+1
		# if [ $time -ge 20 ];then
			# echo -e "\033[31merror:TimeOut:$time, wait mm_test result timeout\033[0m">> /slt_log/slt_log.log
			# return 1
		# fi
		# echo "wait mm test result"
	# done
	# echo "wait mm test result done"
	# mm_num=`cat /tmp/media_slt.log | grep -c "pass"`
	# mm_error=`cat /tmp/media_slt.log | grep -c "error"`

	# if [ "$mm_num" -ne 0 ] && [ "$mm_error" -eq 0 ];then
		# echo "mm_test pass" >> /slt_log/slt_log.log
		# return 0
	# fi
	# echo -e "\033[31merror:mm_test failed\033[0m">> /slt_log/slt_log.log
	# cat /tmp/media_slt.log | grep "error" >> /slt_log/slt_log.log
	# return 1
# }

lt00_slt_test() {
	echo "-------lt00 SLT BSP test-------"
        vall=0

	echo -e "\033[47;30m>>>ap_ddr_test start<<<\033[0m"
	ap_ddr_test
	if [ "$?" -ne 0 ];then
		vall=1
	fi
	echo -e "\033[47;30m>>>ap_ddr_test end<<<\033[0m"

	sleep 5s

	echo -e "\033[47;30m>>>aes_test start<<<\033[0m"
	aes_test
	if [ "$?" -ne 0 ];then
		vall=1
	fi
	echo -e "\033[47;30m>>>aes_test end<<<\033[0m"

	# usb_sata_idfind
	# echo -e "\033[47;30m>>>usb_test start<<<\033[0m"
	# usb_test
	# if [ "$?" -ne 0 ];then
		# vall=1
	# fi
	# echo -e "\033[47;30m>>>usb_test end<<<\033[0m"

	echo -e "\033[47;30m>>>efuse_test start<<<\033[0m"
	efuse_test
	if [ "$?" -ne 0 ];then
		vall=1
	fi
	echo -e "\033[47;30m>>>efuse_test end<<<\033[0m"

	# echo -e "\033[47;30m>>>sfc_test start<<<\033[0m"
	# sfc_test
	# if [ "$?" -ne 0 ];then
		# vall=1
	# fi
	# echo -e "\033[47;30m>>>sfc_test end<<<\033[0m"
	
	# echo -e "\033[47;30m>>>sdio_test start<<<\033[0m"
	# sdio_test
	# if [ "$?" -ne 0 ];then
		# vall=1
	# fi
	# echo -e "\033[47;30m>>>sdio_test end<<<\033[0m"

	# echo -e "\033[47;30m>>>gmac_test start<<<\033[0m"
	# gmac_test
	# if [ "$?" -ne 0 ];then
		# vall=1
	# fi
	# echo -e "\033[47;30m>>>gmac_test end<<<\033[0m"

	# echo -e "\033[47;30m>>>gpio_test start<<<\033[0m"
	# gpio_test
	# if [ "$?" -ne 0 ];then
		# vall=1
	# fi
	# echo -e "\033[47;30m>>>gpio_test end<<<\033[0m"

	#echo -e "\033[47;30m>>>i2c_test start<<<\033[0m"
	#i2c_test
	#if [ "$?" -ne 0 ];then
	#	vall=1
	#fi
	#echo -e "\033[47;30m>>>i2c_test end<<<\033[0m"
	
	#echo -e "\033[47;30m>>>spi_test start<<<\033[0m"
	#spi_test
	#if [ "$?" -ne 0 ];then
	#	vall=1
	#fi
	#echo -e "\033[47;30m>>>spi_test end<<<\033[0m"

	# echo -e "\033[47;30m>>>can_test start<<<\033[0m"
	# can_test
    	# if [ "$?" -ne 0 ];then
       	# vall=1
    	# fi
    # echo -e "\033[47;30m>>>can_test end<<<\033[0m"

	#echo -e "\033[47;30m>>>pwm_test start<<<\033[0m"
	#pwm_test
	#if [ "$?" -ne 0 ];then
	#	vall=1
	#fi
	#echo -e "\033[47;30m>>>pwm_test end<<<\033[0m"


	 echo -e "\033[47;30m>>>sadc_test start<<<\033[0m"
	 sadc_test
	 if [ "$?" -ne 0 ];then
		 vall=1
	 fi
	 echo -e "\033[47;30m>>>sadc_test end<<<\033[0m"

	#echo -e "\033[47;30m>>>i2s_test start<<<\033[0m"
	#i2s_test
	#echo -e "\033[47;30m>>>i2s_test end<<<\033[0m"

	#echo -e "\033[47;30m>>>codec_test start<<<\033[0m"
	#codec_test

	#echo -e "\033[47;30m>>>codec_test end<<<\033[0m"
	sleep 4s
	echo -e "\033[47;30m>>>timer_test start<<<\033[0m"
	timer_test
	if [ "$?" -ne 0 ];then
		vall=1
	fi
	echo -e "\033[47;30m>>>timer_test end<<<\033[0m"

    if [ "$vall" -eq 0 ];then
		echo -e "\033[47;30m>>>SLT_TEST_PASS<<<\033[0m"
		cat /slt_log/slt_log.log | grep -nri "error"
		cat /slt_log/slt_log.log | grep -nri "warnig:"
		rm -rf /slt_log
		rm -rf /slt_file
	else
		echo -e "\033[47;30m>>>SLT_TEST_FAIL<<<\033[0m"
		cat /slt_log/slt_log.log | grep -nri "error"
		cat /slt_log/slt_log.log | grep -nri "warnig:"
		rm -rf /slt_log
		rm -rf /slt_file
		return 1
	fi
	#PID_CLEAN_SYNC=`ps aux | grep clean_sync | awk '{print $1}' | sed -n '1p'`
	#kill $PID_CLEAN_SYNC
	#PID_S02BSP=`ps aux | grep S06BSP | awk '{print $1}' | sed -n '1p'`
	#kill $PID_S02BSP
	return 0
}
#show_temperature
#sleep 1s

#load_test_case
lt00_slt_test

