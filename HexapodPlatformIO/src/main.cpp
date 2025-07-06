#include <Arduino.h>
#include "DynamixelController.h"
#include "Leg.h"
#include "Hexapod.h"
#include "Turret.h"
#include "RCController.h"

DynamixelController dxlController(Serial3, 22);
RCController        rcController(Serial1);
Hexapod*            hexapod;
Turret*             turret;

void setup() {
  Serial.begin(115200);
  Serial.println("SpiderBot Startup...");

  dxlController.begin(57600);
  hexapod = new Hexapod(&dxlController);
  hexapod->initialize();

  turret = new Turret(19, 20, 100, &dxlController);
  turret->initialize();

  rcController.begin(57600);
}

void loop() {
  rcController.readInput();
  RCState state = rcController.getState();

  if (!state.signalValid) {
    hexapod->stop();
    turret->resetTurret();
    return;
  }

  hexapod->setGait(state.gait);
  hexapod->setSpeed(state.speed);

  if (state.direction > 0.1) {
    hexapod->turn(state.direction);
  } else {
    hexapod->moveForward();
  }

  turret->rotateTurret(state.turretPan, state.turretTilt);
}
