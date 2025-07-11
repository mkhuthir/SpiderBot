#ifndef DYNAMIXEL_CONTROLLER_H
#define DYNAMIXEL_CONTROLLER_H

#include <DynamixelWorkbench.h>
#include <vector>

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

    void initializeServo(uint8_t id);
    void moveServo(uint8_t id, uint16_t position, uint16_t speed);
    void enableTorque(uint8_t id);
    void disableTorque(uint8_t id);

    bool torqueOn(uint8_t id);
    bool torqueOff(uint8_t id);
    bool setGoalPosition(uint8_t id, uint32_t position);
    bool setGoalVelocity(uint8_t id, uint32_t velocity);

    bool readPresentPosition(uint8_t id, uint32_t &position);

    // NEW
    std::vector<uint8_t> scan(uint8_t start_id = 1, uint8_t end_id = 253);
    bool syncWritePosition(const std::vector<uint8_t>& ids, const std::vector<uint32_t>& positions);
    bool bulkReadPositions(const std::vector<uint8_t>& ids, std::vector<uint32_t>& positions);
};

#endif

