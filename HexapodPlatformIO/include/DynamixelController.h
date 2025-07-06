#ifndef DYNAMIXEL_CONTROLLER_H
#define DYNAMIXEL_CONTROLLER_H

#include <Dynamixel2Arduino.h>
#include "Config.h"


class DynamixelController {
  public:
    DynamixelController(HardwareSerial& serial, uint8_t dirPin);
    void begin(int baudrate);
    void setPosition(uint8_t id, float angleDeg);
    void setSpeed(uint8_t id, float rpm);
    float readPosition(uint8_t id);
    void initializeServo(uint8_t id, float minAngle, float maxAngle);

  private:
    Dynamixel2Arduino dxl;
    struct ServoLimit {
      float minAngle;
      float maxAngle;
    };
    ServoLimit limits[256];
};

#endif
