# LedControl
Led Control app for TrimUI Brick (It's been reported on reddit that it works with Smart pro too)    
Works with Stock OS and Stockmix, might work with others but not tested

# Installation
You can download the latest version of the Led Control app from the [Releases page](https://github.com/ro8inmorgan/LedControl/releases).   
Just unzip and copy the LedControl folder to the Apps folder on your SD Card, then bootup your TrimUI Brick and the Led Control app will be available in the Apps menu.

# Instructions
L&R buttons to switch between lights
- f1 = function button 1
- f2 = function button 2
- m = top light bar
- lr = trigger buttons
   
D-pad up/down to select the setting and left/right to adjust its value   
Colors are currently in RGB hex format, will add a color preview later

Effect types are shown as numbers will name them properly in the UI later   
Effect types are:
- 1 linear (will use last used color as secondary)
- 2 fading
- 3 interval fading
- 4 static
- 5 blink 1
- 6 blink 2
- 7 blink 3
- 8 rainbow color cycle

***Note on linear effect 1***   
This will use the selected color for on and last used color for off, so for example if your previous effect was set to purple and you choose blue with this effect selected it will switch between blue and purple

B Button to quit the application   
Please quit the app using the B button and not by holding the power button as that will also stop the Led Control background process

# Important notes
This is an initial first version I just wanted to get this out to you guys quickly so I still have to fix some things but please note the following
- After completely turning off your TrimUI Brick (not standby), the Led Control app needs to be started again. 
- Brightness can be controlled in the TrimUI display settings
   
# TODO:
- Implementing auto start after cold boot
- Color picker on screen (now shows hex codes of the colors)
- Add more effects like reactive to controls
- Brightness control in the app itself instead of using settings for this, for ease of use
- General improvements of UI

# Donations
Donations are always welcome https://ko-fi.com/robindev :) :)

