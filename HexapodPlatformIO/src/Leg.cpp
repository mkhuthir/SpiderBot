#include "Leg.h"

Leg::Leg(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, DynamixelController* controller)
  : coxa(coxaID), femur(femurID), tibia(tibiaID), dxlCtrl(controller) {}

void Leg::initialize() {
  dxlCtrl->initializeServo(coxa, -90, 90);
  dxlCtrl->initializeServo(femur, -45, 90);
  dxlCtrl->initializeServo(tibia, -90, 90);
}

void Leg::setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle) {
  dxlCtrl->setPosition(coxa, coxaAngle);
  dxlCtrl->setPosition(femur, femurAngle);
  dxlCtrl->setPosition(tibia, tibiaAngle);
}

void Leg::liftLeg() {
  setJointAngles(0, 30, -45);
}

void Leg::moveLegForward() {
  setJointAngles(10, 15, -30);
}
