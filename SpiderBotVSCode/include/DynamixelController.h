#ifndef DYNAMIXEL_CONTROLLER_H
#define DYNAMIXEL_CONTROLLER_H

#include <DynamixelWorkbench.h>

class DynamixelController
{
  private:
    DynamixelWorkbench dxl;
    const char* device_name;
    const uint32_t baudrate;

  public:
    DynamixelController(const char* device, uint32_t baud);
    bool begin();
    bool ping(uint8_t id);
    bool torqueOn(uint8_t id);
    bool torqueOff(uint8_t id);
    bool setGoalPosition(uint8_t id, uint32_t position);
    bool setGoalVelocity(uint8_t id, uint32_t velocity);
    bool readPresentPosition(uint8_t id, uint32_t &position);
};

#endif
