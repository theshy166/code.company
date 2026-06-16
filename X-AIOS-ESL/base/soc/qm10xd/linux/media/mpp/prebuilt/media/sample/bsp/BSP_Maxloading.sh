#!/bin/sh

function usb_stress()
{
	blk=1024K
	num=1
	disk=`echo $file | grep u`
	if [ -n "$disk"  ];then
		blk=128K
		num=8
		file=`echo $file | awk -F "u" '{print $2}'`
	fi
	run_time=1
	file_name=`echo "$file" | awk -F "/" '{print $3}'`
	mkdir /stress_$file_name
	while true
	do
		echo "---------The $run_time time $file test start-------------------"
		touch /stress_$file_name/"$file_name"_read.log
		dd if=/dev/zero of=/stress_$file_name/"$file_name"_write.log bs=$blk count=$num conv=fsync >/dev/null 2>&1
		sh -c "echo 3 > /proc/sys/vm/drop_caches >/dev/null 2>&1"
		write_md5=`md5sum /stress_$file_name/"$file_name"_write.log | awk 'BEGIN{FS=" "}{print  $1}'`
		
		dd if=/stress_$file_name/"$file_name"_write.log of=$file bs=$blk count=$num conv=fsync >/stress_$file_name/speed.log 2>&1
		sh -c "echo 3 > /proc/sys/vm/drop_caches >/dev/null 2>&1"
		echo "$file write speed :"`cat /stress_$file_name/speed.log | grep MB/s | awk '{print $7}' | awk -F "MB/s" '{print $1}'`"MB/s"
		rm /stress_$file_name/"$file_name"_write.log
		
		dd if=$file of=/dev/null bs=$blk count=$num conv=sync >/stress_$file_name/speed.log 2>&1
		dd if=$file of=/stress_$file_name/"$file_name"_read.log bs=$blk count=$num conv=fsync >/dev/null 2>&1
		sh -c "echo 3 > /proc/sys/vm/drop_caches >/dev/null 2>&1"
		echo "$file read speed :"`cat /stress_$file_name/speed.log | grep MB/s | awk '{print $7}' | awk -F "MB/s" '{print $1}'`"MB/s"
		read_md5=`md5sum /stress_$file_name/"$file_name"_read.log | awk 'BEGIN{FS=" "}{print  $1}'`
		rm /stress_$file_name/"$file_name"_read.log
		run_time=$(( $run_time + 1 ))
		if [ $write_md5 != $read_md5 ];then
			echo "The $run_time time $file test is failed, error write_md5: $write_md5, read_md5: $read_md5,"
		fi
		sh -c "echo 3 > /proc/sys/vm/drop_caches >/dev/null 2>&1"
		sleep 1
	done
}

function sdio_test() {

	file_name=`echo "$file" | awk -F "/" '{print $3}'`
	run_time=1

	while true
	do
		echo "---------The $run_time time $file test start-------------------"
		touch /tmp/"$file_name"_read.bin
		touch /tmp/"$file_name"_write.bin

		dd if=/dev/urandom of=/tmp/"$file_name"_write.bin bs=64k count=16 conv=fsync >/dev/null 2>&1
		dd if=/tmp/"$file_name"_write.bin of=$file bs=64k count=16 conv=fsync >/tmp/"$file_name"_speed.log 2>&1
		sh -c "echo 3 > /proc/sys/vm/drop_caches >/dev/null 2>&1"
		echo "$file write speed :"`cat /tmp/"$file_name"_speed.log | grep MB/s | awk '{print $7}' | awk -F "MB/s" '{print $1}'`"MB/s"

		dd if=$file of=/dev/null bs=64k count=16 conv=sync >/tmp/"$file_name"_speed.log 2>&1
		dd if=$file of=/tmp/"$file_name"_read.bin bs=64k count=16 conv=fsync >/dev/null 2>&1
		sh -c "echo 3 > /proc/sys/vm/drop_caches >/dev/null 2>&1"
		echo "$file read speed :"`cat /tmp/"$file_name"_speed.log | grep MB/s | awk '{print $7}' | awk -F "MB/s" '{print $1}'`"MB/s"

		sd_pass=`diff /tmp/"$file_name"_write.bin /tmp/"$file_name"_read.bin`
		if [ -n "$sd_pass" ];then
			echo "The $run_time time $file test is failed"
		fi
		rm /tmp/"$file_name"_read.bin
		rm /tmp/"$file_name"_write.bin
		run_time=$(( $run_time + 1 ))
	done
}

number_check()
{
	if [ -n "$(echo $1| sed -n "/^[0-9]\+$/p")" ];then
		return 0
	else
		return 1
	fi
}
udisk_id_list=" "
sdisk_id_list=" "
usb_sata_idfind() {
	local size
	local bmg
	local total_sd=`fdisk -l | grep dev/sd.: | awk '{print $2$3$4}' | awk -F "B" '{print $1}'`
	echo "/dev/sd.: $total_sd"
	for i in ${total_sd}
	do
		size=`echo ${i} | awk -F ":" '{print $2}'`
		bmg=${size: -1}
		size=`echo $size | awk -F "$bmg" '{print $1}'`
		number_check ${size}
		if [ $? -eq 0 ];then
			if [ ${size} -gt 400 ];then
				if [ ${bmg} == "G" ];then
					sdisk_id_list="${sdisk_id_list} ${i}"
				else
					udisk_id_list="${udisk_id_list} ${i}"
				fi
			else
				udisk_id_list="${udisk_id_list} ${i}"
			fi
		else
			echo -e "\033[31merror:dev/sd* detect fail \033[0m" >> /slt_log/slt_log.log
			return 1
		fi
	done
	return 0
}
num=0
usb_sata_idfind 
for i in $udisk_id_list
do
	let num=$num+1
done

if [ $num -eq 1 ];then
	let num=0
else
	echo "----------------error : udisk is only $num---------------"
	exit 1
fi 







#sdcard_list=""
#sdcard_find() {

#	for i in `ls /dev/mmc*`
#	do
#		sdcard_list="$sdcard_list ${i}"
#	done

#	for i in $sdcard_list
#	do
#		if [ "${i}" = "/dev/mmcblk1" ];then
#			return 0
#		fi
#	done
#	echo "----------------error:sdcard not find--------------"
#	exit 1
#}
#sdcard_find





for i in $udisk_id_list
do
	udisk_id=`echo ${i} | awk -F ":" '{print $1}'`
	mkfs.vfat $udisk_id
	file="u$udisk_id"
	usb_stress &
done


#sdio_list="/dev/mmcblk1"
#for i in $sdio_list
#do
#	mkfs.vfat $i
#	if [ "$?" -ne 0 ];then
#		echo "$i is not exist"
#		exit 1
#	fi
#	file="$i"
#	sdio_test &
#done

./memcopy_cpu2dmc 32768 &
while true
	do
		./perfmon 5000 1200
		sleep 1
		top -b -n 1
		free -m
		sleep 1
		sync
		echo 1 > /proc/sys/vm/drop_caches
		sleep 1
	done
