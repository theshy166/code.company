#!/bin/sh
mac_nu=10
while [ $mac_nu -gt 9 ]
do
	read -p "please input MAC address number(0,2,4,6,8):" mac_nu
	if [ -n "$(echo ${mac_nu}| sed -n "/^[0-9]\+$/p")" ] && [ $(($mac_nu%2)) -eq 0 ];then
		mac_nu=${mac_nu}
	else
		mac_nu=10
	fi
done
#read -p "please input yuor native ip:" serverip
let ip_nu=190+${mac_nu}
confighw=02:A2:94:A3:A4:A${mac_nu}
configip=192.168.2.${ip_nu}
#serverip=10.0.34.80
#serverip=10.0.38.72
serverip=192.168.2.1
ipaddr=`ifconfig | grep "inet" | awk '{print $2}' | awk -F ":" '{print $2}'`
if [ -n "$ipaddr" ] || [ -z "$ipaddr" ];then
	ifconfig eth0 down
	ifconfig eth0 hw ether "$confighw"
	ifconfig eth0 "$configip" netmask 255.255.255.0 up

	sleep 2s
	ip_linkup=`dmesg | grep "eth0: Link is Up" | tail -1`
	#echo -e "\033[47;30m>ip-linkup:${ip_linkup}\033[0m"
	if [ -z "$ip_linkup" ];then

		echo -e "\033[31merror:gmac_test failed,eth0 ip_linkup failed\033[0m" >> /slt_log/slt_log.log
		return 1
	fi
fi
return 0
