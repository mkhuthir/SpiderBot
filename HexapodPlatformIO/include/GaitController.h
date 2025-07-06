#ifndef GAITCONTROLLER_H
#define GAITCONTROLLER_H

#include "Hexapod.h"

class GaitController {
  public:
    GaitController(Hexapod* hexapod, unsigned long cycleDuration);
    void update();
    void setCycleDuration(unsigned long duration);
  private:
    Hexapod* hexapod;
    unsigned long cycleDuration;
    unsigned long lastPhaseTime;
    bool phase;  // false = A moves, true = B moves
    void executePhase();
};

#endif
