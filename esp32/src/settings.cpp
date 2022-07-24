#include "settings.hpp"

#include <EEPROM.h>

ThermalDetectorSettings Settings;

void ThermalDetectorSettings::Load() {
  EEPROM.begin(sizeof(Settings));
  EEPROM.get(0, Settings);
  EEPROM.end();
}

void ThermalDetectorSettings::Save() {
  EEPROM.begin(sizeof(Settings));
  EEPROM.put(0, Settings);
  EEPROM.end();
}

ThermalDetectorState State;