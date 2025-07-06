#include <Arduino.h>
#include "DynamixelController.h"
#include "Leg.h"
#include "Hexapod.h"
#include "Turret.h"
#include "RCController.h"
#include "GaitController.h"

DynamixelController dxlController(Serial3, 22);
RCController rcController(Serial1);
Hexapod* hexapod;
Turret* turret;
GaitController* gaitController;

void setup() {
  Serial.begin(115200);

  dxlController.begin(57600);
  hexapod = new Hexapod(&dxlController);
  hexapod->initialize();

  turret = new Turret(19, 20, 100, &dxlController);
  turret->initialize();

  rcController.begin(57600);

  gaitController = new GaitController(hexapod, 1000);
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
