#!/bin/sh

	./memcopy_cpu2dmc 1024000 &
while true
	do
		./perfmon 5000 1200 &
		sleep 10
	done
