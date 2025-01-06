# LedControl
A Led Control appication for TrimUI Brick based on [ro8inmorgan's Led Control app for TrimUI Brick](https://github.com/ro8inmorgan/LedControl)

# Installation
Installation/relase instructions will come once bugs are ironed out.

# Dev guide
All dev work has been performed in a docker container based on the [Trimui toolchain Docker image](https://git.crowdedwood.com/trimui-toolchain/) by neonloop. Realistically its simple enough to be built in any linux environment. 

Run 'make deps' to install the required dependencies, then 'make' to build all the targets. 'make package' will package everything up into the 'releases' folder.

## stock OS
- Copy 'LedControl' to the `Apps` folder


## MinUI
- Copy 'LedControl.pak' to 'Tools/tg3040'

# App Instructions 
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
