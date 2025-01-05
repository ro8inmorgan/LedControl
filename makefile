CC=gcc
CFLAGS=-Wall
LDFLAGS=-lSDL2 -lSDL2_ttf -lm
PROJECT_NAME=LedControl

.PHONY: all main clean deps

all: main lcdaemon 

main: main.c option.c
	mkdir -p build
	$(CC) $(CFLAGS) main.c option.c -o build/$@ $(LDFLAGS)

lcdaemon: lcdaemon.c
	mkdir -p build
	$(CC) $(CFLAGS) lcdaemon.c -o build/$@ -lm

package: 
	# Create general project package
	mkdir -p release/${PROJECT_NAME}
	cp config.json icon.png launch.sh main.ttf settings.txt build/* release/${PROJECT_NAME}

	# Create project package for MinUI
	mkdir -p release/${PROJECT_NAME}.pak
	cp config.json icon.png main.ttf settings.txt build/* release/${PROJECT_NAME}.pak
	cp minui_launch.sh release/${PROJECT_NAME}.pak/launch.sh

clean:
	rm -rf build release


deps:
	sudo apt-get update
	sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev