#!/bin/sh

while true; do
    pidof adbd >/dev/null 2>&1 || /usr/bin/adbd >/dev/null 2>&1 &
    sleep 2
done
