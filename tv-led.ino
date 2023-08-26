#include "data.h"
#include "wifi.h"
#include "hub.h"
#include "led.h"


void setup() {
    Serial.begin(115200);
    dataSetup();
    clearOnboardLed();
    wifiSetup();
    hubSetup();
    ledSetup();
}

void loop() {
    dataTick();
    hubTick();
    ledTick();
}

void clearOnboardLed() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}
