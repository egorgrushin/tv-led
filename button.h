#include "GyverButton.h"
#define TOGGLE_BTN_PIN 4

GButton toggleBtn(TOGGLE_BTN_PIN, HIGH_PULL, NORM_OPEN);

void buttonTick() {
    toggleBtn.tick();
    if (toggleBtn.isPress() || toggleBtn.isRelease()) {
        data.isLedEnabled = !data.isLedEnabled;
    }
}
