#include "data.h"
#include "wifi.h"
#include "hub.h"
#include "mqtt.h"
#include "led.h"
#include "button.h"


void setup() {
    dataSetup();
    clearOnboardLed();
    wifiSetup();
    hubSetup();
    mqttSetup();
    ledSetup();
}

void loop() {
    dataTick();
    hubTick();
    ledTick();
    buttonTick();
}

void clearOnboardLed() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}
