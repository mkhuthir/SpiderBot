#ifndef LEG_H
#define LEG_H

#include "DynamixelController.h"

class Leg {
  public:
    Leg(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, DynamixelController* controller);
    void initialize();
    void setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle);
    void liftLeg();
    void moveLegForward();

  private:
    uint8_t coxa, femur, tibia;
    DynamixelController* dxlCtrl;
};

#endif
