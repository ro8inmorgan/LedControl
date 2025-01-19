#!/bin/sh
LCDAEMON_PATH="/etc/LedControl"

cd $(dirname "$0")
rm -R $LCDAEMON_PATH
/etc/init.d/lcservice disable
rm /etc/init.d/lcservice

