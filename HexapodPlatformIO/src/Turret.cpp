#include "Turret.h"

Turret::Turret(uint8_t panID, uint8_t tiltID, uint8_t sensorID, DynamixelController* controller)
  : pan(panID), tilt(tiltID), sensor(sensorID), dxlCtrl(controller) {}


// Initialize the turret servos
void Turret::initialize() {
  dxlCtrl->initializeServo(pan, -90, 90);                           // Initialize pan servo with limits
  dxlCtrl->initializeServo(tilt, -45, 45);                          // Initialize tilt servo with limits
}

// Rotate the turret to specified angles
void Turret::rotateTurret(float panAngle, float tiltAngle) {
  dxlCtrl->setPosition(pan, panAngle);                              // Set pan angle
  dxlCtrl->setPosition(tilt, tiltAngle);                            // Set tilt angle 
}

// Reset the turret to default position
void Turret::resetTurret() {
  rotateTurret(0,0);                                                // Reset turret to default position
}

// Read the sensor value
int Turret::readSensor() {
  return dxlCtrl->readPosition(sensor);                             // Read the sensor
}
