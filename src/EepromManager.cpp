// EepromManager.cpp
#include "EepromManager.h"
#include <EEPROM.h>

#define EEPROM_SIZE 64
#define EEPROM_ADDR 0

void EepromManager_begin() {
  EEPROM.begin(EEPROM_SIZE);
}

void EepromManager_load(LabSettings& settings) {
  EEPROM.get(EEPROM_ADDR, settings);
  if (isnan(settings.i_limit) || isnan(settings.i_fuse)) {
    settings.i_limit = 1.0;
    settings.i_fuse = 2.0;
  }
}

void EepromManager_save(const LabSettings& settings) {
  EEPROM.put(EEPROM_ADDR, settings);
  EEPROM.commit();
}
