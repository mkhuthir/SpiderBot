#include "RCController.h"

RCController::RCController(HardwareSerial& serial) : rcSerial(serial) {
  lastSignalTime = 0;
}

void RCController::begin(int baudrate) {
  rcSerial.begin(baudrate);
}

void RCController::readInput() {
  while (rcSerial.available()) {
    String cmd = rcSerial.readStringUntil('\n');
    parseCommand(cmd);
    lastSignalTime = millis();
  }
}

void RCController::parseCommand(String cmd) {
  if (cmd.length() == 0) return;
  char* p = strtok((char*)cmd.c_str(), ",");
  while (p != NULL) {
    if (p[0]=='S') state.speed = atof(p+1);
    else if (p[0]=='G') state.gait = atoi(p+1);
    else if (p[0]=='D') state.direction = atof(p+1);
    else if (p[0]=='P') state.turretPan = atof(p+1);
    else if (p[0]=='T') state.turretTilt = atof(p+1);
    p = strtok(NULL, ",");
  }
  state.signalValid = true;
}

RCState RCController::getState() {
  if (isSignalLost()) {
    state.signalValid = false;
  }
  return state;
}

bool RCController::isSignalLost() {
  return (millis() - lastSignalTime > 1000);
}
