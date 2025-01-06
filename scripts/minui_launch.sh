#!/bin/sh
# Becomes LedControl.pak/launch.sh

echo $0 $*
cd $(dirname "$0")

# Check if deamon is running
if pgrep -f "lcdaemon" >/dev/null; then
    echo "lcdaemon is already running" > launch.log
else
    ./lcdaemon &
    echo "lcdaemon started" > launch.log
fi

# turn LEDS on
echo 60 > /sys/class/led_anim/max_scale
echo 60 > /sys/class/led_anim/max_scale_lr
echo 60 > /sys/class/led_anim/max_scale_f1f2

./main > ledcontrol.log 2>&1
