#include <GyverHub.h>

GyverHub hub("gh", "TvLed", "");

byte tab = 0;

struct PresetState {
  bool isLocked = true;
};

PresetState states[MAX_PRESETS_SIZE] = {};

void buildPresets(gh::Builder& b) {
  for (byte i = 0; i < MAX_PRESETS_SIZE; i++) {
    bool isLocked = states[i].isLocked;
    String lockIcon = isLocked ? "f023" : "f3c1";
    {
      gh::Row r(b);
      if (b.SwitchIcon(&states[i].isLocked).icon(lockIcon).size(1).noLabel(true).click()) {
        b.refresh();
      };

      {
        gh::Col c(b, 3);
        b.Input(data.presets[i].name).label("Name").disabled(isLocked).maxLen(MAX_PRESET_NAME_LENGTH);
        {
          gh::Row r(b);
          b.Slider(&data.presets[i].hue).label("Hue").range(0, 255, 1).disabled(isLocked);
          b.Slider(&data.presets[i].saturation).label("Saturation").range(0, 255, 1).disabled(isLocked);
          b.Slider(&data.presets[i].brightness).label("Brightness").range(0, 255, 1).disabled(isLocked);
        }
      }
    }
  }
}

void buildSettings(gh::Builder& b) {
  b.Input(data.wifiSsid).label("WiFi SSID");
  b.Pass(data.wifiPass).label("WiFi Pass");
  {
    gh::Row r(b);
    b.Switch(&data.useAPInstead).label("Use AP (default STA)");
    b.Switch(&data.isMqttEnabled).label("Use MQTT");
  }
  b.Input(data.mqttHost).label("MQTT Host");
  b.Input(&data.mqttPort).label("MQTT Port");
  b.Input(data.mqttUser).label("MQTT User");
  b.Pass(data.mqttPass).label("MQTT Pass");
}

void buildSystem(gh::Builder& b) {
  bool isRestartConfirmed;
  if (b.Confirm_("confirm restart", &isRestartConfirmed).text("Confirm restart").click()) {
    if (isRestartConfirmed) {
      dataSaveAndRestart();
    }
  }

  if (b.Button().label("Restart").click()) {
    hub.sendAction("confirm restart");
  }
  
  bool isResetConfirmed;
  if (b.Confirm_("confirm reset", &isResetConfirmed).text("Confirm resetting all settings to default").click()) {
    if (isResetConfirmed)  {
      dataResetAndRestart();
    }
  }

  if (b.Button()
    .label("Reset to defaults")
    .color(gh::Colors::Red)
    .click()) {
      hub.sendAction("confirm reset");
  }
}

void buildMain(gh::Builder& b) {
  {
    gh::Row r(b);
    b.Switch_("LedToggle", &data.isLedEnabled).label("Toggle");
    String presetsNames = "";
    for (byte i = 0; i < MAX_PRESETS_SIZE; i++) {
      presetsNames += String(data.presets[i].name);
      if (i != MAX_PRESETS_SIZE - 1) {
        presetsNames += ";";
      }
    }
    b.Select(&data.currentPresetIndex).text(presetsNames).label("Preset");
  }
}


void buildTabs(gh::Builder& b) {
  if (b.Tabs(&tab).text("Main;Presets").click()) {
    b.refresh();
  }
  if (tab == 0) {
    buildMain(b);
  }
  if (tab == 1) {
    buildPresets(b);
  }
}

void buildMenu(gh::Builder& b) {
  b.Menu("Control;Settings;System");
  b.show(b.menu() == 0);
  buildTabs(b);
  b.show(b.menu() == 1);
  buildSettings(b);
  b.show(b.menu() == 2);
  buildSystem(b);
}




void hubBuild(gh::Builder& b) {
  buildMenu(b);
  if (b.changed()) {
    memory.update();
  }
}

void hubSetup() {
  hub.onBuild(hubBuild);
  hub.begin();
}

void updateToggle() {
  static gh::Timer tmr(1000);
  if (tmr) {
    hub.sendUpdate("LedToggle", data.isLedEnabled);
  }
}

void hubTick() {
  hub.tick();
  updateToggle();
}
