#ifedef DYNAMIXELCONTROLLER_H
#define DYNAMIXELCONTROLLER_H


#include <DynamixelWorkbench.h>
#include <vector>

class DynamixelController {
public:
    DynamixelController();
    bool begin(const char* device_name, uint32_t baudrate);                                             // initialize the controller with device name and baudrate
    bool initializeServo(uint8_t id);                                                                   // initialize a servo with default settings

    bool torqueOn(uint8_t id);                                                                          // turn on torque for a servo  
    bool torqueOff(uint8_t id);                                                                         // turn off torque for a servo

    bool setGoalPosition(uint8_t id, uint32_t position);                                                // set goal position of a servo 
    bool setGoalVelocity(uint8_t id, uint32_t velocity);                                                // set goal velocity of a servo

    bool readPresentPosition(uint8_t id, uint32_t &position);                                           // read current position of a servo

    bool syncWritePosition(const std::vector<uint8_t>& ids, const std::vector<uint32_t>& positions);    // write positions to multiple servos
    bool bulkReadPositions(const std::vector<uint8_t>& ids, std::vector<uint32_t>& positions);          // read positions of multiple servos
    
    DynamixelWorkbench* getWorkbench();                                                                 // if you need to expose the workbench pointer

private:
    DynamixelWorkbench dxl;                                                                             // DynamixelWorkbench instance to handle communication with servos
};

#endif // DYNAMIXELCONTROLLER_H
