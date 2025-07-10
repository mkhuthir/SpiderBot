#include "RC100Controller.h"

RC100Controller::RC100Controller() : latestCommand(0) {}

void RC100Controller::begin(long baud) {
    Serial1.begin(baud);  // UART1 on OpenCR
}

void RC100Controller::update() {
    while (Serial1.available() >= 3) {
        uint8_t header = Serial1.read();
        if (header == 0xFF) {
            uint8_t data1 = Serial1.read();
            uint8_t data2 = Serial1.read();
            latestCommand = data1 | (data2 << 8);
        }
    }
}

uint16_t RC100Controller::getCommand() {
    return latestCommand;
}
// RC100Controller.cpp