

#include <Arduino.h>
#include "Config.h"
#include "DynamixelController.h"
#include "Leg.h"
#include "Hexapod.h"
#include "Turret.h"
#include "RCController.h"
#include "GaitController.h"


DynamixelController dxlController(DXL_SERIAL, DXL_DIR_PIN);
RCController rcController(RC_SERIAL);

Hexapod*        hexapod;
Turret*         turret;
GaitController* gaitController;

void setup() {

  // Initialize Serial for debugging
  Serial.begin(DEBUG_BAUD_RATE);
  // Wait for Serial to be ready
  while (!Serial) {};

  Serial.println("SpiderBot Starting Setup...");

  // Initialize Dynamixel Controller
  dxlController.begin(DXL_BAUD_RATE, DXL_PROTOCOL_VERSION);
  hexapod = new Hexapod(&dxlController);  // Create Hexapod instance with Dynamixel controller
  hexapod->initialize();                  // Initialize all legs
  hexapod->setGait(0);                    // Set default gait
  hexapod->setSpeed(0.5);                 // Set default speed
  Serial.println("Dynamixel Controller initialized.");
  
  // Initialize Sensor Turret
  turret = new Turret(TURRET_PAN_ID, TURRET_TILT_ID, AX_S1_SENSOR_ID, &dxlController);
  turret->initialize();                   // Initialize turret servos
  turret->resetTurret();                  // Reset turret to default position
  Serial.println("Sensor Turret initialized.");

  // Initialize RC Controller
  rcController.begin(RC_BAUD_RATE);
  Serial.println("RC Controller initialized.");
  
  // Initialize Gait Controller
  gaitController = new GaitController(hexapod, DEFAULT_GAIT_CYCLE_MS);
  Serial.println("Gait Controller initialized.");
  
  Serial.println("SpiderBot Setup Complete.");



}

void loop() {

  turret->rotateTurret(290, 60);  // Rotate turret to test position
  delay(3000);                     // Wait for a second
  turret->rotateTurret( 40, 60);  // Rotate turret to test position
  delay(3000);                     // Wait for a second
  turret->resetTurret();           // Reset turret to default position
  delay(3000);                     // Wait for a second


}
