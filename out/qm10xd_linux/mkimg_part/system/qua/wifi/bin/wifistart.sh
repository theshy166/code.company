#!/bin/sh

loop=1


while [ ${loop}  -le  $1  ]; do
#for loop in {1..10}; do

  #echo "Iteration $loop"
  sleep 1
  echo "sleep 1"
  ipaddr=$(ifconfig -a|busybox grep "inet"| busybox grep -v "127.0.0.1"|busybox grep -v "inet6"|awk '{print $2}')

 if [  -n "$ipaddr" ];then
	ipaddr=$(echo "$ipaddr" | busybox grep -oE '([0-9]{1,3}\.){3}[0-9]{1,3}')
	echo "$ipaddr"
	exit 0
  else
    echo "wifi connting ...."
  fi
  loop=$((loop += 1))
done


echo "WiFI connected Failure!!!"

exit 1
