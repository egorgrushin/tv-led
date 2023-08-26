#include <EEManager.h>
#define MAX_PRESETS_SIZE 20


struct Preset {
  char name[20] = "";
  uint8_t hue = 0;
  uint8_t saturation = 0;
  uint8_t brightness = 0;
};

struct Data {
  Preset presets[MAX_PRESETS_SIZE] = {};
  bool isStaModeEnabled = false;
  bool isLedEnabled = true;
  byte currentPresetIndex = 0;
  char wifiSsid[16] = "";
  char wifiPass[16] = "";

};

Data data;
EEManager memory(data);

void initDefaultData() {
  for (byte i = 0; i < MAX_PRESETS_SIZE; i++) {
    char indexChars[2] = "";
    itoa(i, indexChars, DEC);
    strcpy(data.presets[i].name, "Preset ");
    strcat(data.presets[i].name, indexChars);
  }
}


void dataSetup() {
  initDefaultData();
  EEPROM.begin(memory.blockSize());
  memory.begin(0, 'b');
}

void dataTick() {
  memory.tick();
}

void dataSaveAndRestart() {
  memory.updateNow();
  ESP.restart();
}

void dataResetAndRestart() {
  memory.reset();
  ESP.restart();
}

Preset getCurrentPreset() {
  return data.presets[data.currentPresetIndex];
}


uint8_t getPresetsCount() {
  return sizeof(data.presets) / sizeof(data.presets[0]);
}
