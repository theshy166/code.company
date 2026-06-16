#!/bin/sh

output=$(ip a | grep 'wlan0: <NO-CARRIER,')

if [ -n "$output" ]; then

    nocarrier_string=$(echo "$output" | sed -E 's/.*<([^>]+)>.*/\1/')
    echo "Found NO-CARRIER: $nocarrier_string"
    exit 0
else
    echo "wlan0 is in Connecte state "
    exit 1
fi
