#include "Leg.h"

Leg::Leg(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, DynamixelController* controller)
  : coxa(coxaID), femur(femurID), tibia(tibiaID), dxlCtrl(controller) {}

// Initialize the leg servos
void Leg::initialize() {
  dxlCtrl->initializeServo(coxa, -90, 90);    // Initialize coxa servo with limits
  dxlCtrl->initializeServo(femur, -45, 90);   // Initialize femur servo with limits
  dxlCtrl->initializeServo(tibia, -90, 90);   // Initialize tibia servo with limits
}

// Set angles for the leg joints
void Leg::setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle) {
  dxlCtrl->setPosition(coxa, coxaAngle);      // Set coxa angle
  dxlCtrl->setPosition(femur, femurAngle);    // Set femur angle
  dxlCtrl->setPosition(tibia, tibiaAngle);    // Set tibia angle
}

// Lift the leg to a default position
void Leg::liftLeg() {
  setJointAngles(0, 30, -45);   // Lift the leg by setting specific angles
}

// Move the leg forward to a default position
void Leg::moveLegForward() {
  setJointAngles(10, 15, -30);  // Move the leg forward by setting specific angles
}
