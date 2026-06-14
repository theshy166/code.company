cd ko
./loadko.sh -i
cd ..

:<<EOF
busybox devmem 0x102000BC 32 0x00
busybox devmem 0x102000C0 32 0x00
busybox devmem 0x102000C4 32 0x00
busybox devmem 0x102000C8 32 0x00
busybox devmem 0x102000CC 32 0x00
busybox devmem 0x102000D0 32 0x00
busybox devmem 0x102000D4 32 0x00
busybox devmem 0x102000D8 32 0x00
busybox devmem 0x102000DC 32 0x00
busybox devmem 0x102000E0 32 0x00
busybox devmem 0x102000E4 32 0x00
busybox devmem 0x102000E8 32 0x00


busybox devmem 0x102000D4 32 0x00
busybox devmem 0x102000D8 32 0x00
busybox devmem 0x102000DC 32 0x00
busybox devmem 0x102000E0 32 0x00
busybox devmem 0x102000E4 32 0x00
busybox devmem 0x102000E8 32 0x00

busybox devmem 0x102000BC 32 0x00
busybox devmem 0x102000C0 32 0x00
busybox devmem 0x102000C4 32 0x00
busybox devmem 0x102000C8 32 0x00
busybox devmem 0x102000CC 32 0x00
busybox devmem 0x102000D0 32 0x00
EOF

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/sd/quatest/xos/bin:/sd/quatest/xos/lib
./xos/bin/qxosui &
echo "qxosui"
