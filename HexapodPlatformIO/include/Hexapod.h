#ifndef HEXAPOD_H
#define HEXAPOD_H

#include "Leg.h"

class Hexapod {
  public:
    Hexapod(DynamixelController* controller);
    void initialize();
    void setGait(int gaitType);
    void setSpeed(float speed);
    void moveForward();
    void moveBackward();
    void turn(float angle);
    void stop();

  private:
    Leg* legs[6];
    DynamixelController* dxlCtrl;
    float robotSpeed;
    int gaitState;
};

#endif
