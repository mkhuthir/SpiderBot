#include <Arduino.h>
#include "GaitController.h"

GaitController::GaitController(Hexapod* hexapod, unsigned long cycleDuration) {
  this->hexapod = hexapod;
  this->cycleDuration = cycleDuration;
  this->lastPhaseTime = millis();
  this->phase = false;
}

void GaitController::setCycleDuration(unsigned long duration) {
  this->cycleDuration = duration;
}

void GaitController::update() {
  unsigned long now = millis();
  if (now - lastPhaseTime >= cycleDuration) {
    phase = !phase;
    lastPhaseTime = now;
    executePhase();
  }
}

void GaitController::executePhase() {
  if (!phase) {
    // Phase 1: Group A moves
    hexapod->liftAndSwingLegs({1,4,5});
    hexapod->supportLegs({2,3,6});
  } else {
    // Phase 2: Group B moves
    hexapod->liftAndSwingLegs({2,3,6});
    hexapod->supportLegs({1,4,5});
  }
}
