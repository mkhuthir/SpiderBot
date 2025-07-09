#include "DynamixelController.h"

DynamixelController::DynamixelController(HardwareSerial& serial, uint8_t dirPin)
  : dxl(serial, dirPin) {}

// Begin the Dynamixel controller with the specified baud rate
void DynamixelController::begin(int baudrate, float protocolVersion) {
  dxl.begin(baudrate);                              // Initialize the Dynamixel controller
  dxl.setPortProtocolVersion(protocolVersion);      // Set the protocol version
}

// Initialize a servo with its ID and angle limits
void DynamixelController::initializeServo(uint8_t id, float minAngle, float maxAngle) {
  dxl.torqueOff(id);                      // Disable torque before configuration
  dxl.setOperatingMode(id, OP_POSITION);  // Set to Position Control mode
  dxl.torqueOn(id);                       // Enable torque
  limits[id] = {minAngle, maxAngle};      // Store limits for this servo
}

// Check if the controller is connected to the servos
bool DynamixelController::isConnected() {
  return dxl.ping(DXL_BROADCAST_ID);
}

// Set the position of a servo in degrees
void DynamixelController::setServoPosition(uint8_t id, float angleDeg) {
  if (angleDeg < limits[id].minAngle) angleDeg = limits[id].minAngle; // Clamp to min angle
  if (angleDeg > limits[id].maxAngle) angleDeg = limits[id].maxAngle; // Clamp to max angle
  dxl.setGoalPosition(id, angleDeg, UNIT_DEGREE);                     // Set position in degrees
}

// Set the speed of a servo in RPM
void DynamixelController::setServoSpeed(uint8_t id, float percent) {
  dxl.setGoalVelocity(id, percent, UNIT_PERCENT);         // Set speed in RPM
}

// Read the current position of a servo in degrees
float DynamixelController::getServoPosition(uint8_t id) {
  return dxl.getPresentPosition(id, UNIT_DEGREE); // Read position in degrees
}