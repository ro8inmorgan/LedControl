#!/bin/sh

# Check if deamon is running
if pgrep -f "lcdaemon" >/dev/null; then
    echo "lcdaemon is already running" > launch.log
else
    ./lcdaemon &
    echo "lcdaemon started" > launch.log
fi

./main > ledcontrol.log 2>&1
