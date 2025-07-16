// Ina226Manager.h
#pragma once

#include <Arduino.h>

namespace Ina226Manager {
  void begin();
  void update();

  float getVoltage();  // Вольты
  float getCurrent();  // Амперы
  float getPower();    // Ватты
}
