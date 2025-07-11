#include "Turret.h"

Turret::Turret(uint8_t panID, uint8_t tiltID, DynamixelController* controller)
  : pan(panID), tilt(tiltID), dxl(controller) {}

// Initialize the turret servos
void Turret::initialize() {
  dxl->initializeServo(pan);      // Initialize pan servo with limits
  dxl->initializeServo(tilt);     // Initialize tilt servo with limits
}

// Rotate the turret to specified angles
void Turret::rotateTurret(float panAngle, float tiltAngle) {
  dxl->setGoalPosition(pan, panAngle);                              // Set pan angle
  dxl->setGoalPosition(tilt, tiltAngle);                            // Set tilt angle 
}

// Rotate turret to the left
void Turret::rotateTurretLeft() {
  rotateTurret(60,60);  // Rotate turret to the left
}

// Rotate turret to the right
void Turret::rotateTurretRight() {
  rotateTurret(270,60); // Rotate turret to the right
} 

// Rotate turret up
void Turret::rotateTurretUp() {
  rotateTurret(60, 30);  // Rotate turret up
}

// Rotate turret down
void Turret::rotateTurretDown() {
  rotateTurret(60, 90); // Rotate turret down
} 

// Rotate the turret to default position
void Turret::rotateTurretHome() {
  rotateTurret(TURRET_PAN_DEFAULT_DEG, TURRET_TILT_DEFAULT_DEG);    // Reset turret to default position
}

// Print current turret angles to Serial
void Turret::printTurretStatus() {
  Serial.println("\nTurret Status:");
  Serial.print("Pan: ");
  Serial.print(dxl->getGoalPosition(pan));
  Serial.print(" | Tilt: ");
  Serial.println(dxl->getGoalPosition(tilt));
  Serial.println("---");
}