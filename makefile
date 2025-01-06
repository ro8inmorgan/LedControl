CC=/opt/aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
CFLAGS= -Iinclude -Wall
LDFLAGS=-lSDL2 -lSDL2_ttf -lm
PROJECT_NAME=LedControl

.PHONY: all clean deps

all: main lcdaemon test

main:
	mkdir -p build
	$(CC) $(CFLAGS) src/main.c src/option.c -o build/$@ $(LDFLAGS)

lcdaemon:
	mkdir -p build
	$(CC) $(CFLAGS) src/lcdaemon.c -o build/$@ -lm

test:
	mkdir -p build
	$(CC) $(CFLAGS) src/test.c -o build/$@ $(LDFLAGS)

package: 
	# Create general project package
	mkdir -p release/${PROJECT_NAME}
	cp scripts/ledcontrol_launch.sh release/${PROJECT_NAME}/launch.sh
	cp assets/* build/* release/${PROJECT_NAME}

	# Create project package for MinUI
	mkdir -p release/${PROJECT_NAME}.pak
	cp scripts/minui_launch.sh release/${PROJECT_NAME}.pak/launch.sh
	cp assets/* build/* release/${PROJECT_NAME}.pak
	
	# Create test project package for MinUI
	mkdir -p release/TEST.pak
	cp scripts/test_launch.sh release/TEST.pak/launch.sh
	cp build/test release/TEST.pak
	
clean:
	rm -rf build release

deps:
	apt update
	apt install sudo
	sudo apt-get update
	sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev




