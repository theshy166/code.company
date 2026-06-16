#!/system/bin/sh
while true
do
        starttime=`date`
        echo " - start time : $starttime"
        iperf3 -c 10.0.34.xxx  -b 20M -t 86400 -i 20 -p 5204
		
		
		
        endtime=`date`
        echo " - end time : $endtime"
done
