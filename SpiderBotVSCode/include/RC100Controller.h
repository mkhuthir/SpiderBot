#ifndef RC100CONTROLLER_H
#define RC100CONTROLLER_H

#include <Arduino.h>

// RC-100B button values
#define RC100_BTN_U    0x01
#define RC100_BTN_D    0x02
#define RC100_BTN_L    0x04
#define RC100_BTN_R    0x08
#define RC100_BTN_1    0x10
#define RC100_BTN_2    0x20
#define RC100_BTN_3    0x40
#define RC100_BTN_4    0x80

class RC100Controller {
private:
    uint16_t latestCommand;

public:
    RC100Controller();

    void begin(long baud = 57600);
    void update();  // Must be called in loop()
    uint16_t getCommand();  // Returns last command value
};

#endif
// RC100Controller.h