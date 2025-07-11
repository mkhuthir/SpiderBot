#include "Leg.h"

Leg::Leg(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, DynamixelController* controller)
  : coxa(coxaID), femur(femurID), tibia(tibiaID), dxl(controller) {}

// Initialize the leg servos
void Leg::initialize() {
  dxl->initializeServo(coxa, COXA_MIN_ANGLE_DEG, COXA_MAX_ANGLE_DEG);       // Initialize coxa servo with limits
  dxl->initializeServo(femur, FEMUR_MIN_ANGLE_DEG, FEMUR_MAX_ANGLE_DEG);    // Initialize femur servo with limits
  dxl->initializeServo(tibia, TIBIA_MIN_ANGLE_DEG, TIBIA_MAX_ANGLE_DEG);    // Initialize tibia servo with limits
}

// Set angles for the leg joints
void Leg::setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle) {
  dxl->setGoalPosition(coxa, coxaAngle);      // Set coxa angle
  dxl->setGoalPosition(femur, femurAngle);    // Set femur angle
  dxl->setGoalPosition(tibia, tibiaAngle);    // Set tibia angle
}

    float getCoxaAngle(){};                        // Get current coxa angle
    float getFemurAngle(){};                      // Get current femur angle
    float getTibiaAngle(){};                     // Get current tibia angle 

void Leg::printJointAngles() {
  Serial.print("Coxa: ");
  Serial.print(getCoxaAngle());
  Serial.print(" | Femur: ");
  Serial.print(getFemurAngle());
  Serial.print(" | Tibia: ");
  Serial.println(getTibiaAngle());
}

// Lift the leg to a default position
void Leg::legUp() {
  setJointAngles(COXA_UP_ANGLE_DEG, FEMUR_UP_ANGLE_DEG, TIBIA_UP_ANGLE_DEG);        // Lift the leg by setting specific angles
}

// Move the leg forward to a default position
void Leg::legDown() {
  setJointAngles(COXA_DOWN_ANGLE_DEG, FEMUR_DOWN_ANGLE_DEG, TIBIA_DOWN_ANGLE_DEG);  // Move the leg down by setting specific angles
}
