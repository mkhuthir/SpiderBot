#ifndef RCCONTROLLER_H
#define RCCONTROLLER_H

#include <HardwareSerial.h>

struct RCState {
  float speed;
  int gait;
  float direction;
  float turretPan;
  float turretTilt;
  bool signalValid;
};

class RCController {
  public:
    RCController(HardwareSerial& serial);
    void begin(int baudrate);
    void readInput();
    RCState getState();
    bool isSignalLost();

  private:
    HardwareSerial& rcSerial;
    RCState state;
    unsigned long lastSignalTime;
    void parseCommand(String cmd);
};

#endif
