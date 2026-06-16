#!/bin/sh
mkdir -p /tmp/run
LOCKFILE="/tmp/run/$(basename "$0").lock"

# Check if lock file exists
if [ -f "$LOCKFILE" ]; then
    PID=$(cat "$LOCKFILE")

    # Check if process exists using /proc directory
    if [ -d "/proc/$PID" ]; then
        # Verify it's a valid process by checking executable
        if [ -f "/proc/$PID/exe" ]; then
            echo "Script is already running, PID: $PID" >&2
            exit 1
        fi
    fi

    # Stale lock file - remove it
    echo "Removing stale lock file..." >&2
    rm -f "$LOCKFILE"
fi

# Create new lock file
echo $$ > "$LOCKFILE"

# Cleanup function
cleanup() {
    rm -f "$LOCKFILE"
    exit 0
}

# Trap termination signals
trap 'cleanup' SIGINT SIGTERM SIGHUP SIGQUIT

pkill  wpa_supplicant;
pkill  udhcpc;
sleep 1
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vendor/qua/wifi/lib;
wpa_supplicant -iwlan0 -D nl80211 -c /data/wifi/wpa_supplicant.conf -B &
echo 0 >  /proc/$(pidof wpa_supplicant)/oom_score_adj
sleep 1
counter=0
while true; do
    interface_status=$(ip addr show wlan0 | grep LOWER_UP)
    if [ -n "$interface_status" ] || [ $counter -gt 60 ]; then
        echo "wlan0 is ap connected,time to start udhcpc"
        udhcpc -q -iwlan0 &
        break
    fi
    counter=$((counter + 1))
    sleep 0.5
done
exit 0
