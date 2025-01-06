#!/bin/sh
# Becomes LedControl.pak/launch.sh
export BASE_LED_PATH="/sys/class/led_anim"
# export LED_PATH="$BASE_LED_PATH/max_scale"
# export FRONT_LED_PATH="$BASE_LED_PATH/max_scale_f1f2"
# export BACK_LED_PATH="$BASE_LED_PATH/max_scale_lr"
# export MINUI_PAK_SH_PATH="./mnt/SDCARD/.system/tg3040/paks/MinUI.pak/launch.sh"
echo $0 $*
cd $(dirname "$0")

# Allow the application to change the LEDs
chmod a+w $LED_PATH/* file_lock.log 2>&1

# This is a daemon that listens for changes to settings.txt and applies
# them to the corresponding LED file
if pgrep -f "lcdaemon" >/dev/null; then
    echo "lcdaemon is already running" > launch.log
else
    ./lcdaemon &
    echo "lcdaemon started" > launch.log
fi

# turn LEDS on
echo 60 | tee $LED_PATH $FRONT_LED_PATH $BACK_LED_PATH



# Remove lines that disable LEDs and potentially any previous apply_settings.sh calls in MinUI.pak/launch.sh
# sed -i '/echo 0 > \/sys\/class\/led_anim\/max_scale/,+2d; /apply_settings.sh/d' $MINUI_PAK_SH_PATH
# sed -i '/# leds off/a\echo "$(pwd)/apply_settings.sh)"' $MINUI_PAK_SH_PATH

./main > ledcontrol.log 2>&1

# Prevent other applications from changing the LEDs
chmod a-w $LED_PATH/* 

# Kill the lcddaemon when the application exits
killall lcdaemon