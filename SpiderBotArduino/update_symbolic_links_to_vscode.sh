#!/usr/bin/sh

echo "Updating Symbolic Links to point to all files in VSCode folder"

rm *.h
ln -s ../SpiderBotVSCode/include/*.h .

rm *.cpp
ln -s ../SpiderBotVSCode/src/*.cpp .

rm main.cpp
rm SpiderBotArduino.ino 
ln -s ../SpiderBotVSCode/src/main.cpp SpiderBotArduino.ino

