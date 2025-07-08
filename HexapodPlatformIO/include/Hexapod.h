#ifndef HEXAPOD_H
#define HEXAPOD_H

#include "Leg.h"
#include "Config.h"


class Hexapod {
  public:
    Hexapod(DynamixelController* controller); // Constructor
    void initialize();                        // Initialize the hexapod
    void setGaitType(int gaitType);           // Set the gait type
    void setGaitSpeed(float speed);           // Set the gait speed
   
    void standUp();                           // Stand up the hexapod
    void sitDown();                           // Sit down the hexapod
     
  private:
    Leg* legs[6];                             // Array of legs
    DynamixelController* dxlCtrl;             // Dynamixel controller instance
    float robotSpeed;                         // Current gait speed
    int gaitState;                            // Current gait state
};

#endif
