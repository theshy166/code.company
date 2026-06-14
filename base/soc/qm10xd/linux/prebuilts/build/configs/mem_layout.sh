#!/bin/sh

#mem_start:0x40000000

PWD=`pwd`
KO_DIR=${PWD}/../prebuilt/media/ko

source ./mem_layout.cfg

#build mem_layout file
`touch "mmz_layout.cfg"`
`chmod 0755 *layout*`

#mmz_layout
echo "mem_total=64" >> mmz_layout.cfg
echo "mem_mmz_start=$mem_mmz_start" >> mmz_layout.cfg
echo "mem_mmz_size=$mem_mmz_size" >> mmz_layout.cfg

echo "fb0_start=0x$MC_SHOWLOGO_YADDR0" >> mmz_layout.cfg
echo "fb0_size=$MC_DHD0_OSD_SIZE" >> mmz_layout.cfg
echo "fb0_fbc_eb=$MC_DHD0_OSD_FBC_EB" >> mmz_layout.cfg

echo "fb4_start=0x$MC_SHOWLOGO_YADDR1" >> mmz_layout.cfg
echo "fb4_size=$MC_DHD1_OSD_SIZE" >> mmz_layout.cfg
echo "fb4_fbc_eb=$MC_DHD1_OSD_FBC_EB" >> mmz_layout.cfg

mv mmz_layout.cfg $KO_DIR/
