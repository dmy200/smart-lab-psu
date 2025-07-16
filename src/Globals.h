// Globals.h
#pragma once

#include <Arduino.h>

// === Измеренные параметры ===
extern float labV_meas;   // Напряжение
extern float labI_meas;   // Ток
extern float labQ_meas;   // Мощность

// === Параметры ограничения ===
extern float labV_set;     // Установленное напряжение
extern float labI_limit;   // Ограничение по току
extern float labI_fuse;    // Предохранитель
extern bool editingFuse;   // Флаг редактирования предохранителя

// === Флаги ===
extern bool inEdit;        // Флаг редактирования параметра
extern bool ledState;      // Состояние LED-индикатора

extern float labTemp_ntc;  // Температура NTC
extern bool modeAuto;      // Режим автоуправления

extern float tempLimitC;              // Порог отключения по температуре, °C
extern uint8_t ina226AverageSetting;  // Усреднение INA226

// === Пределы системы ===
extern float systemVoutMin;   // Минимальное выходное напряжение
extern float systemVoutMax;   // Максимальное выходное напряжение
extern float systemIlimitMax; // Максимальный ток
extern float systemPowerMax;  // Максимальная доступная мощность, Вт

// === Таймеры ===
extern unsigned long lastSend;
extern unsigned long lastBlink;
extern unsigned long lastDisplayUpdate;

// === Настройки энкодера ===
#define ENC_STEP                0.1f    // Шаг изменения при вращении
#define ENC_LONG_PRESS_MS       600     // ms: длительное нажатие
#define ENC_MULTICLICK_MS       400     // ms: пауза между кликами
#define ENC_INACTIVITY_TIMEOUT  30000   // ms: автовыход из редактирования

