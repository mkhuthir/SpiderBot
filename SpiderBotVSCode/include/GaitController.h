#ifdef GaitController_h
#define GaitController_h

#include <Arduino.h>
#include "Hexapod.h"

enum GaitType {
    GAIT_IDLE,
    GAIT_WAVE,
    GAIT_RIPPLE,
    GAIT_TRIPOD
};

class GaitController {
private:
    Hexapod* robot;
    GaitType gait;
    unsigned long lastUpdate;
    int currentPhase;
    unsigned long stepInterval;

    void doWaveGait();
    void doRippleGait();
    void doTripodGait();

public:
    GaitController(Hexapod* hexapod);

    void setGait(GaitType newGait);
    void setStepInterval(unsigned long interval); // set gait speed
    void update();
};

#endif // GaitController_h
