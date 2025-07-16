//EncoderManager.h
#pragma once

#include <RotaryEncoder.h>
#include <Arduino.h>
#include "Config.h"

namespace EncoderManager {

// === Глобальные переменные состояния ===
extern int encLastPos;
extern bool editingFuse;

// Объект энкодера
extern RotaryEncoder encoder;

// === Интерфейс ===
void begin();
void update();

// Запуск редактирования
void beginEdit(float* target, bool* flagEditing, bool fuse);
void confirmEdit();
void cancelEdit();

} // namespace EncoderManager
