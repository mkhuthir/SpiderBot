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
void Hexapod::setGaitType(int Type) {
  gaitType = Type;
}

// Set the gait speed
void Hexapod::setGaitSpeed(float Speed) {
  gaitSpeed = Speed;
}

// Set the servo speed
void Hexapod::setServoSpeed(float Speed) {
  servoSpeed = Speed;
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

// Print the status of all legs
void Hexapod::printLegsStatus() {
  Serial.println("\nHexapod Legs Status:");
  for (int i = 0; i < 6; i++) {
    Serial.print("Leg ");
    Serial.print(i + 1);
    Serial.print(": ");
    legs[i]->printJointAngles();  // Print joint angles of each leg 
  }
  Serial.println("---");

}




