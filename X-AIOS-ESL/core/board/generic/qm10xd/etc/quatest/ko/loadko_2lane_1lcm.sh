#!/bin/sh
# Useage: ./loadko [ -r|-i|-a ]
#         -r : rmmod all modules
#         -i : insmod all modules
#    default : rmmod all moules and then insmod them
#

####################Variables Definition##########################
source ./mmz_layout.cfg

mmz_start=$mem_mmz_start;	# mmz start addr
mmz_size=$mem_mmz_size;		# mmz size

fb0_start=$fb0_start;
fb0_size=$fb0_size;
fb0_fbc_eb=$fb0_fbc_eb;

fb4_start=$fb4_start;
fb4_size=$fb4_size;
fb4_fbc_eb=$fb4_fbc_eb;

####################hex change to dec#############################
mmz_size=`echo "$mmz_size" |
awk 'BEGIN { temp = 0; }
{
        temp = $1/1024/1024;
}
END { printf("%dM\n", temp); }'`

echo "mmz_size=$mmz_size;"

##################################################################

insert_ko()
{
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

	#driver load
	insmod fyosal.ko
	insmod mmz.ko mmz=anonymous,0,$mmz_start,$mmz_size anony=1 || report_error
	insmod fybase.ko logbuflen=16384
	insmod sys.ko
	insmod i2s.ko
	insmod extac.ko
	insmod fhfb.ko video="vram0_size:4000,vram1_size:0,vram2_size:0,vram3_size:0,vram4_size:0,vram5_size:0,vram6_size:0,vram7_size:0," fbc=0 fbc1=0 fhfb0_start=0 fhfb0_size=0 fhfb0_fbc=0 fhfb4_start=$fb4_start fhfb4_size=$fb4_size fhfb4_fbc=$fb4_fbc_eb 
	insmod lcm_module.ko
	insmod lcm.ko lcm0_id_cfg=1 lcm1_id_cfg=2 lcm0_id=0 lcm1_id=0
	insmod vou.ko
	insmod vdu.ko
	insmod jpeg.ko
	insmod g2d.ko
#	insmod g2d.ko wkbuf=1024 vgsbuf=1024 rotbuf=1024
	insmod acw.ko
}

remove_ko()
{
	rmmod acw
	rmmod g2d
	rmmod jpeg
	rmmod vdu
	rmmod vou
	rmmod lcm
	rmmod lcm_module
	rmmod extac
	rmmod i2s
	rmmod sys
	rmmod fybase
	rmmod mmz
	rmmod fyosal
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

