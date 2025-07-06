#include <Arduino.h>
#include "DynamixelController.h"
#include "Leg.h"
#include "Hexapod.h"
#include "Turret.h"
#include "RCController.h"
#include "GaitController.h"

DynamixelController dxlController(Serial3, 22);
RCController rcController(Serial1);

Hexapod*        hexapod;
Turret*         turret;
GaitController* gaitController;

void setup() {

  // Initialize Serial for debugging
  Serial.begin(115200);
  
  while (!Serial) {
    ; // Wait for Serial to be ready
  }

  Serial.println("SpiderBot Starting Setup...");

  // Initialize Dynamixel Controller
  dxlController.begin(57600);
  hexapod = new Hexapod(&dxlController);
  hexapod->initialize();
  Serial.println("Hexapod initialized.");
  
  // Initialize Turret
  turret = new Turret(19, 20, 100, &dxlController);
  turret->initialize();
  Serial.println("Turret initialized.");

  // Initialize RC Controller
  rcController.begin(57600);
  Serial.println("RC Controller initialized.");
  
  // Initialize Gait Controller
  gaitController = new GaitController(hexapod, 1000);
  Serial.println("Gait Controller initialized.");
  
  Serial.println("SpiderBot Setup Complete.");



}

void loop() {
  rcController.readInput();
  RCState state = rcController.getState();

  if (!state.signalValid) {
    hexapod->stop();
    return;
  }

  gaitController->update();
}
