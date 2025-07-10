#include "GaitController.h"

GaitController::GaitController(Hexapod* hexapod) {
    robot = hexapod;
    gait = GAIT_IDLE;
    lastUpdate = millis();
    currentPhase = 0;
    stepInterval = 500;  // Default 500ms between steps
}

void GaitController::setGait(GaitType newGait) {
    gait = newGait;
    currentPhase = 0;
    lastUpdate = millis();
}

void GaitController::setStepInterval(unsigned long interval) {
    stepInterval = interval;
}

void GaitController::update() {
    if (gait == GAIT_IDLE) return;

    unsigned long now = millis();
    if (now - lastUpdate < stepInterval) return;

    lastUpdate = now;

    switch (gait) {
        case GAIT_WAVE:
            doWaveGait();
            break;
        case GAIT_RIPPLE:
            doRippleGait();
            break;
        case GAIT_TRIPOD:
            doTripodGait();
            break;
        default:
            break;
    }
}

void GaitController::doWaveGait() {
    // One leg swings at a time
    //robot->moveLeg(currentPhase, 512, 300, 700);  // swing phase
    delay(50);
    //robot->moveLeg(currentPhase, 512, 512, 512);  // stance phase

    currentPhase = (currentPhase + 1) % 6;
}

void GaitController::doRippleGait() {
    // Two legs with phase offset
    int swingLegs[2] = {
        currentPhase,
        (currentPhase + 3) % 6
    };

    for (int i = 0; i < 2; ++i) {
        //robot->moveLeg(swingLegs[i], 512, 300, 700);
    }
    delay(50);
    for (int i = 0; i < 2; ++i) {
        //robot->moveLeg(swingLegs[i], 512, 512, 512);
    }

    currentPhase = (currentPhase + 1) % 3;
}

void GaitController::doTripodGait() {
    // Tripod groups:
    // Group A: 0, 3, 4
    // Group B: 1, 2, 5
    int groupA[3] = {0, 3, 4};
    int groupB[3] = {1, 2, 5};

    int* swingGroup;
    if (currentPhase % 2 == 0) {
        swingGroup = groupA;
    } else {
        swingGroup = groupB;
    }

    for (int i = 0; i < 3; ++i) {
        //robot->moveLeg(swingGroup[i], 512, 300, 700);
    }
    delay(50);
    for (int i = 0; i < 3; ++i) {
        //robot->moveLeg(swingGroup[i], 512, 512, 512);
    }

    currentPhase = (currentPhase + 1) % 2;
}
// GaitController.cpp