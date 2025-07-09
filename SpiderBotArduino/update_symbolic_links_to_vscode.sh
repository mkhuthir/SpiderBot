#!/usr/bin/sh

echo "Updating Symbolic Links to point to all files in VSCode folder"
ln -s ../SpiderBotVSCode/include/*.h .
ln -s ../SpiderBotVSCode/src/*.cpp .
ln -s ../SpiderBotVSCode/src/main.cpp SpiderBotArduino.ino

