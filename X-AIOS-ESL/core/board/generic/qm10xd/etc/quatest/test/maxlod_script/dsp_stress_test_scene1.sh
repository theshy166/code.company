#!/bin/sh


echo "[1],install ko"

cd ../../ko/
./loadko_maxlod_24M.sh -i
cd -

sleep 2
echo "[2], run stress test"
cd ../
./maxloading test.ini 0 &
sleep 2

echo "[3], run dsp performance"
cd -
sleep 5
#./performance_DSP.sh &
#./fps_print.sh 1 0 &


