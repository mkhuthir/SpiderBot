#ifndef HEXAPOD_H
#define HEXAPOD_H

#include "Leg.h"
#include "Config.h"


class Hexapod {
  public:
    Hexapod(DynamixelController* controller); // Constructor
    void initialize();                        // Initialize the hexapod
    void setGait(int gaitType);               // Set the gait type
    void setSpeed(float speed);               // Set the gait speed
    void moveForward();                       // Move the hexapod forward 
    void moveBackward();                      // Move the hexapod backward
    void turn(float angle);                   // Turn the hexapod by a specified angle
    void stop();                              // Stop the hexapod

  private:
    Leg* legs[6];                             // Array of legs
    DynamixelController* dxlCtrl;             // Dynamixel controller instance
    float robotSpeed;                         // Current gait speed
    int gaitState;                            // Current gait state
};

#endif
