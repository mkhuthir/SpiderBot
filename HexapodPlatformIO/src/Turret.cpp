#include "Turret.h"

Turret::Turret(uint8_t panID, uint8_t tiltID, uint8_t sensorID, DynamixelController* controller)
  : pan(panID), tilt(tiltID), sensor(sensorID), dxlCtrl(controller) {}


// Initialize the turret servos
void Turret::initialize() {
  dxlCtrl->initializeServo(pan,TURRET_PAN_MIN_ANGLE_DEG, TURRET_PAN_MAX_ANGLE_DEG);     // Initialize pan servo with limits
  dxlCtrl->initializeServo(tilt,TURRET_TILT_MIN_ANGLE_DEG, TURRET_TILT_MAX_ANGLE_DEG);  // Initialize tilt servo with limits
}

// Rotate the turret to specified angles
void Turret::rotateTurret(float panAngle, float tiltAngle) {
  dxlCtrl->setPosition(pan, panAngle);                              // Set pan angle
  dxlCtrl->setPosition(tilt, tiltAngle);                            // Set tilt angle 
}

// Reset the turret to default position
void Turret::resetTurret() {
  rotateTurret(TURRET_PAN_DEFAULT_DEG, TURRET_TILT_DEFAULT_DEG);    // Reset turret to default position
}

// Read the sensor value
int Turret::readSensor() {
  return dxlCtrl->readPosition(sensor);                             // Read the sensor
}
