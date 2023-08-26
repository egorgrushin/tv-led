#define WIFI_AP_SSID "UnknownDevice"
#define WIFI_AP_PASS "87654321"
#include <ESP8266WiFi.h>

void setupAp() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);
}

void tryConnectToWifi(String wifiSsid, String wifiPass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid, wifiPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (millis() > 15000) {
      data.isStaModeEnabled = false;
      dataSaveAndRestart();
    }
  }
}

void wifiSetup() {
  if (data.isStaModeEnabled) {
    tryConnectToWifi(data.wifiSsid, data.wifiPass);
  } else {
    setupAp();
  }
}
