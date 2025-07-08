#ifndef TURRET_H
#define TURRET_H

#include "DynamixelController.h"
#include "Config.h"


class Turret {
  public:
    Turret(uint8_t panID, uint8_t tiltID, DynamixelController* controller); // Constructor
    void initialize();                                                      // Initialize the turret servos
    void rotateTurret(float panAngle, float tiltAngle);                     // Rotate the turret to specified angles
    void resetTurret();                                                     // Reset the turret to default position
        
  private:
    uint8_t pan, tilt;                                                      // Servo IDs for pan, tilt and sensor
    DynamixelController* dxlCtrl;                                           // Pointer to the Dynamixel controller instance
};

#endif
