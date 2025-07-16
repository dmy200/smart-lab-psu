// Globals.cpp
#include "Globals.h"

// === Измеренные параметры ===
float labV_meas = 0.0f;
float labI_meas = 0.0f;
float labQ_meas = 0.0f;

// === Параметры ограничения ===
float labV_set = 0.0f;
float labI_limit = 0.0f;
float labI_fuse = 0.0f;
bool editingFuse = false;

// === Флаги ===
bool inEdit = false;
bool ledState = false;

float labTemp_ntc = 0.0f;
bool modeAuto = false;

float tempLimitC = 70.0f;               // Порог отключения по температуре
uint8_t ina226AverageSetting = 16;      // Усреднение INA226 (по умолчанию AVERAGE_16)

// === Пределы системы ===
float systemVoutMin   = 1.25f;
float systemVoutMax   = 36.0f;
float systemIlimitMax = 3.00f;
float systemPowerMax  = 60.0f;

// === Таймеры ===
unsigned long lastSend = 0;
unsigned long lastBlink = 0;
unsigned long lastDisplayUpdate = 0;


