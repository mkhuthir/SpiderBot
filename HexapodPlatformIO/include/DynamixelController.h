#ifndef DYNAMIXEL_CONTROLLER_H
#define DYNAMIXEL_CONTROLLER_H

#include <Dynamixel2Arduino.h>
#include "Config.h"


class DynamixelController {
  public:
    DynamixelController(HardwareSerial& serial, uint8_t dirPin);              // Constructor
    
    void  begin(int baudrate, float protocolVersion = DXL_PROTOCOL_VERSION);  // Initialize the controller
    void  initializeServo(uint8_t id, float minAngle, float maxAngle);        // Initialize a servo with its ID and angle limits

    bool  isConnected();                                                      // Check if the controller is connected
    void  setPosition(uint8_t id, float angleDeg);                            // Set servo position in degrees
    void  setSpeed(uint8_t id, float rpm);                                    // Set servo speed in RPM
    float readPosition(uint8_t id);                                           // Read current servo position in degrees  
    

  private:
    Dynamixel2Arduino dxl;                // Dynamixel controller instance

    struct ServoLimit {                   // Struct to hold servo limits
      float minAngle;                     // Minimum angle limit in degrees
      float maxAngle;                     // Maximum angle limit in degrees
    };
    
    ServoLimit limits[MAX_NO_OF_SERVOS];  // Array to hold limits for servos
};

#endif
