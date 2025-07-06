#ifndef TURRET_H
#define TURRET_H

#include "DynamixelController.h"
#include "Config.h"


class Turret {
  public:
    Turret(uint8_t panID, uint8_t tiltID, uint8_t sensorID, DynamixelController* controller); // Constructor
    void initialize();                                                                        // Initialize the turret servos
    void rotateTurret(float panAngle, float tiltAngle);                                       // Rotate the turret to specified angles
    void resetTurret();                                                                       // Reset the turret to default position
    int readSensor();                                                                         // Read the sensor value

  private:
    uint8_t pan, tilt, sensor;                                                                // Servo IDs for pan, tilt and sensor
    DynamixelController* dxlCtrl;                                                             // Pointer to the Dynamixel controller instance
};

#endif
