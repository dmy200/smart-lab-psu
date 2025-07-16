// EepromManager.h
#pragma once

#include <Arduino.h>

// Структура для хранения всех настроек
struct LabSettings {
  float i_limit;
  float i_fuse;
  // Добавим при необходимости другие
};

void EepromManager_begin();
void EepromManager_load(LabSettings& settings);
void EepromManager_save(const LabSettings& settings);
