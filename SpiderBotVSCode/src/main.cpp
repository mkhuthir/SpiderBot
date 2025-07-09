

#include <Arduino.h>
#include "Config.h"
#include "DynamixelController.h"
#include "Leg.h"
#include "Hexapod.h"
#include "Turret.h"
#include "RCController.h"
#include "GaitController.h"


DynamixelController dxlController(DXL_SERIAL, DXL_DIR_PIN); // Create an instance of DynamixelController with the specified serial port and direction pin
RCController rcController(RC_SERIAL);                       // Create an instance of RCController with the specified serial port

Hexapod*        hexapod;            // Pointer to Hexapod instance
Turret*         turret;             // Pointer to Turret instance
GaitController* gaitController;     // Pointer to GaitController instance

void setup() {

  // Initialize Serial for debugging
  Serial.begin(DEBUG_BAUD_RATE);
  while (!Serial); // Wait for Serial to be ready

  Serial.println("SpiderBot Starting Setup...");

  // Initialize Dynamixel Controller
  dxlController.begin(DXL_BAUD_RATE, DXL_PROTOCOL_VERSION);

  if (!dxlController.isConnected()) {
    Serial.println("Failed to connect to Dynamixel servos.");   // Print error message
    while (true);                                               // Halt execution if connection fails
  }
  Serial.println("Dynamixel Controller connected successfully.");

  // Initialize Hexapod
  hexapod = new Hexapod(&dxlController);  // Create Hexapod instance with Dynamixel controller
  hexapod->initialize();                  // Initialize all legs
  
  hexapod->setGaitType(0);                // Set default gait
  hexapod->setGaitSpeed(0.5);             // Set default speed
  hexapod->setServoSpeed(0.5);            // Set default servo speed

  Serial.println("Dynamixel Controller initialized.");
  
  // Initialize Sensor Turret
  turret = new Turret(TURRET_PAN_ID, TURRET_TILT_ID, &dxlController); // Create Turret instance with Dynamixel controller
  turret->initialize();                                               // Initialize turret servos
  turret->rotateTurretHome();                                         // Rotate turret to home position
  Serial.println("Sensor Turret initialized.");

  // Initialize RC Controller
  rcController.begin(RC_BAUD_RATE);
  Serial.println("RC Controller initialized.");
  
  // Initialize Gait Controller
  gaitController = new GaitController(hexapod, DEFAULT_GAIT_CYCLE_MS);
  Serial.println("Gait Controller initialized.");
  Serial.println("SpiderBot Setup Complete.");
  Serial.println("Ready to receive commands.");

  hexapod->printLegsStatus();  // Print initial status of all legs
  turret->printTurretStatus(); // Print initial status of turret

}

void loop() {

  turret->rotateTurretLeft();  // Rotate turret to the left for testing
  delay(1000);                 // Wait for 1 second
  turret->rotateTurretRight(); // Rotate turret to the right for testing
  delay(1000);                 // Wait for 1 second
  turret->rotateTurretUp();    // Rotate turret up for testing
  delay(1000);                 // Wait for 1 second
  turret->rotateTurretDown();  // Rotate turret down for testing
  delay(1000);                 // Wait for 1 second
  turret->rotateTurretHome();  // Rotate turret back to home position for testing
  delay(1000);                 // Wait for 1 second

}
