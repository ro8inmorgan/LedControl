# Before you begin, are you using MinUI?
    
Did you know MinUI actually is not doing vsync because of a wrong cores implementation? (just play something horizontal scrolling and it will skip a frame every few seconds, yes the game is not supposed to do that!) 
No worries, I have build a version with a reworked emulator engine
[https://github.com/ro8inmorgan/MinUI](https://github.com/ro8inmorgan/MinUI)

Now onto the Stuff why you here for:
    
# LedControl

Led Control app for TrimUI Brick (It's been reported on reddit that it works with Smart pro too)  
Works with Stock OS (Mix, cross etc) and MinUI. Might work with Knulli too but not tested

![alt text](screenshot.png "Screenshot LedControl")

# Installation

You can download the latest version of the Led Control app from the [Releases page](https://github.com/ro8inmorgan/LedControl/releases).

## Stock/StockMix/Crossmix:

Just unzip and copy the LedControl.pak folder to the Apps folder on your SD Card, then bootup your TrimUI Brick and the Led Control app will be available in the Apps menu.

## MinUI:

_starting with MinUI 20250226-0, the TrimUI Brick and SmartPro folders have been merged, and the pak must be placed in the 5040 folder_

#### MinUI 20250226-0 or later

- Copy the LedControl.pak folder **Tools/tg5040/** folder on your SD card, reboot your brick and app should be available in the Tools section

#### earlier versions

- Copy the LedControl.pak folder **Tools/tg3040/** folder on your SD card, reboot your brick and app should be available in the Tools section

# Instructions

L&R buttons to switch between lights
D-pad up/down to select the setting and left/right to adjust its value

B Button to quit the application  
Please quit the app using the B button and not by holding the power button as that will also stop the Led Control background process

# Uninstall

Use the seperate included LCRemove.pak app, copy it to your SD card same way, run the app and reboot. Leds will be restored to default functions. If you run Led Control app it will install again

# TODO:
- Got many more effects I want to add
- Maybe react to toggle to turn on/off leds
- Create version for Knulli
- If possible maybe ambilight function, need to test if I can access the screen buffer
   
# Special thanks   
Thank you [Retro Game Corps](https://retrogamecorps.com) for mentioning my App in your TrimUI Brick guide!
   
# Donations

Donations are always welcome https://ko-fi.com/robindev :) :)

#Thanks   
[@taylorbird](https://github.com/taylorbird)! for updating README with new instructions
