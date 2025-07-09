#ifndef GAITCONTROLLER_H
#define GAITCONTROLLER_H

#include "Hexapod.h"
#include "Config.h"


class GaitController {
  public:
    GaitController(Hexapod* hexapod, unsigned long cycleDuration);
    void update();
    void setCycleDuration(unsigned long duration);
  private:
    Hexapod* hexapod;
    unsigned long cycleDuration;  // Duration of one complete gait cycle in milliseconds
    unsigned long lastPhaseTime;  // Timestamp of the last phase change
    bool phase;                   // false = A moves, true = B moves
    void executePhase();          // Execute the current phase of the gait
};

#endif
