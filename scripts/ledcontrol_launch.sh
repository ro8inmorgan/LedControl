#!/bin/sh
# Becomes LedControl/launch.sh

# Check if deamon is running
if pgrep -f "settings_daemon" >/dev/null; then
    echo "settings_daemon is already running" > launch.log
else
    ./settings_daemon &
    echo "settings_daemon started" > launch.log
fi

./main > ledcontrol.log 2>&1
