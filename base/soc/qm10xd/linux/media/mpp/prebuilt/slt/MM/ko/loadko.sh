#!/bin/sh
# Useage: ./loadko [ -r|-i|-a ]
#         -r : rmmod all modules
#         -i : insmod all modules
#    default : rmmod all moules and then insmod them
#

####################Variables Definition##########################

mmz_start=0x43000000;	# mmz start addr
mmz_size=16M;		# mmz size

##################################################################
#ulimit -s 2048
#report_error()
#{
#	echo "******* Error: There's something wrong, please check! *****"
#	exit 1
#}

#config_clock()
#{
#	echo clock_vpu_enable > /proc/driver/sys_config
#	echo clock_vpu_400000000 > /proc/driver/sys_config
#
#	echo clock_isp_enable > /proc/driver/sys_config
#	echo clock_isp_333600000 > /proc/driver/sys_config
#
#	echo clock_sensor0_enable > /proc/driver/sys_config
#	echo clock_sensor0_24000000 > /proc/driver/sys_config
#
#	echo clock_sensor1_enable > /proc/driver/sys_config
#	echo clock_sensor1_24000000 > /proc/driver/sys_config
#
#	echo clock_sensor2_enable > /proc/driver/sys_config
#	echo clock_sensor2_24000000 > /proc/driver/sys_config
#}

insert_ko()
{
	#pinmux switch to i2s and i2c
	 #devmem 0x10200064 32 0x4
	 #devmem 0x10200068 32 0x4
	 #devmem 0x1020006c 32 0x4
	 #devmem 0x10200070 32 0x4
	 #devmem 0x10200074 32 0x4
	 #devmem 0x10200080 32 0x2
	 #devmem 0x10200088 32 0x2
	 #devmem 0x0b100044 32 0x22
	 #devmem 0x0b10004c 32 0x22
	# driver load
#	config_clock
	insmod fyosal.ko
	insmod mmz.ko mmz=anonymous,0,$mmz_start,$mmz_size anony=1 || report_error
	insmod fybase.ko logbuflen=16384
#	insmod sys_config.ko
	insmod sys.ko
	#insmod i2s.ko
	#insmod extac.ko
#	insmod media_process.ko
#	insmod vicap_rpc.ko
#	insmod isp.ko
#	insmod vpu.ko
##############################################
	#insmod lcm_module.ko
	#insmod lcm.ko lcm0_id_cfg=1 lcm1_id_cfg=1 lcm0_id=0 lcm1_id=4
	#insmod vou.ko
	#insmod vdu.ko
	#insmod jpeg.ko
	#insmod g2d.ko
##############################################
#	insmod g2d.ko wkbuf=1024 vgsbuf=1024 rotbuf=1024
#	insmod vppu.ko
#	insmod vgs.ko
#	insmod venc_rpc.ko
#	insmod pre_rolling.ko buf_base=0x5F500000 buf_size=0x400000
#	insmod isp460.ko
#	insmod camif.ko
#	insmod src.ko
#	insmod nnp.ko
#	insmod acw.ko
}

remove_ko()
{
	rmmod nnp
	rmmod src
	rmmod camif
	rmmod isp460
	rmmod pre_rolling
	rmmod venc_rpc
	rmmod vgs
	rmmod vppu
	rmmod g2d
	rmmod jpeg
	rmmod vpu
	rmmod isp
	rmmod vicap_rpc
	rmmod media_process
	rmmod sys_config
	rmmod sys
	rmmod fybase
	rmmod mmz
	rmmod fyosal
	rmmod xbus_rpc
}


load_usage()
{
	echo "Usage:  ./loadko.sh [-option]"
	echo "options:"
	echo "    -i                       insert modules"
	echo "    -r                       remove modules"
	echo "    -a                       remove modules first, then insert modules"
	echo "    -h                       help information"
	echo -e "for example: ./loadko.sh -i\n"
}

######################parse arg###################################
b_arg_insmod=0
b_arg_remove=0


for arg in $@
do
	case $arg in
		"-i")
			b_arg_insmod=1;
			;;

		"-r")
			b_arg_remove=1;
			;;
		"-a")
			b_arg_insmod=1;
			b_arg_remove=1;
			;;

		"-h")
			load_usage;
			;;
	esac
done
#######################parse arg end########################

#######################Action###############################
if [ $b_arg_remove -eq 1 ]; then
	remove_ko;
fi
if [ $b_arg_insmod -eq 1 ]; then
	insert_ko;
fi

