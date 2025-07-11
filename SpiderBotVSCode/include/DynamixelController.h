#pragma once    // Include guard to prevent multiple inclusions

#include <DynamixelWorkbench.h>
#include <vector>

class DynamixelController {
public:
    DynamixelController();
    bool begin(const char* device_name, uint32_t baudrate);
    bool initializeServo(uint8_t id);

    bool torqueOn(uint8_t id);
    bool torqueOff(uint8_t id);

    
    bool setGoalPosition(uint8_t id, uint32_t position);
    bool setGoalVelocity(uint8_t id, uint32_t velocity);

    bool readPresentPosition(uint8_t id, uint32_t &position);

    bool syncWritePosition(const std::vector<uint8_t>& ids, const std::vector<uint32_t>& positions);    // write positions to multiple servos
    bool bulkReadPositions(const std::vector<uint8_t>& ids, std::vector<uint32_t>& positions);          // read positions of multiple servos
    
    DynamixelWorkbench* getWorkbench();                                                                 // if you need to expose the workbench pointer

private:
    DynamixelWorkbench dxl;
};