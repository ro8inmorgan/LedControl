cp -f main.c lcdaemon.c fakeleds.c settings.txt main.ttf ../../trimui-smart-pro-toolchain/workspace/
docker exec -it trimui gcc -o fakeleds fakeleds.c -lSDL2 -lm
docker exec -it trimui  gcc -o main main.c -lSDL2 -lSDL2_ttf -lm
docker exec -it trimui gcc -o lcdaemon lcdaemon.c -lSDL2 -lm

mv -f ../../trimui-smart-pro-toolchain/workspace/fakeleds ../../trimui-smart-pro-toolchain/workspace/main ../../trimui-smart-pro-toolchain/workspace/lcdaemon ./build/
cp -f main.ttf colors.txt ./build/
cp -f settings.txt /etc/LedControl/
cp -f settings.txt ./build/