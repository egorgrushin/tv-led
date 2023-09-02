#define GH_INCLUDE_PORTAL

#include <GyverHub.h>

GyverHub hub("gh", "TvLed", "");

uint8_t tabIndex = 0;
GHbutton restartBtn;
GHbutton resetBtn;



bool buildPresets() {
  bool isUpdated = false;

  for (byte i = 0; i < MAX_PRESETS_SIZE; i++) {
    hub.BeginWidgets();
    hub.WidgetSize(80);
    isUpdated |= hub.Input(&data.presets[i].name, GH_CSTR, F("Name"));
    isUpdated |= hub.Slider(&data.presets[i].hue, GH_UINT8, F("Hue"), 0, 255, 1);
    isUpdated |= hub.Slider(&data.presets[i].saturation, GH_UINT8, F("Saturation"), 0, 255, 1);
    isUpdated |= hub.Slider(&data.presets[i].brightness, GH_UINT8, F("Brightness"), 0, 255, 1);
    hub.EndWidgets();
    hub.Space(20);
  }

  return isUpdated;
}

bool buildSettings() {
  bool isUpdated = false;
  hub.BeginWidgets();
  isUpdated |= hub.Input(&data.wifiSsid, GH_CSTR, F("WiFi SSID"));
  isUpdated |= hub.Pass(&data.wifiPass, GH_CSTR, F("WiFi Pass"));
  isUpdated |= hub.Switch(&data.isStaModeEnabled, F("Use STA mode"));
  hub.EndWidgets();
  hub.Button(&restartBtn, F("Restart"));
  hub.BeginWidgets();
  hub.Button(&resetBtn, F("Reset to default (Hold for 5 seconds for reset)"), GHcolor(255, 0, 0));
  hub.EndWidgets();
  return isUpdated;
}

bool buildMain() {
  bool isUpdated = false;
  hub.BeginWidgets();
  hub.WidgetSize(40);

  isUpdated |= hub.Switch_(F("LedToggle"), &data.isLedEnabled, F("Toggle"));
  String presetsNames = "";
  for (byte i = 0; i < MAX_PRESETS_SIZE; i++) {
    presetsNames += String(data.presets[i].name);
    if (i != MAX_PRESETS_SIZE - 1) {
      presetsNames += ",";
    }
  }
  hub.WidgetSize(60);
  isUpdated |= hub.Select(&data.currentPresetIndex, presetsNames, F("Preset"));

  hub.EndWidgets();
  return isUpdated;
}

void hubBuild() {
  bool isUpdated = false;
  if (hub.Tabs(&tabIndex, F("Main,Presets,Settings"))) {
    hub.refresh();
  }
  if (tabIndex == 0) {
    isUpdated |= buildMain();
  }
  if (tabIndex == 1) {
    isUpdated |= buildPresets();
  }
  if (tabIndex == 2) {
    isUpdated |= buildSettings();
  }
  if (isUpdated) {
    memory.update();
  }
}

GHtimer resetTimer;

void hubSetup() {
  hub.onBuild(hubBuild);
  hub.begin();
}

void processBtns() {
  if (restartBtn.changed()) {
    dataSaveAndRestart();
  }
  if (resetBtn.changed()) {
    if (resetBtn) {
      resetTimer.start(5000);
    } else {
      resetTimer.stop();
    }

  }
}

void updateToggle() {
  static GHtimer tmr(1000);
  if (tmr) {
    hub.sendUpdate("LedToggle", String(data.isLedEnabled));
  }
}

void hubTick() {
  hub.tick();
  updateToggle();
  processBtns();
  if (resetTimer) {
    resetTimer.stop();
    if (resetBtn) {
      dataResetAndRestart();
    }
  }
}
