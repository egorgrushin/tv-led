#define WIFI_AP_SSID "TVLedDevice"
#define WIFI_AP_PASS "Lalaept2"
#define MAX_RETRIES_COUNT 50
#define RETRIES_DELAY 10000
#include <ESP8266WiFi.h>
uint8_t retries = 0;

void setupAp() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);
}

void tryConnectToWifi(String wifiSsid, String wifiPass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid, wifiPass);
  while (WiFi.status() != WL_CONNECTED && retries < MAX_RETRIES_COUNT) {
    delay(RETRIES_DELAY);
    retries++;
  }
  if (retries >= MAX_RETRIES_COUNT) {
    setupAp();
  }
}

void wifiSetup() {
  if (data.useAPInstead || strlen(data.wifiSsid) == 0) {
    setupAp();
  } else {
    tryConnectToWifi(data.wifiSsid, data.wifiPass);
  }
}
