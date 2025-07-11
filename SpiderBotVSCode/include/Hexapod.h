#ifndef HEXAPOD_H
#define HEXAPOD_H

#include "Leg.h"
#include "Config.h"


class Hexapod {
  public:
    Hexapod(DynamixelController* controller); // Constructor
    void initialize();                        // Initialize the hexapod
    void setGaitType(int gaitType);           // Set the gait type
    void setGaitSpeed(float gaitSpeed);       // Set the gait speed
    void setServoSpeed(float servoSpeed);     // Set the servo speed
    
    void standUp();                           // Stand up the hexapod
    void sitDown();                           // Sit down the hexapod
    void printLegsStatus();                   // Print the status of all legs
 
  private:
    Leg*                  legs[6];      // Array of legs
    DynamixelController*  dxlCtrl;      // Dynamixel controller instance
    int                   gaitType;     // Current gait type
    float                 gaitSpeed;    // Current gait speed
    float                 servoSpeed;   // Current servo speed
};

#endif // HEXAPOD_H