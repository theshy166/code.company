#!/bin/sh


DDR_TYPE=$1
echo "***************************full maxloading start***************************"
cd ../bsp/
cp iperf3 /bin/
cp libiperf.so.0 /lib/
echo "***************************BSP maxloading start***************************"
./BSP_Maxloading.sh &
echo "***************************NET maxloading start***************************"
sh ../bsp/iperf.sh &

if [ "$?" -ne 0 ];then
	echo "BSP test is not ready"
	exit 1
fi

cd ../maxlod_script/
./dsp_stress_test_scene1.sh &
