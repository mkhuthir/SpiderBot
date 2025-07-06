#include "Hexapod.h"

Hexapod::Hexapod(DynamixelController* controller) : dxlCtrl(controller) {
  legs[0] = new Leg(1,2,3, controller);
  legs[1] = new Leg(4,5,6, controller);
  legs[2] = new Leg(7,8,9, controller);
  legs[3] = new Leg(10,11,12, controller);
  legs[4] = new Leg(13,14,15, controller);
  legs[5] = new Leg(16,17,18, controller);
}

void Hexapod::initialize() {
  for(int i=0; i<6; i++)
    legs[i]->initialize();
}

void Hexapod::setGait(int gaitType) {
  gaitState = gaitType;
}

void Hexapod::setSpeed(float speed) {
  robotSpeed = speed;
}

void Hexapod::moveForward() {
  for(int i=0; i<6; i++)
    legs[i]->moveLegForward();
}

void Hexapod::moveBackward() {}

void Hexapod::turn(float angle) {}

void Hexapod::stop() {
  for(int i=0; i<6; i++)
    legs[i]->setJointAngles(0,0,0);
}


