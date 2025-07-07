#include "Hexapod.h"

Hexapod::Hexapod(DynamixelController* controller) : dxlCtrl(controller) {
  legs[0] = new Leg(1,2,3, controller);
  legs[1] = new Leg(4,5,6, controller);
  legs[2] = new Leg(7,8,9, controller);
  legs[3] = new Leg(10,11,12, controller);
  legs[4] = new Leg(13,14,15, controller);
  legs[5] = new Leg(16,17,18, controller);
}

// Initialize the hexapod
void Hexapod::initialize() {
  for(int i=0; i<6; i++)
    legs[i]->initialize();
}

// Set the gait type
void Hexapod::setGait(int gaitType) {
  gaitState = gaitType;
}

// Set the gait speed
void Hexapod::setSpeed(float speed) {
  robotSpeed = speed;
}

// Stand up the hexapod
void Hexapod::standUp() {
  for(int i=0; i<6; i++)
    legs[i]->legUp();
}

// Sit down the hexapod
void Hexapod::sitDown() {
  for(int i=0; i<6; i++)
    legs[i]->legDown();
}





