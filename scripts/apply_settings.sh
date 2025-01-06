#!/bin/bash


SETTINGS_FILE="assets/settings.txt"
BASE_DIR="/sys/class/led/anim"

while IFS= read -r line; do
    if [[ $line =~ ^\[(.*)\]$ ]]; then
        button_name="${BASH_REMATCH[1]}"
    elif [[ $line =~ ^effect=([0-9]+)$ ]]; then
        effect="${BASH_REMATCH[1]}"
        echo "$effect" > "$BASE_DIR/effect_$button_name"
    elif [[ $line =~ ^color=0x([0-9A-Fa-f]+)$ ]]; then
        color="${BASH_REMATCH[1]}"
        echo "$color" > "$BASE_DIR/effect_rgb_hex_$button_name"
    elif [[ $line =~ ^duration=([0-9]+)$ ]]; then
        duration="${BASH_REMATCH[1]}"
        echo "$duration" > "$BASE_DIR/effect_duration_$button_name"
    fi
done < "$SETTINGS_FILE"
